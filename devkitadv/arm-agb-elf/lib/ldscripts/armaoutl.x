/* Default linker script, for normal executables */
OUTPUT_FORMAT("a.out-arm-little")
OUTPUT_ARCH(arm)
SEARCH_DIR("/tmp/devkitadv-r5-beta-3/arm-aout/lib");
SECTIONS
{
  .text   0x8000 :
  {
    CREATE_OBJECT_SYMBOLS
    __stext_ = .;
    *(.text)
    _etext = ALIGN(32768);
    __etext = ALIGN(32768);
  }
  .data ALIGN(32768) :
  {
    __sdata_ = .;
    *(.data)
    CONSTRUCTORS
    _edata  =  ALIGN(32768);
    __edata  =  ALIGN(32768);
  }
  .bss ALIGN(32768) :
  {
    __bss_start = .;
   *(.bss)
   *(COMMON)
   _end = ALIGN(4) ;
   __end = ALIGN(4) ;
  }
}
