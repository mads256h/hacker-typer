#include "main.h"
#include "config.h"

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>

#include <cstdlib>
#include <cstring>


#if defined(UNIX) || defined(__unix__) || defined(LINUX) || defined(__linux__)
#define OS_UNIX
#endif
#ifdef OS_UNIX
#include <ncurses.h>
#else
#include <conio.h>
#endif

int main(const int argc, const char *argv[]) {
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
void print_help(const char *const program_name) {
  // Print version
  std::cout << program_name << " Version " << hackertyper_VERSION_MAJOR << '.'
            << hackertyper_VERSION_MINOR << std::endl;

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

  if (has_colors()){
    use_default_colors();
    start_color();
    init_pair(1, COLOR_GREEN, -1);
    attron(COLOR_PAIR(1));
  }

  clear();                 // Clear the terminal screen
  noecho();                // Disable echoing typed characters
  cbreak();                // Disable line buffering
  notimeout(stdscr, TRUE); // Makes getch() wait indefinetly for user input
  nodelay(stdscr, FALSE);  // Makes getch() a blocking call
  scrollok(stdscr, TRUE);  // Enable scrolling
#endif
}

// Waits for user input and then prints the contents of the buffer
bool wait_and_print(const char *const buf, std::streamsize size) {
#ifdef OS_UNIX
  // ncurses print
  if (getch() == EXIT_KEY) // wait for input
    return false;
  addnstr(buf, static_cast<int>(size)); // print string
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
  const auto seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::mt19937 random(static_cast<std::mt19937::result_type>(seed));

  char buf[CHARS_TO_READ_MAX]; // Holds the characters that gets printed
  std::streamsize chars_to_read = 0;
  std::streamsize chars_read = 0; // How many characters we read

  do {
    // Gets a random number between CHARS_TO_READ_MIN and CHARS_TO_READ_MAX inclusive
    chars_to_read = static_cast<std::streamsize>(
        (random() % ((CHARS_TO_READ_MAX + 1) - CHARS_TO_READ_MIN)) +
        CHARS_TO_READ_MIN);

    chars_read = file.read(buf, chars_to_read).gcount();

    if (!wait_and_print(buf, chars_read))
      break;

  } while (chars_read ==
           chars_to_read); // If we didn't read CHARS_TO_READ characters we most
                           // likely have reached the end of the file
}

// Does some platform specific cleanup
void cleanup() {
// Clean up ncurses
#ifdef OS_UNIX

  if (has_colors()){
    attroff(COLOR_PAIR(1));
    reset_color_pairs();
  }

  clrtoeol(); // Clear the ncurses window
  refresh();  // Refresh the ncurses window
  endwin();   // Exits ncurses mode
#endif
}
