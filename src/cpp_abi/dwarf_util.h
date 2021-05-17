#pragma once

#include <cstdint>
#include <typeinfo>

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
uintptr_t readULEB128(const uint8_t** data);

/// Read a sleb128 encoded value and advance pointer
/// See Variable Length Data Appendix C in:
/// @link http://dwarfstd.org/Dwarf4.pdf @unlink
/// @param data reference variable holding memory pointer to decode from
/// @returns decoded value
intptr_t readSLEB128(const uint8_t** data);

template<class AsType>
uintptr_t readPointerHelper(const uint8_t*& p);

/// Read a pointer encoded value and advance pointer
/// See Variable Length Data in:
/// @link http://dwarfstd.org/Dwarf3.pdf @unlink
/// @param data reference variable holding memory pointer to decode from
/// @param encoding dwarf encoding type
/// @returns decoded value
uintptr_t readEncodedPointer(const uint8_t** data, uint8_t encoding);


const std::type_info* get_type_info(uint64_t ttypeIndex, const uint8_t* classInfo, uint8_t ttypeEncoding);