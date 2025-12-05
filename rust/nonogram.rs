#![allow(unused)]

type Clues = &'static [&'static [u8]];

static mut BITS:[u32;35] = [0u32; 35];

fn place (block: &[u8], total: u8) -> Vec<u32> {
    let dig = if block.len() == 0 { 0 } else { block[0] as usize } ;
    if dig > total as usize { return vec![]; }

    let start = total - dig as u8;

    unsafe {
        if BITS[dig] == 0 { BITS[dig] = !(!0 << dig); }

        match block.len() {
            0 => return vec![0],
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

fn resize(combs: &Vec<u32>, x:u32, cell:u8) -> Vec<u32> {
    combs.iter().filter(|curr| ((*curr >> x)&1) == cell as u32 ).map(|x| *x).collect::<Vec<_>>()
}

fn reduce(comb: &Vec<u32>, x:u32) -> u8 {
    let mut ht = 0;

    for curr in comb {
        ht |= 1 << ((curr >> x)&1);
        if ht == 3 { return 2; }
    }

    if ht == 1 { return 0; }
    return 1;
}

pub fn solve ((top, left): (Clues, Clues), width: usize, height: usize) -> Vec<Vec<u8>> {
    let mut running = true ;
    let mut grid = vec![vec![2u8;width];height];
    let mut west:Vec<Vec<u32>> = left.iter().map(|x| place(x, width as u8)).collect();
    let mut north:Vec<Vec<u32>> = top.iter().map(|x| place(x, height as u8)).collect();

    let mut idy:Vec<(usize,usize)> = (0..west.len()).map(|x| ( west[x].len(), x) ).collect();
    let mut idx:Vec<(usize,usize)> = (0..north.len()).map(|x| ( north[x].len(), x) ).collect();

    while running {
        running = false;
        idy.sort(); idx.sort();

        for &(_,y) in &idy {
            for &(_,x) in &idx {
                if grid[y][x] == 2 {
                    let a = reduce(&north[x], y as u32);
                    let b = reduce(&west[y], x as u32);

                    if a != 2 { grid[y][x] = a; }
                    if b != 2 { grid[y][x] = b; }
                } else {
                    north[x] = resize(&north[x], y as u32, grid[y][x]);
                    west[y] = resize(&west[y], x as u32, grid[y][x]);
                }

                if grid[y][x] == 2 { running = true; }
            }
        }
    }

    // display(&grid);
    grid
}

fn display(grid:&Vec<Vec<u8>>) {
    for y in 0..grid.len() {
        for x in 0..grid[0].len() {
            match grid[y][x] {
                0 => print!("  "),
                1 => print!("x "),
                2 => print!(". "),
                _ => print!("  "),
            }
        }
        print!("\n");
    }

}
fn show(num:u32, total:u8) {
    for i in 0..total {
        let bit = (num >> i) &1;
        print!("{bit}");
    }

    print!("\n");
}

fn main () {

    type Solution = &'static [&'static [u8]];

    const CLUES: (Clues, Clues) = (
        &[&[3], &[4], &[2, 2, 2], &[2, 4, 2], &[6], &[3]],
        &[ &[4], &[6], &[2, 2], &[2, 2], &[2], &[2], &[2], &[2], &[], &[2], &[2], ],
    );

    let width = 6;
    let height = 11;

    let res = solve(CLUES, width, height);


}
