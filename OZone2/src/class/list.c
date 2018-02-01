#include <stdlib.h>
#include <string.h>

#include "list.h"

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
