#ifndef OZONE_VIDEO_H
#define OZONE_VIDEO_H

#include "class.h"

static char video_class_name[] = "Video";
static char video_class_type[] = "OZone.Video";

struct object* video_test(struct object* object);
struct object* video_tostring(struct object* object);

static struct method video_test_method = { "Test", video_class_type, 1, video_test };

static struct method video_tostring_method = { integer_zero_name, video_class_type, 0, video_tostring };

static struct class video_class = { video_class_name, video_class_type, 3, { &video_tostring_method, &object_getclass_method, &video_test_method } };

#endif
