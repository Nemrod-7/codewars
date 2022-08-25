
use std::cmp::max;

fn sieveoferatosthenes (num: usize) -> Vec<i64> {

    let end = (num as f64).sqrt() as usize;
    let mut prime = vec![0; num];
    let mut sieve = vec![0; num];

    for p in 0..=end {
        if prime[p] == 0 {
            let mut i = p * p;

            while i <= num {
                prime[i] = 1;
                i += p;
            }
        }
    }

    for i in 2..=num {
        if prime[i] == 0 {
            sieve.push(i as i64);
        }
    }

    return sieve;
}
fn maxnum (lst: &Vec<i64>) -> usize {

    let mut maxv = 0;

    for num in lst {
        maxv = max (maxv, num.abs() as usize);
    }

    maxv
}
fn sum_of_divided(lst: Vec<i64>) -> Vec<(i64, i64)> {

    let max = maxnum (&lst) + 1;
    let primes = sieveoferatosthenes (120);
    let mut sum = 0;
    let mut pair:Vec<(i64, i64)> = Vec::new();
    let mut divided = false;

    for p in primes {
        for num in &lst {
            if num.abs() % p == 0 {
                sum += num;
                divided = true;
            }
        }

        if divided == true {
            pair.push((p, sum));
        }

        sum = 0;
        divided = false;
    }

    pair
}

fn main() {

}
