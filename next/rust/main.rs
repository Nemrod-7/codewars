
use std::collections::HashMap;

enum Ast {
    BinOp (String,Box<Ast>,Box<Ast>),
    UnOp (String, i32),
}

struct Compiler {
  // your code
}

impl Compiler {
  fn new() -> Compiler {
    Compiler {}
  }

  fn tokenize<'a>(&self, program : &'a str) -> Vec<String> {
    let mut tokens : Vec<String> = vec![];
    let mut iter = program.chars().peekable();

    loop {
      match iter.peek() {
        Some(&c) => match c {
          'a'...'z'|'A'...'Z' => {
            let mut tmp = String::new();
            while iter.peek().is_some() && iter.peek().unwrap().is_alphabetic() {
              tmp.push(iter.next().unwrap());
            }
            tokens.push(tmp);
          },
          '0'...'9' => {
            let mut tmp = String::new();
            while iter.peek().is_some() && iter.peek().unwrap().is_numeric() {
              tmp.push(iter.next().unwrap());
            }
            tokens.push(tmp);
          },
          ' ' => { iter.next(); },
          _ => {
            tokens.push(iter.next().unwrap().to_string());
          },
        },
        None => break
      }
    }

    tokens
  }

  fn compile (&mut self, program : &str) -> Vec<String> {

      let res = Vec::new();
        /*
        let ast = self.pass1(program);
        let ast = self.pass2(&ast);

        self.pass3(&ast)
        */
        res
  }

  fn pass1 (&mut self, program : &str) {

    let tokens = self.tokenize(program);
    let mut iter = tokens.iter().peekable();


  }

  /*
  fn pass2 (&mut self, ast : &Ast) -> Ast {

      let tree:Ast = Ast::BinOp;


      tree
  }
  fn pass3 (&mut self, ast : &Ast) -> Vec<String> {

      let res = Vec::new();
code[it]
      res
  }
  */
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
fn mktree (code: &Vec<String>) -> Ast {

    let tree = Ast::UnOp("err".to_string(), -1);

    tree
}
fn main() {

    let program = "[ x y z ] ( 2*3*x + 5*y - 3*z ) / (1 + 3 + 2*2)";

    let mut assembly = Compiler::new();
    let code = assembly.tokenize(program);
    
    let mut it = 1;
    let mut args: HashMap<String,usize> = HashMap::new();
    let mut tree:Vec<Ast> = Vec::new();

    while code[it] != "]" {
        args.insert (code[it].to_string(), it);
        it += 1;
    }

    it += 1;

    while it != code.len() {

        if let Some (&idx) = args.get(&code[it]) {
            let value = idx as i32 - 1;
            tree.push (Ast::UnOp("arg".to_string(), value));
            //print!("arg : [{}]->{}\n", code[it], idx - 1);
        } else if code[it] == "(" {
            it += 1;
            let mut sub:Vec<String> = Vec::new();
            print!("sub : [");

            while code[it] != ")" {
                sub.push(code[it].to_string());
                print!("{}", code[it]);
                it += 1;
            }
            print!("]\n");
        } else if code[it].chars().all(char::is_numeric) == true {
            let value = code[it].parse::<i32>().unwrap();
            tree.push (Ast::UnOp("imm".to_string(), value));
            //print!("num : [{}]\n", code[it]);
        } else {
            //print!("ope : [{}]\n", code[it]);
        }

        it += 1;
    }
    /*
    */
/*
    let a = Ast::UnOp("imm".to_string(), 2);
    let b = Ast::UnOp("imm".to_string(), 3);

    let op = Ast::BinOp("+".to_string(), Box::new(a), Box::new(b) );

*/
/*

Ast::BinOp("+".to_string(), Box::new(a), Box::new(b) ); // add subtree a to subtree b
Ast::BinOp("-".to_string(), Box::new(a), Box::new(b) ); // subtract subtree b from subtree a
Ast::BinOp("*".to_string(), Box::new(a), Box::new(b) ); // multiply subtree a by subtree b
Ast::BinOp("/".to_string(), Box::new(a), Box::new(b) ); // divide subtree a from subtree b
Ast::UnOp("arg".to_string(), n) // reference to n-th argument, n integer
Ast::UnOp("imm".to_string(), n) // immediate value n, n integer

*/

    print!("\n");
}
