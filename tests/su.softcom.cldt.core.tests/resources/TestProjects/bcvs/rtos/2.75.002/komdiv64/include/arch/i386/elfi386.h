enum reloc_type
  {
    R_386_NONE = 0,
    R_386_32,
    R_386_PC32,
    R_386_GOT32,
    R_386_PLT32,
    R_386_COPY,
    R_386_GLOB_DAT,
    R_386_JUMP_SLOT,
    R_386_RELATIVE,
    R_386_GOTOFF,
    R_386_GOTPC,
    FIRST_INVALID_RELOC,
    LAST_INVALID_RELOC = 19,
    /* The remaining relocs are a GNU extension.  */
    R_386_16 = 20,
    R_386_PC16,
    R_386_8,
    R_386_PC8,
    R_386_max
  };

