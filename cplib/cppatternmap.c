/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#include "cppatternmap.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* private data */
static int loaded = 0;

/* private methods */
static void init_hash_table(CpPatternMap *map);
static void load_rules(CpPatternMap *map, cpstring filename);
static unsigned create_hash(char *key_str);
static void add_entry(CpPatternMap *map, cpstring rule, cpstring outputmap, int weight);
static char *next_field(char *buf, char *inp);
static int append_outputmap(CpRuleEntry *entry, cpstring outputmap, int weight);

CpPatternMap *cp_patternmap_new()
{
  CpPatternMap *map;
  map = malloc(sizeof(CpPatternMap));
  init_hash_table(map);
  load_rules(map, RULEFILE);
  return map;
}

void cp_patternmap_destroy(CpPatternMap *map)
{
  int i;
  int j;
  CpRuleEntry *entry;
  CpOutputMap *omap;

  for (i = 0; i < HASHTABLESIZE; i++)
  {
    entry = map->entries[i];
    if (entry != NULL)
    {
      for (j = 0; j < entry->mapcnt; j++)
      {
        omap = entry->maps[j];
        free(omap->map);
        free(omap);
      }
    }
    free(entry);
  }
}

CpRuleEntry *cp_patternmap_lookup(CpPatternMap *map, cpstring lookup_str)
{
  unsigned key;
  CpRuleEntry *rule;
  key = create_hash(lookup_str);
  for (rule = map->entries[key]; rule != NULL; rule = rule->next)
  {
    if (strcmp(lookup_str, rule->key) == 0)
    {
      return rule;
    }
  }
  return rule;
}

static void init_hash_table(CpPatternMap *map)
{
  unsigned i;

  for (i = 0; i < HASHTABLESIZE; i++)
  {
    map->entries[i] = NULL;
  }
}

static unsigned create_hash(char *key_str)
{
  unsigned h, key;

  h = ElfHash(key_str);
  key = (h % HASHTABLESIZE);
  return key;
}

static void load_rules(CpPatternMap *map, cpstring filename)
{
  char recbuf[RULERECLEN];
  char rule[RULELEN];
  char outmap[RULELEN];
  char weightstr[RULEWEIGHTLEN];
  int weight;
  char *next;

  FILE *rule_file;
  rule_file = fopen(filename, "r");

  while (!feof(rule_file))
  {
    BLANK_STRING(recbuf);
    fgets(recbuf, RULERECLEN, rule_file);

    if ((next = next_field(weightstr, recbuf)) == NULL)
    {
      break;
    }
    sscanf(weightstr, "%d", &weight);

    next = next_field(rule, next);
    next = next_field(outmap, next);

    add_entry(map, rule, outmap, weight);
  }
}

static void add_entry(CpPatternMap *patternmap, cpstring rule, cpstring map, int weight)
{
  CpRuleEntry *entry;
  CpOutputMap *outmap;
  unsigned hashkey;

  hashkey = create_hash(rule);
  for (entry = patternmap->entries[hashkey]; entry != NULL; entry = entry->next)
  {
    if (strcmp(rule, entry->key) == 0)
    {
      break;
    }
  }

  if (entry == NULL)
  {
    int i;
    /* create new entry */
    entry = calloc(1, sizeof(CpRuleEntry));
    entry->key = malloc(sizeof(char) * (strlen(rule) + 1));
    strcpy(entry->key, rule);
    for (i = 0; i <= MAXRULEMAPS; i++)
    {
      entry->maps[i] = NULL;
    }

    /* add map */
    outmap = calloc(1, sizeof(CpOutputMap));
    outmap->map = calloc(1, sizeof(char) * RULELEN);
    strcpy(outmap->map, map);
    outmap->weight = weight;
    entry->maps[entry->mapcnt++] = outmap;

    /* add to hash table chain */
    entry->next = patternmap->entries[hashkey];
    patternmap->entries[hashkey] = entry;
  }
  else
  {
    append_outputmap(entry, map, weight);
  }
}

static int append_outputmap(CpRuleEntry *entry, cpstring map, int weight)
{
  int i;
  CpOutputMap *outmap;

  /* check if max map count has been reached, if so then fail */
  if (entry->mapcnt >= MAXRULEMAPS)
    return FALSE;

  for (i = 0; i < entry->mapcnt; i++)
  {
    outmap = entry->maps[i];
    if (outmap == NULL)
      break;

    if (strcmp(outmap->map, map) == 0)
    {
      return FALSE; /* found duplicate map */
    }
  }
  outmap = calloc(1, sizeof(CpOutputMap));
  outmap->map = malloc(sizeof(char) * RULELEN);
  outmap->weight = weight;
  entry->maps[entry->mapcnt++] = outmap;
  return TRUE;
}

static char *next_field(char *buf, char *inp)
{
  char c;
  char *d = buf;
  char *s = inp;

  BLANK_STRING(d);
  /* -- space at the beginning of a line will stop the read -- */
  if (isspace(*s))
    return NULL;
  while ((c = *s++) != '\0')
  {
    if (c == '\"' ||
        c == '\r')
      continue; /* -- ignore quotes and carriage returns -- */
    /* -- zero terminate field and record delimiters -- */
    if (c == '\n' ||
        c == ',')
    {
      BLANK_STRING(d);
      return s;
    }
    *d++ = c; /* -- copy it -- */
  }
  return NULL;
}
