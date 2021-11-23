#include <Windows.h>
#include <winnt.h>
#include <cstdio>

// http://www.nynaeve.net/?p=113
// http://www.nynaeve.net/Code/StackWalk64.cpp

static const CHAR *RegNames[16] = { "Rax", "Rcx", "Rdx", "Rbx", "Rsp", "Rbp", "Rsi", "Rdi", "R8", "R9",
                                    "R10", "R11", "R12", "R13", "R14", "R15" };

void StackTrace64() {
  CONTEXT m_context;
  KNONVOLATILE_CONTEXT_POINTERS nonVolatileContext;
  UNWIND_HISTORY_TABLE unwindHTable;
  PRUNTIME_FUNCTION runtimeFunction;
  PVOID handlerData;
  ULONG64 establisherFrame;
  ULONG64 imageBase;

  OutputDebugStringW(L"StackTrace64: Executing stack trace...\n");

  // 1. We'll get the caller's context.
  RtlCaptureContext(&m_context);

  // 2. Initialize the (optional) unwind history table.
  RtlZeroMemory(&unwindHTable, sizeof(UNWIND_HISTORY_TABLE));

  // 3.This unwind loop intentionally skips the first call frame, as it shall
  // correspond to the call to StackTrace64, which we aren't interested in.

  for (ULONG Frame = 0; ; Frame++) {
    // Try to look up unwind metadata for the current function.
    runtimeFunction = RtlLookupFunctionEntry(m_context.Rip, &imageBase, &unwindHTable);
    RtlZeroMemory(&nonVolatileContext, sizeof(KNONVOLATILE_CONTEXT_POINTERS));

    if (!runtimeFunction) {
      // If we don't have a RUNTIME_FUNCTION, then we've encountered a leaf function.  Adjust the stack approprately.
      m_context.Rip = (ULONG64)(*(PULONG64) m_context.Rsp);
      m_context.Rsp += 8;
    } else {
      // Otherwise, call upon RtlVirtualUnwind to execute the unwind for us.
      RtlVirtualUnwind(
        UNW_FLAG_NHANDLER,
        imageBase,
        m_context.Rip,
        runtimeFunction,
        &m_context,
        &handlerData,
        &establisherFrame,
        &nonVolatileContext);
    }

    // If we reach an RIP of zero, this means that we've walked off the end of the call stack and are done.
    if (!m_context.Rip) {
      break;
    }

    // Display the context. Note that we don't bother showing the XMM context, although we have the nonvolatile portion of it.

    printf_s(
      "FRAME %02x: Rip=0x%llx Rsp=0x%llx Rbp=0x%llx\n",
      Frame, m_context.Rip, m_context.Rsp, m_context.Rsp);

    printf_s(
      "r12=0x%llx r13=0x%llx r14=0x%llx\n"
      "rdi=0x%llx rsi=0x%llx rbx=0x%llx\n"
      "rbp=0x%llx rsp=0x%llx\n",
      m_context.R12,
      m_context.R13,
      m_context.R14,
      m_context.Rdi,
      m_context.Rsi,
      m_context.Rbx,
      m_context.Rbp,
      m_context.Rsp
    );

    // If we have stack-based register stores, then display them here.

    for (ULONG i = 0; i < 16; i++) {
      if (nonVolatileContext.IntegerContext[i]) {
        printf_s(
          " -> Saved register '%s' on stack at 0x%llx (=> 0x%llx)\n",
          RegNames[i],
          nonVolatileContext.IntegerContext[i],
          *nonVolatileContext.IntegerContext[i]);
      }
    }

    printf("\n");
  }

  }

void foo() {
  StackTrace64();
}

void testStackTrace64() {
  foo();
}