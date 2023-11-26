
fn postfix_evaluator(expr: &str) -> i64 {

    let mut stack:Vec<i64> = Vec::new();

    for tok in expr.split_whitespace() {
        match tok.parse::<i64>() {
            Ok(num) => stack.push(num),
            _ => {
                let b = stack.pop().unwrap();
                let a = stack.pop().unwrap();

                match tok {
                    "+" => stack.push(a + b),
                    "-" => stack.push(a - b),
                    "*" => stack.push(a * b),
                    "/" => stack.push(a / b),
                    _ => (),
                }
            },
        }
    }

    match stack.last() {
        Some(num) => *num, 
        None => 0,
    }
}

fn main () {

    let res = postfix_evaluator ("4 8 + 7 5 - *");
    let res = postfix_evaluator("4 8 + 6 5 - * 3 2 - 2 2 + * /");

    print!("{res}\n");


}
