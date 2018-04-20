/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#include "cpscanner.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

CpScanner *cp_scanner_new(void)
{
  CpScanner *scanner = malloc(sizeof(CpScanner));
  scanner->tokcnt = 0;
  scanner->text = NULL;
  scanner->tokens = cp_tokenlist_new();
  return scanner;
}

void cp_scanner_destroy(CpScanner *scanner)
{
  cp_tokenlist_destroy(scanner->tokens);
}

void cp_scanner_tokenize(CpScanner *scanner, cpstring line)
{
  int tokpos = 0;
  int toklen = 0;
  int linenum = 1;
  int toktype = UNKNOWN;
  char *toktext;
  CpToken *token;
  unsigned char c;

  scanner->tokcnt = 0;
  scanner->text = line;

  CALLOC_STRING(toktext, MAXTOKENLEN);

  do /* while not null */
  {
    c = *(scanner->text++); /* read next char */

    /* ignored characters */
    if (c == '+' || c == '"' || c == '{' || c == '(' || c == '~' ||
        c == '!' || c == '@' || c == '$' || c == '%' || c == '^' ||
        c == '*' || c == '=' || c == '|' || c == '?' || c == '<')
      continue;

    switch (c)
    {
    /* spacers */
    case ' ':
    case '-':
    case '.':
    case ')':
    case '}':
    case '>':
    case '_':
      if (toktype != UNKNOWN)
      {
        token = cp_token_new(toktext, toklen, tokpos, linenum, toktype);
        token->term = TERM_SPACE;
        cp_tokenlist_append(scanner->tokens, token);
        toktype = UNKNOWN;
        toklen = 0;
        memset(toktext, 0, MAXTOKENLEN);
      }
      break;
    /* delims */
    case '\t':
    case ',':
    case ';':
      if (toktype != UNKNOWN)
      {
        token = cp_token_new(toktext, toklen, tokpos, linenum, toktype);
        token->term = TERM_DELIM;
        cp_tokenlist_append(scanner->tokens, token);
        toktype = UNKNOWN;
        toklen = 0;
        memset(toktext, 0, MAXTOKENLEN);
      }
      break;
    /* eol */
    case '\n':
    case '\r':
    case '\f':
      if (toktype != UNKNOWN)
      {
        token = cp_token_new(toktext, toklen, tokpos, linenum++, toktype);
        token->term = TERM_EOL;
        cp_tokenlist_append(scanner->tokens, token);
        toktype = UNKNOWN;
        toklen = 0;
        memset(toktext, 0, MAXTOKENLEN);
      }
      break;
    case '\0':
      /* eos */
      if (toktype != UNKNOWN)
      {
        token = cp_token_new(toktext, toklen, tokpos, linenum++, toktype);
        token->term = TERM_EOA;
        cp_tokenlist_append(scanner->tokens, token);
        toktype = UNKNOWN;
        toklen = 0;
        memset(toktext, 0, MAXTOKENLEN);
      }
      break;
    /* numbers */
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      if (toktype == UNKNOWN)
      {
        toktype = NUM;
      }
      else if (toktype == ALPHA)
      {
        toktype = ALPHANUM;
      }

      toktext[toklen++] = c;
      break;
      /* fractional */
    case '/':
      if (toktype == NUM)
      {
        toktype = FRACT;
      }

      toktext[toklen++] = c;
      break;
      /* alphabet */
    case 'A':
    case 'a':
    case 'B':
    case 'b':
    case 'C':
    case 'c':
    case 'D':
    case 'd':
    case 'E':
    case 'e':
    case 'F':
    case 'f':
    case 'G':
    case 'g':
    case 'H':
    case 'h':
    case 'I':
    case 'i':
    case 'J':
    case 'j':
    case 'K':
    case 'k':
    case 'L':
    case 'l':
    case 'M':
    case 'm':
    case 'N':
    case 'n':
    case 'O':
    case 'o':
    case 'P':
    case 'p':
    case 'Q':
    case 'q':
    case 'R':
    case 'r':
    case 'S':
    case 's':
    case 'T':
    case 't':
    case 'U':
    case 'u':
    case 'V':
    case 'v':
    case 'W':
    case 'w':
    case 'X':
    case 'x':
    case 'Y':
    case 'y':
    case 'Z':
    case 'z':
      if (toktype == UNKNOWN)
      {
        toktype = ALPHA;
      }
      else if (toktype == NUM)
      {
        toktype = ALPHANUM;
      }

      toktext[toklen++] = c;
      break;
    default:
      WARN_ERR("Unexpexted char found");
    }
  } while (c != 0);
}
