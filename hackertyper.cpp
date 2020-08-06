#include "config.h"

#include <cstring>
#include <fstream>
#include <iostream>

#include <cstdlib>

#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
#define OS_UNIX
#endif
#ifdef OS_UNIX
#include <ncurses.h>
#else
#include <conio.h>
#endif

void print_help(char *program_name);

void initialize();

void wait_and_print(char *const buf, size_t size);

void loop(std::ifstream &file);

void cleanup();

int main(int argc, char *argv[]) {
  if (argc != 2) {
    print_help(argv[0]);
    return EXIT_FAILURE;
  }

  if (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0) {
    print_help(argv[0]);
    return EXIT_SUCCESS;
  }

  std::ifstream file{argv[1]};

  if (!file) {
    std::cerr << "Could not open file!" << std::endl;
    return EXIT_FAILURE;
  }

  initialize();

  loop(file);

  cleanup();

  return 0;
}

void print_help(char *program_name) {
  // Print version
  std::cout << program_name << " Version " << HackerTyper_VERSION_MAJOR << '.'
            << HackerTyper_VERSION_MINOR << std::endl;

  // Print usage
  std::cout << "Usage: " << std::endl
            << "  " << program_name << " file  "
            << "OR" << std::endl
            << "  " << program_name << " -h/--help" << std::endl;
}

void initialize() {
  // Initialize ncurses
#ifdef OS_UNIX
  initscr();
  clear();
  noecho();
  cbreak();
  notimeout(stdscr, TRUE);
  nodelay(stdscr, FALSE);
  idlok(stdscr, TRUE);
  scrollok(stdscr, TRUE);
#endif
}

void wait_and_print(char *const buf, size_t size) {
#ifdef OS_UNIX
  // ncurses print
  getch();                  // wait for input
  addnstr(buf, size); // print string
#else
  _getch();
  std::cout.write(buf, size);
  std::cout << std::flush;
#endif
}

void loop(std::ifstream &file) {

  char buf[CHARS_TO_READ];

  size_t chars_read = 0;

  do {
    chars_read = file.read(buf, CHARS_TO_READ).gcount();

    wait_and_print(buf, chars_read);

  } while (chars_read == CHARS_TO_READ);
}

void cleanup() {
// Clean up ncurses
#ifdef OS_UNIX
  clrtoeol();
  refresh();
  endwin();
#endif
}
