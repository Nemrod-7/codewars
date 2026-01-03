#![allow(warnings)]

const N:usize = 7;

fn bit2int(mut byte:u8) -> u8 {
    let mut dig = 0;
    let mut one = 0;

    while byte != 0 {
        if byte &1 == 1 { 
            one = dig;
            // print!("{} ", dig);
        }
        dig += 1;
        byte >>= 1;
    }
    print!("\n");
    0
}
fn filter (line: Vec<u8>) -> Vec<u8> {

    let mut hist = [0; N + 1];
    let mut uniq = [0; N + 1];

    for i in 0..N {
        let mut byte = line[i];
        let mut dig = 0;
        let mut one = 0;

        while byte != 0 {
            if byte &1 == 1 { 
                one = dig; 
                hist[dig] += 1;
            }
            dig += 1;
            byte >>= 1;
        }
        // if is a power of two
        if u8::count_ones(byte) == 1 { uniq[one] = 1; }
    }

    for i in 0..N {
        let mut cell = line[i];
        let mut byte = line[i];

        if u8::count_ones(cell) > 1 {
            let mut dig = 0;

            while byte != 0 {
                if byte &1 == 1 {
                    if hist[dig] == 1 { cell = 1 << dig; }
                    if uniq[dig] == 1 { cell ^= 1 << dig; }
                }
                dig += 1;
                byte >>= 1;
            }
        }
    }

    line
}

fn main() {

    let mut grid = [0; 49];

    let y = 5;

    let line = vec![254, 128, 48, 6, 12, 4, 2];
    filter(line);

    print!("\n");

    let mut row = [[0; N]; N];
    let mut col = [[0; N]; N];

    for i in 0..N {
        for j in 0..N {
            print!("[{}]", grid[i * N + j]);
        }
        print!("\n");
    }
}
