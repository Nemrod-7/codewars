#![allow(warnings, unused)]

use num::bigint::{BigInt, BigUint};

fn mul (a: &[BigInt; 4], b: &[BigInt; 4]) -> [BigInt; 4] {
    let x = &a[0] * &b[0] + &a[1] * &b[2];
    let y = &a[0] * &b[1] + &a[1] * &b[3];
    let w = &a[2] * &b[0] + &a[3] * &b[2];
    let z = &a[2] * &b[1] + &a[3] * &b[3];

    [x,y,w,z]
}
fn power (seq: &mut [BigInt;4], n: i32) {
    if n == 0 || n == 1 { return }

    power(seq, n / 2);
    *seq = mul (seq, seq);

    if n % 2 != 0 {
        let m = [BigInt::from(1),BigInt::from(1),BigInt::from(1),BigInt::from(0)];
        *seq = mul (seq, &m);
    }
}

fn fib (mut n: i32) -> BigInt {
    if n == 0 { return BigInt::from(0); }

    let sig = if n < 0 { -1 } else { 1 };
    let mut seq = [BigInt::from(1), BigInt::from(1), BigInt::from(1), BigInt::from(0)];

    n *= sig;
    power (&mut seq, n - 1);

    match n % 2 == 0 {
        true => seq[0].clone() * sig,
        false => seq[0].clone(),
    }
}

fn decomp (mut num: u32) -> u32 {
    let mut sum = 0;

    while num > 0 {
        sum += num % 10;
        num /= 10;
    }

    return sum;
}

fn power_sumDigTerm(n: u32) -> BigUint {
    let n = n as usize;
    let mut index = 0;
    let mut sqr = vec![BigUint::from(0u32);64];

    for i in 2..100 {
        let mut num = i;

        for j in 0..20 {
            num *= i;

            if decomp(num) == i {
                sqr[index] = index.into();
            }
        }
    }

    sqr.sort();

    return sqr[n-1].clone();
}

fn main () {

    // print!("{res} ");
    let _vc:Vec<BigUint> = vec![BigUint::from(0u32);64];

    print!("\n");
}
