use std::collections::VecDeque;

fn find_next (code: &Vec<char>, pos:usize) -> usize {

    let size = code.len();
    let mut index = pos;
    let mut pile = 0;
    let mut fwrd:bool = true;
    if code[index] == ']' { fwrd = false };

    while index < size {
        if code[index] == '[' { pile += 1 }
        if code[index] == ']' { pile -= 1 }

        if pile == 0 { return index }
        if fwrd == true { index +=1 } else { index -= 1 }
    }

    return index;
}
fn boolfuck(code: &str, input: Vec<u8>) -> Vec<u8> {

    let code:Vec<char> = code.chars().collect();
    let mut os:Vec<u8> = Vec::new();
    let mut tape = VecDeque::from(vec![0;1]);
    let mut it = 0;
    let mut index = 0;
    let mut bin = 0;
    let mut out = 0;

    while index < code.len() {

        match code[index] {
            '+' => tape[it] ^= 1,
            '<' => {
                    if it == 0 { tape.push_front(0) } else { it -= 1 }
                },
            '>' => {
                    it += 1;
                    if it == tape.len() { tape.push_back(0) }
                },
            ',' => {
                    tape[it] = input[bin / 8] >> (bin % 8) &1;
                    bin += 1;
                },
            ';' => {
                    if out / 8 >= os.len() { os.push (0) }
                    os[out / 8] |= tape[it] << (out % 8);
                    out += 1
                },
            '[' => if tape[it] == 0 { index = find_next (&code, index) },
            ']' => if tape[it] != 0 { index = find_next (&code, index) },
             _  => (),
        }

        index += 1;
    }

    return os;
}
