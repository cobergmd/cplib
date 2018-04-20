/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "cplib/cpparser.h"
#include "cplib/cpaddress.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

static void parse(const char *strin);

int cp_parser_quantity_test(int argc, char *argv[])
{
  int limit = 500000;
  int i;

  /* lets time the test run for kicks */
  clock_t start = clock();

  /* load the parser */
  cp_parser_init();

  /* parse */
  for (i = 0; i < limit; i++)
  {
    parse("1142 pinemont place\napt 2b\nannapolis md 21403");
  }

  /* release the parser */
  cp_parser_destroy();

  /* print execution time */
  printf("\n\nTime elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
}

static void parse(const char *strin)
{
  CpAddress *address;
  address = cp_address_new();
  cp_parser_parse(strin, address);
  cp_address_destroy(address);
}