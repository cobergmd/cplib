/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#ifndef _CP_DICTIONARY_H_
#define _CP_DICTIONARY_H_

#include <cplib/cplib.h>

BEGIN_C_DECLS

typedef struct _CpDictDef CpDictDef;
typedef struct _CpDictEntry CpDictEntry;
typedef struct _CpDictionary CpDictionary;

struct _CpDictionary
{
  CpDictEntry *entries[HASHTABLESIZE];
};

struct _CpDictDef
{
  cpstring phrasekey;
  cpstring std_text;
  int datatype;
};

struct _CpDictEntry
{
  cpstring wordkey;
  int defcnt;
  CpDictDef *defs[MAXDEFINITIONS];
  struct _CpDictEntry *next;
};

CP_API CpDictionary *cp_dictionary_new(void);
CP_API void cp_dictionary_destroy(CpDictionary *dictionary);
CP_API CpDictEntry *cp_dictionary_lookup(CpDictionary *dictionary, cpstring lookup_str);

END_C_DECLS

#endif /* _CP_DICTIONARY_H_ */