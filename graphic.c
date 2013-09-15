#include "bootpack.h"
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
    int x, y;
    for (y = y0; y <= y1; y++)
    {
        for (x = x0; x <= x1; x++)
        {
            vram[y * xsize + x] = c;
        }
    }
}

void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0: �� */
		0xff, 0x00, 0x00,	/*  1: ���� */
		0x00, 0xff, 0x00,	/*  2: ���� */
		0xff, 0xff, 0x00,	/*  3: ���� */
		0x00, 0x00, 0xff,	/*  4: ���� */
		0xff, 0x00, 0xff,	/*  5: ���� */
		0x00, 0xff, 0xff,	/*  6: ǳ���� */
		0xff, 0xff, 0xff,	/*  7: �� */
		0xc6, 0xc6, 0xc6,	/*  8: ���� */
		0x84, 0x00, 0x00,	/*  9: ���� */
		0x00, 0x84, 0x00,	/* 10: ���� */
		0x84, 0x84, 0x00,	/* 11: ���� */
		0x00, 0x00, 0x84,	/* 12: ���� */
		0x84, 0x00, 0x84,	/* 13: ���� */
		0x00, 0x84, 0x84,	/* 14: ǳ���� */
		0x84, 0x84, 0x84	/* 15: ���� */
	};
	set_palette(0, 15, table_rgb);
	return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
    int i, eflags;
    eflags = io_load_eflags();
    io_cli();
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++)
    {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);
    return;
}

void init_screen8(char *vram, int x, int y)
{
	boxfill8(vram, x, COL8_008484,  0,     0,      x -  1, y - 29); // ����
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28); // �������������ķָ�
	boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27); // �������������ķָ�
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1); // ������


	boxfill8(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24); // ���涼�Ǳ߿���ɫ
	boxfill8(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
	boxfill8(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
	boxfill8(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
	boxfill8(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
	boxfill8(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);

	boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);
	return;

}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
    extern char fonts[4096];
    for (; *s != 0x00; s++)
    {
        putfont8(vram, xsize, x, y, c, fonts + *s * 16);
        x += 8;
    }
}
