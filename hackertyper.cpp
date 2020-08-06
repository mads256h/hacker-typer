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

bool wait_and_print(char *const buf, size_t size);

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

  return EXIT_SUCCESS;
}

// Prints usage/help
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

// Does some platform specific initialization
void initialize() {
  // Initialize ncurses
#ifdef OS_UNIX
  initscr();               // Initialize ncurses and stdscr
  clear();                 // Clear the terminal screen
  noecho();                // Disable echoing typed characters
  cbreak();                // Disable line buffering
  notimeout(stdscr, TRUE); // Makes getch() wait indefinetly for user input
  nodelay(stdscr, FALSE);  // Makes getch() a blocking call
  scrollok(stdscr, TRUE);  // Enable scrolling
#endif
}

// Waits for user input and then prints the contents of the buffer
bool wait_and_print(char *const buf, size_t size) {
#ifdef OS_UNIX
  // ncurses print
  if (getch() == EXIT_KEY) // wait for input
    return false;
  addnstr(buf, size); // print string
#else
  _getch();                   // wait for input
  std::cout.write(buf, size); // print string
  std::cout << std::flush;    // flush to see it immediately
#endif

  return true;
}

// Prints CHARS_TO_READ characters when user presses a key until the file has
// been read completely.
void loop(std::ifstream &file) {

  char buf[CHARS_TO_READ]; // Holds the characters that gets printed
  size_t chars_read = 0;   // How many characters we read

  do {
    chars_read = file.read(buf, CHARS_TO_READ).gcount();

    if (!wait_and_print(buf, chars_read))
      break;

  } while (chars_read ==
           CHARS_TO_READ); // If we didn't read CHARS_TO_READ characters we most
                           // likely have reached the end of the file
}

// Does some platform specific cleanup
void cleanup() {
// Clean up ncurses
#ifdef OS_UNIX
  clrtoeol(); // Clear the ncurses window
  refresh();  // Refresh the ncurses window
  endwin();   // Exits ncurses mode
#endif
}
