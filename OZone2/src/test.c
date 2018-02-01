#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <c64.h>
#include <string.h>
#include <joystick.h>
#include "class\class.h"
#include "class\video.h"
#include "class\list.h"

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

static const int class_count = 14;
static const struct class* classes[] = { &object_class, &class_class, &string_class, &list_class, &short_class, &integer_class, &system_class, &audio_class, &video_class, &network_class, &input_class, &joystick_class, &storage_class, &floppydrive_class };

int main(void)
{
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

static char token[256];
static int token_index;
static char character;
static int index;
static int text_length;

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