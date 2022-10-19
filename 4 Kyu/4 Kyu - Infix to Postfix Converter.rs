fn order (tok: &char) -> usize {
    if tok == &'+' || tok == &'-' { return 1 }
    if tok == &'*' || tok == &'/' || tok == &'^' { return 2 }
    0
}

fn to_postfix(infix: &str) -> String {

    let mut oper:Vec<char> = Vec::new();
    let mut postfix = String::new();

    for c in infix.chars() {
        if c.is_digit(10) {
            postfix.push(c)
        } else if c == '^' {
            oper.push (c)
        } else if c == '(' {
            oper.push (c)
        } else if c == ')' {

            while let Some(id) = oper.last() {
                if id != &'(' {
                    postfix.push (oper.pop().unwrap())
                } else {
                    break
                }
            }

            oper.pop();
        } else {

            while let Some(id) = oper.last() {
                if order (id) >= order (&c) {
                    postfix.push (oper.pop().unwrap());
                } else {
                    break
                }
            }

            oper.push(c)
        }
    }

    while let Some(id) = oper.pop() {
        postfix.push(id)
    }

    postfix
}
