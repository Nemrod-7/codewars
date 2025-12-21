#![allow(dead_code, unused)]
use std::fs::File;
use std::io::Write;

mod tests;

fn clear_file(filename: &str) {
    let mut file = match File::create("result") {
        Err(why) => panic!("couldn't create : {}", why),
        Ok(file) => file,
    };
}
fn append_file(filename: &str, text: &str) {
    let mut file = match File::options().append(true).open(filename) {
        Err(why) => panic!("couldn't create : {}", why),
        Ok(file) => file,
    };

    match file.write_all(text.as_bytes()) {
        Err(why) => panic!("couldn't write to : {}", why),
        Ok(_) => (),
    }
}

const WUMP:u8 = 0;
const PITT:u8 = 2;
const GOLD:u8 = 3;
const WIND:u8 = 4;
const SMEL:u8 = 5;
const SAFE:u8 = 6;

fn show(cave: &[[u8;4];4]) -> String {
    let mut os = String::new();

    for i in 0..4 {
        for j in 0..4 {
            let mut lsg = String::new();

            if check(cave[i][j], GOLD) { lsg += "G"; }
            if check(cave[i][j], PITT) { lsg += "P"; }
            if check(cave[i][j], WIND) { lsg += "p"; }
            if check(cave[i][j], WUMP) { lsg += "W"; }
            if check(cave[i][j], SMEL) { lsg += "w"; }
            if check(cave[i][j], SAFE) { lsg += "."; }

            os += &format!("[{:3}]", lsg);
        }
        os += &format!("\n");
    }

    os
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

    append_file("result", & "====================\n");
    append_file("result", &(show(&cave) + "\n"));

    while let Some((x,y)) = queue.pop() {
        if check(cave[y][x], PITT) { continue }
        if check(cave[y][x], WIND) { grid[y][x] |= 1 << WIND; }
        if check(cave[y][x], SMEL) { grid[y][x] |= 1 << SMEL; }
        // print!("{x} {y} : {}  ", grid[y][x]);
        
        append_file("result", &(show(&grid) + "\n"));

        for (nx,ny) in next_dir(x,y) {
            if grid[y][x] == 64 { grid[ny][nx] |= 1 << SAFE; }

            if !check(grid[ny][nx], SAFE) {
                if check(grid[y][x], WIND) { grid[ny][nx] |= 1 << PITT; }
                if check(grid[y][x], SMEL) { grid[ny][nx] |= 1 << WUMP; }

                if check(grid[ny][nx], WUMP) { // check the neighbour of a marked cell
                                               // if there's more than 2 snell around it then then location is known : erase all other traces
                    neigh(&grid, nx, ny, SMEL).iter().for_each( |&(sx,sy)| {
                        // grid[sy][sx] &= !(1 << SMEL);
                        cave[sy][sx] &= !(1 << SMEL);
                    });
                }

                if check(grid[ny][nx], PITT) { // check the neighbour of a marked cell
                                               // if there's more than 2 wind around it then then location is known : erase all other traces

                    print!("{} {} : {} \n", x, y, grid[y][x]);
                    neigh(&grid, nx, ny, WIND).iter().for_each( |&(sx,sy)| {
                        // grid[sy][sx] &= !(1 << WIND);
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


    visit[exit.1][exit.0] == 1
}

pub fn wumpus_world(src: &[[char; 4]; 4]) -> bool {
    let mut exit = (0,0);
    let mut cave = [[0u8;4];4];

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

    for _ in (0..1) {
        if solve(&mut cave, &exit) { 
            return true 
        }
    }

    false
}

fn assert(i:usize, curr: ([[char;4]; 4], bool)) {
    let (maze, result) = curr;

    if wumpus_world(&maze) != result {
        print!("error : {i} expect : {result}\n");
    }

}

fn main () {

    clear_file("result");

    assert(0, tests::TESTS[5]);

    for i in 0..0 {
        assert(i, tests::TESTS[i]);
    }

}
