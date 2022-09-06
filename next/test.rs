use std::collections::HashMap;

fn simple_assembler (code: Vec<&str>) -> HashMap<String, i64> {

    let mut register = HashMap::new();

    let mut it = 0;

    while it != code.len() {

        let com = &code[it][0..3];
        let key = &code[it][4..5];
        let pam = &code[it][5..].trim();

        print!("{} [{}]", com, key);
        match com {
            "mov" => {
                if pam.chars().all(char::is_alphabetic) {
                    if let Some (val) = register.get(*pam) {
                        let vv:i64 = *val;
                        register.insert(key.to_string(), vv);
                    }
                } else {
                    let val:i64 = pam.parse::<i64>().unwrap();
                    register.insert (key.to_string(), val);
                }
            },
            "inc" => {
                if let Some (x) = register.get(key) {
                    register.insert(key.to_string(), x + 1);
                }
            },
            "dec" => {
                if let Some (x) = register.get(key) {
                    register.insert(key.to_string(), x - 1);
                }
            },
            "jnz" => {

            },
              _   => (),
        }
        print! ("\n");
        it += 1
    }
    /*
    std::vector<std::string>::const_iterator it(input.begin());
    std::string op, key;

    while (it != input.end()) {
        op  = it->substr(0,3);
        key = it->at(4);
        int pam1 = isdigit(key[0]) ? stoi(key) : reg[key];

        if (op == "mov") reg[key] = isalpha(it->at(6)) ? reg[{it->at(6)}] : stoi(it->substr(6));
        if (op == "inc") reg[key]++;
        if (op == "dec") reg[key]--;
        if (op == "jnz" && pam1 != 0) {
            int end = input.end() - it, offset = stoi(it->substr(6));
            it += end > offset ? offset : end;
        } else
            it++;
    }
    */


    return register;
}

fn main() {
    let src = "test";

    let program = vec!["mov c 12","mov b 0","mov a 200","dec a","inc b","jnz a -2","dec c","mov a b","jnz c -5","jnz 0 1","mov c a",];
    //let program = vec!["mov a 5", "inc a", "dec a", "dec a", "jnz a -1", "inc a"];
    simple_assembler (program);

    print!("end\n");
}

macro_rules! map {
    ($($key:expr => $value:expr),*) => {{
         let mut map = HashMap::new();
         $(
             map.insert($key.to_string(), $value);
         )*
         map
    }};
}
fn short_tests() {
    let program = vec!["mov a 5", "inc a", "dec a", "dec a", "jnz a -1", "inc a"];
    let expected = map! { "a" => 1 };
    compare_registers(expected, simple_assembler(program));

    let program = vec![
        "mov c 12",
        "mov b 0",
        "mov a 200",
        "dec a",
        "inc b",
        "jnz a -2",
        "dec c",
        "mov a b",
        "jnz c -5",
        "jnz 0 1",
        "mov c a",
    ];
    let expected = map! { "a" => 409600, "c" => 409600, "b" => 409600};
    compare_registers(expected, simple_assembler(program));
}
fn compare_registers(expected: HashMap<String, i64>, actual: HashMap<String, i64>) {
    let result = expected
        .iter()
        .all(|(key, value)| actual.get(key).map(|v| v == value).unwrap_or(false));
    assert!(
        result,
        "Expected the registers to be like that:\n{:#?}\n\nBut got this:\n{:#?}\n",
        expected, actual
    )
}
