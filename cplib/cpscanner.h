/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 *
 * cpscanner.h: Lexical scanner
 */
#ifndef _CP_SCANNER_H_
#define _CP_SCANNER_H_

#include <cplib/cplib.h>
#include <cplib/cptoken.h>

#define STATE_UNKNOWN 0
#define STATE_IGNORE 0
#define STATE_UNKNOWN 0
#define STATE_UNKNOWN 0
#define STATE_UNKNOWN 0
#define STATE_UNKNOWN 0
#define STATE_UNKNOWN 0
#define STATE_UNKNOWN 0
#define STATE_UNKNOWN 0
#define STATE_UNKNOWN 0

BEGIN_C_DECLS

typedef struct _CpScanner CpScanner;

struct _CpScanner
{
  cpstring text;
  int tokcnt;
  CpTokenList *tokens;
};

CP_API CpScanner *cp_scanner_new(void);
CP_API void cp_scanner_destroy(CpScanner *scanner);

CP_API void cp_scanner_tokenize(CpScanner *scanner, cpstring text);

END_C_DECLS

#endif /* _CP_SCANNER_H_ */
