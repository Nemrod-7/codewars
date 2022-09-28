
use std::collections::HashMap;

#[derive(Clone)]
enum Ast {
    Null,
    BinOp (String,Box<Ast>,Box<Ast>),
    UnOp (String, i32),
}

struct Compiler {
    args: HashMap<String,usize>,
}

impl Compiler {
    fn new() -> Compiler {
        Compiler {
            args: HashMap::new(),
        }
    }

    fn tokenize<'a>(&self, program : &'a str) -> Vec<String> {
        let mut tokens : Vec<String> = vec![];
        let mut iter = program.chars().peekable();

        loop {
            match iter.peek() {
                Some(&c) => match c {
                    'a'..='z'|'A'..='Z' => {
                        let mut tmp = String::new();
                        while iter.peek().is_some() && iter.peek().unwrap().is_alphabetic() {
                        tmp.push(iter.next().unwrap());
                        }
                        tokens.push(tmp);
                    },
                    '0'..='9' => {
                        let mut tmp = String::new();
                        while iter.peek().is_some() && iter.peek().unwrap().is_numeric() {
                            tmp.push(iter.next().unwrap());
                        }
                        tokens.push(tmp);
                    },
                    ' ' => { iter.next(); },
                    _ => { tokens.push(iter.next().unwrap().to_string()); },
                },
                None => break
            }
        }

        tokens
    }

    fn order (&self, tok: &str) -> usize {
        if tok == "+" || tok == "-" { return 1 }
        if tok == "*" || tok == "-" { return 2 }
        0
    }
    fn postorder (root: &Ast) -> Vec<Ast> {

        let temp:Ast = root.clone();
        let mut s1:Vec<Ast> = Vec::new();
        let mut s2:Vec<Ast> = Vec::new();

        s1.push (temp);

        while let Some (mut node) = s1.pop() {

            if let Ast::BinOp(id, a, b) = &node {
                s1.push(*a.clone());
                s1.push(*b.clone());
            }
        
            s2.push(node);
        }

        s2
    }
    fn mktree (&self, code: &[String]) -> Ast {

        let size = code.len();
        let mut it = 0;
        let mut tree:Vec<Ast> = Vec::new();
        let mut oper:Vec<Ast> = Vec::new();

        while it != size {

        let tile = &code[it];

        if let Some (&idx) = self.args.get(tile) {
            let value = idx as i32 - 1;
            tree.push (Ast::UnOp("arg".to_string(), value));
        } else if tile == "(" {
            let fst = it + 1;

            while code[it] != ")" {
                it += 1;
            }
            tree.push(self.mktree(&code[fst..it]))
        } else if tile.chars().all(char::is_numeric) == true {
            let value = tile.parse::<i32>().unwrap();
            tree.push (Ast::UnOp("imm".to_string(), value));
        } else {
            let temp = Ast::BinOp(tile.to_string(), Box::new(Ast::Null), Box::new(Ast::Null));

            while let Some(Ast::BinOp(id, _a, _b)) = oper.last() {
                if self.order (id) >= self.order (tile) {
                    let bb = tree.pop().unwrap();
                    let ba = tree.pop().unwrap();
                    tree.push (Ast::BinOp(id.to_string(), Box::new(ba), Box::new(bb)));
                    oper.pop();

                } else {
                    break
                }
            }

                oper.push (temp);
            }

            it += 1;
        }

        while let Some(Ast::BinOp(id, _a, _b)) = oper.pop() {

            let bb = tree.pop().unwrap();
            let ba = tree.pop().unwrap();

            tree.push (Ast::BinOp(id, Box::new(ba), Box::new(bb)));
        }

        if let Some(node) = tree.pop() {
            return node;
        }
        Ast::Null
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

    fn compile (&mut self, program : &str) -> Vec<String> {

        let ast = self.pass1(program);
        let ast = self.pass2(&ast);
        self.pass3(&ast)
    }
    fn pass1 (&mut self, program : &str) -> Ast {

        let code = self.tokenize(program);
        let mut it = 1;

        while code[it] != "]" {
            self.args.insert (code[it].to_string(), it);
            it += 1;
        }

        self.mktree (&code[it+1..code.len()])
    }
    fn pass2 (&mut self, ast : &Ast) -> Ast {

        if let Ast::BinOp(id, a, b) = ast {
            if self.getid(a) == "imm" && self.getid(b) == "imm" {
                let mut value:i32 = 0;

                match id as &str {
                    "+" => value = self.getval(a) + self.getval(b),
                    "-" => value = self.getval(a) - self.getval(b),
                    "*" => value = self.getval(a) * self.getval(b),
                    "/" => value = self.getval(a) / self.getval(b),
                    _  => (),
                }

                return Ast::UnOp("imm".to_string(), value);
            } else {
                let a:Ast = self.pass2(a);
                let b:Ast = self.pass2(b);

                return self.pass2(&Ast::BinOp(id.clone(), Box::new(a), Box::new(b)));
            }

        } else if let Ast::UnOp(id, val) = ast {
            return Ast::UnOp(id.to_string(), *val);
        } else {
            return Ast::Null;
        }
    }
    fn pass3 (&mut self, ast : &Ast) -> Vec<String> {

        let mut asm = Vec::new();

        let mut tree:Vec<Ast> = Vec::new();//= postorder(ast);

        while let Some (node) = tree.pop() {

            if let Ast::BinOp(id, a, b) = &node {

                match id as &str {
                    "+" => asm.push("AD".to_string()),
                    "-" => asm.push("SU".to_string()),
                    "*" => asm.push("MU".to_string()),
                    "/" => asm.push("DI".to_string()),
                    _  => (),
                }
            } else if let Ast::UnOp(id, val) = &node {

                asm.push("PO".to_string());
                asm.push("SW".to_string());
                asm.push("PO".to_string());

                if id == "imm" {
                    asm.push("IM ".to_string() + &val.to_string())
                } else {
                    asm.push("AR ".to_string() + &val.to_string())
                }

            } else {

            }

            asm.push ("PU".to_string());
        }

        asm
    }

}


fn getid (ast: &Ast) -> String {

    if let Ast::BinOp(id, _a, _b) = ast {
        return id.to_string();
    } else if let Ast::UnOp(id, _val) = ast {
        return id.to_string();
    } else {
        return "null".to_string()
    }
}
fn getval (ast: &Ast) -> i32 {

    if let Ast::UnOp(_id, val) = ast {
        return *val;
    }
    0
}


fn shownode (now: &Ast) {

    if let Ast::BinOp(id, a, b) = now {
        print!("Bin({}", id);
    } else if let Ast::UnOp(id, val) = now {
        print!("Arg({} {}", id, val);
    } else {
        print!("(null");
    }

}
fn showtree (now: &Ast) {

    shownode (now);
    if let Ast::BinOp(id, a, b) = now {
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

fn main() {

    let program = "[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)";

    let mut code = Compiler::new();
    let mut tree = code.compile(program);

    /*
    Compiler compiler;
    auto c = compiler.compile (prog);
    Assert::That (simulate (c, {5, 4, 1}), Equals (5));
    showtree (&tree);
    //mktree (&code[it..code.len()]);

    */

    print!("\n");
}
