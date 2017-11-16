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

// Functions
void parse(void);
void parse_class(void);
void parse_number(void);
void parse_function(void);
void parse_method(void);
void convert(void);

// Globals
char text[256];
struct object* result;

// Structs
struct method
{
	char* name;
	char* type;
	int is_static;
	struct object* (*entry)(struct object*);
};

struct class
{
	char* name;
	char* type;
	int method_count;
	struct method* methods[];
};

struct object
{
	struct class* class;
	void* data;
	int length;
};

struct list
{
	int count;
	struct object** items;
};

// Classes
struct class object_class;
struct class class_class;
struct class string_class;
struct class list_class;
struct class short_class;
struct class integer_class;
struct class system_class;
struct class audio_class;
struct class video_class;
struct class network_class;
struct class input_class;
struct class joystick_class;
struct class storage_class;
struct class floppydrive_class;

// Strings
static char object_class_name[] = "Object";
static char object_class_type[] = "OZone.Object";
static char object_tostring_name[] = "ToString";
static char object_getclass_name[] = "GetClass";

static char class_class_name[] = "Class";
static char class_class_type[] = "OZone.Class";
static char class_getname_name[] = "GetName";
static char class_gettype_name[] = "GetType";

static char short_class_name[] = "Short";
static char short_class_type[] = "OZone.Short";
static char short_zero_name[] = "Zero";

static char integer_class_name[] = "Integer";
static char integer_class_type[] = "OZone.Integer";
static char integer_zero_name[] = "Zero";

static char string_class_name[] = "String";
static char string_class_type[] = "OZone.String";

static char list_class_name[] = "List";
static char list_class_type[] = "OZone.List";

static char system_class_name[] = "System";
static char system_class_type[] = "OZone.System";

static char audio_class_name[] = "Audio";
static char audio_class_type[] = "OZone.Audio";

static char video_class_name[] = "Video";
static char video_class_type[] = "OZone.Video";

static char network_class_name[] = "Network";
static char network_class_type[] = "OZone.Network";

static char input_class_name[] = "Input";
static char input_class_type[] = "OZone.Input";

static char joystick_class_name[] = "Joystick";
static char joystick_class_type[] = "OZone.Input.Joystick";

static char storage_class_name[] = "Storage";
static char storage_class_type[] = "OZone.Storage";
static char storage_getfloppydrives_name[] = "GetFloppyDrives";

static char floppydrive_class_name[] = "FloppyDrive";
static char floppydrive_class_type[] = "OZone.Storage.FloppyDrive";

// Object
struct object* object_tostring(struct object* object)
{
	return 0;
}

struct method object_tostring_method = { object_tostring_name, string_class_type, 0, object_tostring };

struct object* object_getclass(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &class_class;
	result->data = object->class;

	return result;
}

struct method object_getclass_method = { object_getclass_name, class_class_type, 0, object_getclass };

struct class object_class = { object_class_name, object_class_type, 2, { &object_tostring_method, &object_getclass_method } };

struct object* class_getname(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = ((struct class*)object->data)->name;

	return result;
}

// Class
struct method class_getname_method = { class_getname_name, string_class_type, 0, class_getname };

struct object* class_gettype(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = ((struct class*)object->data)->type;

	return result;
}

struct method class_gettype_method = { class_gettype_name, string_class_type, 0, class_gettype };

struct method class_tostring_method = { object_tostring_name, string_class_type, 0, class_getname };

struct class class_class = { class_class_name, class_class_type, 4, { &class_tostring_method, &object_getclass_method, &class_getname_method, &class_gettype_method } };

// String
struct object* string_tostring(struct object* object)
{
	return object;
}

struct method string_tostring_method = { object_tostring_name, string_class_type, 0, string_tostring };

static char string_length_name[] = "Length";

struct object* string_length(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &integer_class;
	result->data = malloc(sizeof(int));

	result->data = strlen(object->data);

	return result;
}

struct method string_length_method = { string_length_name, integer_class_type, 0, string_length };

struct class string_class = { string_class_name, string_class_type, 3,{ &string_tostring_method, &string_length_method, &object_getclass_method } };

// List
struct object* list_tostring(struct object* object)
{
	static length;
	static index;
	static index2;
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

struct method list_tostring_method = { object_tostring_name, string_class_type, 0, list_tostring };

static char list_count_name[] = "Count";

struct object* list_count(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &integer_class;
	result->data = malloc(sizeof(int));

	result->data = ((struct list*)object->data)->count;

	return result;
}

struct method list_count_method = { list_count_name, integer_class_type, 0, list_count };

static char list_first_name[] = "First";

struct object* list_first(struct object* object)
{
	struct object* result = ((struct list*)object->data)->items[0];

	return result;
}

struct method list_first_method = { list_first_name, object_class_type, 0, list_first };

struct class list_class = { list_class_name, list_class_type, 4, { &list_tostring_method, &list_count_method, &list_first_method, &object_getclass_method } };

// Short
struct object* short_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	itoa(object->data, result->data, 10);

	return result;
}

struct method short_tostring_method = { object_tostring_name, string_class_type, 0, short_tostring };

struct object* short_zero(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &short_class;
	result->data = malloc(sizeof(int));

	result->data = 0;

	return result;
}

struct method short_zero_method = { short_zero_name, short_class_type, 1, short_zero };

struct class short_class = { short_class_name, short_class_type, 3, { &short_tostring_method, &object_getclass_method, &short_zero_method } };

// Integer
struct object* integer_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct method integer_tostring_method = { object_tostring_name, string_class_type, 0, integer_tostring };

struct object* integer_zero(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &integer_class;
	result->data = malloc(sizeof(long));

	result->data = 0L;

	return result;
}

struct method integer_zero_method = { integer_zero_name, integer_class_type, 1, integer_zero };

struct class integer_class = { integer_class_name, integer_class_type, 3,{ &integer_tostring_method, &object_getclass_method, &integer_zero_method } };

// System
struct object* system_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct method system_tostring_method = { object_tostring_name, string_class_type, 0, system_tostring };

struct object* system_zero(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &system_class;
	result->data = malloc(sizeof(long));

	result->data = 0L;

	return result;
}

struct method system_zero_method = { integer_zero_name, system_class_type, 1, system_zero };

struct class system_class = { system_class_name, system_class_type, 3,{ &system_tostring_method, &object_getclass_method, &system_zero_method } };

// Audio
struct object* audio_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct method audio_tostring_method = { object_tostring_name, string_class_type, 0, audio_tostring };

struct object* audio_zero(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &audio_class;
	result->data = malloc(sizeof(long));

	result->data = 0L;

	return result;
}

struct method audio_zero_method = { integer_zero_name, audio_class_type, 1, audio_zero };

struct class audio_class = { audio_class_name, audio_class_type, 3,{ &audio_tostring_method, &object_getclass_method, &audio_zero_method } };

// Video
struct object* video_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct method video_tostring_method = { object_tostring_name, string_class_type, 0, video_tostring };

struct object* video_zero(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &video_class;
	result->data = malloc(sizeof(long));

	result->data = 0L;

	return result;
}

struct method video_zero_method = { integer_zero_name, video_class_type, 1, video_zero };

struct class video_class = { video_class_name, video_class_type, 3,{ &video_tostring_method, &object_getclass_method, &video_zero_method } };

// Network
struct object* network_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct method network_tostring_method = { object_tostring_name, string_class_type, 0, network_tostring };

struct object* network_zero(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &network_class;
	result->data = malloc(sizeof(long));

	result->data = 0L;

	return result;
}

struct method network_zero_method = { integer_zero_name, network_class_type, 1, network_zero };

struct class network_class = { network_class_name, network_class_type, 3,{ &network_tostring_method, &object_getclass_method, &network_zero_method } };

// Input
struct object* input_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct method input_tostring_method = { object_tostring_name, string_class_type, 0, input_tostring };

char input_getjoysticks_name[] = "GetJoysticks";

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

struct method input_getjoysticks_method = { input_getjoysticks_name, list_class_type, 1, input_getjoysticks };

struct class input_class = { input_class_name, input_class_type, 3,{ &input_tostring_method, &object_getclass_method, &input_getjoysticks_method } };

// Joystick
char joystick_name[] = "Joystick";

struct object* joystick_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = joystick_name;

	return result;
}

struct method joystick_tostring_method = { object_tostring_name, string_class_type, 0, joystick_tostring };

char joystick_getstate_name[] = "GetState";

struct object* joystick_getstate(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &short_class;
	result->data = joy_read((int)object->data);

	return result;
}

struct method joystick_getstate_method = { joystick_getstate_name, short_class_type, 0, joystick_getstate };

struct class joystick_class = { joystick_class_name, joystick_class_type, 3,{ &joystick_tostring_method, &object_getclass_method, &joystick_getstate_method } };

// Storage
struct object* storage_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	ltoa(object->data, result->data, 10);

	return result;
}

struct method storage_tostring_method = { object_tostring_name, string_class_type, 0, storage_tostring };

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

struct method storage_getdrives_method = { storage_getfloppydrives_name, storage_class_type, 1, storage_getfloppydrives };

struct class storage_class = { storage_class_name, storage_class_type, 3,{ &storage_tostring_method, &object_getclass_method, &storage_getdrives_method } };

// Floppy Drive
char floppydrive_name[] = "Floppy Drive";

struct object* floppydrive_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = floppydrive_name;

	return result;
}

struct method floppydrive_tostring_method = { object_tostring_name, string_class_type, 0, floppydrive_tostring };

char floppydrive_getdevicenumber_name[] = "GetDeviceNumber";

struct object* floppydrive_getdevicenumber(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &short_class;
	result->data = (int)object->data;

	return result;
}

struct method floppydrive_getdevicenumber_method = { floppydrive_getdevicenumber_name, short_class_type, 0, floppydrive_getdevicenumber };

char floppydrive_getfiles_name[] = "GetFiles";

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

struct method floppydrive_getfiles_method = { floppydrive_getfiles_name, list_class_type, 0, floppydrive_getfiles };

struct class floppydrive_class = { floppydrive_class_name, floppydrive_class_type, 4, { &floppydrive_tostring_method, &object_getclass_method, &floppydrive_getdevicenumber_method, &floppydrive_getfiles_method } };

// Classes
const int class_count = 14;

struct class* classes[] = { &object_class, &class_class, &string_class, &list_class, &short_class, &integer_class, &system_class, &audio_class, &video_class, &network_class, &input_class, &joystick_class, &storage_class, &floppydrive_class };

int main(void)
{
	static int joystick1;

	textcolor(COLOR_WHITE);
	bgcolor(COLOR_BLACK);
	bordercolor(COLOR_BLACK);

	clrscr();

	joy_install(joy_static_stddrv);

	while (1)
	{
		printf(">");

		fgets(text, 256, stdin);

		parse();

		convert();

		printf("%s\n", text);
	}

	return EXIT_SUCCESS;
}

char token[256];
int token_index;
char character;
int index;
int text_length;

void parse(void)
{
	text_length = strlen(text);
	strcpy(token, "");
	index = 0;
	token_index = 0;

start:
	if (index == text_length)
		goto null;

	character = text[index++];

	if (character == '\n')
		goto null;

	if (character == ' ')
		goto start;

	if (character == '"')
		goto string;

	if (character >= '0' &&
		character <= '9')
	{
		token[token_index++] = character;
		token[token_index] = 0;

		goto number;
	}

	token[token_index++] = character;
	token[token_index] = 0;

	goto class;

	class :
		if (index == text_length)
		{
			parse_class();
			return;
		}

	character = text[index++];

	if (character == '\n')
	{
		parse_class();
		return;
	}

	if (character == '.')
	{
		parse_class();

		strcpy(token, "");
		token_index = 0;

		goto function;
	}

	if (character == '"')
		goto null;

	token[token_index++] = character;
	token[token_index] = 0;

	goto class;

object:
	if (index == text_length)
		return;

	character = text[index++];

	if (character == '\n')
		return;

	if (character == ' ')
		goto object;

	if (character == '.')
	{
		strcpy(token, "");
		token_index = 0;

		goto member;
	}

	goto null;

function:
	if (index == text_length)
	{
		parse_function();
		return;
	}

	character = text[index++];

	if (character == '\n')
	{
		parse_function();
		return;
	}

	if (character == '.')
	{
		parse_function();

		strcpy(token, "");
		token_index = 0;

		goto member;
	}

	if (character == '"')
		goto null;

	token[token_index++] = character;
	token[token_index] = 0;

	goto function;

member:
	if (index == text_length)
	{
		parse_method();
		return;
	}

	character = text[index++];

	if (character == '\n')
	{
		parse_method();
		return;
	}

	if (character == '.')
	{
		parse_method();

		strcpy(token, "");
		token_index = 0;

		goto member;
	}

	if (character == '"')
		goto null;

	token[token_index++] = character;
	token[token_index] = 0;

	goto member;

string:
	if (index == text_length)
		goto null;

	character = text[index++];

	if (character == '\n')
		goto null;

	if (character == '"')
	{
		result = malloc(sizeof(struct object));

		result->class = &string_class;
		result->data = malloc(token_index + 1);

		strcpy(result->data, token);

		goto object;
	}

	token[token_index++] = character;
	token[token_index] = 0;

	goto string;

number:
	if (index == text_length)
	{
		parse_number();
		return;
	}

	character = text[index++];

	if (character == '\n')
	{
		parse_number();
		return;
	}

	if (character == '.')
	{
		parse_number();

		strcpy(token, "");
		token_index = 0;

		goto member;
	}

	if (character >= '0' &&
		character <= '9')
	{
		token[token_index++] = character;
		token[token_index] = 0;

		goto number;
	}

	goto null;

null:
	result = 0;
	return;
}

void parse_class()
{
	int class;

	for (class = 0; class < class_count; class++)
	{
		if (strcmp(classes[class]->name, token) == 0)
		{
			result = malloc(sizeof(struct object));

			result->class = &class_class;
			result->data = classes[class];

			return;
		}
	}

	result = 0;
}

void parse_number()
{
	result = malloc(sizeof(struct object));

	result->class = &integer_class;
	result->data = malloc(sizeof(int));

	result->data = atoi(token);
}

void parse_function()
{
	int method;

	if (token_index != 0)
	{
		for (method = 0; method < ((struct class*)result->data)->method_count; method++)
		{
			if (((struct class*)result->data)->methods[method]->is_static != 0 &&
				strcmp(((struct class*)result->data)->methods[method]->name, token) == 0)
			{
				result = ((struct class*)result->data)->methods[method]->entry(result);

				return;
			}
		}
	}

	result = 0;
}

void parse_method()
{
	int method;

	if (token_index != 0)
	{
		for (method = 0; method < result->class->method_count; method++)
		{
			if (result->class->methods[method]->is_static == 0 &&
				strcmp(result->class->methods[method]->name, token) == 0)
			{
				result = result->class->methods[method]->entry(result);

				return;
			}
		}
	}

	result = 0;
}

void convert()
{
	int method;

	if (result)
	{
		for (method = 0; method < result->class->method_count; method++)
		{
			if (strcmp(result->class->methods[method]->name, object_tostring_name) == 0)
			{
				result = result->class->methods[method]->entry(result);

				strcpy(text, result->data);

				return;
			}
		}
	}

	strcpy(text, "");
}