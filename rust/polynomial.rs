
extern crate regex;
use regex::Regex;

fn tokenize (src: &str) -> Vec<String> {
    let token = Regex::new("((^-| -)?[0-9]*[a-z](\\^[0-9]+)?)|(^-| -)?[0-9]+|[+-/*]").unwrap();
    token.captures_iter(src).map(|x| x[0].to_string()).collect::<Vec<_>>()
}
fn getvar (src: &str) -> (i32,String,i32) {
    let var = src.chars().filter(|x| x.is_alphabetic()).collect::<String>();
    let exp = if let Some(x) = src.chars().position(|x| x == '^') { src[x+1..].parse::<i32>().unwrap() } else { 1 };
    let mut mul = 1;

    match src.chars().position(|x| x.is_alphabetic()) {
        Some(x) => if x > 0 { mul = src[0..x].parse::<i32>().unwrap() } ,
        None => mul = src.parse::<i32>().unwrap(),
    }
    //print!("{}{}^{}\n", mul, var, exp);
    (mul, var, exp)
}
fn show (num: (i32,String,i32)) -> String {
    let mut os = String::new();

    if num.1 == "" {
        os += &format!("{}\n", num.0);
    } else {
        if num.0 > 1 {
            os += &format!("{}", num.0)
        }
        os += &format!("{}", num.1);
        if num.2 > 1 {
            os += &format!("^{}", num.2)
        } 
    }

    os
}

fn polynomial_product(sr1: &str, sr2: &str) -> String {
    let arg1 = tokenize(sr1);
    let arg2 = tokenize(sr2);
    //print!("ex1 : {:?}\n", arg1);
    //print!("ex2 : {:?}\n", arg2);
    let mut res = String::new();
    let mut prod = Vec::new();

    for arg in &arg1 {

        if arg != "+" && arg != "-" {
            let t1 = getvar(&arg);

            for ar2 in &arg2 {
                if ar2 != "+" && ar2 != "-" {
                    let t2 = getvar(ar2);

                    let x0 = t1.0 * t2.0;
                    let mut x1 = t1.1.to_string();
                    let mut x2 = t1.2;

                    if t1.1 == t2.1 {
                        x2 = t1.2 + t2.2;
                    } else {
                        x1 += &t2.1;
                        x2 = std::cmp::max (t1.2,t2.2);
                    }

                    let nx = (x0, x1, x2);
                    prod.push(nx);
                }
            }
            // show(nu1);
        }
    }

    for t1 in prod {
       res += &show(t1);
    }
    print!("{}\n", res);
    /*
    */
    res
}

fn main () {

    polynomial_product("1", "-1"); // "-1"
       polynomial_product("-1", "0"); // "0"
       polynomial_product("1", "p^3"); // "p^3"
       /*
       polynomial_product("2", "4y - 4"); // "8y-8"
       polynomial_product("u^2 + 2u+1", "u + 1");// "u^3+3u^2+3u+1"

       polynomial_product("x^2", "3x - 1"); // "3x^3-x^2"
       polynomial_product("-4r^2 + 1", "-1"); // "4r^2 - 1"
       polynomial_product("0", "2 - x") // "0"
       polynomial_product("v^2 - 1+3v^3", "1+v^2"); // "3v^5+v^4+3v^3-1"
       */

}





