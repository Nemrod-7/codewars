
fn getterm (str: &str) -> (i32, String) {
    let num = str.chars().filter(|x| x.is_digit(10)).collect::<String>() ;
    let val = if num != "" { num.parse::<i32>().unwrap() } else { 1 };
    let var = str.chars().filter(|x| x.is_alphabetic()).collect::<String>();
    let sign = if str.find('-') == None { 1 } else { -1 };
    
    (sign * val, var)
}

fn expand (expr: &str) {

    let mid = expr.find('^').unwrap();
    let ex:i32 = expr[mid + 1..].parse::<i32>().unwrap();
    let term = &expr[1..mid-1];

    let mut t1 = (0i32, "".to_string());
    let mut t2 = (0i32, "".to_string());

    for i in (0..term.len()).rev() {
        if let Some(ch) = term.chars().nth(i) {

            if ch == '+' || ch == '-' {
                t1 = getterm(&term[..i]);
                t2 = getterm(&term[i..]);
                break;
            }
        }
    }

    let size = ex as usize ;
    let mut binom = vec![vec![1; size + 1]; size + 1];

    for i in 0..=size {
        for j in 1..i {
            binom[i][j] = binom[i-1][j-1] + binom[i-1][j];
        }
    }

    for i in 0..binom[size].len() {
        let mul = binom[size][i];
        let n1 = t1.0.pow((size - i) as u32); 
        let n2 = t2.0.pow(i as u32); 


    }

    //print!("{:?}\n", binom[size]);
    //print!("{} ^ {}\n", term, ex);
}

fn main () {

    expand("(x+1)^2");      // returns "x^2+2x+1"
    expand("(p-1)^3");      // returns "p^3-3p^2+3p-1"
    expand("(2f+4)^6");     // returns "64f^6+768f^5+3840f^4+10240f^3+15360f^2+12288f+4096"
    expand("(-2a-4)^0");    // returns "1"
    expand("(-12t+43)^2");  // returns "144t^2-1032t+1849"
    expand("(-x-1)^2");     // returns "x^2+2x+1
                            //expand("(r+0)^203");    // returns "r^203"



}
