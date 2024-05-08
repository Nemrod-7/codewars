
fn balanced_parens(n: u16) -> Vec<String> {

    let mut q1 = vec![ ( 0, vec![' '; 2 * n  as usize], 0,0) ];
    let mut res = Vec::new();

    while let Some ((index, mut comb, open, close)) = q1.pop() {

        if index == comb.len() && open - close == 0 {
            res.push(comb.iter().collect::<String>());
        } else if index < comb.len() {
            if open < n {
                comb[index] = '(';
                q1.push( ( index + 1, comb.clone(), open + 1, close));
            }

            if close < open {
                comb[index] = ')';
                q1.push( ( index + 1, comb.clone(), open, close + 1));
            }
        }
    }

    print!("{:?}\n", res);
    res
}

fn main () {

    let n = 3;


    balanced_parens(3);

}
