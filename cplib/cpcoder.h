/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#ifndef _CP_CODER_H_
#define _CP_CODER_H_

#include <cplib/cplib.h>
#include <cplib/cpaddress.h>

BEGIN_C_DECLS

CP_API void cp_coder_init(); /* must call first */
CP_API void cp_coder_code(CpAddress *address);
CP_API void cp_coder_destroy();

END_C_DECLS

#endif /* _CP_CODER_H_ */