
mod blox {
    use std::cmp::Ordering;
    use std::collections::{BinaryHeap, HashMap};
    //use std::collections::HashMap;

    #[derive(Clone, Eq, PartialEq)] // #[derive(Clone, Eq, PartialEq)]
    struct Vertex {
        index: usize,
        state: usize,
        posit: ((usize,usize),(usize,usize)),
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
    fn direct (pos: ((usize,usize),(usize,usize)), dir:(i32,i32)) -> bool {
        let x1 = pos.1.0 as i32;
        let y1 = pos.1.1 as i32;
        let x2 = pos.0.0 as i32;
        let y2 = pos.0.1 as i32;
        return x1 - x2 == dir.0 && y1 - y2 == dir.1;
    }
    fn distance (a:(usize,usize), b:(usize,usize)) -> usize {
        let xx = (a.0 as i32 - b.0 as i32).abs() as usize;
        let yy = (a.1 as i32 - b.1 as i32).abs() as usize;
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
        let alphab = ["U","R","D","L"];
        let hist = vec![vec![0; width]; height];

        let mut x1; let mut y1;
        let mut x2; let mut y2;

        let mut exit = (0,0);
        let mut heap = BinaryHeap::new();
        let mut level:Vec<Vec<char>> = Vec::new();
        let mut visited:HashMap<Vec<Vec<usize>>,bool> = HashMap::new();

        for y in 0..height {
            level.push (puzzle[y].chars().collect::<Vec<char>>());
            for x in 0..width {

                match level[y][x] {
                    'X' => exit = (x,y),
                    'B' => heap.push (Vertex { state: 1, index: 1, posit: ((x,y),(x,y)), visit: hist.clone(), route: "".to_string() }),
                    _ => (),
                }
            }
        }

        let mut cycle = 0;

        while let Some (Vertex { index, state, posit, visit, route }) = heap.pop() {

            cycle += 1;
            if cycle == 20 { break }
            display (&level, &visit);

            visited.insert(visit.clone(), true);
            if state == 1 && posit.0 == exit {

                return route;
            }
            for i in 0..4 {
                let dir = compas[i];
                let alp = alphab[i];
                x1 = dir.0 + posit.0.0 as i32; y1 = dir.1 + posit.0.1 as i32;

                if state == 1 {
                    x2 = dir.0 * 2 + posit.0.0 as i32; y2 = dir.1 * 2 + posit.0.1 as i32;
                } else {
                    x2 = dir.0 + posit.1.0 as i32; y2 = dir.1 + posit.1.1 as i32;
                }

                if isinside (&level, x2, y2) {
                    let x2 = x2 as usize; let y2 = y2 as usize;
                    let alt = distance ((x2,y2), exit) + 1;
                    let mut grid = visit.clone();
                    grid[y2][x2] = alt;

                    if direct (posit, dir) {
                        if visited.get (&grid) == None {
                             heap.push (Vertex { index: alt, state: 1,  posit: ((x2,y2),(x2, y2)), visit: grid, route: route.clone() + alp }) ;
                        }
                    } else if isinside (&level, x1, y1) {
                        let x1 = x1 as usize; let y1 = y1 as usize;
                        grid[y1][x1] = alt;

                        if visited.get (&grid) == None {
                             heap.push (Vertex { index: alt, state: 2, posit: ((x1, y1),(x2,y2)), visit: grid, route: route.clone() + alp }) ;
                        }
                    }
                }
            }
        }

        "".to_string()
    }
}

fn main() {

    let level1: Vec<&str> = vec![
            "1110000000",
            "1B11110000",
            "1111111110",
            "0111111111",
            "0000011X11",
            "0000001110"
        ];
        //let n2:Vertex = Vertex {state:0, posit:vec![(0,1)]};
        //let str:Vec<Vec<char>> =
    //    example_tests();

    let level2 = vec![
        "000000111111100",
        "111100111001100",
        "111111111001111",
        "1B11000000011X1",
        "111100000001111",
        "000000000000111"];

    let level3 = vec![
            "00011111110000",
            "00011111110000",
            "11110000011100",
            "11100000001100",
            "11100000001100",
            "1B100111111111",
            "11100111111111",
            "000001X1001111",
            "00000111001111"];


    blox::blox_solver (&level3);


     print! ("\n");
}

fn example_tests() {
        let fixed_tests = [
            vec![
                "1110000000",
                "1B11110000",
                "1111111110",
                "0111111111",
                "0000011X11",
                "0000001110"],

            vec![
                "000000111111100",
                "111100111001100",
                "111111111001111",
                "1B11000000011X1",
                "111100000001111",
                "000000000000111"],
        /*
            vec![
                "00011111110000",
                "00011111110000",
                "11110000011100",
                "11100000001100",
                "11100000001100",
                "1B100111111111",
                "11100111111111",
                "000001X1001111",
                "00000111001111"],
            vec![
                "11111100000",
                "1B111100000",
                "11110111100",
                "11100111110",
                "10000001111",
                "11110000111",
                "11110000111",
                "00110111111",
                "01111111111",
                "0110011X100",
                "01100011100"],
            vec![
                "000001111110000",
                "000001001110000",
                "000001001111100",
                "B11111000001111",
                "0000111000011X1",
                "000011100000111",
                "000000100110000",
                "000000111110000",
                "000000111110000",
                "000000011100000"]

            */
        ];
        let fixed_sols = [
            vec!["RRDRRRD","RDDRRDR","RDRRDDR"],
            vec!["ULDRURRRRUURRRDDDRU","RURRRULDRUURRRDDDRU"],
            /*
            vec!["ULURRURRRRRRDRDDDDDRULLLLLLD"],
            vec!["DRURURDDRRDDDLD"],
            vec!["RRRDRDDRDDRULLLUULUUURRRDDLURRDRDDR","RRRDDRDDRDRULLLUULUUURRDRRULDDRRDDR","RRRDRDDRDDRULLLUULUUURRDRRULDDRRDDR"]

            */
        ];

        for (grid,valids) in fixed_tests.iter().zip(fixed_sols.iter()) {
            let user = blox::blox_solver(grid);
            assert!(valids.iter().any(|s| s == &user),"You returned an invalid path");
        }
}
