/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#ifndef _CP_ADDRESS_H_
#define _CP_ADDRESS_H_

#include <cplib/cplib.h>

BEGIN_C_DECLS

typedef enum _CpAddressType CpAddressType;

enum _CpAddressType
{
  unknownaddress,
  residential,
  business,
  pobox,
  military,
  ruralroute,
  highwaycontract,
  generaldelivery
};

/* name and address types */
typedef struct _CpAddress CpAddress;

struct _CpAddress
{
  /* indicates type of address */
  CpAddressType address_type;
  /* standard address components */
  cpstring address_number;
  cpstring pre_directional;
  cpstring street_name;
  cpstring suffix;
  cpstring post_directional;
  cpstring sec_identifier;
  cpstring sec_address_range;
  /* business address components */
  cpstring business_mailstop_code;
  cpstring business_deparment;
  /* PO box address components */
  cpstring box_number;
  /* military address components */
  cpstring military_unit_type;
  cpstring military_unit_number;
  cpstring military_box_number;
  cpstring military_ship_name;
  /* rural route and highway contract address components */
  cpstring rural_route_number;
  cpstring rural_box_number;
  cpstring highway_contract_number;
  /* last line components */
  cpstring city;
  cpstring state;
  cpstring zip5;
  cpstring zip4;
};

/* dynamic ctor and dtor */
CP_API CpAddress *cp_address_new(void);
CP_API void cp_address_destroy(CpAddress *address);
CP_API void cp_address_print(CpAddress *address);
CP_API void cp_address_append(CpAddress *address, int field, cpstring data);
CP_API void cp_address_toxml(CpAddress *address, char *xml);

END_C_DECLS

#endif /* _CP_ADDRESS_H_ */