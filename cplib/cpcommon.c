/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#include "cplib.h"

#include <string.h>
#include <stdlib.h>

#define US sizeof(unsigned)

unsigned ElfHash(char *key_str)
{
  unsigned h;
  unsigned g;
  unsigned c;

  h = 0;
  while ((c = (unsigned)*key_str) != '\0')
  {
    h = (h << US) + c;
    if ((g = h & (~((unsigned)(~0) >> US))))
      h ^= g >> (US * 6);
    h &= ~g;
    key_str++;
  }
  return h;
}

void strcpy_up(char *d, const char *s)
{
  /* -- make an uppercase copy in d of string in s -- */
  for (; *s != '\0'; s++)
  {
    *d++ = (islower(*s) ? toupper(*s) : *s);
  }
  BLANK_STRING(d);
}
