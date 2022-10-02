fn order (expr: char) -> usize {
    if expr == '*' || expr == '/' { return 2 }
    if expr == '+' || expr == '-' { return 1 }
    0
}
fn operate (a:f64, op:char, b:f64) -> f64 {
    match op {
        '+' => return a + b,
        '-' => return a - b,
        '*' => return a * b,
        '/' => return a / b,
        _  => return 0.0,
    }
}

fn is_var (expr: &Vec<char>, it: usize) -> bool {

    if it == 0 { return true }
    if order (expr[it - 1]) > 0 { return true; }

    return false;
}

fn getstk (n:Option<f64>) -> f64 {
    match n {
        Some (n) => return n,
        None => return 0.0,
    }
}
fn getnum (expr: &Vec<char>, it: &mut usize) -> String {
    let mut num:String = String::new();
    let mut index = *it;

    while index < expr.len() && (expr[index].is_digit(10) || expr[index] == '.') {
        num.push (expr[index]);
        index += 1;
    }

    *it = index - 1;

    num
}
fn getsub (expr: &Vec<char>, it: &mut usize) -> String {

    let mut sub:String = String::new();
    let mut index = *it + 1;
    let mut pile = 1;

    while index < expr.len() {
        if expr[index] == '(' { pile += 1 }
        if expr[index] == ')' { pile -= 1 }
        if pile == 0 { break }
        sub.push (expr[index]);
        index += 1
    }
    *it = index;

    sub
}

fn calc (expr: &str) -> f64 {

    let expr:Vec<char> = expr.chars().filter (|x| !x.is_whitespace()).collect();
    let size = expr.len();
    let mut index = 0;
    let mut sign = 1.0;
    let mut stack:Vec<f64> = Vec::new();
    let mut opers:Vec<char> = Vec::new();

    while index < size {

        let tok = expr[index];
        let mut op = order (tok);

        if tok == '-' && is_var (&expr, index) {
            sign = -1.0;
            op = 0;
        }

        if tok == '(' {
            let sub = getsub (&expr, &mut index);
            stack.push (calc (&sub) * sign);
            sign = 1.0;
        } else if expr[index].is_numeric() {
            let num = getnum (&expr, &mut index).parse::<f64>().unwrap();
            stack.push (num * sign);
            sign = 1.0;
        } else if op > 0 {

            while !opers.is_empty() && order (*opers.last().unwrap()) >= order (tok)   {
                let op = opers.pop().unwrap();
                let b = getstk (stack.pop()); let a = getstk (stack.pop());
                print! ("{} {} {}\n", a, op, b);
                stack.push (operate (a, op, b));
            }
            opers.push (tok.clone());

            print! ("[{}]", tok);
        }

        index += 1;
    }

    while !opers.is_empty() {
        let op = opers.pop().unwrap();
        let b = getstk(stack.pop()); let a = getstk(stack.pop());
        stack.push (operate (a, op, b));
    }

    getstk (stack.pop())
}
