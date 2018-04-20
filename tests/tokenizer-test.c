/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "cplib/cplib.h"
#include "cplib/cptoken.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

static void print_results(cpstring addr, CpTokenList *list);

int cp_tokenizer_test(int argc, char *argv[])
{
  CpTokenList *tokenlist;

  /* typical */
  const char *addr1 = "1101 cauf ln\ndena tx\n12122-2250";
  const char *addr2 = "12345 leaf rd\npasadena ca";
  const char *addr3 = "1140 pinemont pl\napt 5b\nannapolis md 21403";
  /* ohh canada */
  const char *caddr1 = "32 bendingroad crescent\nst catharines, on l2n 5r5";
  const char *caddr2 = "1 masseyfield gate\nmarkham on l3r 3m8";
  const char *caddr3 = "72 st regis cresent north\nnorth york on m3j 1z3";

  tokenlist = cp_tokenlist_new();
  cp_tokenizer_tokenize(addr1, tokenlist);
  print_results(addr1, tokenlist);
  cp_tokenlist_destroy(tokenlist);

  tokenlist = cp_tokenlist_new();
  cp_tokenizer_tokenize(addr2, tokenlist);
  print_results(addr2, tokenlist);
  cp_tokenlist_destroy(tokenlist);

  tokenlist = cp_tokenlist_new();
  cp_tokenizer_tokenize(addr3, tokenlist);
  print_results(addr3, tokenlist);
  cp_tokenlist_destroy(tokenlist);
}

static void print_results(cpstring addr, CpTokenList *list)
{
  CpTokenCell *cell;
  CpTokenCell *nextcell;

  printf("\n\n%s\n", addr);
  for (cell = list->first; cell != NULL; cell = nextcell)
  {
    int i;
    nextcell = cell->next;
    printf("\n\t%s\t@%d:%d\tlen:%d\tterm:%d",
           cell->token.data,
           cell->token.line,
           cell->token.position,
           cell->token.length,
           cell->token.term);
    for (i = 0; i <= cell->token.typecnt; i++)
    {
      CpTokenType *def;
      def = cell->token.types[i];
      printf("type %d: %d", i, def->type);
    }
  }
}