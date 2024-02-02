#![allow(dead_code, unused)]

extern crate regex;
use regex::Regex;

use thiserror::Error;
use std::collections::HashMap;

pub type Atom = (String, usize);
pub type Molecule = Vec<Atom>;

#[derive(Error, Debug)]
#[derive(Debug)]
pub struct ParseError {
    info: ErrType,
}
pub enum ErrType {
    ValidErr,            //"Not a valid molecule",
    MismatchErr,         //"Mismatched parenthesis",
    NoErr,               //"ok"
}
impl std::fmt::Debug for ErrType {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match *self {
            ErrType::ValidErr => write!(f, "{}", " Mismatched parenthesis"),
            ErrType::MismatchErr => write!(f, "{}", "Not a valid molecule"),
            _ => write!(f, "{}", "no_err"),
        }
    }
}
impl ParseError {
    pub fn new(information: &str) -> ParseError {
        ParseError { info: get_err_info(information) }
    }
}

pub fn get_err_info(s: &str) -> ErrType {

    let v = _vec(&s);
    let sum_bracket_1 = &v.iter()
        .filter(|x| ["(".to_string(), ")".to_string()].contains(&x))
        .collect::<Vec<_>>()
        .len();

    let sum_bracket_2 = &v.iter()
        .filter(|x| ["[".to_string(), "]".to_string()].contains(&x))
        .collect::<Vec<_>>()
        .len();
    if sum_bracket_1 % 2 != 0 || sum_bracket_2 % 2 != 0 {
        return ErrType::MismatchErr;
    }
    //pie problem
    let mut c = 0;
    let mut _chars: Vec<String> = Vec::new();
    ("abcdefghijklmnopqrstuvwxyz")
        .chars()
        .into_iter()
        .map(|x| {
            _chars.push(x.to_string());
            x
        })
        .collect::<Vec<_>>();

    for e in v {
        if _chars.contains(&(e.to_lowercase())) {
            c += 1;
        } else {
            c = 0;
        }
        if c >= 3 {
            return ErrType::ValidErr;
        }
    }
    return ErrType::NoErr;
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
fn tokenize2 (src: &str) -> Molecule {
    let token = Regex::new("[\\(\\)\\[\\]]|[A-Z][a-z]?|[0-9]").unwrap();
    token.captures_iter(src).map(|x| (x[0].to_string(), 1)).collect::<Vec<_>>()
}
pub fn parse_molecule (src: &str) -> Molecule {
    let mut code = src.chars().map(|x| (x.to_string(), 1)).collect::<Vec<Atom>>();
    let mut map: HashMap<String,usize> = HashMap::new();
    let mut index = 0;

    while index != code.len() {
        let cell = &code[index].0;

        if let Ok(num) = cell.parse::<i32>() {
            let mut it = index - 1;
            let mut search:String = String::new();
            let last = format!("{}", code[it].0);

            match &code[it].0 as &str {
                ")" => search = format!("("),
                "]" => search = "[".to_string(),
                _ => search = code[it].0.clone(),
            }

            loop {
                code[it].1 *= num;

                if code[it].0 == search { break }
                it -= 1;
            }
        }

        index += 1;
    }

    for it in code {
        if it.0.chars().all(|x| x.is_alphabetic()) {

            if map.contains_key(&it.0) ==  false {
                map.insert(it.0, it.1);
            } else {
                *map.entry(it.0.to_owned()).or_default() += it.1;
            }
        }
    }

    //print!("{:?}\n", map);
    map.iter().map(|x| (x.0.clone(), x.1.clone())).collect::<Vec<_>>()
}

fn main () {

    let res = parse_molecule("K4[ON(SO3)2]2");

    let src = "On4H5";


}
