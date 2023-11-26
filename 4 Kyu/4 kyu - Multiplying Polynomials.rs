
#![allow(dead_code, unused)]

fn format (mat: Vec<i64>, var: char) -> String {
    let mut os = String::new();
    let mut ex = mat.len();

    while ex > 1 {
        ex -= 1;
        if mat[ex] == 0 { continue }
        if mat[ex] > 0 && os != "" { os += "+"; }

        match mat[ex] {
            -1 => os += &format!("-{var}"),
            1 => os += &format!("{var}"),
            _ => os += &format!("{}{}",mat[ex], var),
        }

        if ex > 1 { os += &format!("^{ex}"); }
    }

    if mat[0] != 0 {
        if mat[0] > 0 && os != "" { os += "+"; }
        os += &format!("{}", mat[0]);
    }

    os
}

fn getvar (src: &str) -> (i64,String,usize) {
    let var = src.chars().filter(|x| x.is_alphabetic()).collect::<String>();
    let mut exp = if let Some(x) = src.chars().position(|x| x == '^') { src[x+1..].parse::<usize>().unwrap() } else { 1 };
    let mut mul = if src.find('-') == None { 1 } else { -1 };

    match src.chars().position(|x| x.is_alphabetic()) {
        Some(x) => if let Ok(num) = src[0..x].parse::<i64>() { mul = num },
        None => {
            exp = 0;
            mul = src.parse::<i64>().unwrap();
        },
    }

    (mul, var, exp)
}
fn tokenize (src: &str) -> Vec<i64> {
    let src = src .chars().filter(|x| x != &' ').collect::<String>().replace("+", ",+").replace("-", ",-");
    let arg = src.split(',').collect::<Vec<_>>();
    let mut mat = Vec::new();

    for tok in arg.iter().filter(|x| x != &&"") {
        let nu = getvar(tok);

        while nu.2 >= mat.len() {
            mat.push(0);
        }
        mat[nu.2] = nu.0;
    }
    //arg.iter().filter(|x| x != &&"").map(|x| getvar(x)).collect::<Vec<_>>()
    mat
}

fn polynomial_product (s1: &str, s2: &str) -> String {
    if s1 == "0" || s2 == "0" { return format!("0") }

    let mat1 = tokenize(s1);
    let mat2 = tokenize(s2);
    let mut mat3 = vec![0; mat1.len() * mat2.len()];
    //print!("[{}] [{}]\n", s1, s2);
    mat3[0] = mat1[0] * mat2[0];

    for i in 1..mat1.len() {
        for j in 1..mat2.len() {
            mat3[i+j] += mat1[i] * mat2[j];
        }
    }

    for i in 1..mat2.len() {
        mat3[i] += mat1[0] * mat2[i];
    }

    for i in 1..mat1.len() {
        mat3[i] += mat1[i] * mat2[0];
    }

    //print!("{:?}\n", mat3);
    match (s1.to_string() + s2).chars().find(|x| x.is_alphabetic()) {
        Some(x) => format(mat3, x),
        None => format(mat3, 'x'),
    }
}

fn main () {

    let mat = tokenize("2x^3-2x+3x^2+4");
    let var = 'c';

    dotest("-1", "0", "0");
    dotest("0", "2 - x", "0");
    dotest("1", "-1", "-1");
    dotest("1", "p^3", "p^3");
    dotest("x^2", "3x - 1", "3x^3-x^2");
    dotest("2", "4y - 4", "8y-8");
    dotest("-4r^2 + 1", "-1", "4r^2-1");
    dotest("u^2 + 2u+1", "u + 1", "u^3+3u^2+3u+1");
    dotest("v^2 - 1+3v^3", "1+v^2", "3v^5+v^4+3v^3-1");


}

fn dotest(s1: &str, s2: &str, expected: &str) {
    let actual = polynomial_product(s1, s2);
    if actual != expected {
        print!("expected : {} got : {}\n", expected, actual)
    }
}
fn tests () {


}
