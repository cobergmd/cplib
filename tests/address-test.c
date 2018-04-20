/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "cplib/cpaddress.h"

#include <assert.h>
#include <string.h>

int cp_address_test(int argc, char *argv[])
{
  const char *city = "Pasadena";
  const char *state = "CA";
  const char *zip_code = "51122";
  const char *zip4 = "2001";
  const char *house_number = "1445";
  const char *street_name = "Oak Ln";
  const char *sec_designator = "";
  const char *sec_number = "";
  const char *firm_name = "";

  CpAddress *address;

  address = cp_address_new();

  /*	address->house_number = house_number;
	address->street_name = street_name;
	address->sec_designator = sec_designator;
	address->sec_number = sec_number;
	address->firm_name = firm_name;

	assert(strcmp(address->house_number, house_number) == 0);
	assert(strcmp(address->street_name, street_name) == 0);
	assert(strcmp(address->sec_designator, sec_designator) == 0);
	assert(strcmp(address->sec_number, sec_number) == 0);
	assert(strcmp(address->firm_name, firm_name) == 0);
  */
  cp_address_destroy(address);
}