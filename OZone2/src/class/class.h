#ifndef OZONE_CLASS_H
#define OZONE_CLASS_H

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
static struct class object_class;
static struct class class_class;
static struct class string_class;
static struct class short_class;
static struct class integer_class;
static struct class system_class;
static struct class audio_class;
static struct class video_class;
static struct class network_class;
static struct class input_class;
static struct class joystick_class;
static struct class storage_class;
static struct class floppydrive_class;

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

static char system_class_name[] = "System";
static char system_class_type[] = "OZone.System";

static char audio_class_name[] = "Audio";
static char audio_class_type[] = "OZone.Audio";

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
struct object* object_tostring(struct object* object);
struct object* object_getclass(struct object* object);

static struct method object_tostring_method = { object_tostring_name, string_class_type, 0, object_tostring };

static struct method object_getclass_method = { object_getclass_name, class_class_type, 0, object_getclass };

static struct class object_class = { object_class_name, object_class_type, 2,{ &object_tostring_method, &object_getclass_method } };

// Class
struct object* class_getname(struct object* object);
struct object* class_gettype(struct object* object);

static struct method class_getname_method = { class_getname_name, string_class_type, 0, class_getname };

static struct method class_gettype_method = { class_gettype_name, string_class_type, 0, class_gettype };

static struct method class_tostring_method = { object_tostring_name, string_class_type, 0, class_getname };

static struct class class_class = { class_class_name, class_class_type, 4,{ &class_tostring_method, &object_getclass_method, &class_getname_method, &class_gettype_method } };

// String
struct object* string_tostring(struct object* object);
struct object* string_length(struct object* object);

static struct method string_tostring_method = { object_tostring_name, string_class_type, 0, string_tostring };

static char string_length_name[] = "Length";

static struct method string_length_method = { string_length_name, integer_class_type, 0, string_length };

static struct class string_class = { string_class_name, string_class_type, 3,{ &string_tostring_method, &string_length_method, &object_getclass_method } };

// Short
struct object* short_tostring(struct object* object);
struct object* short_zero(struct object* object);

static struct method short_tostring_method = { object_tostring_name, string_class_type, 0, short_tostring };

static struct method short_zero_method = { short_zero_name, short_class_type, 1, short_zero };

static struct class short_class = { short_class_name, short_class_type, 3,{ &short_tostring_method, &object_getclass_method, &short_zero_method } };

// Integer
struct object* integer_tostring(struct object* object);
struct object* integer_zero(struct object* object);

static struct method integer_tostring_method = { object_tostring_name, string_class_type, 0, integer_tostring };

static struct method integer_zero_method = { integer_zero_name, integer_class_type, 1, integer_zero };

static struct class integer_class = { integer_class_name, integer_class_type, 3,{ &integer_tostring_method, &object_getclass_method, &integer_zero_method } };

// System
struct object* system_tostring(struct object* object);
struct object* system_zero(struct object* object);

static struct method system_tostring_method = { object_tostring_name, string_class_type, 0, system_tostring };

static struct method system_zero_method = { integer_zero_name, system_class_type, 1, system_zero };

static struct class system_class = { system_class_name, system_class_type, 3,{ &system_tostring_method, &object_getclass_method, &system_zero_method } };

// Audio
struct object* audio_tostring(struct object* object);
struct object* audio_test(struct object* object);

static struct method audio_tostring_method = { object_tostring_name, string_class_type, 0, audio_tostring };

static char audio_test_name[] = "Test";

static struct method audio_test_method = { audio_test_name, audio_class_type, 1, audio_test };

static struct class audio_class = { audio_class_name, audio_class_type, 3,{ &audio_tostring_method, &object_getclass_method, &audio_test_method } };

// Network
struct object* network_zero(struct object* object);
struct object* network_tostring(struct object* object);

static struct method network_tostring_method = { object_tostring_name, string_class_type, 0, network_tostring };

static struct method network_zero_method = { integer_zero_name, network_class_type, 1, network_zero };

static struct class network_class = { network_class_name, network_class_type, 3,{ &network_tostring_method, &object_getclass_method, &network_zero_method } };

// Input
struct object* input_tostring(struct object* object);
struct object* input_getjoysticks(struct object* object);

static struct method input_tostring_method = { object_tostring_name, string_class_type, 0, input_tostring };

static char input_getjoysticks_name[] = "GetJoysticks";

static struct method input_getjoysticks_method = { input_getjoysticks_name, "OZone.List", 1, input_getjoysticks };

static struct class input_class = { input_class_name, input_class_type, 3,{ &input_tostring_method, &object_getclass_method, &input_getjoysticks_method } };

// Joystick
struct object* joystick_tostring(struct object* object);
struct object* joystick_getstate(struct object* object);

static struct method joystick_tostring_method = { object_tostring_name, string_class_type, 0, joystick_tostring };

static char joystick_getstate_name[] = "GetState";

static struct method joystick_getstate_method = { joystick_getstate_name, short_class_type, 0, joystick_getstate };

static struct class joystick_class = { joystick_class_name, joystick_class_type, 3,{ &joystick_tostring_method, &object_getclass_method, &joystick_getstate_method } };

// Storage
struct object* storage_tostring(struct object* object);
struct object* storage_getfloppydrives(struct object* object);

static struct method storage_tostring_method = { object_tostring_name, string_class_type, 0, storage_tostring };

static struct method storage_getdrives_method = { storage_getfloppydrives_name, storage_class_type, 1, storage_getfloppydrives };

static struct class storage_class = { storage_class_name, storage_class_type, 3,{ &storage_tostring_method, &object_getclass_method, &storage_getdrives_method } };

//Floppy Drive
struct object* floppydrive_tostring(struct object* object);
struct object* floppydrive_getdevicenumber(struct object* object);
struct object* floppydrive_getfiles(struct object* object);

static struct method floppydrive_tostring_method = { object_tostring_name, string_class_type, 0, floppydrive_tostring };

static char floppydrive_getdevicenumber_name[] = "GetDeviceNumber";

static struct method floppydrive_getdevicenumber_method = { floppydrive_getdevicenumber_name, short_class_type, 0, floppydrive_getdevicenumber };

static char floppydrive_getfiles_name[] = "GetFiles";

static struct method floppydrive_getfiles_method = { floppydrive_getfiles_name, "OZone.List", 0, floppydrive_getfiles };

static struct class floppydrive_class = { floppydrive_class_name, floppydrive_class_type, 4,{ &floppydrive_tostring_method, &object_getclass_method, &floppydrive_getdevicenumber_method, &floppydrive_getfiles_method } };

#define SID_VOICE_CTRL(Noise, Pulse,	Saw, Triangle, Test, Ring, Sync, Gate) (Noise << 7 | Pulse << 6 | Saw << 5 | Triangle << 4 | Test << 3 | Ring << 2 | Sync << 1 | Gate)
#define SID_AMP(Channel3Off, HighPass, BandPass, LowPass, Volume) (Channel3Off << 7 | HighPass << 6 | BandPass << 5 | LowPass << 4 | Volume);

#define VIC_CTRL1(interrupt_raster_line, extended_background_mode, bitmap_mode, screen_enable, row_25_enable, vertical_scroll) (interrupt_raster_line << 7 | extended_background_mode << 6 | bitmap_mode << 5 | screen_enable << 4 | row_25_enable << 3 | vertical_scroll)
#define VIC_CTRL2(multicolor_mode, column_40_enable, horizontal_scroll) (multicolor_mode << 4 | column_40_enable << 3 | horizontal_scroll)

#define VIC_ADDR(character_address, bitmap_address, screen_address) (character_address >> 10 | bitmap_address >> 10 | screen_address >> 6)
#endif
