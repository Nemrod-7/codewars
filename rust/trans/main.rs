#![allow(dead_code, unused)]
extern crate regex;
use regex::Regex;

pub struct Token {
    pub text: String,
    pub kind: TokenKind,
}

impl Token {
    pub fn new(text: &str, kind: TokenKind) -> Self {
        Token {
            text: text.to_string(),
            kind: kind,
        }
    }
}

pub enum TokenKind {
    Integer, Boolean, String, Operator, Keyword, Whitespace, Identifier,
}

struct Simplexer {
    code: Vec<String>,
    index: usize,
}

fn tokenize (src: &str) -> Vec<String> {
    let token = Regex::new("[0-9]+|_?[a-zA-Z0-9]+|[-*+/%()=]|[ \t\n]+|(\"[^\"]+\")").unwrap();
    token.captures_iter(src).map(|x| x[0].to_string()).collect::<Vec<_>>()
}

impl Simplexer {

    fn new(expr: &str) -> Self {
        Simplexer { code: tokenize(expr), index: 0 }
    }

    fn identify(tok: &str) -> TokenKind {
        if Regex::new("^[0-9]+$").unwrap().is_match(&tok) {
            TokenKind::Integer
        } else if Regex::new("[+-/*%()=]").unwrap().is_match(&tok) {
            TokenKind::Operator
        } else if Regex::new("^true$|^false$").unwrap().is_match(&tok) {
            TokenKind::Boolean
        } else if Regex::new("if|else|for|while|return|func|break").unwrap().is_match(&tok) {
            TokenKind::Keyword
        } else if Regex::new("[ \t\n]+").unwrap().is_match(&tok) {
            TokenKind::Whitespace
        } else if Regex::new("\".*\"").unwrap().is_match(&tok) {
            TokenKind::String
        } else {
            TokenKind::Identifier
        }
    }
}

impl Iterator for Simplexer {
    type Item = Token;

    fn next(&mut self) -> Option<Self::Item> {

        if self.index < self.code.len() {
            let expr = &self.code[self.index];
            self.index += 1;
            Some(Token::new(expr, Simplexer::identify(expr)))
        } else {
            None
        }
    }
}



fn main () {


    /*
       integer:     Any sequence of one or more decimal digits (leading zeroes allowed, no negative numbers)
       boolean:     Any of the following words: [true, false]
       string:      Any sequence of zero or more characters surrounded by "double quotes"
       operator:    Any of the following characters: [+, -, *, /, %, (, ), =]
       keyword:     Any of the following words: if, else, for, while, return, func, break
       whitespace:  Any sequence of the following characters: [' ', '\t', '\n'] - Consecutive whitespace should be collapsed into a single token
       identifier:  Any sequence of alphanumeric characters, as well as '_' and '$' - Must not start with a digit - Make sure that keywords and booleans aren't matched as identifiers
       */


    let txt = "\"hello\nthere\"\"Commander Spock\"";
    let code = tokenize(txt);


    for cell in code {
        print!("[{cell}]");
    }



}


