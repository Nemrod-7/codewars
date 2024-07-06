use std::fs::File;
use std::io::prelude::*;

fn main () -> std::io::Result<()> {

    let mut file = File::open("lbstests.txt")?;
    let mut buffer = String::new();

    file.read_to_string(&mut buffer)?;

    print!("{buffer}");



    Ok(())
}
