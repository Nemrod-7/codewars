#![allow(unused)]

static mut BITS:[u64;35] = [0u64; 35];
static mut DP:Vec<Vec<u64>> = Vec::new();

fn place (block: &[u8], total: u8) -> Vec<u64> {
    if block.len() == 0 { return vec![0]; }
    if block[0] > total { return vec![]; }

    let dig = block[0] as usize; 
    let start = total - block[0];
    let mut res:Vec<u64> = Vec::new();

    unsafe {
        if BITS[dig] == 0 { BITS[dig] = !(!0 << dig); } 

        while DP.len() < (start + 1) as usize {
            DP.push(vec![]);
        }

        if block.len() == 1 { 
            if DP[dig].len() == 0 {
                for i in 0..(start + 1) {
                    DP[dig].push(BITS[dig] << i);
                }
            }
            
            return DP[dig].clone();
        } else {
            for i in 0..start {
                for sol in place( &block[1..], start - i - 1) {
                    res.push( (BITS[dig] << i) | (sol << (dig as u8 + i + 1 )) );
                }
            }
        }
    }

    res
}


fn main() {

    let comb = place(&[3, 2],10);

    unsafe {

        for i in 0..10 {
            print!("{} ", BITS[i] );
        }

    }

}
