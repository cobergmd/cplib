/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 *
 * cpfieldlist.h: Token linked list
 */

#ifndef _CP_TOKEN_H_
#define _CP_TOKEN_H_

#include <cplib/cplib.h>

#define MAXFLDLEN 200

BEGIN_C_DECLS

typedef struct _CpTokenList CpTokenList;
typedef struct _CpTokenCell CpTokenCell;
typedef struct _CpToken CpToken;
typedef struct _CpTokenType CpTokenType;

struct _CpTokenType
{
  int type;
  cpstring data; /* standardized data from dictionary match or orig token value */
  int len;       /* token count for phrases - 1 for single words, etc. */
};

struct _CpTokenList
{
  CpTokenCell *first;
  CpTokenCell *last;
};

struct _CpToken
{
  cpstring data; /* original parsed data */
  int length;    /* char length */
  int position;
  int line;
  int term;
  CpTokenType *types[MAXTYPES];
  int typecnt;
};

struct _CpTokenCell
{
  CpToken token;
  CpTokenCell *next;
  CpTokenCell *prev;
};

/* dynamic ctor and dtor */
CP_API CpTokenList *cp_tokenlist_new();
CP_API CpToken *cp_token_new(cpstring data, int len, int pos, int line, int type);
CP_API CpTokenType *cp_tokentype_new();
CP_API void cp_token_destroy(CpToken *token);
CP_API void cp_tokenlist_destroy(CpTokenList *list);
CP_API void cp_tokentype_destroy(CpTokenType *def);

/* methods */
CP_API cpbool cp_tokenlist_append(CpTokenList *list, CpToken *token);
CP_API void cp_tokenlist_print(CpTokenList *list);
CP_API CpToken *cp_tokenlist_get(CpTokenList *list, int idx);
CP_API void cp_token_add_type(CpToken *token, int type, cpstring data, int len);

END_C_DECLS

#endif /* _CP_TOKEN_H_ */