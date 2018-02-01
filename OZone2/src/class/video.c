#include <stdlib.h>
#include <conio.h>

#include "class.h"

struct object* video_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct object* video_test(struct object* object)
{
	static unsigned char color;
	static unsigned char value;
	static unsigned char row;
	static unsigned char column;
	static unsigned short addr;
	static unsigned short addr2;

	value = 0;

	for (addr = 0xa000; addr < 0xbf40; addr += 2)
	{
		(*(unsigned char*)addr) = 0x55;
		(*(unsigned char*)(addr + 1)) = 0xaa;
	}

	color = 0x00;

	for (addr = 0x8400; addr < 0x87e8; addr++)
	{
		(*(unsigned char*)addr) = color;
	}

	value = CIA2.pra;

	value &= 0xfc;

	value |= 0x01;

	CIA2.pra = value;

	VIC.ctrl1 = VIC_CTRL1(0, 0, 1, 0, 1, 3);
	VIC.ctrl2 = VIC_CTRL2(0, 1, 0);

	VIC.addr = VIC_ADDR(0, 0x2000, 0x0400);

	VIC.ctrl1 = VIC_CTRL1(0, 0, 1, 1, 1, 3);

	while (1)
	{
		//color++;

		//value = ((color & 0xf) << 4) | ((color + 1) & 0xf);
		//value = ((color & 0xf) << 4) | ((1) & 0xf);
		//value = ((color & 0xf) << 4);
		//value = ((color & 0xf) << 4);
		//value = color;

		//for (addr = 0x8400; addr < 0x87e8; addr += 10)
		//{
		//	waitvsync();

		//	for (addr2 = 0; addr2 < 10; addr2++)
		//		(*(unsigned char*)(addr + addr2)) = value;
		//}

		value--;
		waitvsync();

		for (row = 0; row <= 0xf; row++)
		{
			for (column = 0; column <= 0xf; column++)
			{
				(*(unsigned char*)(0x8400 + (row * 40) + column)) = ((row + value) << 4) + column;
			}
		}
	}

	return 0;
}

struct object* video_test_fillscreen(struct object* object)
{
	static unsigned char color;
	static unsigned char value;
	static unsigned char row;
	static unsigned char column;
	static int addr;
	static int addr2;

	color = 0x23;

	for (addr = 0x8400; addr < 0x87e8; addr++)
	{
		(*(unsigned char*)addr) = color;
	}

	value = 0;

	for (addr = 0xa000; addr < 0xbf40; addr++)
	{
		(*(unsigned char*)addr) = value;
	}

	value = CIA2.pra;

	value &= 0xfc;

	value |= 0x01;

	CIA2.pra = value;

	VIC.ctrl1 = VIC_CTRL1(0, 0, 1, 0, 1, 3);
	VIC.ctrl2 = VIC_CTRL2(0, 1, 0);

	VIC.addr = VIC_ADDR(0, 0x2000, 0x0400);

	VIC.ctrl1 = VIC_CTRL1(0, 0, 1, 1, 1, 3);

	value = 0x00;

	while (1)
	{
		for (addr = 0xa000; addr < 0xbf40; addr += (40 * 8))
		{
			for (row = 0; row < 8; row++)
			{
				for (column = 0; column < 40; column++)
				{
					addr2 = addr + (column * 8) + row;

					(*(unsigned char*)addr2) |= 0x80;
					//waitvsync();

					(*(unsigned char*)addr2) |= 0xc0;
					//waitvsync();

					(*(unsigned char*)addr2) |= 0xe0;
					//waitvsync();

					(*(unsigned char*)addr2) |= 0xf0;
					//waitvsync();

					(*(unsigned char*)addr2) |= 0xf8;
					//waitvsync();

					(*(unsigned char*)addr2) |= 0xfc;
					//waitvsync();

					(*(unsigned char*)addr2) |= 0xfe;
					//waitvsync();

					(*(unsigned char*)addr2) |= 0xff;
					//waitvsync();
				}
			}
		}

		value ^= 0xff;
	}

	return 0;
}

struct object* video_test_fillscreen2(struct object* object)
{
	static unsigned char color;
	static unsigned char value;
	static int addr;

	color = 0x23;

	for (addr = 0x8400; addr < 0x87e8; addr++)
	{
		(*(unsigned char*)addr) = color;
	}

	value = 0;

	for (addr = 0xa000; addr < 0xbf40; addr++)
	{
		(*(unsigned char*)addr) = value;
	}

	value = CIA2.pra;

	value &= 0xfc;

	value |= 0x01;

	CIA2.pra = value;

	VIC.ctrl1 = VIC_CTRL1(0, 0, 1, 0, 1, 3);
	VIC.ctrl2 = VIC_CTRL2(0, 1, 0);

	VIC.addr = VIC_ADDR(0, 0x2000, 0x0400);

	VIC.ctrl1 = VIC_CTRL1(0, 0, 1, 1, 1, 3);

	value = 0x00;

	while (1)
	{
		for (addr = 0xa000; addr < 0xbf40; addr++)
		{
			//(*(unsigned char*)addr) = value;
			(*(unsigned char*)addr) = 0x80;
			//waitvsync();

			(*(unsigned char*)addr) = 0xc0;
			//waitvsync();

			(*(unsigned char*)addr) = 0xe0;
			//waitvsync();

			(*(unsigned char*)addr) = 0xf0;
			//waitvsync();

			(*(unsigned char*)addr) = 0xf8;
			//waitvsync();

			(*(unsigned char*)addr) = 0xfc;
			//waitvsync();

			(*(unsigned char*)addr) = 0xfe;
			//waitvsync();

			(*(unsigned char*)addr) = 0xff;
			//waitvsync();
		}

		value ^= 0xff;
	}

	return 0;
}

struct object* video_test_fillscreen3(struct object* object)
{
	static unsigned char color;
	static unsigned char value;
	static int addr;

	VIC.ctrl1 = VIC_CTRL1(0, 0, 1, 0, 1, 3);
	VIC.ctrl2 = VIC_CTRL2(0, 1, 0);

	VIC.addr = VIC_ADDR(0, 0x2000, 0x0400);

	VIC.ctrl1 = VIC_CTRL1(0, 0, 1, 1, 1, 3);

	color = 0x00;

	while (1)
	{
		value = ((color & 0xf) << 4) | (color & 0xf);

		waitvsync();

		for (addr = 0x0400; addr < 0x07e8; addr++)
		{
			(*(unsigned char*)addr) = value;
		}

		color++;
	}

	return 0;
}

struct object* video_test_sprite(struct object* object)
{
	static int index;
	static int address = 0x0200;

	address = (address + 63) & ~63;  // Align to 64 bytes

	VIC.spr0_color = COLOR_WHITE;
	//VIC.spr0_x = 23;
	//VIC.spr0_y = 50;
	VIC.spr0_x = 23 + 100;
	VIC.spr0_y = 50 + 100;

	(*(unsigned char*)0x07f8) = (unsigned char)(address / 64);

	for (index = 0; index < 63; index++)
	{
		(*(unsigned char*)address) = 0xaa;
		//(*(unsigned char*)address) = 0xff;
		address++;
	}

	VIC.spr_ena = 0x01;

	return 0;
}

struct object* video_test_backbuffer(struct object* object)
{
	static unsigned char value;
	static int addr;

	value = ' ';

	for (addr = 0x8000; addr < 0x8400; addr++)
	{
		(*(unsigned char*)addr) = value;
	}

	value = 'X';

	for (addr = 0x8400; addr < 0x8800; addr++)
	{
		(*(unsigned char*)addr) = value;
	}

	value = CIA2.pra;

	value &= 0xfc;

	value |= 0x01;

	CIA2.pra = value;

	while (1)
	{
		waitvsync();

		value = VIC.addr;

		value &= 0x0f;

		value |= 0x00;

		VIC.addr = value;

		waitvsync();

		value = VIC.addr;

		value &= 0x0f;

		value |= 0x10;

		VIC.addr = value;
	}

	return 0;
}