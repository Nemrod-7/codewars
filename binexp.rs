#![allow(warnings, unused)]

fn binomial (size: usize) -> Vec<Vec<i32>> {
    let mut binom = vec![vec![1; size + 1]; size + 1];

    (0..=size).for_each( |i| (1..i).for_each(
            |j| binom[i][j] = binom[i-1][j-1] + binom[i-1][j]
       )
   );

    binom
}
fn expand (expr: &str) {

    let mid = expr.find('^').unwrap();
    let n = expr[mid + 1..].parse::<usize>().unwrap();
    let terms = &expr[1..mid-1];

    let binom = binomial(n + 1);
    let mut x:&str = &"";
    let mut a:i32 = 0;
    let mut b:i32 = 0;

    for i in (0..terms.len()).rev() {
        if let Some(ch) = terms.chars().nth(i) {
            if ch == '+' || ch == '-' {
                let num = terms[..i-1].chars().filter(|x| x.is_digit(10)).collect::<String>();
                let sign = if &terms[..1] == "-" { 1 } else { -1 };
                x = &terms[i-1..i];

                a = if num != "" { num.parse::<i32>().unwrap() } else { 1 } * sign;
                b = terms[i..].parse::<i32>().unwrap();
                break;
            }
        }
    }

    for k in 0..=n {
        let mul = binom[n][k] * a.pow((n - k) as u32) * b.pow(k as u32);

        if k > 0 && mul > 0 {
            print!("+");
        } 

        if n - k == 0 {
            print!("{mul}");
        } else if n - k != 1 {
            match mul {
                -1 => { print!("-{x}"); },
                1 => { print!("{x}"); },
                _ => { print!("{mul}{x}"); },
            }

            print!("^{}", n - k);
        }
    }

    print!("\n");
}

fn main () {


    expand ("(x+1)^2");      // returns "x^2+2x+1"
    expand ("(p-1)^3");      // returns "p^3-3p^2+3p-1"
    expand ("(2f+4)^6");     // returns "64f^6+768f^5+3840f^4+10240f^3+15360f^2+12288f+4096"
    expand ("(-2a-4)^0");    // returns "1"
    expand ("(-12t+43)^2");  // returns "144t^2-1032t+1849"
    expand ("(-x-1)^2");     // returns "x^2+2x+1
    expand("(-z-12)^1");
    /*
    */

    let size = 5;
    let mut binom = vec![vec![1; size + 1]; size + 1];


    print!("\nend\n");

}
