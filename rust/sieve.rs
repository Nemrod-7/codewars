#![allow(dead_code,unused)]

use std::time::{Duration, Instant};

fn sieve (n:usize) {

    let mut sieve = vec![false; n];
    (3..n)
        .step_by(2)
        .for_each(
            |i| if sieve[i] == false {
                (i*i..n)
                    .step_by(2 * i)
                    .for_each(|j| sieve[j] = true)
            });
}

fn showsize (size:usize) { // show size in human-readable format
                           
    let size = size as f64;
    let sizes = [("TiB",1099511627776.0),("GiB",1073741824.0),("MiB",1048576.0),("KiB",1024.0),("B",1.0)];

    for curr in sizes {
        if size >= curr.1 {
            print!("{:.2} {}\n", size / curr.1, curr.0);
            break;
        }
    }
}

fn find_primes_sextuplet (sum_limit: u32) -> Vec<u32> {

    const WHEEL: [usize;8] = [4,2,4,2,4,6,2,6];
    let max = 1e8 as usize;
    let mut i = 7;
    let mut u = 0;
    let mut seq = Vec::new();
    let mut sieve = vec![0u32; (max >> 5) + 1];

    while i < max {
        if (sieve[i >> 5] >> (i &31)&1) == 0 {
            let mut j = i * i;
            let mut v = u;
            if u < 6 { seq.push(i as u32) }

            while j < max {
                sieve[j >> 5] |= 1 << (j &31);
                j += i * WHEEL[v];
                v = if v == 7 { 0 } else { v + 1 };
            }


        }

        if seq.len() == 6 && seq.iter().sum::<u32>() > sum_limit {
            print!("seq : {:?} \n", seq);
            break;
        }

        if u == 5 && seq.len() > 0 {
            seq.clear()
        }

        i += WHEEL[u];
        u = if u == 7 { 0 } else { u + 1 };
    }

    seq
}

fn dotest(n: u32, expected: &[u32]) {
    let actual = find_primes_sextuplet(n);
    assert!(actual == expected, "With n = {n}\nExpected {expected:?} but got {actual:?}")
}
fn fixed_tests() {
    dotest(70, &[7, 11, 13, 17, 19, 23]);
    dotest(600, &[97, 101, 103, 107, 109, 113]);
    dotest(2000000, &[1091257, 1091261, 1091263, 1091267, 1091269, 1091273]);
}

fn main() {

    let now = Instant::now();
    let limit = 1000;
    const WHEEL: [usize;8] = [4,2,4,2,4,6,2,6];
    // 29 700 000

    find_primes_sextuplet (1000);
    // let primes = (3..n)
    //     .step_by(2)
    //     .filter(|&x| sieve[x] == false)
    //     .collect::<Vec<_>>();

    println!("time elapsed : {} ms", now.elapsed().as_millis());
}
