#include <stdio.h>
#include "bootpack.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;
extern struct TIMERCTL timerctl;;

void HariMain(void)
{
    char s[40], mousebuf[128], keybuf[32];
    struct MOUSE_DEC mdec;
    struct BOOTINFO *binfo;
    int mx, my, data;
    unsigned int memtotal;
    struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
    struct SHTCTL *shtctl;
	struct SHEET *sht_back, *sht_mouse, *sht_win;
	unsigned char *buf_back, buf_mouse[256], *buf_win;
    unsigned int count = 0;

    binfo = (struct BOOTINFO *) 0xff0;

    init_gdtidt();
    init_pic();
    io_sti();
    fifo8_init(&keyfifo, 32, keybuf);
    fifo8_init(&mousefifo, 128, mousebuf);
    init_pit();
    io_out8(PIC0_IMR, 0xf8); 
    io_out8(PIC1_IMR, 0xef);

    init_keyboard();
    enable_mouse(&mdec);

    memtotal = memtest(0x00400000, 0xbfffffff);
    memman_init(memman);
    memman_free(memman, 0x00001000, 0x0009e000);
    memman_free(memman, 0x00400000, memtotal - 0x00400000);

    init_palette();


    shtctl = shtctl_init(memman, binfo->vram, 320, 200);
	sht_back  = sheet_alloc(shtctl);
	sht_mouse = sheet_alloc(shtctl);
	sht_win = sheet_alloc(shtctl);
	buf_back  = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
	buf_win  = (unsigned char *) memman_alloc_4k(memman, 160 * 68);
    init_mouse_cursor8(buf_mouse, 99);
	sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1); 
	sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99);
	sheet_setbuf(sht_win, buf_win, 160, 68, -1);

    init_screen8(buf_back, binfo->scrnx, binfo->scrny);
	make_window8(buf_win, 160, 68, "counter");
	sheet_slide(shtctl, sht_back, 0, 0);
	mx = (binfo->scrnx - 16) / 2; 
	my = (binfo->scrny - 28 - 16) / 2;
	sheet_slide(shtctl, sht_mouse, mx, my);
	sheet_slide(shtctl, sht_win, 80, 72);
	sheet_updown(shtctl, sht_back,  0);
	sheet_updown(shtctl, sht_mouse, 2);
	sheet_updown(shtctl, sht_win, 1);

    mx = (binfo->scrnx - 16) / 2; 
    my = (binfo->scrny - 28 - 16) / 2;

    sprintf(s, "memory %dMB free %dKB", memtotal/(1024 * 1024), memman_total(memman)/1024);
	putfonts8_asc(buf_back, binfo->scrnx, 0, 16, COL8_FFFFFF, s);

	sheet_refresh(shtctl, sht_back, 0, 0, binfo->scrnx, 48);
	sheet_refresh(shtctl, sht_mouse, mx, my, binfo->scrnx, 48);


    for (;;) {
        sprintf(s, "%010d", timerctl.count);
        boxfill8(buf_win, 160, COL8_C6C6C6, 40, 28, 119, 43);
        putfonts8_asc(buf_win, 160, 40, 28, COL8_000000, s);
        sheet_refresh(shtctl, sht_win, 40, 28, 120, 44);
        io_cli();
        if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0 ) {
            io_sti();
        } else {
            if (fifo8_status(&keyfifo) != 0)
            {
                data = fifo8_get(&keyfifo);
                io_sti();

                sprintf(s, "%02X", data);

	            boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
	            putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
            } else if (fifo8_status(&mousefifo) != 0)
            {
                data = fifo8_get(&mousefifo);
                io_sti();

                if (mouse_decode(&mdec, data) == 1)
                {
                    //boxfill8(binfo->vram, binfo->scrnx, COL8_008484, mx, my, mx + 15, my + 15);
                    mx += mdec.x;
                    my += mdec.y;
                    if (mx < 0)
                    {
                        mx = 0;
                    }
                    if (my < 0)
                    {
                        my = 0;
                    }
                    if (mx > binfo->scrnx - 1)
                    {
                        mx = binfo->scrnx - 1;
                    }
                    if (my > binfo->scrny - 1)
                    {
                        my = binfo->scrny - 1;
                    }
                    sprintf(s, "(%3d, %3d)", mx, my);
                    boxfill8(buf_back, binfo->scrnx, COL8_008484, 0, 0, 79, 15);
                    putfonts8_asc(buf_back, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
                    sheet_refresh(shtctl, sht_back, 0, 0, 80, 16);
                    sheet_slide(shtctl, sht_mouse, mx, my);
                }
            }
        }
    }
}

// vim:ts=4
