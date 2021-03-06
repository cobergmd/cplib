%option case-insensitive
%option 8bit
%option fast
%option never-interactive
%option noyywrap

%{
	#include <stdio.h>
	#include "cplib.h"
	#include "cptoken.h"

	CP_API int cp_tokenizer_tokenize(cpstring line, CpTokenList* list);

	static void add_token(cpstring data, int len, int pos, int line, int type);
	static void mark_line();
	static void mark_delim(const char* txt);
	static void mark_spacer(const char* txt);

	int tokpos;
	int linenum = 1;
	int tokcnt = 0;
	CpTokenList* tokenlist;
%}

ignore		["+{(~!@$%^*=|?<]
spacer		[ \-.)}>_]
delim		[\t,;]
char		[a-z]
num			[0-9]
alpha		{char}
alphanum	({char}*{num}*)*{char}+({num}*{char}*)

nl			\n|\r\n|\r|\f
eol			\0

%%

{ignore}+	{	/* ignore */ }

{nl}		|
{eol}		{	++linenum;
				tokpos = 0;
				mark_line();
			}

{delim}+	{	mark_delim(yytext); }

{spacer}+	{	mark_spacer(yytext); }

{num}*"11"[Tt][Hh] |
{num}*"12"[Tt][Hh] |
{num}*"13"[Tt][Hh] |
{num}*"1"[Ss][Tt]  |
{num}*"2"[Nn][Dd]  |
{num}*"3"[Rr][Dd]  |
{num}*[04-9][Tt][Hh] {	++tokpos;
				add_token(yytext, yyleng, tokpos, linenum, ORD);
            }

[13]"/"[2348]+ { ++tokpos;
				add_token(yytext, yyleng, tokpos, linenum, FRACT);
			}

{num}+		{	++tokpos;
				add_token(yytext, yyleng, tokpos, linenum, NUM);
			}

{alpha}+	{	++tokpos;
				add_token(yytext, yyleng, tokpos, linenum, ALPHA);
			}

{alphanum}+ {	++tokpos;
				add_token(yytext, yyleng, tokpos, linenum, ALPHANUM);
			}
%%

int cp_tokenizer_tokenize(cpstring line, CpTokenList* list)
{
    YY_BUFFER_STATE buf;
    tokenlist = list;
    tokcnt = 0;
    linenum = 1;
    tokpos = 0;
    buf = yy_scan_string(line);
    yylex();
    yy_delete_buffer(buf);
    return 0;
}

static void add_token(cpstring data, int len, int pos, int line, int type)
{
	CpToken* token = cp_token_new(data, len, pos, line, type);
	if (token != NULL && tokenlist != NULL)
	{
		tokcnt++;
		cp_tokenlist_append(tokenlist, token);
	}
}

static void mark_line()
{
	CpToken* token;
	token = cp_tokenlist_get(tokenlist, tokcnt-1);
	token->term = TERM_EOL;
}

static void mark_delim(const char* txt)
{
	CpToken* token;
	token = cp_tokenlist_get(tokenlist, tokcnt-1);
	token->term = TERM_DELIM;
}

static void mark_spacer(const char* txt)
{
	CpToken* token;
	token = cp_tokenlist_get(tokenlist, tokcnt-1);
	token->term = TERM_SPACE;
}

