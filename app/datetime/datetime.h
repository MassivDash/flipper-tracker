#ifndef DATETIME_H
#define DATETIME_H
#include "../app.h"
#include <datetime/datetime.h>
#include <storage/storage.h>
void datetime_to_string(char *datetime_str, size_t size,
                        const DateTime *datetime);

int32_t calculate_time_difference_in_minutes(DateTime *start, DateTime *end);
#endif // DATETIME_H