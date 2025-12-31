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
fn show(cave: &[[usize;4];4]) -> String {
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

const WUMP:usize = 0;
const PITT:usize = 2;
const GOLD:usize = 3;
const WIND:usize = 4;
const SMEL:usize = 5;
const SAFE:usize = 6;
const SURR:usize = 7;

fn check(num:usize, x:usize) -> bool { num >> x &1 == 1 }
fn exist(vec: &Vec<Point>, p: &Point) -> bool { vec.iter().find(|&x| x == p) != None }
fn is_inside(x:i8, y:i8) -> bool { x >= 0 && y >= 0 && x < 4 && y < 4 }

fn cross(x:usize, y:usize) -> Vec<(usize,usize)> {
    [(0,1),(0,-1),(1,0),(-1,0)]
        .iter()
        .map(|(dx,dy)| (dx + x as i8, dy + y as i8) )
        .filter(|&(nx,ny)| is_inside(nx, ny))
        .map(|(nx,ny)| (nx as usize, ny as usize))
        .collect::<Vec<_>>()
}
fn diags(x:usize, y:usize) -> Vec<(usize,usize)> {
    [(1,1),(-1,-1),(1,-1),(-1,1)]
        .iter()
        .map(|(dx,dy)| (dx + x as i8, dy + y as i8) )
        .filter(|&(nx,ny)| is_inside(nx, ny))
        .map(|(nx,ny)| (nx as usize, ny as usize))
        .collect::<Vec<_>>()
}
fn neigh (grid: &[[usize;4];4], nx:usize, ny:usize, mark: usize) -> Vec<(usize,usize)> {
        cross(nx,ny).iter().filter(|(sx,sy)| check(grid[*sy][*sx], mark) ).map(|&(sx,sy)| (sx,sy)).collect::<Vec<_>>()
}
    fn sensor(grid: &[[usize;4];4], x:usize, y:usize) -> [usize;8] {
        let mut sense = [0;8];

        for (nx,ny) in cross(x,y) {
            for i in 0..8 {
                sense[i] += if check(grid[ny][nx], i ) { 1 } else { 0 };
            }
            sense[7] += 1;
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
    fn clean(&mut self, grid: &mut [[usize;4];4]) {
        (0..4).for_each(|y|
            (0..4).for_each(|x|
                if check(grid[y][x], SAFE) {
                    grid[y][x] &= !(1 << WUMP);
                    grid[y][x] &= !(1 << PITT);
                }
            ) );
    }

    fn evaluate(&mut self, grid: &mut [[usize; 4]; 4]) {
        for y in 0..4 {
            for x in 0..4 {
                let probe = sensor(grid,x,y);

                if probe[SAFE] >= 2 { // if the cell is surrounded by two explored cell
                    if check(grid[y][x], WUMP) {
                        if probe[SMEL] < 2 { // if there's less than two ptr, then it's a false positive
                            grid[y][x] &= !(1 << WUMP);

                            if grid[y][x] == 0 { grid[y][x] = 64 }
                        } else { // else it's a sure threat grid[y][x] = 65;
                            self.wumpus = Some( (x,y) );
                        }
                    }

                    if check(grid[y][x], PITT) {
                        if probe[WIND] < 2 { // and there less than 2 wind, then it's a false positive.
                            grid[y][x] &= !(1 << PITT);

                            if grid[y][x] == 0 { grid[y][x] = 64 }
                        } else { // otherwise it's a sure threat.
                            if !exist(&self.pits, &(x,y)) { self.pits.push( (x,y) ); }
                        }
                    }
                }
                
                if probe[SURR] - probe[SAFE] == 1 {
                    if check(grid[y][x], SMEL) {

                        self.wumpus = Some(neigh(grid, x,y, WUMP)[0]);
                    }
                    if check(grid[y][x], WIND) && probe[PITT] == 1 {
                        let next = neigh(grid, x,y, PITT);
                        if !exist(&self.pits, &next[0]) { self.pits.push( next[0] ); }
                    }
                }
            }
        }
    }

    fn explore(&mut self, cave: &mut [[usize; 4]; 4], grid: &mut [[usize; 4]; 4], npit: usize) {
        let mut visit = [[0; 4]; 4];
        let mut queue = vec![(0,0)];
        visit[0][0] = 1;

        self.clean(grid);
        self.evaluate(grid);

        if let Some( (x,y) ) = self.wumpus {
            (0..4).for_each(|y| (0..4).for_each(|x| {
                cave[y][x] &= !(1 << SMEL);
                grid[y][x] &= !(1 << SMEL);
                grid[y][x] &= !(1 << WUMP);
            }
            ));
            grid[y][x] |= 1 << SAFE
        }

        for &(x,y) in self.pits.iter() {
            diags(x,y).iter().for_each(|&(nx,ny)| if check(grid[ny][nx], PITT) && !exist(&self.pits, &(nx,ny)) { 
                grid[ny][nx] &= !(1 << PITT); 
                grid[ny][nx] |= 1 << SAFE;
            }) ;

            cross(x,y).iter().for_each(|&(nx,ny)| if check(grid[ny][nx], WIND) { 
                grid[ny][nx] &= !(1 << WIND);                
                cave[ny][nx] &= !(1 << WIND);
            }) ;
        }

        // append_file("result","==========================\n");
        // append_file("result", &(show(&grid) + "\n"));

        while let Some((x,y)) = queue.pop() {
            if check(cave[y][x], PITT) { continue }
            if check(cave[y][x], WIND) { grid[y][x] |= 1 << WIND; }
            if check(cave[y][x], SMEL) { grid[y][x] |= 1 << SMEL; }
            if check(cave[y][x], WUMP) { grid[y][x] &= !(1 << WUMP); }
            // print!("{} {} {}\n", x, y, grid[y][x]);
            for (nx,ny) in cross(x,y) {
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

        // append_file("result", &(show(&grid) + "\n"));
    }
}
pub fn wumpus_world(src: &[[char; 4]; 4]) -> bool {

    let mut puzz = Wumpus::new();
    let mut exit = (0,0);
    let mut cave = [[0;4];4];
    let mut grid = [[0;4];4];
    let mut npit = 0;
    grid[0][0] = 64;

    (0..4).for_each(|y|
        (0..4).for_each(|x|
            match src[y][x] {
                'G' => {
                    exit = (x,y);
                    cave[y][x] |= 1 << GOLD;
                }
                'P' => {
                    npit += 1;
                    cave[y][x] |= 1 << PITT;
                    cross(x,y).iter().for_each(|&(nx,ny)| cave[ny][nx] |= 1 << WIND );
                }
                'W' => {
                    cave[y][x] |= 1 << WUMP;
                    cross(x,y).iter().for_each(|&(nx,ny)| cave[ny][nx] |= 1 << SMEL );
                }
                _ => (),
            }
        )
    );
    let probe = sensor(&cave, exit.0, exit.1);
    if probe[PITT] == probe[SURR] { return false }
    // append_file("result", &(show(&cave) + "\n"));
    // print!("[{} {}] -> {:?}\n", exit.0, exit.1, sensor(&cave, exit.0, exit.1));
    for _ in (0..5) {
        puzz.explore(&mut cave, &mut grid, npit);

        if check(grid[exit.1][exit.0], SAFE) {
            return true
        }
    }

    false
}
// fn score(&mut self, grid: &mut [[usize; 4]; 4]) -> Vec<(Option<usize>, Point)> {
//     let mut hist = vec![];
//
//     for &(x,y) in self.pits.iter() {
//         let minv = cross(x,y).iter().filter(|&&(nx,ny)| check(grid[ny][nx], WIND)).map(|(nx,ny)| {
//                 let mut probe = sensor(grid,*nx,*ny);
//                 probe[SURR] - probe[SAFE]
//         } ).min();
//         // print!("[{} {}] -> {}\n", x, y, minv);
//         hist.push( (minv, (x,y)) );
//     }
//     hist.sort();
//     hist
// }
fn assert(i:usize, curr: ([[char;4]; 4], bool)) {
    let (maze, result) = curr;

    print!("::{i}::\n");
    if wumpus_world(&maze) != result {
        print!("error : {i} expect : {result}\n");
    }

}

fn main () {

    // clear_file("result");

    // assert(0, tests::TESTS[11]);

    // print!("{}", tests::TESTS.len() );
    for i in 0..tests::TESTS.len() {
        assert(i, tests::TESTS[i]);
    }

}
