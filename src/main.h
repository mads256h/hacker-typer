#ifndef _MAIN_H_
#define _MAIN_H_

#include <istream>

void print_help(const char *const program_name);

void initialize();

bool wait_and_print(const char *const buf, std::streamsize size);

void loop(std::ifstream &file);

void cleanup();

#endif
