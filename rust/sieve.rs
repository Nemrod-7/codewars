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

fn getsize (size:usize) { // human-readable size

    let size = size as f64;
    let sizes = [("TiB",1099511627776.0),("GiB",1073741824.0),("MiB",1048576.0),("KiB",1024.0),("B",1.0)];

    for curr in sizes {
        if size >= curr.1 {
            print!("{:.2} {}\n", size / curr.1, curr.0);
            break;
        }
    }
}

fn main() {

    let now = Instant::now();
    let limit = 1000;
    const WHEEL: [usize;8] = [4,2,4,2,4,6,2,6];
    // 29 700 000
    let max = 1e2 as usize;
    let mut i = 7;
    let mut u = 0;
    let mut cnt = 0;
    let mut sieve = vec![0u32; (max >> 5) + 1];

    //getsize(max >> 6);
    // (i..max).step_by(2).for_each(|i| {})
    print!("2 3 5 ");

    while i < max {
        if (sieve[i >> 5] >> (i &31)&1) == 0 {
            let mut j = i * i;
            cnt += 1;
            print!("{} ", i);
            while j < max {
                sieve[j >> 5] |= 1 << (j &31);
                j += 2 * i;
            }
        } else {
            cnt = 0;
        }

        i += WHEEL[u];
        u = if u == 7 { 0 } else { u + 1 };
    }

//            for &j in &seq {
//                if i > 22 {
//                    let ix = i - j;
//
//                    if (sieve[ix >> 5] >> (ix &31)&1) == 1 { 
//                        sum = 0;
//                        break;
//                    } else {
//                        sum += ix
//                    }
//                }
//            }
//
//            if sum > 0 {
//                print!("{sum} = ");
//                for j in &seq {
//                    print!("{} ", i - j);
//                }
//
//                print!("\n");
//            }

    // let primes = (3..n)
    //     .step_by(2)
    //     .filter(|&x| sieve[x] == false)
    //     .collect::<Vec<_>>();

    println!("time elapsed : {} ms", now.elapsed().as_millis());
}
