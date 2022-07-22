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

fn brain_luck (code: &str, input: Vec<u8>) -> Vec<u8> {

    let code: Vec<char> = code.chars().collect();
    let mut input = input.into_iter();

    let mut os:Vec<u8> = Vec::new();
    let mut tape: [u8; 10000] = [0; 10000];
    let mut index = 0;
    let mut it = 0;

    while index < code.len() {

        match code[index] {
            '+' => tape[it] = tape[it].wrapping_add (1),
            '-' => tape[it] = tape[it].wrapping_sub (1),
            '>' => it += 1,
            '<' => it -= 1,
            '.' => os.push(tape[it]),
            ',' => tape[it] = input.next().unwrap(),
            '[' =>
                if tape[it] == 0 {
                    index = find_next (&code, index);
                },
            ']' =>
                if tape[it] != 0 {
                    index = find_next (&code, index);
                },

             _  => (),
        }
        index += 1;
    }

    return os;
}
