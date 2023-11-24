
#![allow(dead_code, unused)]

fn getvar (src: &str) -> (i64,String,i64) {
    let var = src.chars().filter(|x| x.is_alphabetic()).collect::<String>();
    let exp = if let Some(x) = src.chars().position(|x| x == '^') { src[x+1..].parse::<i64>().unwrap() } else { 1 };
    let mut mul = if src.find('-') == None { 1 } else { -1 };

    match src.chars().position(|x| x.is_alphabetic()) {
        Some(x) => if let Ok(num) = src[0..x].parse::<i64>() { mul = num },
        None => mul = src.parse::<i64>().unwrap(),
    }

    (mul, var, exp)
}
fn tokenize (src: &str) -> Vec<(i64,String,i64)> {
    let src = src .chars().filter(|x| x != &' ').collect::<String>().replace("+", ",+").replace("-", ",-");    
    let arg = src.split(',').collect::<Vec<_>>();

    arg.iter().filter(|x| x != &&"").map(|x| getvar(x)).collect::<Vec<_>>()
}
fn show (num: &(i64,String,i64)) -> String {
    let mut os = String::new();

    if num.1 == "" {
        os += &format!("{}", num.0);
    } else if num.0 != 0 {
        match num.0 {
            -1 => os += &format! ("-{}", num.1),
            1  => os += &format! ("{}", num.1),
            _  => os += &format! ("{}{}", num.0, num.1), 
        }

        if num.2 > 1 {
            os += &format!("^{}", num.2)
        } 
    }

    os
}
fn polynomial_product(s1: &str, s2: &str) -> String {
    let arg1 = tokenize(s1);
    let arg2 = tokenize(s2); 
    //print!("[{}] [{}]\n", s1, s2);
    let mut i = 0;
    let mut res = String::new();
    let mut prod = Vec::new();

    for t1 in &arg1 {
        for t2 in &arg2 {
            let mut nx = (t1.0 * t2.0, t1.1.to_string(),std::cmp::max (t1.2,t2.2));
            if t1.1 == t2.1 {
                nx.2 = if t1.1 == "" { 1 } else { t1.2 + t2.2 };
            } else {
                nx.1 += &t2.1;
            }

            prod.push(nx);
        }
    }

    while i < prod.len() {
        let mut j = i + 1;
        while j < prod.len() {
            if prod[i].1 == prod[j].1 && prod[i].2 == prod[j].2 {
                prod[i].0 += prod[j].0;
                prod[j] = (0, format!(" "), 0);
          //      prod.remove(j);
          //  } else {
            }
            j += 1;
        }
        i += 1;
    }

    let mut prod = prod.iter().filter(|x| x.0 != 0).collect::<Vec<_>>();
    prod.sort_by( 
        |a,b| if a.2 != b.2 { b.2.cmp(&a.2) } else if a.1 != b.1 { b.1.cmp(&a.1) } else { b.0.cmp(&a.0) }
    );

    for i in 0..prod.len() {
        //print!("{}[{}]{}\n", prod[i].0, prod[i].1, prod[i].2); 
        if prod[i].0 > 0 && i > 0 {
            res += "+";
        }

        res += &show(&prod[i]);
    }

    match res.len() {
        0 => format!("0"),
        _ => res,
    }
}

fn main () {

let a = "[34n^36+n^12+46n^23+49n^29+n^47-6n^44+n^14-n^3-n^5+n^11-6n^28-36n^19+44n^2+40n^4+3n^10-20n^48-3n^7-36n^45+18n^22-n^26-15n^16-n^40+n+2n^21+n^38+n^34+44n^24+n^43+n^33";
let b = "[-45n^18+9n^47+49n^31-n^37-n^16+n^41+20n^5+20n^42-n^19-n^32-n^49+21n^7+n^20-36n^9-n^27-n^2+21n^25-45n^43+n^17+28n^15+19-3n^35+n^13-15n-45n^48-13n^44";

    tests();


}

fn dotest(s1: &str, s2: &str, expected: &str) {
    let actual = polynomial_product(s1, s2);
    if actual != expected {
        print!("expected : {} got : {}\n", expected, actual)
    }
}
fn tests () {
    dotest("x^2", "3x - 1", "3x^3-x^2");

    dotest("v^2 - 1+3v^3", "1+v^2", "3v^5+v^4+3v^3-1");
    dotest("u^2 + 2u+1", "u + 1", "u^3+3u^2+3u+1");
    dotest("2", "4y - 4", "8y-8");
    dotest("-4r^2 + 1", "-1", "4r^2-1");
    dotest("1", "p^3", "p^3");
    dotest("1", "-1", "-1");
    dotest("0", "2 - x", "0");
    dotest("-1", "0", "0");

}

