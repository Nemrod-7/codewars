
fn factorial (n:u128) -> u128 {
    let mut mult = 1;

    for i in 1..=n {
        mult *= i;
    }
    mult
}

fn list_position (word: &str) -> u128 {

    let mut k = 1;
    let mut freq = [0;26];
    let mut word = word.to_string();
    let mut order = Vec::new();

    while let Some(ch) = word.pop() {
        order.push (ch);
        order.sort_by(|a, b| a.cmp(b));
        freq[ch as usize - 65] += 1;

        let mut maxv = factorial(order.len() as u128 - 1);

        for it in freq {
            maxv /= factorial(it);
        }


        print!("{maxv} :: ");
        for j in 0..order.len() {
            if order[j] == ch {
                let select = j as u128 * maxv;
                print!("{} ", j);
            }
        }

        print!("\n");
        //print!("{:?} \n",order);
        let pos = order.iter().position(|x| x == &ch).unwrap() as u128;
        k += pos * maxv;
    }
    print!("\n{k}\n");
    k 
}

fn main () {

    let test_data = [
        (                  "A", 1),
        (               "ABAB", 2),
        (               "AAAB", 1),
        (               "BAAA", 4),
        (               "YMYM", 5),
        (           "QUESTION", 24572),
        (         "BOOKKEEPER", 10743),
        //("IMMUNOELECTROPHORETICALLY", 718393983731145698173i128),
    ];

    let k = list_position ("MYMY");

    /*
    for (test, result) in test_data {
        let k = list_position (test);
        print! (" => {test} {k} :: {result} \n\n");
    }
    */

}
