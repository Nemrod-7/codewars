
extern crate regex;
use regex::Regex;

fn tokenize (src: &str) -> Vec<String> {
    let token = Regex::new("[\\+\\-]?[0-9]*[a-z]+").unwrap();
    token.captures_iter(src).map(|x| x[0].to_string()).collect::<Vec<_>>()
}
fn sort (src: Vec<char>) -> String {
    let mut src = src;
    src.sort_by(|a,b| a.cmp(b));
    src.iter().collect::<String>()
}

fn getvar (src: &str) -> (i32, String) {
    let sig = if src.find('-') == None { 1 } else { -1 };
    let num = src.chars().filter(|x| x.is_digit(10)).collect::<String>().parse::<i32>();
    let var = sort(src.chars().filter(|x| x.is_alphabetic()).collect::<Vec<_>>());

    match num {
        Ok(dig) => (sig * dig, var),
        Err(_) => (sig, var), 
    }
}

fn simplify (poly: &str) -> String {

    let os = String::new();
    let expr = tokenize(poly);

    for cell in &expr {
        let curr = getvar(&cell);
        
    }

    //print!("{:?}", expr);
    os
}

fn main () {

    let res = simplify("3x-yx+2xy-x");

}
