//Square and Cube of a Number Become Prime When Reversed

use std::sync::Mutex;
static MEM:Mutex<Vec<u64>> = Mutex::new(Vec::new());

fn is_prime (num: u64) -> bool {

    if num % 2 == 0 || num % 3 == 0 { return false }
    if num == 1 { return false }

    let end = (num as f64).sqrt() as u64;
    let mut i = 5;

    while i <= end {
    //print! (" {num} ");
        if num % i == 0 || num % (i + 2) == 0 { return false }
        i += 6;
    }
    
    true
}
fn rev (mut num: u64) -> u64 {
    
    let mut ret:u64 = 0;
    
    while num != 0 {
        let prod = ret * 10;
        ret = prod + num % 10;
        num /= 10;
    }
    
    ret
}
fn sq_cub_rev_prime (n: u32) -> u32 {
    
    let n = n as usize;
    let mut num;
    let mut seq = MEM.lock().unwrap();

    if seq.len() >= n { return seq[n - 1] as u32 }

    match seq.last() {
        Some (val) => num = *val,
        None => num = 88,
    }
  
    while seq.len() != n {
        num += 1;
        let sqr = num * num;
        let cub = sqr * num;

        if is_prime (rev(sqr)) == true && is_prime (rev(cub)) == true {
            seq.push(num);
        }
    }

    num as u32
}
