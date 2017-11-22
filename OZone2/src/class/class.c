#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <c64.h>
#include <device.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <cc65.h>
#include <joystick.h>

#include "class.h"

// Object
struct object* object_tostring(struct object* object)
{
	return 0;
}

struct object* object_getclass(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &class_class;
	result->data = object->class;

	return result;
}

struct object* class_getname(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = ((struct class*)object->data)->name;

	return result;
}

// Class
struct object* class_gettype(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = ((struct class*)object->data)->type;

	return result;
}

// String
struct object* string_tostring(struct object* object)
{
	return object;
}

struct object* string_length(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &integer_class;
	result->data = malloc(sizeof(int));

	result->data = strlen(object->data);

	return result;
}

// List
struct object* list_tostring(struct object* object)
{
	static int length;
	static int index;
	static int index2;
	static struct object* item;
	static struct object* string;

	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(sizeof(char) * 256);

	strcpy(result->data, "");

	length = 0;

	for (index = 0; index < ((struct list*)object->data)->count; index++)
	{
		item = ((struct list*)object->data)->items[index];

		for (index2 = 0; index2 < item->class->method_count; index2++)
		{
			if (item->class->methods[index2]->is_static == 0 &&
				strcmp(item->class->methods[index2]->name, object_tostring_name) == 0)
			{
				string = item->class->methods[index2]->entry(item);

				if (index != 0)
					strcat(result->data, "\r\n");

				strcat(result->data, string->data);
			}
		}
	}

	return result;
}

struct object* list_count(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &integer_class;
	result->data = malloc(sizeof(int));

	result->data = ((struct list*)object->data)->count;

	return result;
}

struct object* list_first(struct object* object)
{
	struct object* result = ((struct list*)object->data)->items[0];

	return result;
}

// Short
struct object* short_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	itoa(object->data, result->data, 10);

	return result;
}

struct object* short_zero(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &short_class;
	result->data = malloc(sizeof(int));

	result->data = 0;

	return result;
}

// Integer
struct object* integer_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct object* integer_zero(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &integer_class;
	result->data = malloc(sizeof(long));

	result->data = 0L;

	return result;
}

// System
struct object* system_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct object* system_zero(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &system_class;
	result->data = malloc(sizeof(long));

	result->data = 0L;

	return result;
}

// Audio
struct object* audio_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct object* audio_test(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	SID.v1.freq = 0x1d1e;
	SID.v1.ad = 0x00;
	SID.v1.pw = 0x0800;
	SID.v1.sr = 0xf0;

	SID.amp = SID_AMP(0, 0, 0, 0, 0x8);

	SID.v1.ctrl = SID_VOICE_CTRL(0, 0, 0, 1, 0, 0, 0, 1);

	result = 0;

	return result;
}

// Video
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

struct object* video_testsprites(struct object* object)
{
	static int index;
	static int address = 0x0400;

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

// Network
struct object* network_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct object* network_zero(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &network_class;
	result->data = malloc(sizeof(long));

	result->data = 0L;

	return result;
}

// Input
struct object* input_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct object* input_getjoysticks(struct object* object)
{
	static struct object* item;

	struct object* result = malloc(sizeof(struct object));

	result->class = &list_class;
	result->data = malloc(sizeof(struct list));

	((struct list*)result->data)->items = malloc(sizeof(void*) * 2);

	item = malloc(sizeof(struct object));

	item->class = &joystick_class;
	item->data = malloc(sizeof(int));

	item->data = JOY_1;

	((struct list*)result->data)->items[0] = item;

	item = malloc(sizeof(struct object));

	item->class = &joystick_class;
	item->data = malloc(sizeof(int));

	item->data = JOY_2;

	((struct list*)result->data)->items[1] = item;

	((struct list*)result->data)->count = 2;

	return result;
}

// Joystick
char joystick_name[] = "Joystick";

struct object* joystick_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = joystick_name;

	return result;
}

struct object* joystick_getstate(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &short_class;
	result->data = joy_read((int)object->data);

	return result;
}

// Storage
struct object* storage_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct object* storage_getfloppydrives(struct object* object)
{
	static int device;
	static int count;
	static struct object* item;

	struct object* result = malloc(sizeof(struct object));

	result->class = &list_class;
	result->data = malloc(sizeof(struct list));

	((struct list*)result->data)->items = malloc(sizeof(void*) * 8);

	count = 0;

	device = getfirstdevice();

	while (device != INVALID_DEVICE)
	{
		item = malloc(sizeof(struct object));
		item->class = &floppydrive_class;
		item->data = malloc(sizeof(int));

		item->data = device;

		((struct list*)result->data)->items[count] = item;

		count++;

		device = getnextdevice(device);
	}

	((struct list*)result->data)->count = count;

	return result;
}

// Floppy Drive
char floppydrive_name[] = "Floppy Drive";

struct object* floppydrive_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = floppydrive_name;

	return result;
}

struct object* floppydrive_getdevicenumber(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &short_class;
	result->data = (int)object->data;

	return result;
}

struct object* floppydrive_getfiles(struct object* object)
{
	static int device;
	static int count;
	static struct object* item;
	static char filename[FILENAME_MAX];
	static DIR* directory;
	static struct dirent* entry;

	struct object* result = malloc(sizeof(struct object));

	result->class = &list_class;
	result->data = malloc(sizeof(struct list));

	((struct list*)result->data)->items = malloc(sizeof(void*) * 32);

	count = 0;

	device = (int)object->data;

	itoa(filename, device, 10);

	chdir(filename);

	directory = opendir(".");

	while (entry = readdir(directory))
	{
		item = malloc(sizeof(struct object));
		item->class = &string_class;
		item->data = malloc(strlen(entry->d_name) + 1);

		strcpy(item->data, entry->d_name);

		((struct list*)result->data)->items[count] = item;

		count++;
	}

	((struct list*)result->data)->count = count;

	return result;
}
