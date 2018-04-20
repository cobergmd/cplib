/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "cplib/cpscanner.h"

#include <assert.h>
#include <string.h>

int cp_scanner_test(int argc, char *argv[])
{
  int i = 0;
  const char *text = "  123 any lane\nanytown ca 12345-5211";

  CpScanner *scanner;
  CpTokenList *tokens;
  CpTokenCell *cell;
  CpTokenCell *nextcell;

  scanner = cp_scanner_new();
  cp_scanner_tokenize(scanner, text);
  tokens = scanner->tokens;

  for (cell = tokens->first; cell != NULL; cell = nextcell)
  {
    nextcell = cell->next;
    printf("%d: %s \n", ++i, cell->token.data);
  }

  cp_scanner_destroy(scanner);
}