#ifndef OZONE_LIST_H
#define OZONE_LIST_H

#include "class.h"

static char list_class_name[] = "List";
static char list_class_type[] = "OZone.List";

struct object* list_tostring(struct object* object);
struct object* list_count(struct object* object);
struct object* list_first(struct object* object);

static struct method list_tostring_method = { object_tostring_name, string_class_type, 0, list_tostring };

static char list_count_name[] = "Count";

static struct method list_count_method = { list_count_name, integer_class_type, 0, list_count };

static char list_first_name[] = "First";

static struct method list_first_method = { list_first_name, object_class_type, 0, list_first };

static struct class list_class = { list_class_name, list_class_type, 4,{ &list_tostring_method, &list_count_method, &list_first_method, &object_getclass_method } };

#endif
