/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#include "cpdictionary.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* private data */
static int loaded = 0;

/* private methods */
static void init_hash_table(CpDictionary *dictionary);
static void load_dictionary(CpDictionary *dictionary, cpstring filename);
static unsigned create_hash(char *key_str);
static void add_entry(CpDictionary *dictionary, cpstring key, cpstring phrase, cpstring std, int type);
static char *next_field(char *buf, char *inp);
static CpDictDef *create_def(cpstring phrase, cpstring std, int type);
static int append_def(CpDictEntry *entry, cpstring phrase, cpstring std, int type);

CpDictionary *cp_dictionary_new()
{
  CpDictionary *dictionary;
  dictionary = malloc(sizeof(CpDictionary));
  init_hash_table(dictionary);
  load_dictionary(dictionary, DICTFILE);
  return dictionary;
}

void cp_dictionary_destroy(CpDictionary *dictionary)
{
  int i;
  int j;
  CpDictEntry *entry;
  CpDictDef *def;

  for (i = 0; i < HASHTABLESIZE; i++)
  {
    entry = dictionary->entries[i];
    if (entry != NULL)
    {
      for (j = 0; j < entry->defcnt; j++)
      {
        def = entry->defs[j];
        free(def->phrasekey);
        free(def->std_text);
        free(def);
      }
    }
    free(entry);
  }
}

CpDictEntry *cp_dictionary_lookup(CpDictionary *dictionary, cpstring lookup_str)
{
  unsigned key;
  CpDictEntry *entry;
  key = create_hash(lookup_str);
  for (entry = dictionary->entries[key]; entry != NULL; entry = entry->next)
  {
    if (strcmp(lookup_str, entry->wordkey) == 0)
    {
      return entry;
    }
  }
  return entry;
}

static void init_hash_table(CpDictionary *dictionary)
{
  unsigned i;
  for (i = 0; i < HASHTABLESIZE; i++)
  {
    dictionary->entries[i] = NULL;
  }
}

static unsigned create_hash(char *key_str)
{
  unsigned h, key;

  h = ElfHash(key_str);
  key = (h % HASHTABLESIZE);
  return key;
}

static void load_dictionary(CpDictionary *dictionary, cpstring filename)
{
  char recbuf[DICTRECLEN];
  char wordkey[DICTKEYLEN];
  char phrasekey[DICTKEYLEN];
  char stdstr[DICTSTDLEN];
  char typestr[DICTTYPELEN];
  int type;
  char *next;

  FILE *dict_file;
  dict_file = fopen(filename, "r");

  if (dict_file == NULL)
  {
    FATAL_ERR("Dictionary file not found.");
  }

  while (!feof(dict_file))
  {
    BLANK_STRING(recbuf);
    fgets(recbuf, DICTRECLEN, dict_file);

    if ((next = next_field(typestr, recbuf)) == NULL)
    {
      break;
    }
    sscanf(typestr, "%d", &type);

    next = next_field(wordkey, next);
    next = next_field(phrasekey, next);
    next = next_field(stdstr, next);

    add_entry(dictionary, wordkey, phrasekey, stdstr, type);
  }
}

static void add_entry(CpDictionary *dictionary, cpstring key, cpstring phrase, cpstring std, int type)
{
  CpDictEntry *entry;
  unsigned hash;

  hash = create_hash(key);
  for (entry = dictionary->entries[hash]; entry != NULL; entry = entry->next)
  {
    if (strcmp(key, entry->wordkey) == 0)
    {
      break;
    }
  }

  if (entry == NULL)
  {
    int i;
    /* create new entry */
    entry = calloc(1, sizeof(CpDictEntry));
    entry->wordkey = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(entry->wordkey, key);
    for (i = 0; i <= MAXDEFINITIONS; i++)
    {
      entry->defs[i] = NULL;
    }

    /* add to hash table chain */
    entry->next = dictionary->entries[hash];
    dictionary->entries[hash] = entry;

    /* add basic token definition */
    entry->defs[0] = create_def(phrase, std, type);
    entry->defcnt = 1;
  }
  else
  {
    /* append new def to existing entry */
    append_def(entry, phrase, std, type);
  }
}

static CpDictDef *create_def(cpstring phrase, cpstring std, int type)
{
  CpDictDef *def;
  def = calloc(1, sizeof(CpDictDef));
  def->phrasekey = malloc(sizeof(char) * (strlen(phrase) + 1));
  strcpy(def->phrasekey, phrase);
  def->std_text = malloc(sizeof(char) * (strlen(std) + 1));
  strcpy(def->std_text, std);
  def->datatype = type;
  return def;
}

static int append_def(CpDictEntry *entry, cpstring phrase, cpstring std, int type)
{
  int i;
  CpDictDef *def;

  /* check if max def count has been reached, if so then fail */
  if (entry->defcnt >= MAXDEFINITIONS)
    return FALSE;

  for (i = 0; i < entry->defcnt; i++)
  {
    def = entry->defs[i];
    if (def == NULL)
      break;

    if (strcmp(def->phrasekey, phrase) == 0)
    {
      return FALSE; /* dupe def */
    }
  }
  def = create_def(phrase, std, type);
  entry->defs[entry->defcnt] = def;
  entry->defcnt += 1;
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
