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

int cp_parser_test(int argc, char *argv[])
{
  /* time the test run */
  clock_t start = clock();

  /* load the parser */
  cp_parser_init();

  /* typical */
  parse("1500 leaf lane\npasadena ca 91122-1200");
  parse("140 leaf lane\npasadena ca");
  parse("1042 oakmont place apt 5b\nannapolis md 21403");
  parse("1042 hickory place\napt 12b\nannapolis md 21403");
  parse("2100 business place\nsuite 800\nbrookhaven, pa 19706");
  /* trouble makers */
  parse("151 st anne av\nanytown pa 12345");
  parse("123 north street\nanytown pa 12345");
  parse("345th ne south st\nanytown pa 12345");
  /* mixed order */
  parse("apt a1\n123 main st\n12345 anytown pa");
  parse("123 main st\n12345 anytown pa");
  parse("anytown pa 12345\n123 main st");
  /* all in one line */
  parse("452 pinemont rd apt 2b aston pa 19403");
  /* a piece at a time */
  parse("421 pinewood pl apt 15b");
  parse("chevy chase, md 21706");
  parse("apt a");
  parse("po box 12");
  /* po box */
  parse("p o box 123\nanytown pa 12345");
  parse("post office box 123\nanytown pa 12345");
  parse("po box 123\nanytown pa 12345");
  /* oh canada */
  parse("32 bendingroad crescent\nst catharines, on l2n 5r5");
  parse("1 masseyfield gate\nmarkham on l3r 3m8");
  parse("72 st regis cresent north\nnorth york on m3j 1z3");

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