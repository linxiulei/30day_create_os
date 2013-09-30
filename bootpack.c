#include <stdio.h>
#include "bootpack.h"

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

void HariMain(void)
{
    char s[40], mcursor[256], keybuf[32], mousebuf[256];
    unsigned char data, mouse_dbuf[3], mouse_phase;
    struct BOOTINFO *binfo;
    int mx, my;

    binfo = (struct BOOTINFO *) 0xff0;

    init_gdtidt();
    init_pic();
    io_sti();

    fifo8_init(&keyfifo, 32, keybuf);
    fifo8_init(&mousefifo, 256, mousebuf);

    init_palette();
    init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);

    io_out8(PIC0_IMR, 0xf9); 
    io_out8(PIC1_IMR, 0xef);

    init_keyboard();
    //putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "abc");

    mx = (binfo->scrnx - 16) / 2; 
    my = (binfo->scrny - 28 - 16) / 2;
    init_mouse_cursor8(mcursor, COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);

    enable_mouse();
    mouse_phase = 0;


    for (;;) {
        io_cli();
        if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0 ) {
            io_stihlt();
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
                if (mouse_phase == 0) 
                {
                    if (data == 0xfa)
                     {
                         mouse_phase = 1;
                     }
                } else if (mouse_phase == 1)
                {
                    mouse_dbuf[0] = data;
                    mouse_phase = 2;
                } else if (mouse_phase == 2)
                {
                    mouse_dbuf[1] = data;
                    mouse_phase = 3;
                } else if (mouse_phase == 3)
                {
                    mouse_dbuf[2] = data;
                    mouse_phase = 1;

                    sprintf(s, "%02X %02X %02X", mouse_dbuf[0], mouse_dbuf[1], mouse_dbuf[2]);

	                boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 8 * 8 - 1, 31);
	                putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
                }
            }
        }
    }
}

#define PORT_KEYDAT              0x0060
#define PORT_KEYSTA              0x0064
#define PORT_KEYCMD              0x0064
#define KEYSTA_SEND_NOTREADY     0x02
#define KEYCMD_WRITE_MODE        0x0060
#define KBC_MODE                 0x47

void wait_KBC_sendready(void)
{
    for (;;) {
        if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
            break;
        }
    }
    return;
}

void init_keyboard(void)
{
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, KBC_MODE);
    return;
}

#define KEYCMD_SENDTO_MOUSE         0xd4
#define MOUSECMD_ENABLE             0xf4

void enable_mouse(void)
{
    wait_KBC_sendready();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_KBC_sendready();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
    return;
}


// vim:ts=4
