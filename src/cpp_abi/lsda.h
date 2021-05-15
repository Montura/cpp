#pragma once

namespace LSDA {
//  https://itanium-cxx-abi.github.io/cxx-abi/exceptions.pdf

  /**
 * The LSDA is a read only place in memory; we'll create a typedef for
 * this to avoid a const mess later on; LSDA_ptr refers to readonly and
 * &LSDA_ptr will be a non-const pointer to a const place in memory
 */
  typedef const uint8_t* LSDA_ptr;
  typedef const void* LSDA_type_info;

  struct LSDA_Header {
    /**
     * Read the LSDA table into a struct; advances the lsda pointer
     * as many bytes as read
     */
    explicit LSDA_Header(LSDA_ptr* lsda) {
      LSDA_ptr header = *lsda;

      // Copy the LSDA fields
      start_encoding = header[0];
      type_encoding = header[1];
      type_table_offset = header[2];

      printf("\tLSDA header = %p\n", &header);
      printf("\t\theader->start_encoding = %u\n", start_encoding);
      printf("\t\theader->type_encoding = %u\n", type_encoding);
      printf("\t\theader->call_site_table_length = %u\n", type_table_offset);

      // Advance the lsda pointer
      *lsda = header + sizeof(LSDA_Header);
    }

    uint8_t start_encoding;
    uint8_t type_encoding;

    // This is the offset, from the end of the header, to the types table
    uint8_t type_table_offset;
  };

  struct LSDA_Call_Site_Header {
    explicit LSDA_Call_Site_Header(LSDA_ptr *lsda) {
      LSDA_ptr read_ptr = *lsda;

      // Copy the LSDA call site header fields
      encoding = read_ptr[0];
      length = read_ptr[1];

      printf("\tLSDA Call Site header = %p\n", read_ptr);
      printf("\t\theader->encoding = %u\n", encoding);
      printf("\t\theader->length = %u\n", length);

      // Advance the lsda pointer
      *lsda = read_ptr + sizeof(LSDA_Call_Site_Header);
    }

    uint8_t encoding;
    uint8_t length;
  };

  struct LSDA_Call_Site {
    explicit LSDA_Call_Site(LSDA_ptr * lsda) {
      if (lsda) {
        LSDA_ptr cs_entry = *lsda;

        // Copy the LSDA call site fields
        start = cs_entry[0];
        len = cs_entry[1];
        lp = cs_entry[2];
        action = cs_entry[3];

        printf("\tFound a CS:\n");
        printf("\t\tcs_start: %i\n", start);
        printf("\t\tcs_len: %i\n", len);
        printf("\t\tcs_lp: %i\n", lp);
        printf("\t\tcs_action: %i\n", action);

        // Advance the lsda pointer
        *lsda = cs_entry + sizeof(LSDA_Call_Site);
      }
    }
    // Note start, len and lp would be void*'s, but they are actually relative addresses:
    //  - start and lp are relative to the start of the function
    //  - len is relative to start

    uint8_t start;   // the offset in the function which points to a "throw"
    uint8_t len;     // the length of the block that might throw
    uint8_t lp;      // landing pad offset
    uint8_t action;  // the offset in the action table + 1 (0 means no action). Used to run destructors
  };

  struct LSDA_Call_Site_Table {
    // A pointer to the start of the call site table
    LSDA_ptr const cs_table_start;

    // A pointer to the end of the call site table
    LSDA_ptr const cs_table_end;

    explicit LSDA_Call_Site_Table(LSDA_ptr raw_lsda, uintptr_t call_site_table_length) :
      cs_table_start(raw_lsda),
      cs_table_end(raw_lsda + call_site_table_length)
    {
      cs_entry_ptr = cs_table_start;
    }

    const LSDA_Call_Site* next_call_site_entry() {
      if (cs_entry_ptr >= cs_table_end) {
        return nullptr;
      }

      // Copy the call site table and advance the cursor by sizeof(LSDA_Call_Site).
      // We need to copy the struct here because there might be alignment issues otherwise
      cs_entry = LSDA_Call_Site(&cs_entry_ptr);

      return &cs_entry;
    };

  private:
    LSDA_Call_Site cs_entry {nullptr };
    LSDA_ptr cs_entry_ptr;
  };

  struct LSDA {
    LSDA_Header const header;

    // The types_table_start holds all the types this stack frame could handle
    // - This table will hold pointers to struct type_info so this is actually a pointer to a list of pointers
    LSDA_type_info* const types_table_start;

    // With the call site header we can calculate the length of the call site table
    LSDA_Call_Site_Header const cs_header;

    LSDA_Call_Site_Table cs_table;

    // A pointer to the start of the action table, where an action is
    // defined for each call site
    LSDA_ptr const action_table_start;

    explicit LSDA(LSDA_ptr raw_lsda) :
      // Read LSDA header, advance the ptr
      header(&raw_lsda),
      // Read Type Info Table
      //  !!! Actually this ptr points to the end of the table,
      //  but since the action index will hold a negative index for this table we assume it's the beginning
      types_table_start((const void**)(raw_lsda + header.type_table_offset)),
      // Read LSDA call site header, advance the ptr
      cs_header(&raw_lsda),
      // Read the call site table, it starts immediately after the Call Site header
      cs_table(raw_lsda, cs_header.length),
      // Read the action table, it starts immediately after the Call Site Table
      action_table_start(cs_table.cs_table_end)
    {}

  };
}