/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#include "cpaddress.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* private methods */
static int append_field(cpstring target, cpstring source);

CpAddress *cp_address_new(void)
{
  CpAddress *address;
  address = malloc(sizeof(CpAddress));
  address->address_type = unknownaddress;
  /* standard address components */
  CALLOC_STRING(address->address_number, ADDRNUMLEN);
  CALLOC_STRING(address->pre_directional, DIRECTLEN);
  CALLOC_STRING(address->street_name, STNAMELEN);
  CALLOC_STRING(address->suffix, SUFFIXLEN);
  CALLOC_STRING(address->post_directional, DIRECTLEN);
  CALLOC_STRING(address->sec_identifier, UNITDESGLEN);
  CALLOC_STRING(address->sec_address_range, UNITRNGLEN);
  /* business address components */
  CALLOC_STRING(address->business_mailstop_code, MAILSTOPLEN);
  CALLOC_STRING(address->business_deparment, DEPTLEN);
  /* PO box address components */
  CALLOC_STRING(address->box_number, BOXNUMLEN);
  /* military address components */
  CALLOC_STRING(address->military_unit_type, MILUNITTYPELEN);
  CALLOC_STRING(address->military_unit_number, MILUNITNUMLEN);
  CALLOC_STRING(address->military_box_number, MILBOXNUMLEN);
  CALLOC_STRING(address->military_ship_name, MILSHIPNAMELEN);
  /* rural route and highway contract address components */
  CALLOC_STRING(address->rural_route_number, RRNUMLEN);
  CALLOC_STRING(address->rural_box_number, RRBOXLEN);
  CALLOC_STRING(address->highway_contract_number, HWYCONTRACTLEN);
  /* last line components */
  CALLOC_STRING(address->city, CITYLEN);
  CALLOC_STRING(address->state, STATELEN);
  CALLOC_STRING(address->zip5, ZIP5LEN);
  CALLOC_STRING(address->zip4, ZIP4LEN);

  return address;
}

void cp_address_destroy(CpAddress *address)
{
  free(address->address_number);
  free(address->pre_directional);
  free(address->street_name);
  free(address->suffix);
  free(address->post_directional);
  free(address->sec_identifier);
  free(address->sec_address_range);
  /* business address components */
  free(address->business_mailstop_code);
  free(address->business_deparment);
  /* PO box address components */
  free(address->box_number);
  /* military address components */
  free(address->military_unit_type);
  free(address->military_unit_number);
  free(address->military_box_number);
  free(address->military_ship_name);
  /* rural route and highway contract address components */
  free(address->rural_route_number);
  free(address->rural_box_number);
  free(address->highway_contract_number);
  /* last line components */
  free(address->city);
  free(address->state);
  free(address->zip5);
  free(address->zip4);

  free(address);
}

void cp_address_print(CpAddress *address)
{
  printf("\n\n-------- Address Report ----------");
  printf("\nAddress Number: %s", address->address_number);
  printf("\nStreet: %s", address->street_name);
  printf("\nSuffix: %s", address->suffix);
  printf("\nUnit: %s", address->sec_identifier);
  printf("\nRange: %s", address->sec_address_range);
  printf("\nBox Number: %s", address->box_number);
  printf("\nRR Number: %s", address->rural_route_number);
  printf("\nCity: %s", address->city);
  printf("\nState: %s", address->state);
  printf("\nZip5: %s", address->zip5);
  printf("\nZip4: %s", address->zip4);
}

void cp_address_toxml(CpAddress *address, char *xml)
{
  strcat(xml, "<address>");
  strcat(xml, "</address>");
}

void cp_address_append(CpAddress *address, int field, cpstring data)
{
  switch (field)
  {
  case HOUSE:
    append_field(address->address_number, data);
    break;
  case PREDIR:
    append_field(address->pre_directional, data);
    break;
  case POSTDIR:
    append_field(address->post_directional, data);
    break;
  case STREET:
    append_field(address->street_name, data);
    break;
  case SUFFIX:
    append_field(address->suffix, data);
    break;
  case UNIT:
    append_field(address->sec_identifier, data);
    break;
  case RANGE:
    append_field(address->sec_address_range, data);
    break;
  case POBOX:
    append_field(address->box_number, data);
    break;
  case BOX:
    append_field(address->box_number, data);
    break;
  case RROUTE:
    append_field(address->rural_route_number, data);
    break;
  case ROUTE:
    append_field(address->rural_route_number, data);
    break;
  case HWCNTRCT:
    append_field(address->highway_contract_number, data);
    break;
  case GENDELIV:
    append_field(address->highway_contract_number, data);
    break;
  case CITY:
    append_field(address->city, data);
    break;
  case STATE:
    append_field(address->state, data);
    break;
  case ZIP5:
    append_field(address->zip5, data);
    break;
  case ZIP4:
    append_field(address->zip4, data);
    break;
  case PCT:
    append_field(address->zip5, data);
    break;
  case PCH:
    append_field(address->zip4, data);
    break;
  default:
    printf("\nField %d not found in output address object.", field);
  }
}

static int append_field(cpstring target, cpstring source)
{
  if (strlen(target) > 0)
    strcat(target, " ");
  strcat(target, source);
}
