/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#include "cpname.h"

#include <string.h>
#include <stdlib.h>

CpName *cp_name_new(void)
{
  CpName *name;
  name = malloc(sizeof(CpName));
  name->first = NULL;
  name->last = NULL;
  name->middle = NULL;
  name->nickname = NULL;
  name->prefix = NULL;
  name->suffix = NULL;
  return name;
}

void cp_name_destroy(CpName *name)
{
  free(name);
}

void cp_name_parse(CpName *name, cpstring name_string)
{
}