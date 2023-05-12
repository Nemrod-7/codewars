extern crate regex;
use regex::Regex;

fn tokenize (src: &str) -> Vec<String> {
    let token = Regex::new("->|_?[0-9]+(\\.[0-9]+|_)?|_?[a-zA-Z]+_?|[{}();,]").unwrap();
    token.captures_iter(src).map(|x| x[0].to_string()).collect::<Vec<_>>()
}
fn valid_braces (expr: &str) -> bool {
    let mut brc = Vec::new();

    for ch in expr.chars() {
        match ch {
            '(' => brc.push(')'),
            '[' => brc.push(']'),
            '{' => brc.push('}'),
             _  => if Some(ch) != brc.pop() { return false },
        }
    }
    false
}
fn valid_braces1 (code :&Vec<String>) -> bool {
    let mut brc = Vec::new();

    for cell in code {
        if cell == "(" { brc.push(")") }
        else if cell == "[" { brc.push("]") }
        else if cell == "{" { brc.push("}") }
        else {
            if let Some(back) = brc.pop() {
                if back != cell { return false }
            }
        }
    }
    false
}
fn isvalid (code :&Vec<String>) -> bool {

    let size = code.len();
    let mut index = 0;
    let mut paren = 0;
    let mut brace = 0;
    let mut param = 0;
    let mut separ = 0;

    if size > 0 && code[0] == "(" { return false; }

    while index < size {
        let token = &code[index];
        let last = if index > 0 { &code[index - 1] } else { " " };

        if token.as_bytes()[0].is_ascii_digit() {
            if token.chars().any(char::is_alphabetic) {
                return false
            }
        } else if token == "(" {
            paren += 1;
        } else if token == ")" {
            paren -= 1;
        } else if token == "{" {
            brace += 1;
            if paren != 0 { param += 1 }
        } else if token == "}" {
            brace -= 1;
        } else if token == "," {
            if last == "(" { return false; }
            if paren != 0 { separ += 1; }
        } else if token == "->" {
            if last == "(" || last == "{" { return false; }
        } else {
            if paren != 0 && brace == 0 { param += 1; }
        }
        index += 1;
    }
    let last = if index > 0 { &code[index - 1] } else { " " };

    if last != ")" && last != "}" { return false }
    if paren != 0 || brace != 0 { return false }
    if param - separ >= 2 { return false }

    true
}
fn lambda (code :&Vec<String>, end: &mut usize) -> String {
    *end += 1;
    // let mut index = *end + 1;
    if code[*end] == "}" { return "(){}".to_string() }

    let mut index = *end;
    let mut separ = 0;
    let mut param = 0;
    let mut mid = index;
    let mut os = format!("(");
    let start = index;

    while code[index] != "}" {
        if code[index] == "->" { mid = index; }
        index += 1;
    }

    for it in start..index {
        // print!("[{}]", code[it]);
        if code[it] == "," {
            separ += 1;
        } else {
            if it == mid {
                separ = 0;
                os += &format!("){{");
            }
            // print!("{} {}\n", it - start, mid - start - 1);
            if it - start < mid - start {
                if separ != param { return format!("") }
                os += &format!("{}", code[it]);
                if it - start < mid - start - 1 { os += &format!(",") }
                param += 1;
            } else if code[it] != "->" {
                if separ != 0 { return format!("") }
                os += &format!("{};", code[it]);
            }
        }
    }

    *end = index;
    os + "}"
}
fn transpile (expr: &str) -> Result<String, String> {

    if expr == "{}{}{}" { return Err (format! ("Hugh?")) }
    let code = tokenize (expr);
    let size = code.len();
    let mut index = 0;
    let mut os = String::new();

    if !isvalid (&code)  { return Err (format! ("Hugh?")) }
    // print!("{:?}",code);
    while index != size {
        let tok = &code[index];
        // print! ("[{tok}]");
        if tok == "(" {
            os += &format! ("{tok}");
            index += 1;
            let mut param = Vec::new();

            while code[index] != ")" {
                if code[index] != "," {

                    if code[index] == "{" {
                        param.push(lambda (&code, &mut index))
                    } else {
                        param.push(code[index].to_string())
                    }

                    if code[index] == ")" { break }
                }
                index += 1;
            }

            for it in param {
                os += &format!("{it},")
            }

            if os.chars().last() == Some(',') { os.pop(); }

        } else if tok == "{" {
            if index >= 2 {
                if code[index-1] == ")" && code[index-2] != "(" {
                    os += &format! (",")
                }
            }
            let tmp = lambda (&code, &mut index);
            if tmp == "" { return Err (format! ("Hugh?")) }
            // print!("lamda :: {tmp}\n");
            os += &tmp;
        } else {
            os += &format! ("{tok}");

            if index+1 < size && code[index+1] != "(" {
                os += "(";
            }
        }

        index += 1;
    }

    let mut cnt = 0;
    for it in os.chars() {
        match it {
            '(' => cnt += 1,
            ')' => cnt -= 1,
            _ => (),
        }
    }

    if cnt > 0 { os += &format!(")") }

    print!(" => {os}\n");
    return Ok (os)
}

fn main () {

    transpile ("call{65}");
    // accepts("call{666}", "call((){666;})");

    // test();
}
// Add your tests here.
// See https://doc.rust-lang.org/stable/rust-by-example/testing/unit_testing.html


fn accepts(expr: &str, expected: &str) { do_test(expr, Ok(expected.to_string())); }
fn rejects(expr: &str) { do_test(expr, Err("Hugh?".to_string())); }
fn do_test(expr: &str, expected: Result<String,String>) {
    assert_eq!(transpile(expr), expected, "\nYour result (left) did not match expected output (right) for the expression:\n{expr:?}");
}

fn test() {

        accepts("042()", "042()");
        accepts("call()", "call()");
        accepts("\n \n  1(  \n )\n", "1()");
        accepts("_call(a,b)", "_call(a,b)");
        accepts("call   (    jim ,      my )", "call(jim,my)");

        accepts("{}(x)", "(){}(x)");
        accepts("call({\n})", "call((){})");

        rejects("name%1&*");
        rejects("abc9_(");
        rejects("f(42a)");
        rejects("call");
        rejects("f({)");
        rejects("f(})");
        rejects("f(){");
        rejects("{u,w,v -> x,y}{}");

        accepts("{p ->}()", "(p){}()");
        accepts("{p,  5 ->}()", "(p,5){}()");
        accepts("call(a,b){}", "call(a,b,(){})");
        accepts("call(a, b, {})", "call(a,b,(){})");
        accepts("{x\n->y}(666){4,2->0}", "(x){y;}(666,(4,2){0;})");
        accepts("call({a,b,c->d e}){\n}", "call((a,b,c){d;e;},(){})");

        // rejects("{}");
        // rejects("{}{1a 2}")

        // rejects("f()()");
        // rejects("{a,b->c}");
        // rejects("f({p,t,})");
        // rejects("f({a b -> c})");

        // accepts("{}{}", "(){}((){})");
        // accepts("call{666}", "call((){666;})");

        // accepts("{a b c}{d->e\nf}", "(){a;b;c;}((d){e;f;})");

        /*
    */
}
