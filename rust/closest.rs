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

fn getweight (num: &str) -> i32 {
    num.chars().map(|x| x as i32 - 48).fold(0, |sum, val| sum + val)
}
fn closest(s: &str) -> String {
    let number = s.split(' ').collect::<Vec<_>>();
    let weight = number.iter().map(|x| getweight(x)).collect::<Vec<_>>();

    let mut res = String::new();
    let mut minv:i32 = 9999;
    let mut base = (0..number.len()).map(|i| (weight[i], i, format!("{}", number[i]))).collect::<Vec<_>>();
 for i in 0..base.len() {
        print!("{} {} {}\n", base[i].0, base[i].1, base[i].2);
    }

    base.sort_by(|a,b| a.0.cmp(&b.0));

   
    for i in 0..base.len() - 1 {
        let a = &base[i];
        let b = &base[i+1];
        let dist = b.0 - a.0;

        if dist < minv {
            minv = dist;
            res = format!("[({},{},{})({},{},{})]", a.0, a.1, a.2, b.0, b.1, b.2);
        }
    }

    res
}

fn main () {
    let s = "140912 680 387350 117 511681 927 753277 498 257792 493 774432 597 55599 379 194605 7 116156 808 42482 588 300474 794 45973 235 714922 801";

    let res = closest(&s);

    print!("{res}\n");
    /*
  left: `"[(9,4,117)(9,26,801)]"`,
 right: `"[(9,3,117)(9,25,801)]"`: Testing:  

       print!("{} {} {}\n", weight[pair.0], pair.0, number[pair.0]);
       print!("{} {} {}\n", weight[pair.1], pair.1, number[pair.1]);
       */

}


