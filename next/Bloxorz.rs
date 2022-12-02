#![allow(warnings, unused)]

mod blox {
    use std::cmp::Ordering;
    use std::collections::{BinaryHeap, HashMap};

    #[derive(Clone, Eq, PartialEq)] // #[derive(Clone, Eq, PartialEq)]
    struct Vertex {
        index: usize,
        posit: ((i32,i32),(i32,i32)),
        visit: Vec<Vec<usize>>,
        route: String,
    }

    impl Ord for Vertex {
        fn cmp(&self, other: &Self) -> Ordering {
            other.index.cmp(&self.index)
        }
    }

    impl PartialOrd for Vertex {
        fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
            Some(self.cmp(other))
        }
    }

    fn isinside (grid: &Vec<Vec<char>>, x: i32, y: i32) -> bool {
        if x >= 0 && y >= 0 && y < grid.len() as i32 && x < grid[0].len() as i32 {
            if grid[y as usize][x as usize] != '0' { return true }
        }
        false
    }
    fn distance (a:(i32,i32), b:(i32,i32)) -> usize {
        let xx = (a.0 - b.0).abs() as usize;
        let yy = (a.1 - b.1).abs() as usize;
        xx + yy
    }
    fn display (level: &Vec<Vec<char>>, visit: &Vec<Vec<usize>>) {
        let width = level[0].len();
        let height = level.len();

        for y in 0..height {
            for x in 0..width {
                if level[y][x] == '0' {
                    print! (" ")
                } else {
                    print! ("{}", visit[y][x])
                }
            }
            print! ("\n");
        }
        print! ("\n");
    }

    pub fn blox_solver (puzzle: &[&str]) -> String {

        let width = puzzle[0].len();
        let height = puzzle.len();
        let compas = [(0,-1),(1,0),(0,1),(-1,0)];
        let letter = ["U","R","D","L"];

        let mut start = (0,0);
        let mut exit = (0,0);
        let mut hist = vec![vec![0; width]; height];
        let mut heap = BinaryHeap::new();
        let mut level:Vec<Vec<char>> = Vec::new();
        //let mut visited:HashMap<Vec<Vec<usize>>,bool> = HashMap::new();
        let mut visited:HashMap<((i32,i32),(i32,i32)), bool> = HashMap::new();

        for y in 0..height {
            level.push (puzzle[y].chars().collect::<Vec<char>>());
            for x in 0..width {

                match level[y][x] {
                    'X' => exit = (x as i32,y as i32),
                    'B' => start = (x as i32,y as i32),
                    _ => (),
                }
            }
        }

        hist[start.1 as usize][start.0 as usize] = 1;
        heap.push (Vertex { index:0,  posit:(start,start), visit:hist.clone(), route:"".to_string() });

        let mut cycle = 0;

        while let Some (Vertex { index, posit, visit, route }) = heap.pop() {

            cycle += 1;
            if cycle == 1900 { break }

            let state = if posit.0 == posit.1 { 1 } else { 2 };

            if posit.0 == exit && state == 1 {
                display (&level, &visit);
                //print! ("{route} => {cycle} cycles\n");
                return route
            }

            for i in 0..4 {
                let dir = compas[i];
                let dx = posit.1.0 - posit.0.0;
                let dy = posit.1.1 - posit.0.1;

                let x1 = dir.0 + posit.0.0;
                let y1 = dir.1 + posit.0.1;
                let x2 = if state == 1 { dir.0 * 2 + posit.0.0 } else { dir.0 + posit.1.0 };
                let y2 = if state == 1 { dir.1 * 2 + posit.0.1 } else { dir.1 + posit.1.1 };

                if isinside (&level, x2, y2) && isinside (&level, x1, y1) {

                    let id;
                    let blok;
                    let path = route.clone() + letter[i];
                    let alt = route.len();// + distance ((x2,y2), exit);
                    let mut grid = visit.clone();

                    if state == 1 || (dx != dir.0 && dy != dir.1) {
                        id = if (x2 - x1).abs() == 0 { 2 } else { 3 };
                        blok = ((x1,y1),(x2,y2));
                    } else {
                        id = 1;
                        blok = if dx == dir.0 && dy == dir.1 { ((x2,y2),(x2,y2)) } else { ((x1,y1),(x1,y1)) }
                    }

                    grid[blok.0.1 as usize][blok.0.0 as usize] = id;
                    grid[blok.1.1 as usize][blok.1.0 as usize] = id;

                    if visited.get (&blok) == None {
                        heap.push (Vertex { index:alt, posit:blok, visit:grid.clone(), route:path });
                        visited.insert(blok, true);
                    }
                }
            }
        }

        "".to_string()
    }
}

fn main() {

    let level1: Vec<&str> = vec![ "1110000000", "1B11110000", "1111111110", "0111111111", "0000011X11", "0000001110" ];
    let level2 = vec![ "000000111111100", "111100111001100", "111111111001111", "1B11000000011X1", "111100000001111", "000000000000111"];
    let level3 = vec![ "00011111110000", "00011111110000", "11110000011100", "11100000001100", "11100000001100", "1B100111111111", "11100111111111", "000001X1001111", "00000111001111"];
    let level5 = vec!["01100000000000000000", "11110000011001111111", "11111111111001111111", "1B111111111001110011", "11110000011111110011", "11110000011111110011", "00000000000000000011", "11111111101110011011", "11X11011101111111111", "11110011111110011000"]; // => "RRRRRRDRRRURURRRDDDDLLLLLDLURDLLLUULLDL" 
let level6 = vec!["00000000011110000", "00000011111111100", "00000011111001110", "11101111100001111", "1B1111100000011X1", "11101111000000111", "00000011100110110", "00000011111111110", "00000001111111100"]; // => "RRRDRDRDRURRRURU"
    //blox::blox_solver (&level3);
    example_tests();

     print! ("\n");
}

fn example_tests() {

    let fixed_tests = [
        vec![ "1110000000", "1B11110000", "1111111110", "0111111111", "0000011X11", "0000001110"],
        vec![ "000000111111100", "111100111001100", "111111111001111", "1B11000000011X1", "111100000001111", "000000000000111"],
        vec![ "00011111110000", "00011111110000", "11110000011100", "11100000001100", "11100000001100", "1B100111111111", "11100111111111", "000001X1001111", "00000111001111"],
        vec![ "11111100000", "1B111100000", "11110111100", "11100111110", "10000001111", "11110000111", "11110000111", "00110111111", "01111111111", "0110011X100", "01100011100"],
        vec![ "000001111110000", "000001001110000", "000001001111100", "B11111000001111", "0000111000011X1", "000011100000111", "000000100110000", "000000111110000", "000000111110000", "000000011100000"],
        vec!["01100000000000000000", "11110000011001111111", "11111111111001111111", "1B111111111001110011", "11110000011111110011", "11110000011111110011", "00000000000000000011", "11111111101110011011", "11X11011101111111111", "11110011111110011000"],
        vec!["00000000011110000", "00000011111111100", "00000011111001110", "11101111100001111", "1B1111100000011X1", "11101111000000111", "00000011100110110", "00000011111111110", "00000001111111100"]

    ];


    let fixed_sols = [
        vec!["RRDRRRD","RDDRRDR","RDRRDDR"],
        vec!["ULDRURRRRUURRRDDDRU","RURRRULDRUURRRDDDRU"],
        vec!["ULURRURRRRRRDRDDDDDRULLLLLLD"],
        vec!["DRURURDDRRDDDLD"],
        vec!["RRRDRDDRDDRULLLUULUUURRRDDLURRDRDDR","RRRDDRDDRDRULLLUULUUURRDRRULDDRRDDR","RRRDRDDRDDRULLLUULUUURRDRRULDDRRDDR"],
        vec!["RRRRRRDRRRURURRRDDDDLLLLLDLURDLLLUULLDL","RRRRRRDRRRURURRRDDDDLLLLLDLLLURDLUULLDL"],
        vec!["RRRDRDRDRURRRURU"]
    ];

    for (grid,valids) in fixed_tests.iter().zip(fixed_sols.iter()) {
        let user = blox::blox_solver(grid);

        if valids.iter().any(|x| x == &user) == false {
            print!("got : {user}\nexp : ");

            for it in valids {
                print!("{it} ");
            }
            print!("\n");
        }
        //assert!(valids.iter().any(|s| s == &user),"You returned an invalid path");
    }
}
