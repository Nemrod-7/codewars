#![allow(dead_code, unused)]

fn decomp (num:u32) -> Vec<u32> {
    let mut num = num.to_string().chars().map(|x| x as u32 - 48).collect::<Vec<_>>();
    num.sort_by(|a,b| b.cmp(a));
    num
}
fn count(num:u32) -> usize {
    let dc = decomp(num);
    let end = (dc.iter().fold(0,|sum, val| sum * 10 + val) as f32).sqrt() as u32;
    (1..=end).filter(|val| decomp(val * val) == dc).count()
}
fn sort_by_perfsq(arr: &[u32]) -> Vec<u32> {
    let mut res = arr.to_vec();
    res.sort_by(|a,b|
        {
            let cna = count(*a);
            let cnb = count(*b);

            if cna == cnb { a.cmp(b) } else { cnb.cmp(&cna) }
        }
    );
    res
}
fn closest(s: &str) -> String {
    let vc = s.split(' ').collect::<Vec<_>>();
    let mut os = String::new();

    os
}
fn main () {
    let mut s = "456899 50 11992 176 272293 163 389128 96 290193 85 52"; // [(13, 9, "85"), (14, 3, "176")]

    let vc = s.split(' ').collect::<Vec<_>>();

    let num = "123";
    let weight = num.chars().map(|x| x as u32 - 48).fold(0, |sum, val| sum * 10 + val);

    print!("{:?}\n", weight );

}
