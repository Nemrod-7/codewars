use std::time::{Duration, Instant};

use std::sync::Mutex;
static MEM:Mutex<Vec<u32>> = Mutex::new(Vec::new());

fn stream () -> impl Iterator<Item = u32> {
    let mut seq = MEM.lock().unwrap();

    if seq.len() == 0 {
        let lim = 1e8 as usize;
        let half = (lim / 3) + 1;
        let mut p = 5;
        let mut step = 2;
        let mut sieve = vec![true; half];
        *seq = vec![2,3];

        while p <= lim {
            if sieve[p / 3] == true {
                seq.push(p as u32);
                let mut j = p * p;
                let mut t = step;

                while j <= lim {
                    sieve[j / 3] = false;
                    j += p * t;
                    t = 6 - t;
                }
            }
            p += step;
            step = 6 - step;
        }
        sieve.clear();
    }
 
    seq.clone().into_iter()
}

fn test_segment(start: u32, numbers: [u32; 10]){
    let mut prime_iterator = stream();
    for _ in 0..start{
        prime_iterator.next();
    }
    for i in numbers{
        assert_eq!(Some(i), prime_iterator.next(),
        "\nYour result (left) did not match the expected output (right)");
    }
}

fn main () {

    let start = Instant::now();

    let mach = stream();

    println!("testing segment from 0");
    test_segment(0, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]);

    println!("testing segment from 10");
    test_segment(10, [31, 37, 41, 43, 47, 53, 59, 61, 67, 71]);

    println!("testing segment from 100");
    test_segment(100, [547, 557, 563, 569, 571, 577, 587, 593, 599, 601]);

    println!("testing segment from 1,000");
    test_segment(1_000, [7927, 7933, 7937, 7949, 7951, 7963, 7993, 8009, 8011, 8017]);

    let duration = start.elapsed();    
    println!("Duration: {:?}", duration);
}
