/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#ifndef _CP_NAME_H_
#define _CP_NAME_H_

#include <cplib/cplib.h>

BEGIN_C_DECLS

/* name and address types */
typedef struct _CpName CpName;

struct _CpName
{
  cpstring prefix;
  cpstring first;
  cpstring middle;
  cpstring last;
  cpstring suffix;
  cpstring nickname;
};

/* dynamic ctor and dtor */
CpName *cp_name_new(void);
void cp_name_destroy(CpName *name);

/* methods */
void cp_name_parse(CpName *name, cpstring name_string);
void cp_name_parse_firm();

END_C_DECLS

#endif /* _CP_NAME_H_ */