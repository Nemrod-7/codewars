#![allow(warnings)]
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

type Point = (usize, usize);

const WUMP:u8 = 0;
const PITT:u8 = 2;
const GOLD:u8 = 3;
const WIND:u8 = 4;
const SMEL:u8 = 5;
const SAFE:u8 = 6;

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

    surr
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

struct Wumpus {
    pits : Vec<Point>,
    wumpus : Option<Point>,
}

impl Wumpus {
    fn new() -> Wumpus {
        Wumpus { pits:vec![], wumpus:None }
    }

    fn clean(&mut self, grid: &mut [[u8;4];4]) {
        (0..4).for_each(|y| 
            (0..4).for_each(|x| 
                if check(grid[y][x], SAFE) { 
                    grid[y][x] &= !(1 << WUMP);
                    grid[y][x] &= !(1 << PITT); 
                } 
        ) );
    }
    fn evaluate(&mut self, grid: &mut [[u8; 4]; 4]) {
        let mut wump:Option<(usize,usize)> = None;
        let mut pitt:Vec<(usize,usize)> = Vec::new();
        // evaluation
        for y in 0..4 {
            for x in 0..4 {
                let surr = next_dir(x,y);
                let probe = sensor(grid,x,y);
                // print!("{} {} {:?}\n", x, y, sensor(grid, x, y));

                if check(grid[y][x], SMEL) { // if neigh - safe == 1 { wumpus found = true; }
                    if surr.len() - probe[SAFE as usize] == 1 {
                        wump = Some(neigh(grid, x,y, WUMP)[0]);
                    }
                } 

                if check(grid[y][x], WIND) {
                    if surr.len() - probe[SAFE as usize] == 1 {
                        let next = neigh(grid, x,y, PITT);

                        if pitt.iter().find(|&p| *p == next[0]) == None {
                            pitt.push( next[0] );
                        }
                    }
                }

                if check(grid[y][x], WUMP) {
                    if probe[SAFE as usize] >= 2 { // if the cell is surrounded by two explored cell 
                        if probe[SMEL as usize] < 2 { // if there's less than two ptr, then it's a false positive
                            grid[y][x] |= 1 << SAFE;
                        } else { // else it's a sure threat
                            wump = Some((x,y));
                        }
                    }        
                } 

                if check(grid[y][x], PITT) {
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
    }

    fn explore(&mut self, cave: &mut [[u8; 4]; 4], grid: &mut [[u8; 4]; 4], npit: usize) {
        let mut visit = [[0; 4]; 4];
        let mut queue = vec![(0,0)];
        visit[0][0] = 1;

        self.clean(grid);
        self.evaluate(grid);

        if let Some( (x,y) ) = self.wumpus {
        //         (0..4).for_each(|y| (0..4).for_each(|x|  grid[y][x] &= !(1 << SMEL) ));
        //         (0..4).for_each(|y| (0..4).for_each(|x|  grid[y][x] &= !(1 << WUMP) ));
        //         // print!("wumpus {:?}\n", (x,y));
        //         grid[y][x] |= 1 << WUMP;
        //         grid[y][x] |= 1 << SAFE;
        }
        if self.pits.len() == npit {
        //     // print!("{} pitts discovered: {:?}.\n", npits, pitt);
        //     (0..4).for_each(|y| (0..4).for_each(|x| 
        //         if check(grid[y][x], PITT) { 
        //             grid[y][x] |= 1 << SAFE;
        //             grid[y][x] &= !(1 << PITT);
        //         }
        //     ));
        //
        //     for (x,y) in pitt { 
        //         grid[y][x] |= 1 << PITT; 
        //         grid[y][x] &= !(1 << SAFE);
        //     }
        } 

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


    }
}
pub fn wumpus_world(src: &[[char; 4]; 4]) -> bool {

    let mut puzz = Wumpus::new();
    let mut exit = (0,0);
    let mut cave = [[0u8;4];4];
    let mut grid = [[0u8;4];4];
    let mut npit = 0;
    grid[0][0] = 64;

    (0..4).for_each(|y|
        (0..4).for_each(|x|
            match src[y][x] {
                'G' => {
                    exit = (x,y);
                    // cave[y][x] |= 1 << GOLD;
                }
                'P' => {
                    npit += 1;
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

    for _ in (0..2) {
        puzz.explore(&mut cave, &mut grid, npit);

        if check(grid[exit.1][exit.0], SAFE) { 
            return true 
        }
    }

    false
}



fn main() {


}
