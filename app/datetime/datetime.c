#include "datetime.h"

void datetime_to_string(char *datetime_str, size_t size,
                        const DateTime *datetime) {
  snprintf(datetime_str, size, "%04d-%02d-%02d %02d:%02d:%02d", datetime->year,
           datetime->month, datetime->day, datetime->hour, datetime->minute,
           datetime->second);
}
// Helper function to calculate the number of days in a month
static int days_in_month(int year, int month) {
  static const int days_per_month[] = {31, 28, 31, 30, 31, 30,
                                       31, 31, 30, 31, 30, 31};
  if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
    return 29; // February in a leap year
  }
  return days_per_month[month - 1];
}

// Helper function to convert DateTime to total minutes since epoch
static int64_t datetime_to_total_minutes(const DateTime *datetime) {
  int64_t total_minutes = 0;
  // Calculate total days from year
  for (int year = 1970; year < datetime->year; ++year) {
    total_minutes += (datetime_is_leap_year(year) ? 366 : 365) * 24 * 60;
  }
  // Calculate total days from month
  for (int month = 1; month < datetime->month; ++month) {
    total_minutes += days_in_month(datetime->year, month) * 24 * 60;
  }
  // Add days, hours, and minutes
  total_minutes += (datetime->day - 1) * 24 * 60;
  total_minutes += datetime->hour * 60;
  total_minutes += datetime->minute;
  return total_minutes;
}

int32_t calculate_time_difference_in_minutes(DateTime *start, DateTime *end) {
  int64_t start_total_minutes = datetime_to_total_minutes(start);
  int64_t end_total_minutes = datetime_to_total_minutes(end);
  return (int32_t)(end_total_minutes - start_total_minutes);
}