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
// const THRE:u8 = 6;

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

            os += &format!("[{:4}]", lsg);
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
fn sensor(grid: &[[u8;4];4], x:usize, y:usize) -> [usize;8] {
    let mut sense = [0;8];

    for (nx,ny) in next_dir(x,y) {
        for i in 0..8 {
            sense[i] += if check(grid[ny][nx], i as u8) { 1 } else { 0 };
        }
    } 

    sense
}
fn neigh (grid: &[[u8;4];4], nx:usize, ny:usize, mark: u8) -> Vec<(usize,usize)> {
    let surr = next_dir(nx,ny)
        .iter()
        .filter(|(sx,sy)| check(grid[*sy][*sx], mark) )
        .map(|&(sx,sy)| (sx,sy))
        .collect::<Vec<_>>();

    surr
}

fn evaluate(grid: &mut [[u8; 4]; 4]) -> bool {
    let mut found = false;
    // if let Some((wx,wy)) = detect_wumpus(grid) {
    //     // (0..4).for_each(|y| (0..4).for_each(|x| if check(cave[y][x], SMEL) { cave[y][x] &= !(1 << SMEL) }));
    //     // (0..4).for_each(|y| (0..4).for_each(|x| if check(grid[y][x], SMEL) { grid[y][x] &= !(1 << SMEL) }));
    //     // (0..4).for_each(|y| (0..4).for_each(|x| if check(grid[y][x], WUMP) { grid[y][x] &= !(1 << WUMP) }));
    //
    //     // grid[wy][wx] |= 1 << WUMP;
    // }
    // (0..4).for_each(|y| (0..4).for_each(|x| if check(grid[y][x], SAFE) { grid[y][x] &= !(1 << WUMP) }) );

    let mut wump:Option<(usize,usize)> = None;
    let mut pitt:Vec<(usize,usize)> = Vec::new();
    // map cleaning
    for y in 0..4 {
        for x in 0..4 {
            let surr = next_dir(x,y);
            let probe = sensor(grid,x,y);
            // print!("{} {} {:?}\n", x, y, sensor(grid, x, y));
            if check(grid[y][x], SAFE) {
                grid[y][x] &= !(1 << WUMP);
                grid[y][x] &= !(1 << PITT);
            }

            if check(grid[y][x], SMEL) { // if neigh - safe == 1 { wumpus found = true; }
                // print!("{:?}\n", neigh(grid, x,y, WUMP));
                if surr.len() - probe[SAFE as usize] == 1 {
                    found = true;
                    wump = Some(neigh(grid, x,y, WUMP)[0]);
                }
            } else if check(grid[y][x], WIND) {

                if surr.len() - probe[SAFE as usize] == 1 {
                    let next = neigh(grid, x,y, PITT);

                    if pitt.iter().find(|&p| *p == next[0]) == None {
                        pitt.push( next[0] );
                    }
                }
            } else if check(grid[y][x], PITT) {

                if probe[SAFE as usize] >= 2 { // if the cell is surrounded by two explored cell 

                    if probe[WIND as usize] < 2 { // and there less than 2 wind, then it's a false positive.
                        grid[y][x] &= !(1 << PITT);
                    } else { // otherwise it's a sure threat.
                        if pitt.iter().find(|&p| *p == (x,y) ) == None {
                            pitt.push( (x,y) );
                        }
                    }
                } 
            }
        }
    }

    if pitt.len() == 3 {
        print!("three pitts discovered: {:?}.\n", pitt);
        // (0..4).for_each(|y| (0..4).for_each(|x|  grid[y][x] &= !(1 << WIND) ));
        // (0..4).for_each(|y| (0..4).for_each(|x|  grid[y][x] &= !(1 << PITT) ));
        (0..4).for_each(|y| (0..4).for_each(|x| if check(grid[y][x], PITT) { 
            grid[y][x] |= 1 << SAFE;
            grid[y][x] &= !(1 << PITT);
        }));

        for (x,y) in pitt { 


            grid[y][x] |= 1 << PITT; 
            grid[y][x] &= !(1 << SAFE)
        }
    }

    if let Some ( (x,y) ) = wump {
        (0..4).for_each(|y| (0..4).for_each(|x|  grid[y][x] &= !(1 << SMEL) ));
        (0..4).for_each(|y| (0..4).for_each(|x|  grid[y][x] &= !(1 << WUMP) ));

        grid[y][x] |= 1 << WUMP;
        grid[y][x] |= 1 << SAFE;
    }

    found
}
fn explore(cave: &mut [[u8; 4]; 4], grid: &mut [[u8; 4]; 4], found: &mut bool) {
    let mut visit = [[0; 4]; 4];
    let mut queue = vec![(0,0)];

    if evaluate(grid) { 
        (0..4).for_each(|y| (0..4).for_each(|x| { cave[y][x] &= !(1 << SMEL) }));
        *found = true 
    }

    visit[0][0] = 1;
    // (0..4).for_each(|y| (0..4).for_each(|x| if cave[y][x] == 0 { grid[y][x] = 64; }));
    // next_dir(x,y).iter().for_each(|&(sx,sy)| grid[sy][sx] |= 1 << SAFE )

    append_file("result", & "====================\n");
    append_file("result", &(show(&grid) + "\n"));

    while let Some((x,y)) = queue.pop() {
        if check(cave[y][x], PITT) { continue }
        if check(cave[y][x], WIND) { grid[y][x] |= 1 << WIND; }
        if check(cave[y][x], SMEL) { grid[y][x] |= 1 << SMEL; }
        if check(cave[y][x], WUMP) { grid[y][x] &= !(1 << WUMP) }


        for (nx,ny) in next_dir(x,y) {
            if grid[y][x] == 64 { grid[ny][nx] |= 1 << SAFE; }

            if !check(grid[ny][nx], SAFE) {
                if check(grid[y][x], WIND) { grid[ny][nx] |= 1 << PITT; }
                if check(grid[y][x], SMEL) { grid[ny][nx] |= 1 << WUMP; }
            }

            if check(grid[ny][nx], SAFE) && visit[ny][nx] == 0 {
                visit[ny][nx] = 1;
                queue.push( (nx, ny) );
            }
        }
    }

    append_file("result", &(show(&grid) + "\n"));
}

pub fn wumpus_world(src: &[[char; 4]; 4]) -> bool {
    let mut exit = (0,0);
    let mut cave = [[0u8;4];4];
    let mut grid = [[0u8; 4]; 4];
    let mut found = false;
    grid[0][0] = 64;

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

    append_file("result", &(show(&cave) + "\n"));

    for _ in (0..5) {
        explore(&mut cave, &mut grid, &mut found);

        if check(grid[exit.1][exit.0], SAFE) { 
            return true 
        }
    }

    false
}

fn assert(i:usize, curr: ([[char;4]; 4], bool)) {
    let (maze, result) = curr;

    print!("::{i}::\n");
    if wumpus_world(&maze) != result {
        print!("error : {i} expect : {result}\n");
    }

}

fn main () {

    clear_file("result");

    assert(0, tests::TESTS[6]);

    for i in 0..11 {
        // assert(i, tests::TESTS[i]);
    }

}
