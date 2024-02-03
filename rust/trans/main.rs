#![allow(dead_code, unused)]

extern crate regex;

use regex::Regex;
use std::collections::HashMap;

fn tokenize (src: &str) -> Molecule {
    let token = Regex::new("[\\(\\)\\[\\]\\{\\}]|[A-Z][a-z]?|[0-9]+").unwrap();
    token.captures_iter(src).map(|x| (x[0].to_string(), 1)).collect::<Vec<_>>()
}

fn valid_braces (expr: &str) -> bool {
    let mut brc = Vec::new();

    for index in 0.. expr.len() {
        let ch = expr.chars().nth(index).unwrap();

        match ch {
            '(' => brc.push(')'),
            '[' => brc.push(']'),
            '{' => brc.push('}'),
            ')' => if Some(ch) != brc.pop() { return false },
            '}' => if Some(ch) != brc.pop() { return false },
            ']' => if Some(ch) != brc.pop() { return false },
             _  => (),
        }
    }

    brc.is_empty()
}

pub fn parse_molecule (src: &str) {
    let mut code = tokenize(src);
    let mut hist: HashMap<String,usize> = HashMap::new();

}

fn main () {

    let src = "As2{Be4C5[BCo3(CO2)3]2}4Cu5" ;


    //print!("{:?}\n", res);
}

