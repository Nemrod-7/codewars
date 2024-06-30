#![allow(dead_code, unused)]

fn divbase (mut num: u64, base:u64) -> u64 {
            
    while num % base == 1 {
        num /= base;
    }
    num
}

fn get_min_base(number: u64) -> u64 {

    for base in 2..=(number as f64).sqrt() as u64 {
        let mut pow:u64 = base + 1;

        while pow <= number {

            if pow == number {
                return base;
            }

            if u64::MAX / base < pow {
                break
            }
            pow = pow * base + 1;
        }

        //if divbase(number, base) == 0 {
        //    return base;
        //}
    }

    number - 1
}

fn dotest(n: u64, expected: u64) {
    let actual = get_min_base(n);

    if actual != expected {
        print!( "With n = {n}\nExpected {expected} but got {actual}\n");
    }
}

fn tests() {

    for (n, expected) in [(3, 2), (7, 2), (21, 4), (57, 7), (1111, 10)] {
        dotest(n, expected)
    }

    for (n, expected) in [(1000002, 1000001), (1000000002, 1000000001), (1000000000000, 999999999999)] {
        dotest(n, expected)
    }
    for (n, expected) in [(1001001, 1000), (1001001001, 1000), (1001001001001, 1000), (2500050001, 50000),
    (125002500050001, 50000)] {
        dotest(n, expected)
    }
}

fn main() {

    let n:u64 = 149760653732457804; // Expected 2723 but got 149760653732457803 at src/lib.rs:52:9


    tests();

    let end = (n as f64).sqrt() as u64;
    //let base = get_min_base(n);
    //print!("{} : {} \n", n, base);

    let limit = 50;
    let base = 2;

    let mut pow = base + 1;

    while pow < limit {


        print!("{pow} ");

        pow = pow * base + 1;
    }



    //print!("end : {end}\n");



}
