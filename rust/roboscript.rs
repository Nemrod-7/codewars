#![allow(dead_code,unused)]
use std::iter::Peekable;

fn tokenize<'a>( program : &'a str) -> Vec<String> {
    let mut tokens : Vec<String> = vec![];
    let mut iter = program.chars().peekable();

    loop {
        match iter.peek() {
            Some(&c) => match c {
                'a'..='z'|'A'..='Z' => {
                    let mut tmp = String::new();
                    while iter.peek().is_some() && iter.peek().unwrap().is_alphabetic() {
                        tmp.push(iter.next().unwrap());
                    }
                    tokens.push(tmp);
                },
                '0'..='9' => {
                    let mut tmp = String::new();
                    while iter.peek().is_some() && iter.peek().unwrap().is_numeric() {
                        tmp.push(iter.next().unwrap());
                    }
                    tokens.push(tmp);
                },
                ' ' => { iter.next(); },
                _ => {
                    tokens.push(iter.next().unwrap().to_string());
                },
            },
            None => break
        }
    }

    tokens
}
fn highlight(code :&str) -> String {

    let mut code = code.chars().peekable();
    let mut os = String::new();

    while let Some(ch) = code.next() {
        let mut com = format!("{ch}");

        match ch {
            '0'..='9' => {
                while let Some(nxt) = code.peek() {
                    match nxt {
                        '0'..='9' => com.push(code.next().unwrap()),
                        _ => break,
                    }
                }

                os += &format!("<span style=\"color: orange\">{com}</span>");
            }
            _ => {
                while code.peek() == Some(&ch) {
                    com.push( code.next().unwrap()) ;
                }

                match ch {
                    '(' => os += &format!("{com}"),
                    ')' => os += &format!("{com}"),
                    'F' => os += &format!("<span style=\"color: pink\">{com}</span>"),
                    'L' => os += &format!("<span style=\"color: red\">{com}</span>"),
                    'R' => os += &format!("<span style=\"color: green\">{com}</span>"),
                    _ => unreachable!(),    
                }
                os += "\n";
            },
        }
    }

    print!("{os}");
    os
}

fn getnum (code : &mut Peekable<impl Iterator<Item = char>>) -> i32 {
    let mut tmp = String::new(); 

    while let Some(dig) = code.peek() {
        match dig {
            '0'..='9' => tmp.push(code.next().unwrap()),
            _ => break,
        }
    }

    match tmp.parse::<i32>() {
        Ok(num) => { num },
        Err(_) => { 1 },
    }
}
fn format(path:  &Vec<(i32,i32)>) -> String {

    let minx = path.iter().min_by_key(|x| x.0).unwrap().0;
    let maxx = path.iter().max_by_key(|x| x.0).unwrap().0;
    let miny = path.iter().min_by_key(|x| x.1).unwrap().1;
    let maxy = path.iter().max_by_key(|x| x.1).unwrap().1;

    let offx = std::cmp::min(minx,0).abs(); 
    let offy = std::cmp::min(miny,0).abs();
    let width = minx.abs() + maxx.abs() + 1;
    let height = miny.abs() + maxy.abs() + 1;

    let mut grid = vec![vec![' '; width as usize]; height as usize];

    path.iter()
        .for_each(|x| { grid[(x.1 + offy) as usize][(x.0 + offx) as usize] = '*'; });

    grid.iter()
        .map(|line| format!("{}\r\n", line.iter().collect::<String>()))
        .collect::<String>()
}
pub fn execute (code: &str) -> String {

    let x = [1,0,-1,0];
    let y = [0,1,0,-1];

    let mut ix = 0;
    let mut code = code.chars().peekable();
    let mut hist = vec![(0,0)]; 

    while let Some(curr) = code.next() {
        let coef = getnum(&mut code);

        (0..coef).for_each(|_|
            match curr {
                'F' => if let Some(last) = hist.last() {
                    hist.push((x[ix] + last.0, y[ix] + last.1)) 
                },
                'R' => ix = if ix == 3 { 0 } else { ix + 1 },
                'L' => ix = if ix == 0 { 3 } else { ix - 1 },
                _ => {},
            }
        );
    }

    //print!("{:?}\n", hist);
    let mut res = format(&hist);
    res.pop(); res.pop();
    res
}

macro_rules! expect_equal {
    ($actual:expr, $expected:expr $(,)*) => {{
        let actual = $actual;
        let expected = $expected;
        assert_eq!(actual, expected, "\ngot:\n{}\n\nexpected:\n{}\n", actual, expected);
    }};
}

fn examples_in_description() {
    expect_equal!(execute(""), "*");
    expect_equal!(execute("FFFFF"), "******");
    expect_equal!(
        execute("FFFFFLFFFFFLFFFFFLFFFFFL"),
        "******\r\n*    *\r\n*    *\r\n*    *\r\n*    *\r\n******",
    );
    expect_equal!(
        execute("LFFFFFRFFFRFFFRFFFFFFF"),
        "    ****\r\n    *  *\r\n    *  *\r\n********\r\n    *   \r\n    *   ",
    );
    expect_equal!(
        execute("LF5RF3RF3RF7"),
        "    ****\r\n    *  *\r\n    *  *\r\n********\r\n    *   \r\n    *   ",
    );
}
fn main() {

     examples_in_description();

    //  print!("{} {} {} {} {}\n", minx, maxx, miny, maxy, offy);
    //  print!("width : {} height : {} \n", width, height);

    //print!("{:?}", curr);

}
