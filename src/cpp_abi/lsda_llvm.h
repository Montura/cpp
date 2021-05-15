#pragma once

#include <cstring>
#include "cxa_exception.h"

namespace LSDA_llvm {
  using namespace cxa_exception;

  // DWARF Constants
  enum {
    DW_EH_PE_absptr = 0x00,
    DW_EH_PE_uleb128 = 0x01,
    DW_EH_PE_udata2 = 0x02,
    DW_EH_PE_udata4 = 0x03,
    DW_EH_PE_udata8 = 0x04,
    DW_EH_PE_sleb128 = 0x09,
    DW_EH_PE_sdata2 = 0x0A,
    DW_EH_PE_sdata4 = 0x0B,
    DW_EH_PE_sdata8 = 0x0C,
    DW_EH_PE_pcrel = 0x10,
    DW_EH_PE_textrel = 0x20,
    DW_EH_PE_datarel = 0x30,
    DW_EH_PE_funcrel = 0x40,
    DW_EH_PE_aligned = 0x50,
    DW_EH_PE_indirect = 0x80,
    DW_EH_PE_omit = 0xFF
  };

  /// Read a uleb128 encoded value and advance pointer
/// See Variable Length Data Appendix C in:
/// @link http://dwarfstd.org/Dwarf4.pdf @unlink
/// @param data reference variable holding memory pointer to decode from
/// @returns decoded value
  static uintptr_t readULEB128(const uint8_t** data) {
    uintptr_t result = 0;
    uintptr_t shift = 0;
    unsigned char byte;
    const uint8_t* p = *data;
    do {
      byte = *p++;
      result |= static_cast<uintptr_t>(byte & 0x7F) << shift;
      shift += 7;
    } while (byte & 0x80);
    *data = p;
    return result;
  }

  /// Read a sleb128 encoded value and advance pointer
/// See Variable Length Data Appendix C in:
/// @link http://dwarfstd.org/Dwarf4.pdf @unlink
/// @param data reference variable holding memory pointer to decode from
/// @returns decoded value
  static intptr_t readSLEB128(const uint8_t** data) {
    uintptr_t result = 0;
    uintptr_t shift = 0;
    unsigned char byte;
    const uint8_t* p = *data;
    do {
      byte = *p++;
      result |= static_cast<uintptr_t>(byte & 0x7F) << shift;
      shift += 7;
    } while (byte & 0x80);
    *data = p;
    if ((byte & 0x40) && (shift < (sizeof(result) << 3)))
      result |= static_cast<uintptr_t>(~0) << shift;
    return static_cast<intptr_t>(result);
  }

  template<class AsType>
  uintptr_t readPointerHelper(const uint8_t*& p) {
    AsType value;
    memcpy(&value, p, sizeof(AsType));
    p += sizeof(AsType);
    return static_cast<uintptr_t>(value);
  }

  /// Read a pointer encoded value and advance pointer
/// See Variable Length Data in:
/// @link http://dwarfstd.org/Dwarf3.pdf @unlink
/// @param data reference variable holding memory pointer to decode from
/// @param encoding dwarf encoding type
/// @returns decoded value
  static uintptr_t readEncodedPointer(const uint8_t** data, uint8_t encoding) {
    uintptr_t result = 0;
    if (encoding == DW_EH_PE_omit)
      return result;
    const uint8_t* p = *data;
    // first get value
    switch (encoding & 0x0F) {
      case DW_EH_PE_absptr:
        result = readPointerHelper<uintptr_t>(p);
        break;
      case DW_EH_PE_uleb128:
        result = readULEB128(&p);
        break;
      case DW_EH_PE_sleb128:
        result = static_cast<uintptr_t>(readSLEB128(&p));
        break;
      case DW_EH_PE_udata2:
        result = readPointerHelper<uint16_t>(p);
        break;
      case DW_EH_PE_udata4:
        result = readPointerHelper<uint32_t>(p);
        break;
      case DW_EH_PE_udata8:
        result = readPointerHelper<uint64_t>(p);
        break;
      case DW_EH_PE_sdata2:
        result = readPointerHelper<int16_t>(p);
        break;
      case DW_EH_PE_sdata4:
        result = readPointerHelper<int32_t>(p);
        break;
      case DW_EH_PE_sdata8:
        result = readPointerHelper<int64_t>(p);
        break;
      default:
        // not supported
        abort();
        break;
    }
    // then add relative offset
    switch (encoding & 0x70) {
      case DW_EH_PE_absptr:
        // do nothing
        break;
      case DW_EH_PE_pcrel:
        if (result)
          result += (uintptr_t) (*data);
        break;
      case DW_EH_PE_textrel:
      case DW_EH_PE_datarel:
      case DW_EH_PE_funcrel:
      case DW_EH_PE_aligned:
      default:
        // not supported
        abort();
        break;
    }
    // then apply indirection
    if (result && (encoding & DW_EH_PE_indirect))
      result = *((uintptr_t*) result);
    *data = p;
    return result;
  }


  static const std::type_info* get_type_info(uint64_t ttypeIndex,
                                             const uint8_t* classInfo,
                                             uint8_t ttypeEncoding) {
    if (classInfo == nullptr) {
      // this should not happen.  Indicates corrupted eh_table.
      exit(255);
    }
    switch (ttypeEncoding & 0x0F) {
      case DW_EH_PE_absptr:
        ttypeIndex *= sizeof(void*);
        break;
      case DW_EH_PE_udata2:
      case DW_EH_PE_sdata2:
        ttypeIndex *= 2;
        break;
      case DW_EH_PE_udata4:
      case DW_EH_PE_sdata4:
        ttypeIndex *= 4;
        break;
      case DW_EH_PE_udata8:
      case DW_EH_PE_sdata8:
        ttypeIndex *= 8;
        break;
      default:
        // this should not happen.   Indicates corrupted eh_table.
        exit(255);
    }
    classInfo -= ttypeIndex;
    return (const std::type_info*) readEncodedPointer(&classInfo, ttypeEncoding);
  }

  static _Unwind_Reason_Code scan_eh_tab(_Unwind_Action actions, _Unwind_Context* context) {
    // Start scan by getting exception table address
    auto * lsda = (const uint8_t*) _Unwind_GetLanguageSpecificData(context);
    if (lsda == nullptr) {
      // There is no exception table
      return _URC_CONTINUE_UNWIND;
    }
    // Get the current instruction pointer and offset it before next
    // instruction in the current frame which threw the exception.
    uintptr_t ip = _Unwind_GetIP(context) - 1;
    // Get beginning current frame's code (as defined by the
    // emitted dwarf code)
    uintptr_t funcStart = _Unwind_GetRegionStart(context);
    uintptr_t ipOffset = ip - funcStart;
    const uint8_t* classInfo = nullptr;

    // Parse LSDA header.
    uint8_t lpStartEncoding = *lsda++;
    auto * lpStart = (const uint8_t*) readEncodedPointer(&lsda, lpStartEncoding);
    if (lpStart == nullptr) {
      lpStart = (const uint8_t*) funcStart;
    }
    uint8_t ttypeEncoding = *lsda++;
    if (ttypeEncoding != DW_EH_PE_omit) {
      // Calculate type info locations in emitted dwarf code which
      // were flagged by type info arguments to llvm.eh.selector
      // intrinsic
      uintptr_t classInfoOffset = readULEB128(&lsda);
      classInfo = lsda + classInfoOffset;
    }
    // Walk call-site table looking for range that
    // includes current PC.
    uint8_t callSiteEncoding = *lsda++;

    auto callSiteTableLength = static_cast<uint32_t>(readULEB128(&lsda));
    const uint8_t* callSiteTableStart = lsda;
    const uint8_t* callSiteTableEnd = callSiteTableStart + callSiteTableLength;
    const uint8_t* actionTableStart = callSiteTableEnd;
    const uint8_t* callSitePtr = callSiteTableStart;
    while (callSitePtr < callSiteTableEnd) {
      // There is one entry per call site.

      // The call sites are non-overlapping in [start, start+length)
      // The call sites are ordered in increasing value of start
      uintptr_t start = readEncodedPointer(&callSitePtr, callSiteEncoding);
      uintptr_t length = readEncodedPointer(&callSitePtr, callSiteEncoding);
      uintptr_t landingPad = readEncodedPointer(&callSitePtr, callSiteEncoding);
      uintptr_t actionEntry = readULEB128(&callSitePtr);
      if ((start <= ipOffset) && (ipOffset < (start + length))) {
        // Found the call site containing ip.
        if (landingPad == 0) {
          // No handler here
          return _URC_CONTINUE_UNWIND;
        }
        landingPad = (uintptr_t) lpStart + landingPad;

        if (actionEntry == 0) {
          // Found a cleanup
          // If this is a type 1 or type 2 search, there are no handlers
          // If this is a type 3 search, you want to install the cleanup.
          if ((actions & _UA_CLEANUP_PHASE) && !(actions & _UA_HANDLER_FRAME)) {
            return _URC_HANDLER_FOUND;
          }
          return _URC_CONTINUE_UNWIND;
        }
        // Convert 1-based byte offset into
        const uint8_t* action = actionTableStart + (actionEntry - 1);
        // Scan action entries until you find a matching handler, cleanup, or the end of action list
        while (true) {
          const uint8_t* actionRecord = action;
          int64_t ttypeIndex = readSLEB128(&action);
          if (ttypeIndex > 0) {
            // Found a catch, does it actually catch?
            // First check for catch (...)
            const std::type_info* catchType =
                get_type_info(static_cast<uint64_t>(ttypeIndex), classInfo, ttypeEncoding);
            const char* const name = catchType->name();
            printf("THE NAME of THROWN EXCEPTION IS FOUND = %s\n", name);
            // Scan next action ...
          } else if (ttypeIndex < 0) {
            // Found an exception spec.  If this is a foreign exception,
            //   it is always caught.

            // Scan next action ...
          } else { // ttypeIndex == 0
            // Found a cleanup
            // If this is a type 1 search, ignore it and continue scan
            // If this is a type 2 search, ignore it and continue scan
            // If this is a type 3 search, save state and return _URC_HANDLER_FOUND
            if ((actions & _UA_CLEANUP_PHASE) && !(actions & _UA_HANDLER_FRAME)) {
              return _URC_HANDLER_FOUND;
            }
          }
          const uint8_t* temp = action;
          int64_t actionOffset = readSLEB128(&temp);
          if (actionOffset == 0) {
            // End of action list, no matching handler or cleanup found
            return _URC_CONTINUE_UNWIND;
          }
          // Go to next action
          action += actionOffset;
        }  // there is no break out of this loop, only return
      } else if (ipOffset < start) {
        // There is no call site for this ip
        // Something bad has happened.  We should never get here.
        // Possible stack corruption.
        exit(1);
      }
    }  // there might be some tricky cases which break out of this loop

    // It is possible that no eh table entry specify how to handle
    // this exception. By spec, terminate it immediately.
    exit(1);
  }
}
