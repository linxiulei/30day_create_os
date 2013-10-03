#include "bootpack.h"

void print_debug(char *str)
{
    struct BOOTINFO *binfo;
    binfo = (struct BOOTINFO *) 0xff0;
    boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 128, 320, 128 + 16);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 128, COL8_FFFFFF, str);
}
