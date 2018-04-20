/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "cplib/cpdictionary.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

int cp_dictionary_test(int argc, char *argv[])
{
  CpDictEntry *entry;
  CpDictionary *dictionary;
  int i;

  dictionary = cp_dictionary_new();

  entry = cp_dictionary_lookup(dictionary, "STREET");
  assert(entry != NULL);

  printf("found: %s", entry->wordkey);
  for (i = 0; i <= entry->defcnt; i++)
  {
    printf("  def %d: %s", i, entry->defs[i]->std_text);
  }

  cp_dictionary_destroy(dictionary);
}