MEMORY
{
  out:     org=0x7FF0, len=0xffffff
  zero:    org=0, len=0x0100
  b0:      org=0x8000, len=0x8000
  b1:      org=0x8000, len=0x8000
  chr:     org=0x0000, len=0x2000
  ram:     org=0x0300, len=0x0500
}

SECTIONS
{
  text:   {*(text)} >b0 AT>out
  .dtors: { *(.dtors) } >b0 AT>out
  .ctors: { *(.ctors) } >b0 AT>out
  rodata: {*(rodata)}  >b0 AT>out
  init:   {*(init)}  >b0 AT>out
  data:   {*(data)} >ram AT>out
  /* fill program bank */
  fill: { .=.+0x10000-6-ADDR(init)-SIZEOF(init)-SIZEOF(data);} >b0 AT>out
  vectors:{
  	BYTE(0x00);BYTE(0x80);
  	BYTE(0x00);BYTE(0x80);
  	BYTE(0x00);BYTE(0x80);
  } >b0 AT>out

  zpage (NOLOAD) : {*(zpage) *(zp1) *(zp2)} >zero
  bss (NOLOAD): {*(bss)} >ram

  __DS = ADDR(data);
  __DE = ADDR(data) + SIZEOF(data);
  __DC = LOADADDR(data);

  __STACK = 0x800;

  ___heap = ADDR(bss) + SIZEOF(bss);
  ___heapend = __STACK;
}
