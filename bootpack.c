#include<stdio.h>
#include "bootpack.h"

void HariMain(void)
{
    char s[40];
    struct BOOTINFO *binfo;

    binfo = (struct BOOTINFO *) 0xff0;

    init_gdtidt();
    init_pic();
    io_sti();

    init_palette();
    init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

    io_out8(PIC0_IMR, 0xf9); 
    io_out8(PIC1_IMR, 0xef);
    //putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "abc");



    for (;;) {
        io_hlt();
    }
}

// vim:ts=4
