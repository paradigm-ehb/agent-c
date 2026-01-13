#include "base.h"

/**
 * Helper function to parse strings to int using ascii codes
 * */
u64
parse_u64(char *buf, size_t len)
{
  u64 value = 0;

  for (
    size_t buffer_idx = 0;
    buffer_idx < len;
    ++buffer_idx)
  {
    char c = buf[buffer_idx];
    if (c < '0' || c > '9')
    {
      break;
    }
    value = value * 10 + (c - '0');
  }

  return value;
}

/*
 * is_numeric - Check if a string contains only digits
 * @s: String to check
 *
 * Return: 1 if string contains only numeric characters, 0 otherwise
 */
b8
is_numeric(const char *s)
{
  for (; *s; ++s)
  {
    if (*s < '0' || *s > '9')
    {
      return 0;
    }
  }
  return 1;
}
