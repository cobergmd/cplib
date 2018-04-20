/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */
#ifndef _CP_LIB_H_
#define _CP_LIB_H_

#include <stdlib.h>
#include <stdio.h>

/* custom types */
typedef const char *cpstring;
typedef int cpbool;

/* compile time constants */
#define EOL '\0'

#define TRUE  1
#define FALSE 0

#define MAXTOKENLEN     64
#define MAXTOKENS	      30
#define MAXSTRLEN	      128
#define MAXPATLEN	      256
#define MAXSCHEMES      16
#define MAXMAPS         5
#define MAXTYPES        5
#define MAXDEFINITIONS	10

#define DICTRECLEN      MAXSTRLEN
#define DICTKEYLEN	    MAXTOKENLEN
#define DICTSTDLEN	    MAXTOKENLEN
#define DICTTYPELEN	    4

#define RULERECLEN	    MAXSTRLEN
#define RULELEN		      30
#define RULEWEIGHTLEN	  4
#define MAXRULEMAPS	    5

#define HASHTABLESIZE 8893  /* prime number */

#define DICTFILE ".\\dictionary.txt"
#define RULEFILE ".\\rules.txt"

/* CpAddress field lengths */
#define ADDRNUMLEN	    10
#define DIRECTLEN		    5
#define STNAMELEN	      30
#define SUFFIXLEN	      10
#define UNITDESGLEN	    10
#define UNITRNGLEN	    10
#define MAILSTOPLEN	    10
#define DEPTLEN		      20
#define BOXNUMLEN		    20
#define MILUNITTYPELEN	10
#define MILUNITNUMLEN	  10
#define MILBOXNUMLEN	  10
#define MILSHIPNAMELEN	20
#define RRNUMLEN		    5
#define RRBOXLEN		    5
#define HWYCONTRACTLEN	5
#define CITYLEN		      30
#define STATELEN		    5
#define ZIP5LEN		      10
#define ZIP4LEN		      10

/* terminator types */
#define TERM_EOA	      0	/* end of address */
#define TERM_EOL	      1	/* end of address line */
#define TERM_DELIM	    2
#define TERM_SPACE	    3

/* basic token types */
#define UNKNOWN   -1
#define NUM		     0
#define ALPHA	     1
#define ALPHANUM   2
#define ORD		     3
#define FRACT      4
#define NUMSYM	   5	/* # */
#define AMPER      6	/* & */

/* address token types - used for both pattern and output maps */
#define HOUSE	   20	/* NUM | FRACT | ALPHANUM */
#define DIRECT	 21	/* pre and post directional for patterns */
#define PREDIR	 22
#define POSTDIR  23
#define STREET   24
#define SUFFIX   25
#define UNIT	   26
#define RANGE    27
#define NRUNIT   28	/* units that do not require secondary ranges */
#define POBOX	   29
#define BOX      30
#define RROUTE   31
#define ROUTE    32
#define HWCNTRCT 33
#define GENDELIV 34
#define CITY	   35
#define STATE	   36
#define ZIP5	   37
#define ZIP4	   38
#define PCT		   39
#define PCH		   40

/* global utility function decls */
unsigned ElfHash(char *key_str);
void strcpy_up(char *d, const char *s);

/* macros */
#define WARN_ERR(STR) \
  { \
    fprintf(stderr, "\nWarning: %s\n", STR); \
  }

#define FATAL_ERR(STR) \
  { \
    fprintf(stderr, "\nFatal Error: %s\n", STR); \
    exit(1); \
  }

#define MEM_ERR(PTR) \
  if (PTR == NULL) \
    FATAL_ERR("Insufficient Memory") ;

#define BLANK_STRING(STR) *STR = EOL

#define CALLOC_STRING(PTR, SIZE) \
  PTR = (char*) calloc(SIZE ,sizeof(char)); \
  MEM_ERR(PTR)

#ifdef  __cplusplus
  #define BEGIN_C_DECLS  extern "C" {
  #define END_C_DECLS    }
#else
  #define BEGIN_C_DECLS
  #define END_C_DECLS
#endif

/* DLL Export/Import declarations. */
#ifdef WIN32
  #if defined(DLL_EXPORTS)
    #define CP_API   __declspec(dllexport)
  #else
    #define CP_API   __declspec(dllimport)
  #endif
#else
  #define CP_API
#endif

#endif /* _CP_LIB_H_ */
