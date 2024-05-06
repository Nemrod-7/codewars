
struct Parentesis {
    data: Vec<String>,
}
impl Parentesis {
    fn new() -> Parentesis {
        Parentesis {
            data : Vec::new()
        }
    }

    fn backtrack (mut comb: Vec<char>, index: usize, open: u32, close: u32) {

        let n = comb.len() / 2;

        if index == comb.len()  && open - close == 0 {
            let str = comb.iter().collect::<String>();

            print!("{:?}\n", str);
        } else if index < comb.len() {

            if (open as usize) < comb.len() {
                comb[index] = '(';
                Self::backtrack (comb.clone(), index + 1, open + 1, close);
            }

            if open - close > 0 && (close as usize) < comb.len() {
                comb[index] = ')';
                Self::backtrack (comb.clone(), index + 1, open, close + 1);
            }
        }

    }

}

fn main () {

    let n = 3;
    let mut comb = vec![' '; n * 2];

    let vc = Parentesis::new(); 
    Parentesis::backtrack(comb, 0, 0, 0);

}
