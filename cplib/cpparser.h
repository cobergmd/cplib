/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#ifndef _CP_PARSER_H_
#define _CP_PARSER_H_

#include <cplib/cplib.h>
#include <cplib/cptoken.h>
#include <cplib/cpaddress.h>
#include <cplib/cpscanner.h>
#include <cplib/cpdictionary.h>
#include <cplib/cppatternmap.h>

BEGIN_C_DECLS

CP_API void cp_parser_init(); /* should change to new */
CP_API void cp_parser_parse(cpstring line, CpAddress *address);
CP_API void cp_parser_destroy();

END_C_DECLS

#endif /* _CP_PARSER_H_ */