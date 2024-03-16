#![allow(warnings, unused)]

use num::bigint::{BigInt};

fn getterm (str: &str) -> (i32, Option<char>) {
    let num = str.chars().filter(|x| x.is_digit(10)).collect::<String>() ;
    let val = if num != "" { num.parse::<i32>().unwrap() } else { 1 };
    let sign = if str.find('-') == None { 1 } else { -1 };
    let var = str.chars().find(|x| x.is_alphabetic());

    (sign * val, var)
}
fn formexponent (var:char, ex:usize) -> String {
      match ex {
        0 => format!(""),
        1 => format!("{var}"),
        _ => format!("{var}^{ex}"),
    }
}
fn binomial(size: usize) -> Vec<Vec<BigInt>> {
    let mut tri = vec![vec![BigInt::from(1); size + 1]; size + 1];

    (0..=size).for_each(
        |i| (1..i).for_each(
            |j| tri[i][j] = tri[i-1][j].clone() + tri[i-1][j-1].clone()
        )
    );

    tri
}

fn expand (expr: &str) -> String {
    let zero = BigInt::from(0);
    let mid = expr.find('^').unwrap();
    let exp = expr[mid + 1..].parse::<usize>().unwrap();
    let term = &expr[1..mid-1];
    let binom = binomial(exp);

    let mut res = String::new();
    let mut v0 = [(0i32, None);2];

    for i in (0..term.len()).rev() {
        if let Some(ch) = term.chars().nth(i) {
            if ch == '+' || ch == '-' {
                v0[0] = getterm(&term[..i]);
                v0[1] = getterm(&term[i..]);
                break;
            }
        }
    }

    print!("{:?} {:?}\n", v0[0], v0[1]);
    for i in 0..binom[exp].len() {
        let sig = [exp-i, i];
        let mul = (0..2).fold(binom[exp][i].clone(), |mul, x| mul * BigInt::from(v0[x].0).pow(sig[x] as u32));
        let term = (0..2).filter(|&x| v0[x].1 != None).map(|x| formexponent(v0[x].1.unwrap(), sig[x])).collect::<String>();

        if mul != zero {
            if i > 0  && mul > zero {
                res += &format!("+");
            }
            if term == "" || (mul > BigInt::from(1) || mul < BigInt::from(-1)) {
                res += &format!("{}", mul);
            }

            res += &format!("{}", term);
        }
    }

    print!("{res}\n");
    res
}

fn main () {


   // expand ("(x+1)^2");      // returns "x^2+2x+1"
   // expand ("(p-1)^3");      // returns "p^3-3p^2+3p-1"
   // expand ("(2f+4)^6");     // returns "64f^6+768f^5+3840f^4+10240f^3+15360f^2+12288f+4096"
   // expand ("(-2a-4)^0");    // returns "1"
   // expand ("(-12t+43)^2");  // returns "144t^2-1032t+1849"
   // expand ("(-x-1)^2");     // returns "x^2+2x+1

   // expand ("(-x+1)^1");

    expand("(-z-12)^1");

    // expand ("(r+0)^203");    // returns "r^203"
    //expand ("(94y+61)^10");
    //9223372036854775807y^10+349526829359456189440y^9+1020692709140114085120y^8+1766305113689275438080y^7+2005883732833299500160y^6+1562028608972314078848y^5+844714052724389705760y^4+313237429885639951680y^3+76226661793978871220y^2+10992497327264091600y+713342911662882560

    print!("\n");
}
