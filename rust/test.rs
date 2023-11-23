#![allow(dead_code, unused)]

fn getvar (src: &str) -> (i32,String,i32) {
    let sig = if src.find('-') == None { 1 } else { -1 };
    let var = src.chars().filter(|x| x.is_alphabetic()).collect::<String>();
    let exp = if let Some(x) = src.chars().position(|x| x == '^') { src[x+1..].parse::<i32>().unwrap() } else { 1 };
    let mut mul = 1;

    match src.chars().position(|x| x.is_alphabetic()) {
        Some(x) => if x > 0 { mul = src[0..x].parse::<i32>().unwrap() } ,
        None => mul = src.parse::<i32>().unwrap(),
    }
    //print!("{}{}^{}\n", mul, var, exp);
    (sig * mul, var, exp)
}
fn tokenize (src: &str) {
    let src = src .chars().filter(|x| x != &' ').collect::<String>().replace("+", ",+").replace("-", ",-");    
    let arg = src.split(',').collect::<Vec<_>>();

}

fn main() {

    let src = "v^2 - 1+3v^3";

    let src = src .chars().filter(|x| x != &' ').collect::<String>().replace("+", ",+").replace("-", ",-");    
    let arg = src.split(',').collect::<Vec<_>>();
    let arg = arg.iter().map(|x| getvar(x)).collect::<Vec<_>>();

    //print!("{:?}\n", arg);
    //show(getvar(t1));
}
