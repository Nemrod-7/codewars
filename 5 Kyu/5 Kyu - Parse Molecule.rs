#![allow(dead_code, unused)]

extern crate regex;
extern crate thiserror;

use regex::Regex;
use thiserror::Error;
use std::collections::HashMap;

pub type Atom = (String, usize);
pub type Molecule = Vec<Atom>;

#[derive(Debug)]
pub struct ParseError {
    info: ErrType,
}
pub enum ErrType {
    Invalid,         
    Mismatch,
}
impl std::fmt::Debug for ErrType {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match *self {
            ErrType::Invalid => write!(f, "{}", " Mismatched parenthesis"),
            ErrType::Mismatch => write!(f, "{}", "Not a valid molecule"),
        }
    }
}


impl std::fmt::Display for ErrType {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "error")
    }
}

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
            '-' => if expr.chars().nth(index+1) != Some('>') { return false } ,
            '>' => if expr.chars().nth(index-1) != Some('-') { return false } ,
             _  => (),
        }
    }

    brc.is_empty()
}

pub fn parse_molecule (src: &str) -> Result<Molecule, ParseError>  {
    let mut code = tokenize(src);
    let mut hist: HashMap<String,usize> = HashMap::new();
    let brace = HashMap::from([("}","{"), ("]","["), (")","(")]);

    if !valid_braces(src) {
        return Err(ParseError{info: ErrType::Mismatch});
    }

    for i in 0..code.len() {
        let atom = &code[i].0;

        if let Ok(mul) = atom.parse::<usize>() {
            let mut j = i - 1;
            // ... the trait bound `&str: Borrow<String>` is not satisfied
            let last = &code[j].0.to_string() as &str;
            // ... cannot borrow `code` as mutable because it is also borrowed as immutable
            let next = if let Some(val) = brace.get(last) { val.to_string() } else { last.to_string() } ;
         
            loop {
                print!("{} ", code[j].0);
                code[j].1 *= mul;
                if code[j].0 == next { break }
                j -= 1;
            }
            print!("\n");
        }
    }

    for atom in code.iter() {
        *hist.entry(atom.0.clone()).or_insert(0) += atom.1;
    }

    let res = hist.into_iter().filter(|x| x.0.chars().nth(0).unwrap().is_alphabetic()).collect::<Vec<_>>();

    match res.len() {
        0 => Err(ParseError{info:ErrType::Invalid}),
        _ => Ok(res),
    }
}

fn main () {

    let src = "As2{Be4C5[BCo3(CO2)3]2}4Cu5" ;
    let res = parse_molecule(src);

    print!("{:?}\n", res);
}

