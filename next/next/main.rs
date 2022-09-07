
extern crate regex;
use regex::Regex;

use std::collections::HashMap;

fn getreg (register: &HashMap<String, i64>, key: &str) -> i64 {
    match key.len() {
        0 => return 0,
        _ => {
            if key.chars().all(char::is_alphabetic) {
                if let Some (&val) = register.get(key) {
                    return val;
                }
            } else {
                return key.parse::<i64>().unwrap();
            }
        },
    }
    0
}
fn simple_assembler (code: Vec<&str>) -> HashMap<String, i64> {

    let mut it = 0;
    let mut running = true;
    let mut register = HashMap::new();

    while running {
        // let line = code[it].split_whitespace(); // program[i].split(' ').collect::<Vec<&str>>();
        let com = &code[it][0..3];
        let key = &code[it][4..5];
        let pam = &code[it][5..].trim();
        let mut nxt:i64 = 1;

        let mut kval:i64 = getreg (&register, key);
        let mut pval:i64 = getreg (&register, pam);

        match com {
            "mov" => { register.insert(key.to_string(), pval); },
            "inc" => { register.insert(key.to_string(), kval + 1); },
            "dec" => { register.insert(key.to_string(), kval - 1); },
            "jnz" => { if kval != 0 { nxt = pval } },
              _   => (),
        }

        it = (it as i64 + nxt) as usize;
        if it >= code.len() { running = false }
    }
    /*
    for (key, val) in &register {
        print! ("[{}]->{}\n", key,val);
    }
    */
    return register;
}

fn interpret (input: &str) -> Option<String> {

    let code = input.split('\n').filter(|x| x.len() > 0).collect::<Vec<_>>();

    let mut it = 0;
    let mut nxt;
    let mut key;
    let mut pam;
    let mut running = true;
    let mut oss = String::new();
    let mut register:HashMap<String, i64> = HashMap::new();
    let re = Regex::new ("\\w+|\'.+?\'").unwrap();

    while running {

        let line = re.captures_iter(code[it]).collect::<Vec<_>>();

        let cmd = &line[0][0];
        if line.len() > 1 { key = &line[1][0]; } else { key = "" }
        if line.len() > 2 { pam = &line[2][0]; } else { pam = "" }
        nxt = 1;

        //let mut kval:i64 = getreg (&register, key);

        print! ("[{}][{}]{}", cmd, key, pam);
        match cmd {
             "mov" => {
                 let mut pval:i64 = getreg (&register, pam);
                 register.insert (key.to_string(), pval);
             }
             "inc" => {
                 let mut kval:i64 = getreg (&register, key);
                 register.insert (key.to_string(), kval + 1);
             }
             "dec" => {
                 let mut kval:i64 = getreg (&register, key);
                 register.insert (key.to_string(), kval - 1);
             }
             "add" => {
                 // reg[key] += val;
             }
             "sub" => {
                 // reg[key] -= val;
             }
             "mul" => {
                 // reg[key] *= val;
             }
             "div" => {
                 // reg[key] /= val;
             }
             "cmp" => {
                 //comp = reg[key] - val;
             }
             "ret" => {
                 
             }
             "msg" => {
                 //os = mk_msg (prog.com[index], reg);
             }
             "end" => {
                 //return os;  //
             }
             "jmp" => {
                 //jump = true;
             }
             "jne" => {
                 //if (comp != 0) jump = true;
             }
             "je " => {
                 //if (comp == 0) jump = true;
             }
             "jge" => {
                 //if (comp >= 0) jump = true;
             }
             "jg " => {
                 //if (comp > 0) jump = true;
             }
             "jle" => {
                 //if (comp <= 0) jump = true;
             }
             "jl " => {
                 //if (comp < 0) jump = true;
             }
             "call"=> {

             },
               _   => (),
        }

        //print! ("\n");
        it = it + nxt;
        if it >= code.len() { running = false }
    }

    Option::Some(oss)
}

fn main() {
    let src = "test";
    let program = &[
                "\n; My first program\nmov  a, 5\ninc  a\ncall function\nmsg  '(5+1)/2 = ', a    ; output message\nend\n\nfunction:\n    div  a, 2\n    ret\n",
                "\nmov   a, 5\nmov   b, a\nmov   c, a\ncall  proc_fact\ncall  print\nend\n\nproc_fact:\n    dec   b\n    mul   c, b\n    cmp   b, 1\n    jne   proc_fact\n    ret\n\nprint:\n    msg   a, '! = ', c ; output text\n    ret\n",
                "\nmov   a, 8            ; value\nmov   b, 0            ; next\nmov   c, 0            ; counter\nmov   d, 0            ; first\nmov   e, 1            ; second\ncall  proc_fib\ncall  print\nend\n\nproc_fib:\n    cmp   c, 2\n    jl    func_0\n    mov   b, d\n    add   b, e\n    mov   d, e\n    mov   e, b\n    inc   c\n    cmp   c, a\n    jle   proc_fib\n    ret\n\nfunc_0:\n    mov   b, c\n    inc   c\n    jmp   proc_fib\n\nprint:\n    msg   'Term ', a, ' of Fibonacci series is: ', b        ; output text\n    ret\n",
                "\nmov   a, 11           ; value1\nmov   b, 3            ; value2\ncall  mod_func\nmsg   'mod(', a, ', ', b, ') = ', d        ; output\nend\n\n; Mod function\nmod_func:\n    mov   c, a        ; temp1\n    div   c, b\n    mul   c, b\n    mov   d, a        ; temp2\n    sub   d, c\n    ret\n",
                "\nmov   a, 81         ; value1\nmov   b, 153        ; value2\ncall  init\ncall  proc_gcd\ncall  print\nend\n\nproc_gcd:\n    cmp   c, d\n    jne   loop\n    ret\n\nloop:\n    cmp   c, d\n    jg    a_bigger\n    jmp   b_bigger\n\na_bigger:\n    sub   c, d\n    jmp   proc_gcd\n\nb_bigger:\n    sub   d, c\n    jmp   proc_gcd\n\ninit:\n    cmp   a, 0\n    jl    a_abs\n    cmp   b, 0\n    jl    b_abs\n    mov   c, a            ; temp1\n    mov   d, b            ; temp2\n    ret\n\na_abs:\n    mul   a, -1\n    jmp   init\n\nb_abs:\n    mul   b, -1\n    jmp   init\n\nprint:\n    msg   'gcd(', a, ', ', b, ') = ', c\n    ret\n",
                "\ncall  func1\ncall  print\nend\n\nfunc1:\n    call  func2\n    ret\n\nfunc2:\n    ret\n\nprint:\n    msg 'This program should return null'\n",
                "\nmov   a, 2            ; value1\nmov   b, 10           ; value2\nmov   c, a            ; temp1\nmov   d, b            ; temp2\ncall  proc_func\ncall  print\nend\n\nproc_func:\n    cmp   d, 1\n    je    continue\n    mul   c, a\n    dec   d\n    call  proc_func\n\ncontinue:\n    ret\n\nprint:\n    msg a, '^', b, ' = ', c\n    ret\n"];


    interpret (program[0]);
    print!("end\n");
}
