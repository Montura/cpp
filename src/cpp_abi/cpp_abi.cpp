#include <cstdio>
#include <cstdlib>
#include <typeinfo>
#include <unwind.h>
#include <cstdint>

#include "cxa_exception.h"
#include "lsda.h"
#include "lsda_llvm.h"

namespace __cxxabiv1 {
  struct __class_type_info {
    virtual void foo() {}
  } ti;
}

namespace std {
  void terminate() noexcept {
    printf("Call terminate\n");
    exit(-1);
  }
}

extern "C" {
  using namespace cxa_exception;
  using namespace LSDA;

  void* __cxa_allocate_exception(size_t thrown_size) {
    printf("alloc ex %lu\n", thrown_size);
    if (thrown_size > EXCEPTION_BUFF_SIZE) {
      printf("Exception too big");
    }
    return &exception_buff;
  }

  void __cxa_free_exception(void* thrown_exception);

  uintptr_t getIP(_Unwind_Context* const context) {
    // Calculate what IP was just before the exception was thrown for this stack frame
    //  1. The processor always increments the IP when it executes an instruction. Throwing exception is not an exception.
    //  Thus when the processor executes it, it increments IP by one and starts executing personal routine (__gxx_personality_v0).
    //  2. Therefore when the personal routine is executing, the caller's IP is already one byte after
    //  the "throw" statement and you have to move IP one byte back.
    uintptr_t ip = _Unwind_GetIP(context) - 1;
    printf("\tthrow ip = %p\n", reinterpret_cast<void*>(ip));
    return ip;
  }

  void setIP(_Unwind_Context* const context, _Unwind_Exception* unwind_exception, uintptr_t func_start, uint8_t cs_length) {
    int r0 = __builtin_eh_return_data_regno(0);
    int r1 = __builtin_eh_return_data_regno(1);

    // Set data registers for exception handling (hardcodes the exception type; we'll fix that later on)

    // See http://www.ucw.cz/~hubicka/papers/abi/node25.html#SECTION00925000000000000000
    // In that case, only registers %rdi, %rsi, %rdx, %rcx should be used.
    // These scratch registers are reserved for passing arguments between the personality routine and the landing pads.

    // See 1.6.3 Transferring Control to a Landing Pad (https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html#base-personality)
    //  - The landing pad may receive arguments from the runtime,
    //    typically passed in registers set using _Unwind_SetGR by the personality routine.
    //    For a landing pad that can call to _Unwind_Resume, one argument must be the exceptionObject pointer,
    //    which must be preserved to be passed to _Unwind_Resume.
    //  - The landing pad may receive other arguments, for instance a switch value indicating the type of the exception.

    _Unwind_SetGR(context, r0, (uintptr_t)(unwind_exception));
    _Unwind_SetGR(context, r1, (uintptr_t)(1));

    /* Setup the address we should jump at to reach the code where there is the "something" we found.  */
    _Unwind_SetIP(context, func_start + cs_length);
  }

  bool IP_is_valid(uintptr_t throw_ip, uintptr_t curr_stack_frame, LSDA_Call_Site const* cs) {
    // Calculate the range of the IP range valid for this landing pad;
    // - If this LP can handle the current exception then the IP for this stack frame must be in this range

    uintptr_t try_start = curr_stack_frame + cs->start;
    uintptr_t try_end = curr_stack_frame + cs->start + cs->len;

    bool is_valid = (throw_ip >= try_start) && (throw_ip <= try_end);

    printf("\tIP is %s try:\n", (is_valid ? "inside" : "outside"));
    printf("\t\tip = %p;\n\t\ttry_start = %p;\n\t\ttry_end = %p\n", (void*) throw_ip, (void*) try_start, (void*) try_end);
    return is_valid;
  }
  
  _Unwind_Reason_Code parse_LSDA(_Unwind_Context* const context, _Unwind_Action actions,
                                 _Unwind_Exception* unwind_exception)
  {
    // Get the beginning of the raw LSDA
    auto lsda_ptr = reinterpret_cast<LSDA_ptr>(_Unwind_GetLanguageSpecificData(context));
    if (!lsda_ptr) {
      return _URC_CONTINUE_UNWIND;
    }

    struct LSDA lsda(lsda_ptr);

    // Loop through each entry in the CS table
    LSDA_Call_Site_Table& cs_table = lsda.cs_table;
    const LSDA_Call_Site* cs;

    // Landing pad offset is valid
    uintptr_t throw_ip = getIP(context);
    uintptr_t func_start = _Unwind_GetRegionStart(context); // points to the current stack frame (function)

    while ((cs = cs_table.next_call_site_entry())) {
      if (not cs->lp || not IP_is_valid(throw_ip, func_start, cs)) {
        continue;
      }
      // We found a landing pad for this exception; resume execution
      // Get the offset into the action table for this LP
      if (cs->action > 0) {
        // cs->action is the offset + 1; that way cs->action == 0
        // means there is no associated entry in the action table
        const size_t action_offset = cs->action - 1;
        const LSDA_ptr action = lsda.action_table_start + action_offset;

        // For a landing pad with a catch the action table will
        // hold an index to a list of types
        int type_index = action[0];

        // todo: fix Segmentation fault! Wrong address for type_ifo!!!!
        const void* catch_type_info = lsda.types_table_start[ -1 * type_index ];
        const std::type_info *catch_ti = (const std::type_info *) catch_type_info;

        // Get the type of the original exception being thrown
        __cxa_exception* exception_header = (__cxa_exception*)(unwind_exception + 1) - 1;
        std::type_info *org_ex_type = exception_header->exceptionType;

        const char* const origin_name = org_ex_type->name();
//        const char* const my_name = catch_ti->name();
        printf("%s thrown, catch handles %s\n", origin_name, "");

        // Check if the exception being thrown is of the same type
        // than the exception we can handle
//        if (org_ex_type->name() != catch_ti->name())
//          continue;
      }

      // If we are on search phase, tell _Unwind_ we can handle this one
      if (actions & _UA_SEARCH_PHASE) {
        return _URC_HANDLER_FOUND;
      }

      setIP(context, unwind_exception, func_start, cs->lp);
      return _URC_INSTALL_CONTEXT;
    }
    return _URC_CONTINUE_UNWIND;
  }

  void __cxa_throw(void* thrown_exception,
                   std::type_info *tinfo,
                   void (* dest)(void*))
  {
    printf("__cxa_throw called, thrown_exception = %p\n", thrown_exception);

    __cxa_exception *header = ((__cxa_exception *) thrown_exception + 1) - 1;

    // We need to save the type info in the exception header _Unwind_ will
    // receive, otherwise we won't be able to know it when unwinding
    header->exceptionType = tinfo;
    _Unwind_RaiseException(&header->unwindHeader);

    // __cxa_throw never returns
    printf("no one handled __cxa_throw, terminate!\n");
    exit(0);
  }

  _Unwind_Reason_Code __gxx_personality_v0 (int version,
                                            _Unwind_Action actions,
                                            uint64_t exceptionClass,
                                            _Unwind_Exception* unwind_exception,
                                            _Unwind_Context* context)
  {
    printf("Personality function begin!\n");
    printf("\tversiov = %d, action = %d \n", version, actions);
    printf("\texceptionClass = %p\n", reinterpret_cast<void*>(exceptionClass));
    printf("\tunwind_exception = %p\n", unwind_exception);
    printf("\tcontext = %p\n", context);

    _Unwind_Reason_Code res;
    if (actions &_UA_SEARCH_PHASE) {
      printf("\tphase: _UA_SEARCH_PHASE\n");
      res = _URC_HANDLER_FOUND;
    } else if (actions & _UA_CLEANUP_PHASE) {
      printf("\tphase: _UA_CLEANUP_PHASE\n");
//      res = parse_LSDA(context, actions, unwind_exception);
      res = LSDA_llvm::scan_eh_tab( actions, context);
    } else {
      printf("\tPersonality function, error\n");
      res = _URC_FATAL_PHASE1_ERROR;
    }

    printf("Personality function end!\n");
    return res;
  }

  void __cxa_begin_catch() {
    printf("__cxa_begin_catch\n");
  }

  void __cxa_end_catch() {
    printf("__cxa_end_catch\n");
  }
} // extern "C"