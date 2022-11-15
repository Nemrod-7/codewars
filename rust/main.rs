use std::sync::Mutex;
static MEM:Mutex<Vec<u64>> = Mutex::new(Vec::new());

fn is_prime (num: u64) -> bool {

    match num {
        1 => return false,
        2 => return true,
        3 => return true,
        _ => {
            if num % 2 == 0 || num % 3 == 0 { return false }
            let mut i = 5;
            while i * i <= num {
                if num % i == 0 || num % (i + 2) == 0 { return false }
                i += 6;
            }
        },
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
fn show (n:u32) {

    let res = sq_cub_rev_prime (n);
    print!("[{n}] => {res}\n");
}
fn main () {
    
    let ve:Vec<u64> = Vec::with_capacity(250);

    show(50); // 10499
    show(1); 
    show(50); // 10499


    for k in 1..16 {
    //    sq_cub_rev_prime (k);
    }


    print!("\n");
}

/*

struct An {
    n: i64,
    elements: Vec<i64>,
    generated: bool,
}
impl An {
    fn new() -> An {
        An {
            n: 1,
            elements: vec![7],
            generated: false,
        }
    }
}
impl Iterator for An {
    type Item = i64;

    fn next(&mut self) -> Option<Self::Item> {
        if self.generated {
            let previous_last = *(self.elements.last().unwrap_or(&7i64));

            self.elements
                .push(previous_last + gcd(self.n, previous_last));
        }
        self.n += 1;
        self.generated = true;
        Some(*(self.elements.last().unwrap()))
    }
}

*/


