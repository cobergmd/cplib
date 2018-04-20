/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 *
 * cpmaprules.h: In-memory list of token mapping rules
 */
#ifndef _CP_PATTERNMAP_H_
#define _CP_PATTERNMAP_H_

#include <cplib/cplib.h>

BEGIN_C_DECLS

typedef struct _CpOutputMap CpOutputMap;
typedef struct _CpRuleEntry CpRuleEntry;
typedef struct _CpPatternMap CpPatternMap;

struct _CpPatternMap
{
  CpRuleEntry *entries[HASHTABLESIZE];
};

struct _CpOutputMap
{
  cpstring map;
  int weight;
};

struct _CpRuleEntry
{
  cpstring key;
  int mapcnt;
  CpOutputMap *maps[MAXRULEMAPS];
  struct _CpRuleEntry *next;
};

CP_API CpPatternMap *cp_patternmap_new();
CP_API void cp_patternmap_destroy(CpPatternMap *map);
CP_API CpRuleEntry *cp_patternmap_lookup(CpPatternMap *map, cpstring lookup_str);

END_C_DECLS

#endif /* _CP_PATTERNMAP_H_ */
