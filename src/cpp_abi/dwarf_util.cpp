#include <cstring>

#include "dwarf_util.h"

/// Read a uleb128 encoded value and advance pointer
/// See Variable Length Data Appendix C in:
/// @link http://dwarfstd.org/Dwarf4.pdf @unlink
/// @param data reference variable holding memory pointer to decode from
/// @returns decoded value
uintptr_t readULEB128(const uint8_t** data) {
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
intptr_t readSLEB128(const uint8_t** data) {
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
uintptr_t readEncodedPointer(const uint8_t** data, uint8_t encoding) {
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


const std::type_info* get_type_info(uint64_t ttypeIndex, const uint8_t* classInfo, uint8_t ttypeEncoding) {
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