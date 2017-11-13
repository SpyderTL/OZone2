#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <c64.h>
#include <device.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <cc65.h>

struct method
{
	char* name;
	char* type;
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

struct class object_class;
struct class class_class;
struct class string_class;
struct class integer_class;

static char object_class_name[] = "Object";
static char object_class_type[] = "OZone.Object";

static char class_class_name[] = "Class";
static char class_class_type[] = "OZone.Class";

static char integer_class_name[] = "Integer";
static char integer_class_type[] = "OZone.Integer";

static char string_class_name[] = "String";
static char string_class_type[] = "OZone.String";

static char object_tostring_name[] = "ToString";
static char object_getclass_name[] = "GetClass";

static char class_getname_name[] = "GetName";
static char class_gettype_name[] = "GetType";

struct object* object_tostring(struct object* object)
{
	return 0;
}

struct method object_tostring_method = { object_tostring_name, string_class_type, object_tostring };

struct object* object_getclass(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &class_class;
	result->data = object->class;

	return result;
}

struct method object_getclass_method = { object_getclass_name, class_class_type, object_getclass };

struct class object_class = { object_class_name, object_class_type, 2, { &object_tostring_method, &object_getclass_method } };

struct object* class_getname(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = ((struct class*)object->data)->name;

	return result;
}

struct method class_getname_method = { class_getname_name, string_class_type, class_getname };

struct object* class_gettype(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = ((struct class*)object->data)->type;

	return result;
}

struct method class_gettype_method = { class_gettype_name, string_class_type, class_gettype };

struct method class_tostring_method = { object_tostring_name, string_class_type, class_getname };

struct class class_class = { class_class_name, class_class_type, 4, { &class_tostring_method, &object_getclass_method, &class_getname_method, &class_gettype_method } };

struct object* string_tostring(struct object* object)
{
	return object;
}

struct method string_tostring_method = { object_tostring_name, string_class_type, string_tostring };

static char string_length_name[] = "Length";

struct object* string_length(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &integer_class;
	result->data = malloc(sizeof(int));

	result->data = strlen(object->data);

	return result;
}

struct method string_length_method = { string_length_name, integer_class_type, string_length };

struct class string_class = { string_class_name, string_class_type, 3, { &string_tostring_method, &string_length_method, &object_getclass_method } };

struct object* integer_tostring(struct object* object)
{
	struct object* result = malloc(sizeof(struct object));

	result->class = &string_class;
	result->data = malloc(16);

	itoa(object->data, result->data, 10);

	return result;
}

struct method integer_tostring_method = { object_tostring_name, string_class_type, integer_tostring };

struct class integer_class = { integer_class_name, integer_class_type, 2, { &integer_tostring_method, &object_getclass_method } };

void parse(void);
void number(void);
void execute(void);
void convert(void);

char text[256];
struct object* result;

int main(void)
{
	textcolor(COLOR_WHITE);
	bgcolor(COLOR_BLACK);
	bordercolor(COLOR_BLACK);

	clrscr();

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

member:
	if (index == text_length)
	{
		execute();
		return;
	}

	character = text[index++];

	if (character == '\n')
	{
		execute();
		return;
	}

	if (character == '.')
	{
		execute();

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
		number();
		return;
	}

	character = text[index++];

	if (character == '\n')
	{
		number();
		return;
	}

	if (character == '.')
	{
		number();

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

void number()
{
	result = malloc(sizeof(struct object));

	result->class = &integer_class;
	result->data = malloc(sizeof(int));

	result->data = atoi(token);
}

void execute()
{
	int method;

	if (token_index != 0)
	{
		for (method = 0; method < result->class->method_count; method++)
		{
			if (strcmp(result->class->methods[method]->name, token) == 0)
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