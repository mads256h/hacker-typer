use std::env;
use std::fs::File;
use std::io::prelude::*;
use rand::prelude::*;
use terminal::{error, Action, Clear, Color, Event, Value, Retrieved, KeyCode, Terminal};

fn main() -> error::Result<()>{
    let arguments: Vec<String> = env::args().collect();

    if arguments.len() != 2 {
        println!("USAGE: hacker-typer <file>");
        return Ok(());
    }

    let file_name = &arguments[1];


    let mut rng = rand::thread_rng();


    let mut file = File::open(file_name)?;

    let mut contents : String = String::new();

    match file.read_to_string(&mut contents) {
        Err(why) => panic!("Cannot read file: {}", why),
        Ok(r) => r,
    };

    let mut term = terminal::stdout();
    term.act(Action::ClearTerminal(Clear::All))?;
    term.act(Action::SetForegroundColor(Color::Green))?;
    term.act(Action::EnableRawMode)?;
    term.act(Action::MoveCursorTo(0,0))?;


    let mut key_code = wait_for_input(&term)?;

    while key_code != KeyCode::Esc {
        key_code = wait_for_input(&term)?;

        let wanted_chars = rng.gen_range(5..=10);
        let len = if contents.len() > wanted_chars { wanted_chars } else { contents.len() };
        if len == 0 { break; }
        term.write(contents[0..len].replace("\n", "\r\n").as_bytes())?;
        term.flush()?;
        contents = contents[len..].to_string();
    }
    

    //cleanup();
    term.act(Action::ResetColor)?;
    term.act(Action::DisableRawMode)?;

    Ok(())
}

fn wait_for_input<W>(term : &Terminal<W>) -> error::Result<KeyCode>
where W : Write
{
    loop {
        if let Retrieved::Event(event) = term.get(Value::Event(None))? {
            match event {
                Some(e) => {
                    match e {
                        Event::Key(k) => return Ok(k.code),
                        _ => {}
                    }
                }
                _ => {}
            };
        }
    }
}