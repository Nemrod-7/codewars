extern crate regex;
use regex::Regex;

use std::collections::HashMap;

struct Interpreter {
    vars: HashMap<String, f32>,
    func: HashMap<String, (String, String)>,
}

impl Interpreter {
    fn new() -> Interpreter {
        Self { vars: HashMap::new(), func: HashMap::new() }
    }

    fn format (&mut self, func: (String,String), args: Vec<String>) -> Result<String,String> {

        let vars = tokenize(&func.0);
        let body = tokenize(&func.1);
        let mut sub = String::new();
        if args.len() != vars.len() { return Err (format! ("Invalid function.")) }

        for mut cell in body {
            let exist = vars.iter().position(|x| x == &cell);

            if let Some (pos) = exist {
                match self.input(&args[pos]) {
                    Ok(Some(value)) => cell = value.to_string(),
                    _ => return Err (format! ("Invalid function.")),
                }
            }
            sub += &format! (" {}", &cell);
        }

        Ok (sub)
    }
    fn getargs (&mut self, index: &mut usize, expr: &Vec<String>) -> Vec<String> {

        let args = &self.func.get (&expr[*index]).unwrap().0;
        let nvar = tokenize (args).len();
        let mut args = Vec::new();

        while *index + 1 < expr.len() && args.len() < nvar {
            *index += 1;
            let mut arg = expr[*index].to_string();

            if self.func.contains_key (&arg) == true {
                let sub = self.getargs (index, expr).join(" ");
                arg += &format! (" {}", sub);
            }

            args.push(arg.to_string());
        }

        args
    }
    fn input (&mut self, input: &str) -> Result<Option<f32>,String> {

        let number = Regex::new("^-?[0-9]+(.[0-9]+)?$").unwrap();
        let variab = Regex::new("_?[a-zA-Z]+_?|_[0-9]+").unwrap();
        let code = tokenize (input);
        let size = code.len();

        let mut it = 0;
        let mut tree:Vec<f32> = Vec::new();
        let mut oper:Vec<String> = Vec::new();

        while it < size {
            let mut sign = 1.0;

            if is_var (&code, it) {
                sign = -1.0;
                it += 1;
            }

            let tile = &code[it];

            if number.is_match(tile) {
                tree.push (tile.parse::<f32>().unwrap() * sign);
            } else if variab.is_match(tile) {

                if code[it] == "fn" {
                    let name = code[it + 1].to_string();

                    if let Some (mid) = code.iter().position(|x| x == "=>") {
                        if self.vars.contains_key (&name) { return Err (format!("a variable {name} already exist.")) }

                        let vars = &code[2..mid];
                        let body = &code[mid+1..];
                        let mut args:HashMap<String, bool> = HashMap::new();

                        for var in vars {
                            if body.iter().position(|x| x == var) == None || args.contains_key (var) == true {
                                return Err (format!("Invalid function"))
                            }

                            args.insert (var.to_string(), true);
                        }

                        self.func.insert(name, (vars.join(" "), body.join(" ")));
                        return Ok(None);
                    } else {
                        return Err (format!("Invalid function"))
                    }
                } else if let Some (_) = code.iter().position(|x| x == "=") { // declare variable

                    match self.func.contains_key (tile) {
                        true  => return Err (format!("a variable of the same name already exist.")),
                        false => {
                            let res = self.input(&code[it+2..].join(" "));

                            if let Ok(Some(val)) = res {
                                self.vars.insert(tile.to_string(), val);
                            }
                            return res;
                        },
                    }

                } else if let Some (val) = self.vars.get (tile) { // get variable
                    tree.push (*val);
                } else if let Some (fnc) = self.func.get (tile) { // execute function
                    let fnc = fnc.clone();
                    let args = self.getargs(&mut it, &code.to_vec());
                    let sub = self.format(fnc.clone(), args);

                    match &sub {
                        Ok(sub) => {
                            match self.input (&sub) {
                                Ok(value) => tree.push(value.unwrap()),
                                Err(error) => return Err (error),
                            };
                        },
                        Err(error) => return Err (format!("{}", error)),
                    };

                } else {
                    return Err ("Unknown identifier.".to_string());
                }

            } else if tile == "(" {
                it += 1;

                let fst = it;
                let mut pile = 1;

                while pile != 0 {
                    it += 1;
                    if code[it] == ")" { pile -= 1 }
                    if code[it] == "(" { pile += 1 }
                }

                match self.input (&code[fst..it].join(" ")) {
                    Ok(res) => {
                        match res {
                            Some (value) => tree.push(value * sign),
                            None => return Err(format!("Error")),
                        }
                    },
                    Err(error) => return Err(error),
                };

            } else if order (&tile) != 0 {
                while let Some(id) = oper.last() {
                    if order (id) >= order (tile) {
                        calc (&mut tree, id);
                        oper.pop();
                    } else {
                        break
                    }
                }

                oper.push (tile.to_string());
            } else {
                return Err (format!("Invalid operator."))
            }

            it += 1;
        }

        while let Some(id) = &oper.pop() {
            calc (&mut tree, id);
        }

        match tree.len() {
            0 => Ok(None),
            1 => Ok(tree.pop()),
            _ => Err("Invalid stack size.".to_string()),
        }
    }
}

fn is_var (expr: &Vec<String>, it: usize) -> bool {

    if expr[it] != "-" { return false }
    if it == 0 { return true }
    if order (&expr[it - 1]) > 0 { return true  }
    return false;
}
fn tokenize (src: &str) -> Vec<String> {
    let token = Regex::new("=>|_?[0-9]+(\\.[0-9]+|_)?|[-+*/%()=\\[\\]]|_?[a-zA-Z]+_?").unwrap();
    token.captures_iter(src).map(|x| x[0].to_string()).collect::<Vec<_>>()
}
fn order (tok: &str) -> usize {
    if tok == "+" || tok == "-" { return 1 }
    if tok == "*" || tok == "/" || tok == "%" { return 2 }
    0
}
fn calc (tree: &mut Vec<f32>, op: &str) {

    if tree.len() > 1 {
        let b = tree.pop().unwrap();
        let a = tree.pop().unwrap();

        match op as &str {
            "+" => tree.push (a + b),
            "-" => tree.push (a - b),
            "*" => tree.push (a * b),
            "/" => tree.push (a / b),
            "%" => tree.push (a % b),
            _  => (),
        }
    }
}

fn main() {


    let mut i = Interpreter::new();

    assert_eq!(i.input("fn avg x y => (x + y) / 2"), Ok(None));
    assert_eq!(i.input("fn add x y => x + y"), Ok(None));
    assert_eq!(i.input("fn echo x => x"), Ok(None));
    assert_eq!(i.input(""), Ok(None));

    assert_eq!(i.input("1 + 1"), Ok(Some (2.0)));
    assert_eq!(i.input("2 - 1"), Ok(Some (1.0)));
    assert_eq!(i.input("2 * 3"), Ok(Some (6.0)));
    assert_eq!(i.input("8 / 4"), Ok(Some (2.0)));
    assert_eq!(i.input("7 % 4"), Ok(Some (3.0)));
    assert_eq!(i.input("4 / 2 * 3"), Ok(Some (6.0)));

    assert_eq!(i.input("x = 1"), Ok(Some (1.0)));
    assert_eq!(i.input("x"), Ok(Some (1.0)));
    assert_eq!(i.input("x = x + 3"), Ok(Some (4.0)));

    assert_eq!(i.input("add echo 5 echo 2"), Ok(Some (7.0)));
    assert_eq!(i.input("avg 2 4"), Ok(Some (3.0)));
    assert_eq!(i.input("avg 4 2 + avg 10 30"), Ok(Some (23.0)));
    assert_eq!(i.input("(8 - (4 + 2)) * 3"), Ok(Some (6.0)));

    assert!(i.input("fn x => 0").is_err());
    assert!(i.input("avg = 5").is_err());
    assert!(i.input("avg 7").is_err());
    assert!(i.input("avg 2 4 3").is_err());

    assert!(i.input("fn add x x => x + x").is_err());
    assert!(i.input("42 = 3.14").is_err());

    assert_eq!(i.input("4-6"), Ok(Some (-2.0)));

    i.input("(fn f => 1)");
    /*

     it_should_throw_an_error_when_assigning_a_value_to_an_invalid_identifier
     assertion failed: `(left == right)` (left: `Ok(..)`, right: `Err(..)`): (input: `42 = 3.14`)

    it_should_throw_an_error_when_function_is_declared_within_an_expression
    called `Option::unwrap()` on a `None` value at src/lib.rs:154:50

    */


    /*
    //assert_eq!(i.input("avg 4 2"), Ok(Some(3.0)));

    for (key,val) in i.vars {
        print!("[{}] -> {}\n",key, val);
    }

    for (key,val) in i.func {
        print!("[{}] -> {}\n",key, val);
    }
    */


    print!("\n");
}

/*

#[test]
fn conflicts() {
    let mut i = Interpreter::new();
    assert_eq!(i.input("x = 1"), Ok(Some(1.0)));
    assert_eq!(i.input("fn avg x y => (x + y) / 2"), Ok(None));


}
Footer

*/
