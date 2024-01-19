#![allow(dead_code, unused)]

use std::collections::HashMap;

fn main () {

    let lim = 499;

    let lim = lim as usize;
    let mut prime = vec![2];
    let mut sieve = vec![0; (lim >> 1)  + 1];
    let mut res:(usize, Vec<u32>) = (0, vec![]);
    let mut hist:HashMap<usize, Vec<u32>> = HashMap::new();

    for p in (4..=lim).step_by(2) {
        //sieve[p] = 1;
    }

    for p in (3..=lim).step_by(2) {
        if sieve[p >> 1] == 0 {
            for k in (p * p..=lim).step_by(2 * p) {
                sieve[k >> 1] = 1;
            }
        }
    }

    prime.extend((3..lim).step_by(2).filter(|x| sieve[x >> 1] == 0).map(|x| x as u32).collect::<Vec<u32>>());

    for i in 0..prime.len() {
        let mut j = i + 1;
        let mut sum = prime[i];

        while j < prime.len() && sum + prime[j] < lim as u32 {
            sum += prime[j];

            if sum % 2 != 0 && sieve[sum as usize >> 1] == 0 {
                let cnt = j - i;

                match hist.get_mut(&cnt) {
                    Some(v) => { v.push(sum); },
                    None => { hist.insert(cnt, vec![sum]); },
                }
            }

            j += 1;
        }
    }
    //for it in hist.iter() {
    //  print!("{} {:?}\n", it.0, it.1);
    //}
    
    let maxv = hist.iter().max_by_key(|x| x.0).unwrap().1;

    print!("\n res : {:?}\n", maxv);
}

