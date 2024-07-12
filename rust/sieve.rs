#![allow(dead_code,unused)]

use std::time::{Duration, Instant};

fn main() {

    let now = Instant::now();
    let n = 100;

    let mut sieve = vec![false; n];
    let mut primes = vec![2];

    (3..n)
        .step_by(2)
        .for_each(
            |i| if sieve[i] == false {
                primes.push(i);

                (i*i..n)
                    .step_by(2 * i)
                    .for_each(|j| sieve[j] = true)
            });


    let seq = [ 4,6,10,12,16 ];

    for i in (3..n).step_by(2) {
        let mut flag = true;

        if sieve[i] == false {
            for j in seq {

                if sieve[j] == true {
                    flag = false;
                    break;
                }
            }
        }

        if flag == true {
            for j in seq {

            }
        }
    }

    //print!("{:?}\n", primes);

    // let primes = (3..n)
    //     .step_by(2)
    //     .filter(|&x| sieve[x] == false)
    //     .collect::<Vec<_>>();

    println!("time elapsed : {} ms", now.elapsed().as_millis());
}
