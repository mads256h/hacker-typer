use std::env;
use std::fs::File;
use std::io::prelude::*;
use rand::prelude::*;
use ncurses;

fn main() {
    let arguments: Vec<String> = env::args().collect();

    if arguments.len() != 2 {
        println!("USAGE: hacker-typer <file>");
        return;
    }

    let file_name = &arguments[1];


    let mut rng = rand::thread_rng();
    initialize();

    let mut file = match File::open(file_name) {
        Err(why) => panic!("Cannot open file: {}", why),
        Ok(file) => file,
    };

    let mut contents : String = String::new();

    match file.read_to_string(&mut contents) {
        Err(why) => panic!("Cannot read file: {}", why),
        Ok(r) => r,
    };

    let mut char = ncurses::getch();
    while char != 27 {
        char = ncurses::getch();

        let wanted_chars = rng.gen_range(5..=10);
        let len = if contents.len() > wanted_chars { wanted_chars } else { contents.len() };
        if len == 0 { break; }
        ncurses::addstr(&contents[0..len]);
        contents = contents[len..].to_string();
    }

    

    cleanup();
}

fn initialize() {
    ncurses::initscr();

    if ncurses::has_colors() {
        ncurses::use_default_colors();
        ncurses::start_color();
        ncurses::init_pair(1, ncurses::COLOR_GREEN, -1);
        ncurses::attron(ncurses::COLOR_PAIR(1));
    }

    ncurses::clear();
    ncurses::noecho();
    ncurses::cbreak();
    ncurses::notimeout(ncurses::stdscr(), true);
    ncurses::nodelay(ncurses::stdscr(), false);
    ncurses::scrollok(ncurses::stdscr(), true);
}

fn cleanup() {
    if ncurses::has_colors() {
        ncurses::attroff(ncurses::COLOR_PAIR(1));
    }

    ncurses::clrtoeol();
    ncurses::refresh();
    ncurses::endwin();
}
