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
    let mut register = HashMap::new();

    while it != code.len() {
        // let line = code[it].split_whitespace(); // program[i].split(' ').collect::<Vec<&str>>();
        let com = &code[it][0..3];
        let key = &code[it][4..5];
        let pam = &code[it][5..].trim();

        let mut kval:i64 = getreg (&register, key);
        let mut pval:i64 = getreg (&register, pam);

        if com == "mov" { register.insert(key.to_string(), pval); }
        if com == "inc" { register.insert(key.to_string(), kval + 1); }
        if com == "dec" { register.insert(key.to_string(), kval - 1); }
        if com == "jnz" && kval != 0 {
            if pval > (code.len() - it) as i64 {
                pval = (code.len() - it) as i64;
            }
            it = (it as i64 + pval) as usize;
        } else {
            it += 1;
        }
    }

    return register;
}

fn main() {
    let src = "test";

    //let program = vec!["mov a 5", "inc a", "dec a", "dec a", "jnz a -1", "inc a"];
    let program = vec!["mov d 100", "dec d", "mov b d", "jnz b -2", "inc d", "mov a d", "jnz 5 10", "mov c a"];
    let program = vec!["mov a 1", "mov b 1", "mov c 0", "mov d 26", "jnz c 2", "jnz 1 5", "mov c 7", "inc d", "dec c", "jnz c -2", "mov c a", "inc a", "dec b", "jnz b -2", "mov b c", "dec d", "jnz d -6", "mov c 18", "mov d 11", "inc a", "dec d", "jnz d -2", "dec c", "jnz c -5"];

    //simple_assembler (program);
    print!("end\n");
}
