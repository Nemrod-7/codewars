#![allow(dead_code, unused)]
mod tests;

const WUMP:u8 = 0;
const PITT:u8 = 2;
const GOLD:u8 = 3;
const WIND:u8 = 4;
const SMEL:u8 = 5;
const SAFE:u8 = 6;

fn show(cave: &[[u8;4];4]) {
    for i in 0..4 {
        for j in 0..4 {
            let mut lsg = String::new();

            if check(cave[i][j], GOLD) { lsg += "G"; } 
            if check(cave[i][j], PITT) { lsg += "P"; }
            if check(cave[i][j], WUMP) { lsg += "W"; }
            if check(cave[i][j], WIND) { lsg += "w"; }
            if check(cave[i][j], SMEL) { lsg += "s"; }

            print!("[{:2}]", lsg);
        }       
        print!("\n");
    }
    print!("\n");
}

fn check(num:u8, x:u8) -> bool { num >> x &1 == 1 }
fn is_inside(x:i8, y:i8) -> bool { x >= 0 && y >= 0 && x < 4 && y < 4 }

fn next_dir(x:usize, y:usize) -> Vec<(usize,usize)> {
    [(0,1),(0,-1),(1,0),(-1,0)]
        .iter()
        .map(|(dx,dy)| (dx + x as i8, dy + y as i8) )
        .filter(|&(nx,ny)| is_inside(nx, ny))
        .map(|(nx,ny)| (nx as usize, ny as usize))
        .collect::<Vec<_>>()
}
fn neigh (grid: &[[u8;4];4], nx:usize, ny:usize, mark: u8) -> Vec<(usize,usize)> {
    let surr = next_dir(nx,ny)
        .iter()
        .filter(|(sx,sy)| check(grid[*sy][*sx], mark) )
        .map(|&(sx,sy)| (sx,sy))
        .collect::<Vec<_>>();

    if surr.len() < 2 { return vec![]; }
    surr
}
fn solve(cave: &mut [[u8; 4]; 4], exit: &(usize,usize)) -> bool {
    let mut grid = [[0; 4]; 4];
    let mut visit = [[0; 4]; 4];
    let mut queue = vec![(0,0)];

    visit[0][0] = 1;

    (0..4).for_each(|y| (0..4).for_each(|x| if cave[y][x] == 0 { grid[y][x] = 64; }));
    // next_dir(x,y).iter().for_each(|&(sx,sy)| grid[sy][sx] |= 1 << SAFE )

    while let Some((x,y)) = queue.pop() {
        if check(cave[y][x], PITT) { continue }
        if check(cave[y][x], WIND) { grid[y][x] |= 1 << WIND; }
        if check(cave[y][x], SMEL) { grid[y][x] |= 1 << SMEL; }
        // print!("{x} {y} : {}  ", grid[y][x]);
        for (nx,ny) in next_dir(x,y) {
            if grid[y][x] == 64 { grid[ny][nx] |= 1 << SAFE; }

            if !check(grid[ny][nx], SAFE) {
                if check(grid[y][x], WIND) { grid[ny][nx] |= 1 << PITT; }
                if check(grid[y][x], SMEL) { grid[ny][nx] |= 1 << WUMP; }

                if check(grid[ny][nx], WUMP) { // check the neighbour of a marked cell 
                // if there's more than 2 snell around it then then location is known : erase all other traces
                    neigh(&grid, nx, ny, SMEL).iter().for_each( |&(sx,sy)| { 
                        grid[sy][sx] &= !(1 << SMEL);
                        cave[sy][sx] &= !(1 << SMEL);
                    });
                }
                if check(grid[ny][nx], PITT) { // check the neighbour of a marked cell 
                // if there's more than 2 wind around it then then location is known : erase all other traces
                    neigh(&grid, nx, ny, WIND).iter().for_each( |&(sx,sy)| { 
                        grid[sy][sx] &= !(1 << WIND);
                        cave[sy][sx] &= !(1 << WIND);
                    });
                }

            }
            if check(grid[ny][nx], SAFE) && visit[ny][nx] == 0 {
                visit[ny][nx] = 1;
                queue.push( (nx, ny) );
            }

        }
    }

    // show(&grid);
    visit[exit.1][exit.0] == 1
}

pub fn wumpus_world(src: &[[char; 4]; 4]) -> bool {
    let mut exit = (0,0);
    let mut cave = [[0u8;4];4];
    let mut visit = [[0; 4]; 4];

    (0..4).for_each(|y|
        (0..4).for_each(|x|
            match src[y][x] {
                'G' => {
                    cave[y][x] |= 1 << GOLD;
                    exit = (x,y);
                } 
                'P' => {
                    cave[y][x] |= 1 << PITT;
                    next_dir(x,y).iter().for_each(|&(nx,ny)| cave[ny][nx] |= 1 << WIND );
                } 
                'W' => {
                    cave[y][x] |= 1 << WUMP;
                    next_dir(x,y).iter().for_each(|&(nx,ny)| cave[ny][nx] |= 1 << SMEL );
                }
                _ => (),
            }
        )
    );

    for _ in (0..4) {
        show(&cave);
        if solve(&mut cave, &exit) { return true }
    }

    false
}

// fn mk_grid(src: &Vec<String>) -> Vec<Vec<u8>> {
//     let src:Vec<Vec<char>> = src.iter().map(|x| x.chars().collect()).collect::<Vec<_>>();
//     let height = src.len();
//     let width = src[0].len();
//     let mut grid = vec![vec![0; 4]; 4];
//
//     for i in 0..height {
//         for j in 0..width {
//             let (x,y) = ((j * 4) / width, (i * 4) / height) ;
//
//             if let Some(pos) = "WAPGws".chars().position(|ch| ch == src[i][j] ) {
//                 grid[y][x] |= 1 << pos;
//             }
//
//             // if !"WAPGws".contains(src[i][j]) { continue; }
//             // if !grid[y][x].contains(src[i][j]) { grid[y][x] += &format!("{}",src[i][j] ) ; }
//         }
//     }
//
//     grid[0][0] = 0;
//     grid
// }
fn assert(i:usize, curr: ([[char;4]; 4], bool)) {
    let (maze, result) = curr;

    if wumpus_world(&maze) != result {
        print!("error : {i} expect : {result}\n");
    }

}
fn main () {

    let src = [
        "|------------|------------|------------|------------|",
        "|            |            |            |            |",
        "|            |            |            |            |",
        "|    Agent                                          |",
        "|            |            |            |  wwwwwwww  |",
        "|            |            |            |  wwwwwwww  |",
        "|-----  -----|-----  -----|-----  -----|-----  -----|",
        "|            |            |            |            |",
        "|            |            |         ww |            |",
        "|                                   ww       Pit    |",
        "|  ssssssss  |            |  wwwwwwwww |            |",
        "|  ssssssss  |            |  wwwwwwww  |            |",
        "|-----  -----|-----  -----|-----  -----|-----  -----|",
        "|            |            |            |  wwwwwwww  |",
        "|            | ss      ww |            | wwwwwwwww  |",
        "|   Wumpus     ss Gold ww       Pit      ww         |",
        "|            | ss      ww |            | ww         |",
        "|            |            |            |            |",
        "|-----  -----|-----  -----|-----  -----|-----  -----|",
        "|  ssssssss  |            |  wwwwwwww  |            |",
        "|  ssssssss  |            |  wwwwwwww  |            |",
        "|                                                   |",
        "|            |            |            |            |",
        "|            |            |            |            |",
        "|------------|------------|------------|------------|"];

    // let src = src.iter().map(|x| format!("{x}")).collect::<Vec<String>>();
    // let res = solve(&src);

    let i = 4;

    for i in 5..6 {

        assert(i, tests::TESTS[i]);
    }

}
