
#include <cstdio>
#include <cstdlib>
#include <typeinfo>
#include <unwind.h>

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

#define EXCEPTION_BUFF_SIZE 255
char exception_buff[EXCEPTION_BUFF_SIZE];

extern "C" {
  typedef void (*unexpected_handler)(void);
  typedef void (*terminate_handler)(void);

  struct __cxa_exception {
    std::type_info* exceptionType;

    void (* exceptionDestructor)(void*);

    unexpected_handler unexpectedHandler;
    terminate_handler terminateHandler;
    __cxa_exception* nextException;

    int handlerCount;
    int handlerSwitchValue;
    const char* actionRecord;
    const char* languageSpecificData;
    void* catchTemp;
    void* adjustedPtr;

    _Unwind_Exception unwindHeader;
  };

  void* __cxa_allocate_exception(size_t thrown_size) {
    printf("alloc ex %lu\n", thrown_size);
    if (thrown_size > EXCEPTION_BUFF_SIZE) {
      printf("Exception too big");
    }
    return &exception_buff;
  }

  void __cxa_free_exception(void* thrown_exception);

  void __cxa_throw(void* thrown_exception,
                   struct std::type_info* tinfo,
                   void (* dest)(void*))
  {
    printf("__cxa_throw called\n");

    __cxa_exception *header = ((__cxa_exception *) thrown_exception - 1);
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
    printf("\texceptionClass = %p\n", (void *) exceptionClass);
    printf("\tunwind_exception = %p\n", unwind_exception);
    printf("\tcontext = %p\n", context);
    _Unwind_Reason_Code res;
    if (actions &_UA_SEARCH_PHASE) {
      printf("\tphase: _UA_SEARCH_PHASE\n");
      res = _URC_HANDLER_FOUND;
    } else if (actions & _UA_CLEANUP_PHASE) {
      printf("\tphase: _UA_CLEANUP_PHASE\n");
      res = _URC_INSTALL_CONTEXT;
    } else {
      printf("\tPersonality function, error\n");
      res = _URC_FATAL_PHASE1_ERROR;
    }

    auto* lsda = (const uint8_t*)_Unwind_GetLanguageSpecificData(context);
    uintptr_t ip = _Unwind_GetIP(context) - 1;
    uintptr_t funcStart = _Unwind_GetRegionStart(context);
    uintptr_t ipOffset = ip - funcStart;

    printf("\tlsda = %p\n", lsda);
    printf("\tip = %p\n", (void*) ip);
    printf("\tfuncStart = %p\n", (void*) funcStart);
    printf("\tipOffset = %p\n", (void*) ipOffset);

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