fn cntbits (mut mask: u16) -> usize {

    let mut cnt:usize = 0;

    while mask != 0 {
        if mask &1 != 0 { cnt += 1 }
        mask >>= 1;
    }

    cnt
}
fn exist (mask:u16, pos:usize) -> bool { mask &1 << pos != 0 }
fn mkmask (col:u16, row:u16, sub:u16) -> u16 {

    let mut mask:u16 = 0;

    for j in 1..=9 {
        if !exist (col, j) && !exist (row, j) && !exist (sub, j) {
            mask |= 1 << j;
        }
    }

    mask
}

fn backtrack (grid: &mut [[u8; 9]; 9],col: &mut Vec<u16>, row: &mut Vec<u16>, sub: &mut Vec<u16>, hist:&mut Vec<(usize,(usize,usize))>, index: usize) -> bool {

    if index == hist.len() { return true }

    let x = hist[index].1.0;
    let y = hist[index].1.1;
    let z = y / 3 * 3 + x / 3;

    for dig in 1..=9 {
        if !exist (col[x], dig) && !exist (row[y], dig) && !exist (sub[z], dig) {
            col[x] ^= 1 << dig; row[y] ^= 1 << dig; sub[z] ^= 1 << dig;
            grid[y][x] = dig as u8;

            if backtrack (grid, col, row, sub, hist, index + 1) { return true }

            grid[y][x] = 0;
            col[x] ^= 1 << dig; row[y] ^= 1 << dig; sub[z] ^= 1 << dig;
        }
    }

    false
}
fn sudoku (grid: &mut [[u8; 9]; 9]) {

    let mut hist:Vec<(usize,(usize,usize))> = Vec::new();
    let mut col:Vec<u16> = vec![0;10];
    let mut row:Vec<u16> = vec![0;10];
    let mut sub:Vec<u16> = vec![0;10];

    for y in 0..9 {
        for x in 0..9 {
            if grid[y][x] != 0 {
                let z = y / 3 * 3 + x / 3;

                col[x] |= 1 << grid[y][x];
                row[y] |= 1 << grid[y][x];
                sub[z] |= 1 << grid[y][x];
            }
        }
    }

    for y in 0..9 {
        for x in 0..9 {
            if grid[y][x] == 0 {
                let z = y / 3 * 3 + x / 3;
                let mask = mkmask (col[x], row[y], sub[z]);
                hist.push ((cntbits (mask), (x,y)));
            }
        }
    }

    hist.sort();

    backtrack (grid, &mut col, &mut row, &mut sub, &mut hist, 0);

}
