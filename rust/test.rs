#![allow(warnings)]

use std::fs::File;
use std::io::Write;

    static LOREM_IPSUM: &str =
        "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod
tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,
quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo
consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse
cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non
proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
";

fn clear_file(filename: &str) {
    let mut file = match File::create("result") {
        Err(why) => panic!("couldn't create : {}", why),
        Ok(file) => file,
    };
}

fn append(filename: &str, text: &str) {
    let mut file = match File::options().append(true).open(filename) {
        Err(why) => panic!("couldn't create : {}", why),
        Ok(file) => file,
    };

    match file.write_all(text.as_bytes()) {
        Err(why) => panic!("couldn't write to : {}", why),
        Ok(_) => println!("success"),
    }
}

fn main() {

    let filename = "result";
    let text = LOREM_IPSUM;


}
