
fn mkunit (code: &str, c: char) -> usize {
    code.split(c)
        .filter_map (|x| (!x.is_empty()).then(||x.len()))
        .min()
        .unwrap_or(usize::MAX)
}
pub fn decode_bits (code: &str) -> String {

    let code = code.trim_matches ('0');
    let unit = std::cmp::min (mkunit (code, '0'), mkunit (code, '1'));
    let code = code.as_bytes();
    let mut os = String::new();
    let mut it = 0;

    while it != code.len() {
        let bit = code[it];
        let mut len = 0;

        loop {
            it += 1; len += 1;
            if it == code.len() || code[it] != bit { break }
        }

        match len / unit {
            1 => if bit == 49 { os.push_str (".") }
            3 => if bit == 49 { os.push_str ("-") } else { os.push_str (" ") }
            7 => if bit != 49 { os.push_str ("  ") }
            _ => (),
        }
    }

    os
}
pub fn decode_morse (morse: &str) -> String {
    let mut os = String::new();

    for tok in morse.split (' ') {
        if !tok.is_empty() {
            //os.push_str (&MORSE_CODE[tok])
        } else {
            //os.push_str (" ")
        }
    }

    os.trim().replace ("  ", " ")
}

fn main () {

        let code = "110011001111110011";
        let code = code.trim_matches ('0');
        let unit = std::cmp::min (mkunit (code, '0'), mkunit (code, '1'));

        let ccc = code.chars().step_by(unit).collect::<String>();

        for tok in ccc.chars() {

            print! ("{}\n", tok);
        }
        ;
}
