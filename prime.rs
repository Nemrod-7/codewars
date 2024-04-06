
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

impl Simplexer {
    fn new(txt: &str) -> Self {
        let ext = Vec::new();

        Simplexer {
            code: ext, index: 0 
        }
    }

    fn identify(cell: &str) -> Token {

        Token::new(cell, TokenKind::Identifier)
    }
}

impl Iterator for Simplexer {
    type Item = Token;

    fn next(&mut self) -> Option<Self::Item> {

        match self.index < self.code.len() {
            true => Some( Simplexer::identify(&self.code[self.index])),
            false => None,
        }
    }
}

fn main () {

    let mut curr = Simplexer::new("this is a test");

    let num = curr.next();


}
