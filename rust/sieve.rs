#![allow(dead_code,unused)]


fn main() {

    let n = 100;

    let mut sieve = vec![false; n >> 1];
    let mut primes = vec![2];

    (3..n)
        .step_by(2)
        .for_each(
            |i| if sieve[i >> 1] == false {
                primes.push(i);

                (i*i..n)
                    .step_by(2 * i)
                    .for_each(|j| sieve[j >> 1] = true)
            });

    // print!("{:?}\n", primes);

    // let primes = (3..n)
    //     .step_by(2)
    //     .filter(|&x| sieve[x] == false)
    //     .collect::<Vec<_>>();

    let re = 1;

    for i in 1..8 {
        let nu = re << i;
        print!("{i} : {nu}\n");
    }

}
