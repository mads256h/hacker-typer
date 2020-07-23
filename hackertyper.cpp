#include "config.h"

#include <cstring>
#include <fstream>
#include <iostream>

#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
#define OS_UNIX
#endif
#ifdef OS_UNIX
#include <ncurses.h>
#else
#include <conio.h>
#endif

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

void loop(const char *file_name);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    print_help(argv[0]);
    return 1;
  }

  if (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0) {
    print_help(argv[0]);
    return 0;
  }

  loop(argv[1]);

  return 0;
}

void cleanup() {
// Clean up ncurses
#ifdef OS_UNIX
  clrtoeol();
  refresh();
  endwin();
#endif
}

void loop(const char *file_name) {
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

  std::ifstream file{file_name};

  if (!file) {
    cleanup();
    std::cerr << "Could not open file!" << std::endl;
    return;
  }

  char buf[CHARS_TO_READ];

  size_t chars_read = 0;

  do {
    chars_read = file.read(buf, CHARS_TO_READ).gcount();

// ncurses print
#ifdef OS_UNIX
    getch();                  // wait for input
    addnstr(buf, chars_read); // print string
#else
    _getch();
    std::cout.write(buf, chars_read);
    std::cout << std::flush;
#endif
  } while (chars_read == CHARS_TO_READ);

  cleanup();
}