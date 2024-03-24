
fn sieve(num: usize) -> Vec<i64> {
    let mut sieve = vec![true; (num >> 1) + 1];
    let mut prime = vec![2];

    (3..=num)
        .step_by(2)
        .for_each(
            |i| if sieve[i >> 1] == true {
                prime.push(i as i64);
                (i*i..=num).step_by(2 * i).for_each(|j| sieve[j >> 1] = false)
            } 
            );

    prime
}

fn sudoku (grid: [[u8;9];9]) {

    let mut x;
    let mut y;
    let mut z;

    for i in 0..81 {
        y = i / 9, x = i % 9, z = y / 3 * 3 + x / 3;

        if grid[y][x] != 0 {
            //col[x] |= 1 << grid[y][x];
            //row[y] |= 1 << grid[y][x];
            //sub[z] |= 1 << grid[y][x];
        }
        print!("{y} {x}\n");
    }


}
fn cntbits1 (mut mask: u16) -> usize {

    let mut cnt:usize = 0;

    while mask != 0 {
        if mask &1 != 0 { cnt += 1 }
        mask >>= 1;
    }

    cnt
}

fn cntbits2 (mut mask: u16) -> usize {
    let mut cnt:usize = 0;

    while mask != 0 {
        mask &= (mask - 1);
    }

    cnt
}
fn main () {

    for i in 0..10 {
        
    }


}
