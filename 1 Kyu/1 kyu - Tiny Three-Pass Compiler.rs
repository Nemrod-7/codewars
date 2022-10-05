extern crate regex;
use regex::Regex;

#[derive(Clone,Debug)]
enum Ast {
    UnOp (String, i32),
    BinOp (String,Box<Ast>,Box<Ast>),
}

struct Compiler {
    args: Vec<String>,
}

impl Compiler {
    fn new() -> Compiler {
        Compiler {
            args: Vec::new(),
        }
    }

    fn getid (&self, ast : &Ast) -> String {

        if let Ast::BinOp(id, _a, _b) = ast {
            return id.to_string();
        } else if let Ast::UnOp(id, _val) = ast {
            return id.to_string();
        } else {
            return "null".to_string()
        }
    }
    fn getval (&self, ast : &Ast) -> i32 {

        if let Ast::UnOp(_id, val) = ast {
            return *val;
        }
        0
    }

    fn order (&self, tok: &str) -> usize {
        if tok == "+" || tok == "-" { return 1 }
        if tok == "*" || tok == "-" { return 2 }
        0
    }
    fn postorder (&self, root: &Ast) -> Vec<Ast> {

        let mut s1:Vec<Ast> = vec![root.clone()];
        let mut s2:Vec<Ast> = vec![];

        while let Some (node) = s1.pop() {

            if let Ast::BinOp(_id, a, b) = &node {
                s1.push(*a.clone());
                s1.push(*b.clone());
            }
            s2.push(node);
        }

        s2
    }
    fn mktree (&self, code: &[String]) -> Ast {

        let size = code.len();
        let number = Regex::new("^-?[0-9]+$").unwrap();
        let mut it = 0;
        let mut tree:Vec<Ast> = Vec::new();
        let mut oper:Vec<String> = Vec::new();

        while it != size {
            let tile = &code[it];

            if let Some (index) = self.args.iter().position(|x| x == tile) {
                tree.push (Ast::UnOp("arg".to_string(), index as i32));
            } else if tile == "(" {
                let fst = it + 1;

                while code[it] != ")" {
                    it += 1;
                }
                tree.push(self.mktree(&code[fst..it]))
            } else if number.is_match(tile) { // tile.chars().all(char::is_numeric) == true
                let value = tile.parse::<i32>().unwrap();
                tree.push (Ast::UnOp("imm".to_string(), value));
            } else {

                while let Some(id) = oper.last() {
                    if self.order (id) >= self.order (tile) {
                        let b = tree.pop().unwrap();
                        let a = tree.pop().unwrap();
                        tree.push (Ast::BinOp(id.to_string(), Box::new(a), Box::new(b)));
                        oper.pop();
                    } else {
                        break
                    }
                }

                oper.push (tile.to_string());
            }

            it += 1;
        }

        while let Some(id) = oper.pop() {

            let b = tree.pop().unwrap();
            let a = tree.pop().unwrap();
            tree.push (Ast::BinOp(id, Box::new(a), Box::new(b)));
        }

        if let Some(node) = tree.pop() { return node } else { Ast::UnOp ("null".to_string(),-1) }
    }

    fn tokenize<'a>(&self, program : &'a str) -> Vec<String> {
        let token = Regex::new("[-+*/()\\[\\]]|[A-Za-z]+|\\d+").unwrap();
        token.captures_iter(program).map(|x| x[0].to_string()).collect::<Vec<_>>()
    }
    fn compile (&mut self, program : &str) -> Vec<String> {

        let ast = self.pass1(program);
        let ast = self.pass2(&ast);
        self.pass3(&ast)
    }
    fn pass1 (&mut self, program : &str) -> Ast {

        let code = self.tokenize(program);
        let mut it = 1;

        while code[it] != "]" {
            self.args.push (code[it].to_string());
            it += 1;
        }

        self.mktree (&code[it+1..code.len()])
    }
    fn pass2 (&mut self, ast : &Ast) -> Ast {

        if let Ast::BinOp(id, a, b) = ast {
            let a:Ast = self.pass2(a);
            let b:Ast = self.pass2(b);

            if self.getid(&a) == "imm" && self.getid(&b) == "imm" {
                let value:i32;

                match id as &str {
                    "+" => value = self.getval(&a) + self.getval(&b),
                    "-" => value = self.getval(&a) - self.getval(&b),
                    "*" => value = self.getval(&a) * self.getval(&b),
                    "/" => value = self.getval(&a) / self.getval(&b),
                     _  => panic!("unknown operator"),
                }

                return Ast::UnOp("imm".to_string(), value);
            } else {
                return Ast::BinOp(id.clone(), Box::new(a), Box::new(b));
            }
        }

        return ast.clone();
    }
    fn pass3 (&mut self, ast : &Ast) -> Vec<String> {

        let mut asm = Vec::new();
        let mut tree:Vec<Ast> = self.postorder(ast);

        while let Some (node) = tree.pop() {

            if let Ast::BinOp(id, _a, _b) = &node {

                asm.push("PO".to_string());
                asm.push("SW".to_string());
                asm.push("PO".to_string());

                match id as &str {
                    "+" => asm.push("AD".to_string()),
                    "-" => asm.push("SU".to_string()),
                    "*" => asm.push("MU".to_string()),
                    "/" => asm.push("DI".to_string()),
                     _  => panic!("Invalid instruction"),
                }
            } else if let Ast::UnOp(id, val) = &node {

                match id as &str {
                    "imm" => asm.push("IM ".to_string() + &val.to_string()),
                    "arg" => asm.push("AR ".to_string() + &val.to_string()),
                      _   => panic!("Invalid instruction"),
                }
            }

            asm.push ("PU".to_string());
        }

        asm
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
fn shownode (now: &Ast) {

    if let Ast::BinOp(id, _a, _b) = now {
        print!("Bin({}", id);
    } else if let Ast::UnOp(id, val) = now {
        print!("Arg({} {}", id, val);
    } else {
        print!("(null");
    }

}
fn showtree (now: &Ast) {

    shownode (now);
    if let Ast::BinOp(_id, a, b) = now {
        showtree (a);
        showtree (b);
    }
    print!(")");
}

fn simulator() {
    assert_eq!(simulate(vec!["IM 7".to_string()], vec![3]), 7);
    assert_eq!(simulate(vec!["AR 1".to_string()], vec![1,2,3]), 2);
}
fn simulate (assembly : Vec<String>, argv : Vec<i32>) -> i32 {
    let mut r = (0, 0);
    let mut stack : Vec<i32> = vec![];

    for ins in assembly {
        let mut ws = ins.split_whitespace();
        match ws.next() {
            Some("IM") => r.0 = i32::from_str_radix(ws.next().unwrap(), 10).unwrap(),
            Some("AR") => r.0 = argv[i32::from_str_radix(ws.next().unwrap(), 10).unwrap() as usize],
            Some("SW") => r = (r.1,r.0),
            Some("PU") => stack.push(r.0),
            Some("PO") => r.0 = stack.pop().unwrap(),
            Some("AD") => r.0 += r.1,
            Some("SU") => r.0 -= r.1,
            Some("MU") => r.0 *= r.1,
            Some("DI") => r.0 /= r.1,
            _ => panic!("Invalid instruction encountered"),
        }
    }
    r.0
}
///////////////////////////////////////////////////////////////////////////////////////////////////

fn main() {

    let program = "[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)";

    let mut asm = Compiler::new();
    let tree = asm.pass1 (program);
    let tree = asm.pass2 (&tree);
    let code = asm.pass3 (&tree);

    let res = simulate (code, vec![5, 4, 1]);

    print! ("{} => {}",program, res);

    /*
    println!("AST1: {:?}", tree);
    //showtree(&tree);
    */

    print!("\n");
}
