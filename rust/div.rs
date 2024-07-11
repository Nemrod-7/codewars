#![allow(dead_code,unused)]
use std::time::{Duration, Instant};

const ERR_MSG: &str = "\nYour result (left) did not match the expected output (right)";

fn bouncy(mut num:u32) -> bool {

    let mut inc = true;
    let mut dec = true;
    let mut last = num % 10;
    
    while num > 0 {
        let curr = num % 10;

        if curr > last {
            dec = false;
        } else if curr < last {
            inc = false;
        }

        last = curr;
        num /= 10;
    }

    inc == false && dec == false
}
fn bouncy_ratio(expec: f64) -> Option<u32> {

    if expec < 0.0 || expec > 0.99 { return None }
    if expec == 0.0 { return Some(1) }

    let mut index = 0;
    let mut total:f64 = 0.0;
    let mut ratio:f64 = 0.0;

    while ratio < expec {    
        index += 1;
        
        if bouncy(index) == true { total += 1.0; }
        ratio = total / index as f64;
    }

    Some(index)
}

fn dotest(p: f64, expected: Option<u32>) {
    assert_eq!(bouncy_ratio(p), expected, "{ERR_MSG} with ratio = {p}")
}
fn fixed_tests() {

    dotest(0.0, Some(1));
    dotest(0.999, None);
    dotest(0.15, Some(160));
    dotest(0.5, Some(538));
    dotest(0.75, Some(3088));
    dotest(0.9, Some(21780));
    dotest(0.99,Some(1587000));
    dotest(0.17743912876947998, Some(171));
    dotest(0.2464640436992307, Some(193));
}

fn main () {

    let now = Instant::now();

    fixed_tests();


    println!("time elapsed : {} ms", now.elapsed().as_millis());
}
