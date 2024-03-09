#![allow(dead_code, unused)]

fn sum_pow_dig_seq(start: u32, n: u32, k: u32) -> (u32, Vec<u32>, u32, u32) {

    let mut r1:u32 = start;
    let mut s1:Vec<u32> = vec![r1];
    let mut ipow:Vec<Vec<u32>> = vec![vec![1;10];10];

    for i in 0..10 {
        (1..10).for_each(|j| ipow[i][j] = ipow[i][j-1] * i as u32);
    }

    s1.append(&mut (0..k).map(|_| { r1 = r1.to_string().chars().fold(0,|sum, dig| sum + ipow[dig as usize - 48][n as usize]); r1 }).collect());

    for i in 0..s1.len() {
        if let Some(j) = (i+1 .. s1.len()).find(|&x| s1[i] == s1[x]) {
            let s2 = s1[i..j].to_vec();
            return (i as u32, s2.clone(), s2.len() as u32, r1);
        }
    }

    (0, vec![], 0, 0)
}

fn main () {

    let res = sum_pow_dig_seq(420, 4, 30);

    print!("{:?}\n", res);

}
