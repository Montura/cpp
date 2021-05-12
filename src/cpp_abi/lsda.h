#pragma once

namespace LSDA {
//  https://itanium-cxx-abi.github.io/cxx-abi/exceptions.pdf

  /**
 * The LSDA is a read only place in memory; we'll create a typedef for
 * this to avoid a const mess later on; LSDA_ptr refers to readonly and
 * &LSDA_ptr will be a non-const pointer to a const place in memory
 */
  typedef const uint8_t* LSDA_ptr;

  struct LSDA_Header {
    /**
     * Read the LSDA table into a struct; advances the lsda pointer
     * as many bytes as read
     */
    explicit LSDA_Header(LSDA_ptr *lsda) {
      LSDA_ptr read_ptr = *lsda;

      // Copy the LSDA fields
      start_encoding = read_ptr[0];
      type_encoding = read_ptr[1];
      call_site_table_length = read_ptr[2];

      // Advance the lsda pointer
      *lsda = read_ptr + sizeof(LSDA_Header);
    }

    uint8_t start_encoding;
    uint8_t type_encoding;
    uint8_t call_site_table_length;
  };

  struct LSDA_Call_Site_Header {
    explicit LSDA_Call_Site_Header(LSDA_ptr *lsda) {
      LSDA_ptr read_ptr = *lsda;

      // Copy the LSDA call site header fields
      encoding = read_ptr[0];
      length = read_ptr[1];

      // Advance the lsda pointer
      *lsda = read_ptr + sizeof(LSDA_Call_Site_Header);
    }

    uint8_t encoding;
    uint8_t length;
  };

  struct LSDA_Call_Site {
    explicit LSDA_Call_Site(LSDA_ptr *lsda) {
      LSDA_ptr read_ptr = *lsda;

      // Copy the LSDA call site fields
      start = read_ptr[0];
      len = read_ptr[1];
      lp = read_ptr[2];
      action = read_ptr[3];

      // Advance the lsda pointer
      *lsda = read_ptr + sizeof(LSDA_Call_Site);
    }
    // Note start, len and lp would be void*'s, but they are actually relative addresses:
    //  - start and lp are relative to the start of the function
    //  - len is relative to start

    uint8_t start;   // the offset in the function which points to a "throw"
    uint8_t len;     // the length of the block that might throw
    uint8_t lp;      // landing pad offset
    uint8_t action;  // the offset in the action table + 1 (0 means no action). Used to run destructors
  };
}