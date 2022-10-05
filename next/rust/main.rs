extern crate regex;
use regex::Regex;

use std::collections::HashMap;

pub struct Interpreter {
    pub vars: HashMap<String, f32>,
    pub func: HashMap<String, String>,
}

impl Interpreter {
    fn new() -> Interpreter {
        Interpreter { vars: HashMap::new(), func: HashMap::new() }
    }

    fn tokenize (&self, src: &str) -> Vec<String> {
        let token = Regex::new("=>|(_|-)?[0-9]+(\\.[0-9]+|_)?|[-+*/%()=\\[\\]]|_?[a-zA-Z]+_?").unwrap();
        token.captures_iter(src).map(|x| x[0].to_string()).collect::<Vec<_>>()
    }
    fn order (&self, tok: &str) -> usize {
        if tok == "+" || tok == "-" { return 1 }
        if tok == "*" || tok == "-" || tok == "%" { return 2 }
        0
    }
    fn calc (&self, a: f32, id: &str, b: f32) -> f32 {
        match id as &str {
            "+" => return a + b,
            "-" => return a - b,
            "*" => return a * b,
            "/" => return a / b,
            "%" => return a % b,
            _  => panic!("unknown operator"),
        }
    }

    fn form (&mut self, func: String, args: Vec<String>) -> String {

        let mut sub = String::new();
        let func = self.tokenize(&func);
        let nvar = func.iter().position(|x| x == "=>").unwrap();

        if args.len() != nvar { panic! ("Invalid func argument") }

        for cell in func[nvar + 1 ..func.len()].iter() {
            let pos = func.iter().position(|x| x == cell).unwrap();

            if pos < nvar {
                sub += &format! ("{} ", self.input(&args[pos]));
            } else {
                sub += &format! ("{} ",cell);
            }
        }

        sub
    }

    fn input (&mut self, src: &str) -> f32 {

        let code = self.tokenize (src);
        let size = code.len();
        if size == 0 { panic!("Empty expression.") }

        let number = Regex::new("^-?[0-9]+(.[0-9]+)?$").unwrap();
        let variab = Regex::new("_?[a-zA-Z]+_?|_[0-9]+|[0-9]+_").unwrap();

        let mut it = 0;
        let mut tree:Vec<f32> = Vec::new();
        let mut oper:Vec<String> = Vec::new();

        while it < size {
            let tile = &code[it];

            if number.is_match(tile) {
                tree.push (tile.parse::<f32>().unwrap());
            } else if variab.is_match(tile) {

                let increment = code.iter().position(|x| x == "=");

                if tile == "fn" { // declare func
                    let name = code[1].to_string();

                    if let Some (mid) = code.iter().position(|x| x == "=>") {
                        let vars = &code[2..mid];
                        let body = &code[mid+1..size];

                        for var in vars {
                            if body.iter().position(|x| x == var) == None {
                                panic!("Invalid function")
                            }
                        }

                        let sub = code[it+2..size].join(" ");
                        self.func.insert(name, sub);
                        it = size;

                    } else {
                        panic!("Invalid function")
                    }
                } else if let Some(_pos) = increment { // declare var
                    if self.func.contains_key (tile) { panic! ("Invalid initializer.") }

                    let sub = code[it+2..size].join(" ");
                    let val = self.input(&sub);

                    self.vars.insert(tile.to_string(), val);
                    return val;
                } else if let Some (val) = self.vars.get (tile) { // get var
                    tree.push (*val);
                } else if let Some (fnc) = self.func.get (tile) { // get func
                    it += 1;

                    let mut buf = String::new();
                    let mut args = Vec::new();

                    while it < size {
                        buf += &format!("{} ", code[it]);

                        if !self.func.contains_key (&code[it]) {
                            args.push(buf.to_string());
                            buf.clear();
                        }
                        it += 1;
                    }

                    let sub = self.form(fnc.clone(), args);
                    return self.input (&sub);
                } else {
                    panic! ("Unknown identifier.")
                }

            } else if tile == "(" {
                it += 1;

                if let Some (len) = code[it..size].iter().position(|x| x == ")") {
                    let sub = code[it..it+len].join(" ");
                    tree.push(self.input(&sub));
                    it += len;
                }

            } else if tile != ")" {

                while let Some(id) = oper.last() {
                    if self.order (id) >= self.order (tile) {
                        let b = tree.pop().unwrap();
                        let a = tree.pop().unwrap();
                        tree.push (self.calc (a,id,b));
                        oper.pop();
                    } else {
                        break
                    }
                }

                oper.push (tile.to_string());
            }

            it += 1;
        }

        while let Some(id) = &oper.pop() {

            let b = tree.pop().unwrap();
            let a = tree.pop().unwrap();
            tree.push (self.calc (a,id,b));
        }

        if let Some(node) = tree.pop() { return node } else { 0.0 }
    }
}

fn main() {


    let mut i = Interpreter::new();

    assert_eq!(i.input("1 + 1"), 2.0);
    assert_eq!(i.input("2 - 1"), 1.0);
    assert_eq!(i.input("2 * 3"), 6.0);
    assert_eq!(i.input("8 / 4"), 2.0);
    assert_eq!(i.input("7 % 4"), 3.0);

    assert_eq!(i.input("x = 1"), 1.0);
    assert_eq!(i.input("x"), 1.0);
    assert_eq!(i.input("x = x + 3"), 4.0);

    assert_eq!(i.input("fn avg x y => (x + y) / 2"), 0.0);
    assert_eq!(i.input("fn add x y => x + y"), 0.0);
    assert_eq!(i.input("fn echo x => x"), 0.0);
    assert_eq!(i.input("avg 2 4"), 3.0);
    assert_eq!(i.input("add echo 5 echo 2"), 7.0);
    //print!("[{}]\n", res);

    let code = i.tokenize("fn prod x y=>(a+b)*(x+y)");
    //print!("{:?}", code);
    /*

    for (key,val) in i.vars {
        print!("[{}] -> {}\n",key, val);
    }

    for (key,val) in i.func {
        print!("[{}] -> {}\n",key, val);
    }
    */
    /*
    assert_eq!(i.input("fn avg x y => (x + y) / 2"), Ok(None));
    assert_eq!(i.input("avg 4 2"), Ok(Some(3.0)));
    assert!(i.input("avg 7").is_err());
    assert!(i.input("avg 7 2 4").is_err());
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
