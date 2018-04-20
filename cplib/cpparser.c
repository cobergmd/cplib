/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#include "cpparser.h"

#include <stdio.h>
#include <string.h>

/* private data */
static CpScanner *scanner;
static CpTokenList *tokenlist;
static int init_done = 0;
static CpToken *linetokens[MAXTOKENS];
static int linetokcnt = 0;
static CpDictionary *dictionary;
static CpPatternMap *patternmap;

/* represents an address line pattern with output mappings */
typedef struct _CpLineScheme CpLineScheme;
struct _CpLineScheme
{
  cpstring pattern;
  CpTokenType *types[MAXTOKENS];
  int typecnt;
  cpstring map;
  int weight;
};
static CpLineScheme *schemes[MAXSCHEMES];
static int schemecnt = 0;

/* private methods */
static void lookup_tokens(CpTokenList *tokenlist);
static void load_address(CpTokenList *tokenlist, CpAddress *address);
static void build_match_phrase(char *dest, CpToken *tokens[], int len);
static void match_phrase_def(CpDictDef *def, CpTokenCell *cell);
static void create_defs(CpDictEntry *match_entry, CpTokenCell *cell);
static void gen_schemes(int curridx, int tokcnt, CpTokenType *types[], int typecnt);
static void add_scheme(CpTokenType *types[], int cnt);
static CpLineScheme *get_scheme_winner();
static void print_schemes(int linenumber);
static void free_schemes();
static void match_patterns();
static void map_address(CpAddress *address);

void cp_parser_init()
{
  dictionary = cp_dictionary_new();
  patternmap = cp_patternmap_new();
  init_done = 1;
}

void cp_parser_destroy()
{
  cp_dictionary_destroy(dictionary);
  cp_patternmap_destroy(patternmap);
}

void cp_parser_parse(cpstring line, CpAddress *address)
{
  if (!init_done)
  {
    FATAL_ERR("Parser is not initialized.");
  }

#ifdef _DEBUG
  printf("\n\n================ Input ================");
  printf("\n%s", line);
  printf("\n=======================================");
#endif

  scanner = cp_scanner_new();
  cp_scanner_tokenize(scanner, line);
  tokenlist = scanner->tokens;
  lookup_tokens(tokenlist);

#ifdef _DEBUG
  cp_tokenlist_print(tokenlist);
#endif

  load_address(tokenlist, address);

#ifdef _DEBUG
  cp_address_print(address);
#endif

  cp_scanner_destroy(scanner);
}

static void lookup_tokens(CpTokenList *tokenlist)
{
  CpDictEntry *match_entry; /* matching hash table entry */

  CpTokenCell *cell;
  CpTokenCell *nextcell;

  for (cell = tokenlist->first; cell != NULL; cell = nextcell)
  {
    nextcell = cell->next;

    /* find matches in dictionary - words or phrases */
    if ((match_entry = cp_dictionary_lookup(dictionary, cell->token.data)) != NULL)
    {
      /* create field defs for matches */
      create_defs(match_entry, cell);
    }
  }
}

static void create_defs(CpDictEntry *match_entry, CpTokenCell *cell)
{
  int i;          /* def idx */
  CpDictDef *def; /* def ptr */

  /* each matching dict entry may have multiple defs */
  for (i = 0; i < match_entry->defcnt; i++)
  {
    def = match_entry->defs[i];

    /* is this a word or phrase definition? */
    if (strcmp(def->phrasekey, "") == 0)
    {
      /* word match - straight def append */
      cp_token_add_type(&(cell->token),
                        def->datatype,
                        def->std_text, 1);
    }
    else
    {
      /* phrase match - execute match to get defs */
      match_phrase_def(def, cell);
    }
  }
}

static void match_phrase_def(CpDictDef *def, CpTokenCell *cell)
{
  int offset = 0;
  CpToken *tokens[MAXTOKENS];
  CpTokenCell *nextcell;
  CpTokenCell *matched_cell = cell;

  char *phrase;
  phrase = malloc(sizeof(char) * MAXSTRLEN);

  /* spin through fields until term or number to get offset */
  for (; cell != NULL; cell = nextcell)
  {
    nextcell = cell->next;
    if (cell->token.term == TERM_DELIM ||
        cell->token.term == TERM_EOL)
      break;
    tokens[offset++] = &(cell->token);
  }

  /* scan for phrase left to right */
  for (; offset >= 0; offset--)
  {
    BLANK_STRING(phrase);
    build_match_phrase(phrase, tokens, offset);

    if (strcmp(def->phrasekey, phrase) == 0)
    {
      cp_token_add_type(&(matched_cell->token),
                        def->datatype,
                        def->std_text,
                        offset);
      break;
    }
  }
  free(phrase);
}

static void build_match_phrase(char *dest, CpToken *fields[], int len)
{
  int i;
  for (i = 0; i < len; i++)
  {
    strcat(dest, fields[i]->data);
    if (i < (len - 1))
      strcat(dest, " ");
  }
}

static void load_address(CpTokenList *tokenlist, CpAddress *address)
{
  CpTokenCell *cell;
  CpTokenCell *nextcell;
  CpTokenType *types[MAXTOKENS];
  int linecnt = 0;
  int tokcnt = 0;

  /* spin through fields and create array of token types by line */
  for (cell = tokenlist->first; cell != NULL; cell = nextcell)
  {
    nextcell = cell->next;

    linetokens[tokcnt++] = &(cell->token);

    /* get the token type set for each line */
    if (cell->token.term == TERM_EOL ||
        cell->token.term == TERM_EOA)
    {
      linecnt++;

      /* recursive method that walks all combinations
				of token types and creates array of address
				line schemes. */
      gen_schemes(0, tokcnt, types, 0);
      match_patterns();

#ifdef _DEBUG
      print_schemes(linecnt);
#endif

      /* copy values to address via pattern map */
      map_address(address);

      /* free pattern list */
      free_schemes();
      tokcnt = 0;
    }
  }
}

static void gen_schemes(int curridx, int tokcnt, CpTokenType *types[], int typecnt)
{
  int i;
  int candcnt = 0;
  CpTokenType *intypes[MAXTOKENS];
  CpTokenType *outtypes[MAXTOKENS];
  int outcnt = 0;

  /* initialize to null */
  for (i = 0; i < MAXTOKENS; i++)
    outtypes[i] = NULL;
  for (i = 0; i < MAXTOKENS; i++)
    intypes[i] = NULL;

  /* save the types passed in for each type iteration below */
  for (i = 0; i < typecnt; i++)
  {
    intypes[i] = types[i];
  }

  /* iterate the types for the current token */
  for (i = 0; i < linetokens[curridx]->typecnt; i++)
  {
    int j;
    int next;
    CpTokenType *toktype = linetokens[curridx]->types[i];

    for (j = 0; j < MAXTOKENS; j++)
    {
      if (intypes[j] == NULL)
        break;
      outtypes[j] = intypes[j];
    }
    outtypes[j] = toktype;
    outcnt = j + 1;

    /* recurse the next field in the line */
    next = curridx + toktype->len;
    if (next < tokcnt)
    {
      gen_schemes(next, tokcnt, outtypes, outcnt);
    }
    else
    {
      add_scheme(outtypes, outcnt);
    }
  }
}

static void add_scheme(CpTokenType *types[], int cnt)
{
  int i;
  CpLineScheme *scheme = calloc(1, sizeof(CpLineScheme));
  char *pstr = malloc(sizeof(char) * MAXPATLEN);
  BLANK_STRING(pstr);

  if (scheme != NULL)
  {
    scheme->pattern = malloc(sizeof(char) * MAXPATLEN);
    scheme->map = malloc(sizeof(char) * MAXPATLEN);
    scheme->typecnt = 0;
    scheme->weight = 0;
    for (i = 0; i < MAXTOKENS; i++)
      scheme->types[i] = NULL;
  }

  for (i = 0; i < cnt; i++)
  {
    char *typestr = malloc(sizeof(char) * 5);
    BLANK_STRING(typestr);
    scheme->types[i] = types[i];

    itoa(scheme->types[i]->type, typestr, 10);
    strcat(pstr, typestr);
    if (i < cnt - 1)
      strcat(pstr, " ");
    free(typestr);
  }
  strcpy(scheme->pattern, pstr);
  scheme->typecnt = i;
  schemes[schemecnt++] = scheme;
  free(pstr);
}

static void match_patterns()
{
  int i;
  CpLineScheme *scheme;
  CpRuleEntry *match_entry;

  for (i = 0; i < schemecnt; i++)
  {
    scheme = schemes[i];

    /* find map matches for patterns */
    if ((match_entry = cp_patternmap_lookup(patternmap, scheme->pattern)) != NULL)
    {
      int j;
      /* TODO: strategy for multiple map hits */
      for (j = 0; j < match_entry->mapcnt; j++)
      {
        strcpy(scheme->map, match_entry->maps[j]->map);
        scheme->weight = match_entry->maps[j]->weight;
      }
    }
    else
    {
      memset(scheme->map, 0, sizeof(scheme->map));
    }
  }
}

static void print_schemes(int linecnt)
{
  int i;
  printf("\n\n-------- Schemes & Maps Matrix --------");
  printf("\n              >> line %d <<", linecnt);
  for (i = 0; i < schemecnt; i++)
  {
    int j;
    CpLineScheme *scheme = schemes[i];

    printf("\n\n- pattern: %d -", i + 1);

    /* print token types */
    for (j = 0; j < scheme->typecnt; j++)
    {
      printf("\ntoken: type= %d data= %s",
             scheme->types[j]->type,
             scheme->types[j]->data);
    }

    /* print output maps */
    if (strcmp(scheme->map, "\0") != 0)
      printf("\nmap:   %s", scheme->map);
    else
      printf("\nmap:   NO MATCH");
  }
}

static void free_schemes()
{
  int i;
  for (i = 0; i < schemecnt; i++)
  {
    free(schemes[i]->map);
    free(schemes[i]->pattern);
    free(schemes[i]);
  }
  schemecnt = 0;
}

static void map_address(CpAddress *address)
{
  CpLineScheme *highscheme;
  int highweight = -1;
  char *map;
  char delims[] = " ";
  char *field;
  int i;

  /* get highest weighted scheme */
  for (i = 0; i < schemecnt; i++)
  {
    if (schemes[i]->weight > highweight)
    {
      highscheme = schemes[i];
      highweight = schemes[i]->weight;
    }
  }

  /* get map from winning scheme */
  map = malloc(sizeof(char) * MAXPATLEN);
  if (strcmp(highscheme->map, "\0") != 0)
  {
    strcpy(map, highscheme->map);
  }
  else
  {
    printf("\nNo output map found.  Cannot map tokens to address fields.");
    return;
  }

  /* copy data to address fields */
  i = 0;
  field = strtok(map, delims);
  while (field != NULL)
  {
    int fieldnum;

    if (i > highscheme->typecnt)
    {
      printf("\nMapping mismatch occured.");
      break;
    }
    fieldnum = atoi(field);
    cp_address_append(address,
                      fieldnum,
                      highscheme->types[i++]->data);
    field = strtok(NULL, delims);
  }
  free(map);
}
