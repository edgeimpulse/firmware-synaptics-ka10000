MEMORY
{
  RAM (rwx) : ORIGIN = (((0x20000000) - (0x30000000) + ((((((((0x30000000) + (0x00000000))+(0x00001F00))+ ( (((((0x30000000) + (0x00000000))+(0x00001F00)))%(32)) ? (32 -(((((0x30000000) + (0x00000000))+(0x00001F00)))%(32))) : 0 ) ))+(0x00000100))+ ( ((((((((0x30000000) + (0x00000000))+(0x00001F00))+ ( (((((0x30000000) + (0x00000000))+(0x00001F00)))%(32)) ? (32 -(((((0x30000000) + (0x00000000))+(0x00001F00)))%(32))) : 0 ) ))+(0x00000100)))%(256)) ? (256 -((((((((0x30000000) + (0x00000000))+(0x00001F00))+ ( (((((0x30000000) + (0x00000000))+(0x00001F00)))%(32)) ? (32 -(((((0x30000000) + (0x00000000))+(0x00001F00)))%(32))) : 0 ) ))+(0x00000100)))%(256))) : 0 ) )))), LENGTH = (0x0001E000)
  EXTRAM (rwx) : ORIGIN = 0x28000000, LENGTH = 0x40000
  SYSMEM (rw) : ORIGIN = 0x41501000, LENGTH = 0xC7000
  SYSMEM_LLE (rw) : ORIGIN = 0x41501000 + 0xC7000, LENGTH = 0x8000
  SYSMEM_LLEA (rw) : ORIGIN = 0x41501000 + 0xC7000 + 0x8000, LENGTH = 0x8000
  SYSMEM_CAPE2A (rw) : ORIGIN = 0x41501000 + 0xC7000 + 0x8000 + 0x8000, LENGTH = 0x8000
}
ENTRY(Reset_Handler_ns)
SECTIONS
{
 .ext_text : ALIGN(8)
  {
   . = ALIGN(8);
   *(.ext_text*)
  *(.text)
  *(.text*)
  *(.rodata*)
  __ext_text_end__ = .;
   . = ALIGN(8);
  } > EXTRAM
    .text : ALIGN(8)
 {
  KEEP(*(.vectors))
  __rst_hdl_start = .;
  KEEP(*(.rst_hdl))
  KEEP(*(.init))
  KEEP(*(.fini))
  *crtbegin.o(.ctors)
  *crtbegin?.o(.ctors)
  *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
  *(SORT(.ctors.*))
  *(.ctors)
   *crtbegin.o(.dtors)
   *crtbegin?.o(.dtors)
   *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
   *(SORT(.dtors.*))
   *(.dtors)
  KEEP(*(.eh_frame*))
 } > RAM
 .ARM.extab :
 {
  *(.ARM.extab* .gnu.linkonce.armextab.*)
 } > RAM
 __exidx_start = .;
 .ARM.exidx :
 {
  *(.ARM.exidx* .gnu.linkonce.armexidx.*)
 } > RAM
 __exidx_end = .;
 . = ALIGN(4);
 __etext = .;
 .data : AT (__etext)
 {
  . = ALIGN(4);
  __data_start__ = .;
  *(vtable)
  *(.data*)
  . = ALIGN(4);
  PROVIDE_HIDDEN (__preinit_array_start = .);
  KEEP(*(.preinit_array))
  PROVIDE_HIDDEN (__preinit_array_end = .);
  . = ALIGN(4);
  PROVIDE_HIDDEN (__init_array_start = .);
  KEEP(*(SORT(.init_array.*)))
  KEEP(*(.init_array))
  PROVIDE_HIDDEN (__init_array_end = .);
  . = ALIGN(4);
  PROVIDE_HIDDEN (__fini_array_start = .);
  KEEP(*(SORT(.fini_array.*)))
  KEEP(*(.fini_array))
  PROVIDE_HIDDEN (__fini_array_end = .);
  KEEP(*(.jcr*))
  . = ALIGN(4);
  __data_end__ = .;
 } > RAM
 .reclaim : ALIGN(8)
 {
  . = ALIGN(8);
  __reclaim_start__ = .;
  . = ALIGN(8);
  . = __reclaim_start__ + 0x100;
  *(.reclaim*)
  . = ALIGN(8);
  __reclaim_end__ = .;
 } > RAM
 .heap (COPY):
 {
  __HeapBase = .;
  end = __HeapBase;
  KEEP(*(.heap*))
  . = __reclaim_start__ + 40960;
  __HeapLimit = .;
  . = ALIGN(4);
 } > RAM
 .bss :
 {
  . = ALIGN(4);
  __bss_start__ = .;
  *(.bss*)
  *(COMMON)
  . = ALIGN(4);
  __bss_end__ = .;
 } > RAM
 .sm_data :
 {
  __sm_data_start__ = .;
  *(.sm_data*)
  __sm_data_end__ = .;
 } > SYSMEM
 .sm_rodata :
 {
  __sm_rodata_start__ = .;
  *(.sm_rodata*)
  __sm_rodata_end__ = .;
 } > SYSMEM
 .sm_bss :
 {
  __sm_bss_start__ = .;
  *(.sm_bss*)
  __sm_bss_end__ = .;
 } > SYSMEM
 .lle_cm :
 {
 __lle_cm_start__ = .;
 . = +0x8000;
 __lle_sb_end__ = .;
 } > SYSMEM_LLE
 .llea_sb :
 {
 __llea_sb_start__ = .;
 . = +0x8000;
 __llea_sb_end__ = .;
 } > SYSMEM_LLEA
 .cape2a_sb :
 {
 __cape2a_sb_start__ = .;
 . = +0x8000;
 __cape2a_sb_end__ = .;
 } > SYSMEM_CAPE2A
 .stack_dummy (COPY):
 {
  . = ALIGN(8);
  __StackLimit = .;
  KEEP(*(.stack*))
  . = ALIGN(8);
  __StackTop = .;
 } > RAM
 PROVIDE(__stack = __StackTop);
 ASSERT(__StackLimit >= __bss_end__, "region RAM overflowed with stack")
 ASSERT(__StackTop < ((0x20000000)-(0x30000000)+(((0x30000000)+(0x00008000)+(0x00008000)+(0x00008000)+(0x00008000)))),"NS Stack running out of physical memory")
}
