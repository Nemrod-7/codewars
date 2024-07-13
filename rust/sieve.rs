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
    let max = 1e9 as usize;

    let seq = [0,4,6,10,12,16];
    let seq = vec![16,12,10,6,4,0];
    //let mut sieve = vec![true; max];

    let mut searching = true;
    let mut i = 3;

    getsize(max >> 1);
    /*
       while searching && i < max {

       if sieve[i] {
       let mut sum:i32 = 0;

       for &j in &seq {
       let ix = (i as i32) - (j as i32);

       if ix < 2 || !sieve[ix as usize] { 
       sum = 0;
       break;
       } else {
       sum += ix
       }
       }

       if sum > 0 {
       print!("{sum} = ");
       for j in &seq {
       print!("{} ", i - j);
       }

       print!("\n");
       }

       (i*i..max)
       .step_by(2 * i)
       .for_each(|j| sieve[j] = false)            
       }

       i += 2;
       }
       */

    // let primes = (3..n)
    //     .step_by(2)
    //     .filter(|&x| sieve[x] == false)
    //     .collect::<Vec<_>>();

    println!("time elapsed : {} ms", now.elapsed().as_millis());
}
