#![allow(dead_code,unused)]

use std::fs;
use std::io::{BufRead, BufReader};
use std::collections::HashMap;

fn loadtable() -> HashMap<String,String> {

    let mut ELEMENTS:HashMap<String, String> = HashMap::new();

    if let Ok(file) = fs::File::open("table") {
        let reader = BufReader::new(file);

        // Read the file line by line using the lines() iterator from std::io::BufRead.
        for (_, line) in reader.lines().enumerate() {
            let line = line.unwrap(); 
            let token = line.split(" ").map(|x| x.chars().filter(|&x| !x.is_whitespace()).collect::<String>()).collect::<Vec<_>>();

            if token.len() > 4 {
                ELEMENTS.insert(token[2].to_string(), token[1].to_string());
            }
        }
    }

    ELEMENTS
}

fn elementalForms (word:&str) -> Vec<Vec<String>> {

    let ELEMENTS = loadtable();
    let word = word.to_lowercase();
    let mut res = Vec::new();
    let mut stack = vec![( "".to_string(), vec![])];

    while let Some((curr, hist)) = stack.pop() {

        if curr == word {
            print!("{:?}\n", hist);
            res.push(hist.to_vec());
        }

        for elt in ELEMENTS.iter() {
            let next = format!("{}{}", curr, elt.0.to_lowercase());

            if next.len() <= word.len() && next == &word[0..next.len()] {
                let mut nxh = hist.to_vec();
                nxh.push(format!("{} ({})", elt.1, elt.0));

                stack.push((next, nxh));
            }
        }
    }

    res
}

fn main()  {

    //elementalForms("snack");

    elementalForms("");



}
