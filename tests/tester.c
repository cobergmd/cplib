/* cplib - Coding and Parsing Library
 * Copyright © 2009 Craig Oberg.  All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 *
 * tester.c - Runs the cplib unit tests
 */

#include <stdio.h>

int main(int argc, char *argv[])
{
  /*	cp_dictionary_test(argc, argv);  */
  /*	cp_tokenizer_test(argc, argv);  */
  /*    cp_scanner_test(argc, argv); */
  /*	cp_parser_test(argc, argv);     */
  cp_parser_quantity_test(argc, argv);
  printf("done");
  /*	cp_address_test(argc, argv);
	cp_name_test(argc, argv);    */
}