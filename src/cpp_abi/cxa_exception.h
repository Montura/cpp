#pragma once

namespace cxa_exception {
  #define EXCEPTION_BUFF_SIZE 255
  char exception_buff[EXCEPTION_BUFF_SIZE];

  typedef void (* unexpected_handler)(void);
  typedef void (* terminate_handler)(void);

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
}