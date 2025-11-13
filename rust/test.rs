#![allow(warnings)]

static mut BITS:[u64;35] = [0u64; 35];
static mut DP:Vec<Vec<u64>> = Vec::new();

fn place (block: &[u8], total: u8) -> Vec<u64> {
    if block.len() == 0 { return vec![0]; }
    if block[0] > total { return vec![]; }

    let dig = block[0] as usize; 
    let start = total - block[0];

    unsafe {
        if BITS[dig] == 0 { BITS[dig] = !(!0 << dig); } 

        match block.len() {
            1 => return (0..start + 1).map(|i| BITS[dig] << i).collect(),
            _ => return (0..start + 0).map(|i|
                place( &block[1..], start - i - 1)
                .iter()
                .map(|sol| (BITS[dig] << i) | (sol << (dig as u8 + i + 1 )) )
                .collect::<Vec<_>>()
            ).flatten().collect(),
        }
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

    let res = (0..10)
        .map(|i| 
            (0..5)
            .map(|x| i + x).collect::<Vec<_>>()
            ).flatten() .collect::<Vec<_>>();


    print!("{:?}", res);
}
