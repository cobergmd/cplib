/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#include "cptoken.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CpToken *cp_token_new(cpstring data, int len, int pos, int line, int type)
{
  int i;
  CpToken *token = calloc(1, sizeof(CpToken));
  if (token != NULL)
  {
    /* all token data is uppercased */
    char *uppr = malloc(MAXTOKENLEN);
    strcpy_up(uppr, data);
    /* create token */
    token->data = malloc(MAXTOKENLEN);
    strcpy(token->data, uppr);
    token->length = len;
    token->position = pos;
    token->line = line;
    for (i = 0; i <= MAXTYPES; i++)
      token->types[i] = NULL;
    /* add tokenizer type definition */
    cp_token_add_type(token, type, uppr, 1);
    free(uppr);
  }
  return token;
}

CpTokenList *cp_tokenlist_new()
{
  CpTokenList *list = calloc(1, sizeof(CpTokenList));
  return list;
}

CpTokenType *cp_tokentype_new()
{
  CpTokenType *type = calloc(1, sizeof(CpTokenType));
  type->len = 1; /* default for the more common word type */
  return type;
}

void cp_token_destroy(CpToken *token)
{
  int i;
  for (i = 0; i < token->typecnt; i++)
    cp_tokentype_destroy(token->types[i]);
  free(token->data);
}

void cp_tokentype_destroy(CpTokenType *def)
{
  free(def->data);
  free(def);
}

void cp_tokenlist_destroy(CpTokenList *list)
{
  CpTokenCell *cell;
  CpTokenCell *temp;

  cell = list->first;
  while (cell != NULL)
  {
    temp = cell->next;
    cp_token_destroy(&(cell->token));
    free(cell);
    cell = temp;
  }

  free(list);
}

cpbool cp_tokenlist_append(CpTokenList *list, CpToken *field)
{
  CpTokenCell *cell = calloc(1, sizeof(CpTokenCell));

  if (cell == NULL)
    return FALSE;

  cell->token = *field;
  cell->next = NULL;
  cell->prev = NULL;

  if (list->first == NULL)
    list->first = list->last = cell;
  else
  {
    list->last->next = cell;
    cell->prev = list->last;
    list->last = cell;
  }

  return TRUE;
}

void cp_token_add_type(CpToken *token, int type, cpstring data, int len)
{
  CpTokenType *toktype = cp_tokentype_new();
  toktype->type = type;
  toktype->len = len;
  toktype->data = malloc(sizeof(char) * (strlen(data) + 1));
  strcpy(toktype->data, data);
  token->types[token->typecnt++] = toktype;
}

void cp_tokenlist_print(CpTokenList *list)
{
  CpTokenCell *cell;
  CpTokenCell *nextcell;
  CpToken token;

  printf("\n\n---------- Tokenizer Output -----------");
  for (cell = list->first; cell != NULL; cell = nextcell)
  {
    int i = 1;
    nextcell = cell->next;
    token = cell->token;

    printf("\n\ntoken %d: %s", i++, token.data);
    printf("\n line     = %d", token.line);
    printf("\n position = %d", token.position);
    printf("\n length   = %d", token.length);
    if (token.term == EOL || token.term == TERM_EOL)
      printf("\n term     = EOL", token.line);
    else if (token.term == EOL || token.term == TERM_EOA)
      printf("\n term     = EOA", token.line);
    else if (token.term == EOL || token.term == TERM_DELIM)
      printf("\n term     = DELIM", token.line);
    else if (token.term == EOL || token.term == TERM_SPACE)
      printf("\n term     = SPACE", token.line);
    for (i = 0; i <= token.typecnt; i++)
    {
      CpTokenType *def;
      def = token.types[i];
      if (def != NULL)
      {
        printf("\n type     = %d %s", def->type, def->data);
      }
    }
  }
}

CpToken *cp_tokenlist_get(CpTokenList *list, int idx)
{
  int i = 0;
  CpTokenCell *cell;
  CpTokenCell *nextcell;

  for (cell = list->first; cell != NULL; cell = nextcell)
  {
    nextcell = cell->next;
    if (i++ == idx)
      break;
  }

  if (cell == NULL)
    return NULL;
  else
    return &(cell->token);
}
