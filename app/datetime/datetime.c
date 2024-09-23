#include "datetime.h"

void datetime_to_string(char *datetime_str, size_t size,
                        const DateTime *datetime) {
  snprintf(datetime_str, size, "%04d-%02d-%02d %02d:%02d:%02d", datetime->year,
           datetime->month, datetime->day, datetime->hour, datetime->minute,
           datetime->second);
}

void datetime_to_string_iso8601(char *buffer, size_t size,
                                const DateTime *datetime) {
  snprintf(buffer, size, "%04d-%02d-%02dT%02d:%02d:%02d", datetime->year,
           datetime->month, datetime->day, datetime->hour, datetime->minute,
           datetime->second);
}

// Parse ISO 8601 string to DateTime
void string_to_datetime_iso8601(const char *str, DateTime *datetime) {
  int year, month, day, hour, minute, second;
  sscanf(str, "%04d-%02d-%02dT%02d:%02d:%02d", &year, &month, &day, &hour,
         &minute, &second);
  datetime->year = (uint16_t)year;
  datetime->month = (uint16_t)month;
  datetime->day = (uint16_t)day;
  datetime->hour = (uint16_t)hour;
  datetime->minute = (uint16_t)minute;
  datetime->second = (uint16_t)second;
}

// Calculate the difference in minutes between two DateTimes
int32_t calculate_time_difference_in_minutes(DateTime *start, DateTime *end) {
  // Convert start and end DateTime to UNIX timestamps
  uint32_t start_timestamp = datetime_datetime_to_timestamp(start);
  uint32_t end_timestamp = datetime_datetime_to_timestamp(end);

  // Calculate the difference in seconds
  int32_t difference_seconds = end_timestamp - start_timestamp;

  // Convert the difference from seconds to minutes
  int32_t difference_minutes = difference_seconds / 60;

  return difference_minutes;
}