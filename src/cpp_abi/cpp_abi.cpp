
#include <cstdio>
#include <cstdlib>
#include <typeinfo>
#include <unwind.h>

#include "cxa_exception.h"
#include "lsda.h"

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

  LSDA_ptr read_lsda_header(_Unwind_Context* const context) {
    // Get the beginning of the raw LSDA
    auto lsda = reinterpret_cast<LSDA_ptr>(_Unwind_GetLanguageSpecificData(context));
    printf("\tLSDA = %p\n", lsda);

    if (lsda == 0) {
      return 0;
    }

    // Read LSDA header for the LSDA
    LSDA_Header header(&lsda);
    printf("\tLSDA header = %p\n", &header);
    printf("\t\tLSDA header->lsda_start_encoding = %u\n", header.start_encoding);
    printf("\t\tLSDA header->lsda_type_encoding = %u\n", header.type_encoding);
    printf("\t\tLSDA header->lsda_call_site_table_length = %u\n", header.call_site_table_length);
    return lsda;
  }
  
  void process_lsda_cs_table(LSDA_ptr lsda, _Unwind_Context* const context, _Unwind_Exception* unwind_exception) {
    // Read the LSDA CS header
    LSDA_Call_Site_Header cs_header(&lsda);
    printf("\tLSDA cs_header = %p\n", &cs_header);

    // Calculate where the end of the LSDA CS table is
    const LSDA_ptr lsda_cs_table_end = lsda + cs_header.length;

    // Loop through each entry in the CS table
    while (lsda < lsda_cs_table_end) {
      LSDA_Call_Site cs(&lsda);
      printf("\tFound a CS:\n");
      printf("\t\tcs_start: %i\n", cs.start);
      printf("\t\tcs_len: %i\n", cs.len);
      printf("\t\tcs_lp: %i\n", cs.lp);
      printf("\t\tcs_action: %i\n", cs.action);

      // Landing pad offset is valid
      if (cs.lp) {
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
        uintptr_t func_start = _Unwind_GetRegionStart(context);
        _Unwind_SetIP(context, func_start + cs.lp);
        break;
      }
    }
  }

  void __cxa_throw(void* thrown_exception,
                   struct std::type_info* tinfo,
                   void (* dest)(void*))
  {
    printf("__cxa_throw called\n");

    __cxa_exception *header = (reinterpret_cast<__cxa_exception *>(thrown_exception) - 1);
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

    uintptr_t ip = _Unwind_GetIP(context) - 1; // points to the IP pos where the next stack frame will be called
    uintptr_t funcStart = _Unwind_GetRegionStart(context); // points to the current stack frame (function)
    uintptr_t ipOffset = ip - funcStart;

    printf("\t\tip = %p\n", reinterpret_cast<void*>(ip));
    printf("\t\tfuncStart = %p\n", reinterpret_cast<void*>(funcStart));
    printf("\t\tipOffset = %p\n", reinterpret_cast<void*>(ipOffset));

    _Unwind_Reason_Code res;
    if (actions &_UA_SEARCH_PHASE) {
      printf("\tphase: _UA_SEARCH_PHASE\n");
      res = _URC_HANDLER_FOUND;
    } else if (actions & _UA_CLEANUP_PHASE) {
      printf("\tphase: _UA_CLEANUP_PHASE\n");
      LSDA_ptr lsda = read_lsda_header(context);
      if (!lsda) {
        return _URC_CONTINUE_UNWIND;
      }
      process_lsda_cs_table(lsda, context, unwind_exception);
      res = _URC_INSTALL_CONTEXT;
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