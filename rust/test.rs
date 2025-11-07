#![allow(warnings)]

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
        // while DP.len() <= dig { DP.push(vec![]); }
        if block.len() == 1 { 
            // print!("len 1 :[{}] \n", dig);
            // if DP[dig].len() == 0 {
                for i in 0..(start + 1) {
                    // DP[dig].push(BITS[dig] << i);
                    res.push(BITS[dig] << i);
                }
            // }
        } else {

            for i in 0..start {
                for sol in place( &block[1..], start - i - 1) {
                    // DP[dig].push( (BITS[dig] << i) | (sol << (dig as u8 + i + 1 )) );
                    res.push( (BITS[dig] << i) | (sol << (dig as u8 + i + 1 )) );

                }
            }
        }

        return res;
    }
}

fn show(num:u32, total:u8) {
    for i in 0..total {
        let bit = (num >> i) &1;
        print!("{bit}");
    }

    print!("\n");
}

fn main() {

    let total = 6;
    let comb = place(&[2, 2], total);

    for co in comb {
        show(co as u32, total);
    }

    unsafe {

        for i in 0..DP.len() {
            for j in 0..DP[i].len() {
                show(DP[i][j] as u32, total);

            }
            print!("\n");
        }


        for i in 0..10 {
            print!("{} ", BITS[i] );
        }

    }

}
