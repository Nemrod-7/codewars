
fn order (expr: &str) -> usize {
    match expr {
        "*" => return 2,
        "/" => return 2,
        "+" => return 1,
        "-" => return 1,
        _  => return 0,
    }
}

fn is_var (tok: &String) -> bool {
    tok.chars().last().unwrap().is_numeric()
}
fn is_num (tok: &String) -> bool {

    if tok.len() == 1 && !tok.chars().next().unwrap().is_numeric() {
        return false;
    }
    return true;
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

fn operate (a:f64, op:&str, b:f64) -> f64 {
    //print!("{} {} {}\n", a, op, b);
    match op {
        "+" => return a + b,
        "-" => return a - b,
        "*" => return a * b,
        "/" => return a / b,
         _  => return 0.0,
    }
}
fn calc (expr: &str) -> f64 {

    let size = expr.len();
    let expr:Vec<char> = expr.chars().collect();
    let mut index = 0;
    let mut stack:Vec<f64> = Vec::new();
    let mut opers:Vec<String> = Vec::new();

    let mut prev = String::from(" ");

    while index < size {

        while expr[index] == ' ' { index += 1 }

        let mut tok;

        if expr[index].is_numeric() {
            tok = getnum (&expr, &mut index);
        } else {
            tok = expr[index].to_string();

            if expr[index] == '-' && !is_var (&prev) {
                index += 1;
                tok += &getnum (&expr, &mut index);
            }
        }

        if is_num (&tok) {
            stack.push (tok.parse::<f64>().unwrap());
        } else {
            if tok == "(" {
                let sub = getsub (&expr, &mut index);
                let num = calc (&sub);
                stack.push (num);

            } else if tok != ")" {

                while !opers.is_empty() && order (opers.last().unwrap()) > order (&tok)   {
                    let op = opers.pop().unwrap();
                    let b = getstk (stack.pop()); let a = getstk (stack.pop());
                    stack.push (operate (a, &op, b));
                }
                opers.push (tok.clone());
            }
        }
        prev = tok;
        index += 1;
    }

    while !opers.is_empty() {
        let op = opers.pop().unwrap();
        let b = getstk(stack.pop()); let a = getstk(stack.pop());

        stack.push (operate (a, &op, b));
    }

    getstk (stack.pop())
}

fn main () {
    //"1 - -(-(-(-4)))"
    //"(123.45*(678.90 / (-2.5+ 11.5)-(((80 -(19))) *33.25)) / 20)";

    let expr = "1 - -(-(-(-4)))" ;
    //let res = calc (&expr);
    let a = 1;
    let b = -4;
    let op = "-";

    print! ("{} {} {} => {}\n", a,op,b, a - b);
}
