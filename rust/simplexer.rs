
//#[derive(Debug)]
pub struct Token {
    pub text: String,
    pub kind: TokenKind,
}

impl Token {
    pub fn new(text: &str, kind: TokenKind) -> Self {
        Token {
            text: text.to_string(),
            kind:kind,
        }
    }
}

pub enum TokenKind {
    Integer, Boolean, String, Operator, Keyword, Whitespace, Identifier,
}


fn search (ve: &Vec<&str>, expr: &str) -> bool {

    match ve.iter().find(|&x| *x == expr) {
        Some(_) => true,
        None => false,
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

    let src = "id1 + id2 = 245 \"operation\" ";

    let _txt = src.chars().collect::<Vec<_>>();

    let boolean = vec!["true","false"];
    let operator = vec!["+","-","*","/","%","(",")","="];
    let keyword = vec!["if","else","for","while","return","func","break"];

    let token = ["123", "tpok18", "false", "="];


    for tok in token {


        if search(&operator, tok) {
            print!("oper: {}\n", tok);
        } else if search(&boolean, tok) {
            print!("bool: {}\n", tok);
        } else if search(&keyword, tok) {
            print!("key : {}\n", tok);
        }


    }


}
