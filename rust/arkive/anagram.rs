
fn factorial (n:u128) -> u128 {
    let mut mult = 1;

    for i in 1..=n {
        mult *= i;
    }
    mult
}

fn list_position (word: &str) -> u128 {
    let n = word.len();
    let mut k = 1;

    for i in 0..n {
        let ch = word.chars().nth(i).unwrap();
        let less:usize = (i+1..n).filter(|x| word.chars().nth(*x).unwrap() < ch).count();
        let mut freq = [0;26];

        for j in i..n {
            freq[word.chars().nth(j).unwrap() as usize - 65] += 1;
        }
        let mut maxs = factorial ((n - i - 1) as u128) * less as u128;

        for val in freq {
            maxs /= factorial (val)
        }

        k += maxs
    }

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

    let alp = "QUESTION";
    let res = list_position(alp);

    print! ("{res}\n");
    // let k = list_position ("MYMY");

    /*
    for (test, result) in test_data {
        let k = list_position (test);
        print! (" => {test} {k} :: {result} \n\n");
    }
    */

}
