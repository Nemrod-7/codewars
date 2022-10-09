extern crate regex;
use regex::Regex;

use std::collections::HashMap;

pub struct Interpreter {
    pub vars: HashMap<String, f32>,
    pub func: HashMap<String, (String, String)>,
}

impl Interpreter {
    fn new() -> Interpreter {
        Interpreter { vars: HashMap::new(), func: HashMap::new() }
    }

    fn format (&mut self, func: (String,String), args: Vec<String>) -> Vec<String> {

        let vars = tokenize(&func.0);
        let body = tokenize(&func.1);
        let mut sub = Vec::new();

        if args.len() != vars.len() { panic! ("Invalid func argument") }

        for mut cell in body {
            let exist = vars.iter().position(|x| x == &cell);

            if let Some (pos) = exist {
            //    cell = self.interpret(&tokenize (&args[pos])).unwrap().to_string();
            }
            sub.push (cell)
        }

        sub
    }
    fn getargs (&mut self, index: &mut usize, expr: &Vec<String>) -> Vec<String> {

        let args = &self.func.get (&expr[*index]).unwrap().0;
        let nvar = tokenize (args).len();

        let mut args = Vec::new();

        while args.len() < nvar {
            *index += 1;
            let mut arg = expr[*index].to_string();

            if let Some (fnc) = self.func.get (&arg) {
                let sub = self.getargs (index, expr).join(" ");
                arg += &format! (" {}", sub);
            }

            args.push(arg.to_string());
        }

        args
    }

    fn interpret (&mut self, code: &[String]) -> Result<Option<f32>,String> {

        let number = Regex::new("^-?[0-9]+(.[0-9]+)?$").unwrap();
        let variab = Regex::new("_?[a-zA-Z]+_?|_[0-9]+").unwrap();
        let size = code.len();

        let mut it = 0;
        let mut tree:Vec<f32> = Vec::new();
        let mut oper:Vec<String> = Vec::new();

        while it < size {
            let tile = &code[it];

            /*
            if number.is_match(tile) {
                tree.push (tile.parse::<f32>().unwrap());
            } else if variab.is_match(tile) {

                if let Some(_pos) = code.iter().position(|x| x == "=") { // declare var
                    if self.func.contains_key (tile) { panic! ("a variable of the same name already exist.") }
                    let val = self.interpret(&code[it+2..]);

                    self.vars.insert(tile.to_string(), val.unwrap());
                    return val;
                } else if let Some (val) = self.vars.get (tile) { // get var
                    tree.push (*val);
                } else if let Some (fnc) = self.func.get (tile) { // get func
                    let fnc = fnc.clone();

                    let args = self.getargs(&mut it, &code.to_vec());
                    let sub = self.format(fnc.clone(), args);

                    tree.push(self.interpret (&sub).unwrap());

                } else {
                    panic! ("Unknown identifier.")
                }

            } else if tile == "(" {
                it += 1;

                if let Some (len) = code[it..size].iter().position(|x| x == ")") {
                    let sub = &code[it..it+len];
                    tree.push(self.interpret (&sub).unwrap());
                    it += len;
                }

            } else if tile != ")" {

                while let Some(id) = oper.last() {
                    if order (id) >= order (tile) {
                        let b = tree.pop().unwrap();
                        let a = tree.pop().unwrap();
                        tree.push (calc (a,id,b));
                        oper.pop();
                    } else {
                        break
                    }
                }

                oper.push (tile.to_string());
            }
            */
            it += 1;
        }
        while let Some(id) = &oper.pop() {

            let b = tree.pop().unwrap();
            let a = tree.pop().unwrap();
            tree.push (calc (a,id,b));
        }

        Ok(tree.pop())
    }

    fn input (&mut self, input: &str) -> Result<Option<f32>, String> {

        let code = tokenize (input);
        let size = code.len();
        if size == 0 { return Err ("Empty expression.".to_string());}

        if code[0] == "fn" {
            let name = code[1].to_string();

            if let Some (mid) = code.iter().position(|x| x == "=>") {
                if self.vars.contains_key (&name) { return Err ("a variable of the same name already exist.".to_string()) }

                let vars = &code[2..mid];
                let body = &code[mid+1..];

                for var in vars {
                    if body.iter().position(|x| x == var) == None {
                        return Err ("Invalid function".to_string());
                    }
                }

                self.func.insert(name, (vars.join(" "), body.join(" ")));
                return Ok(None);
            } else {
                return Err ("Invalid function".to_string());
            }
        }

        self.interpret(&code)
    }
}

fn tokenize (src: &str) -> Vec<String> {
    let token = Regex::new("=>|(_|-)?[0-9]+(\\.[0-9]+|_)?|[-+*/%()=\\[\\]]|_?[a-zA-Z]+_?").unwrap();
    token.captures_iter(src).map(|x| x[0].to_string()).collect::<Vec<_>>()
}
fn order (tok: &str) -> usize {
    if tok == "+" || tok == "-" { return 1 }
    if tok == "*" || tok == "-" || tok == "%" { return 2 }
    0
}
fn calc (a: f32, id: &str, b: f32) -> f32 {
    match id as &str {
        "+" => return a + b,
        "-" => return a - b,
        "*" => return a * b,
        "/" => return a / b,
        "%" => return a % b,
        _  => panic!("unknown operator"),
    }
}

fn main() {


    let mut i = Interpreter::new();

    assert_eq!(i.input("fn avg x y => (x + y) / 2"), Ok(None));
    assert_eq!(i.input("fn add x y => x + y"), Ok(None));
    assert_eq!(i.input("fn echo x => x"), Ok(None));
    assert_eq!(i.input("1 + 1"), Ok(Some (2.0)));
    assert_eq!(i.input("2 - 1"), Ok(Some (1.0)));
    assert_eq!(i.input("2 * 3"), Ok(Some (6.0)));
    assert_eq!(i.input("8 / 4"), Ok(Some (2.0)));
    assert_eq!(i.input("7 % 4"), Ok(Some (3.0)));

    assert_eq!(i.input("x = 1"), Ok(Some (1.0)));
    assert_eq!(i.input("x"), Ok(Some (1.0)));
    assert_eq!(i.input("x = x + 3"), Ok(Some (4.0)));

    assert_eq!(i.input("add echo 5 echo 2"), Ok(Some (7.0)));
    assert_eq!(i.input("avg 2 4"), Ok(Some (3.0)));
    assert_eq!(i.input("avg 4 2 + avg 10 30"), Ok(Some (23.0)));



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

    assert!(i.input("fn x => 0").is_err());
    assert!(i.input("avg = 5").is_err());
}
Footer

*/
