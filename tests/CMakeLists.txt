include_directories (${CPLIB_SOURCE_DIR}/cplib)

link_directories (${CPLIB_BINARY_DIR}/cplib)

add_executable (tester tester.c)

target_link_libraries (tester cplib)