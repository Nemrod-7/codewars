#![allow(dead_code, unused)]

const N:usize = 7;
struct Sky {
    clue:[usize; 28],
    cell:[u8; 49],
    grid:[u8; 49],
}

impl Sky {
    fn new(clues: &[u8; 28]) -> Sky {
        Sky {
            clue: clues.map(|x| x as usize),
            cell: [254;49],
            grid: [0;49],
        }
    }
    fn getrow(&self, x: usize) -> (usize,usize) { (self.clue[((N * 4) - 1) - x], self.clue[N + x]) }
    fn getcol(&self, x: usize) -> (usize,usize) { (self.clue[x], self.clue[((N * 4) -1) - x - N]) }
    fn construct (&mut self) {

        for i in 0..N {
            let north = i;
            let east = N + i;
            let west = ((N * 4) - 1) - i;
            let south = ((N * 4) - 1) - i - N;

            self.init(north, (i,0), (0,1));
            self.init(east, (N - 1, i), (-1,0));
            self.init(west, (0, i), (1,0));
            self.init(south, (i, N - 1), (0,-1));
        }
    }
    fn init (&mut self, pos: usize, p: (usize, usize), dir: (i8,i8)) {
        let clue = self.clue[pos];
        if clue == 0 { return }

        let (mut x, mut y) = p;

        if clue == 1 {
            self.cell[y * N + x] = 1 << N;
        } else if clue == N {
            for i in 1..=N {
                self.cell[y * N + x] = 1 << i;
                x = (x as i8 + dir.0) as usize;
                y = (y as i8 + dir.1) as usize;
            }

        } else {
            let maxv = N - clue + 2;

            for i in 0..(clue-1) {
                for j in (maxv + i)..=N {
                    self.cell[y * N + x] &= !(1 << j);
                }
                self.cell[y * N + x] &= !(1 << N);
                x = (x as i8 + dir.0) as usize;
                y = (y as i8 + dir.1) as usize;
            }
        }
    }
}

fn equals (a: &(usize,usize), b: &(usize,usize)) -> bool {

    if b.0 == 0 && b.1 == 0 {
        return true;
    } else if a.0 == 0 && a.1 == b.1 {
        return true;
    } else if a.1 == 0 && a.0 == b.0 {
        return true;
    } else if b.0 == 0 && a.1 == b.1 {
        return true;
    } else if b.1 == 0 && a.0 == b.0 {
        return true;
    } else if a == b {
        return true;
    }

    return false;
}
fn check_num(comb: &[u8; N]) -> (usize,usize) {
    let mut clue = (0,0);
    let mut index = N;
    let (mut first, mut sec) = (0,0);

    while index > 0 {
        index -= 1;

        if comb[index] > sec {
            sec = comb[index];
            clue.1 += 1;
        }
        if comb[N - 1 - index] > first {
            first = comb[N - 1 - index];
            clue.0 += 1;
        }
    }

    clue
}

fn search_comb (sky : &mut Sky, line: Vec<usize>, csp: (usize,usize)) {       // make unique permutations of subsets
    if csp == (0,0) { return; }
    // eg : [ [1,2,3], [4], [2,3], [1,2] ]

    let mut list = vec![0; N];
    let mut heap = vec![(0,0, [0;N])];

    while let Some((index, mask, mut comb)) = heap.pop() {
        if index == N {
            if equals(&check_num(&comb), &csp) { // if the combination respect the clue of the line, make it as valid.
                (0..N).for_each(|i| list[i] |= 1 << comb[i]);
            }
        } else {
            let mut dig = 0;
            let mut byte = sky.cell[line[index]];

            while byte != 0 {
                if (byte &1 == 1) && ((mask >> dig&1) == 0) { // if the digit of a subset has not been already been used, push it on this position
                    comb[index] = dig;
                    heap.push((index + 1, (mask | 1 << dig), comb.clone()));
                }
                dig += 1;
                byte >>= 1;
            }

        }
    }
    // print!("{:?} {:?}\n", comb, check_num(&comb));
    (0..N).for_each(|index| sky.cell[line[index]] = list[index]);
}
fn filter (sky: &mut Sky, line: Vec<usize>) {

    let mut hist = [0; N + 1];
    let mut uniq = [0; N + 1];
    // for each cell of a line
    for i in 0..N {
        let cell = sky.cell[line[i]];
        let mut byte = cell;
        let mut dig = 0;
        let mut one = 0;
        // collect all possible digits
        while byte != 0 {
            if byte &1 == 1 {
                one = dig;
                hist[dig] += 1;
            }
            dig += 1;
            byte >>= 1;
        }
        // if a cell contains only one digit then it cannot be elsewhere
        if u8::count_ones(cell) == 1 {
            uniq[one] = 1;
            sky.grid[line[i]] = one as u8;
        }
    }

    for i in 0..N {
        let mut byte = sky.cell[line[i]];

        if u8::count_ones(byte) > 1 {
            let mut dig = 0;

            while byte != 0 {
                if byte &1 == 1 {
                    if hist[dig] == 1 { sky.cell[line[i]] = 1 << dig; } // if of all possible digits of a cell, one of them appears only once on a line, then it is only on this cell.
                    if uniq[dig] == 1 { sky.cell[line[i]] &= !(1 << dig); } // if a unique digit has already assigned on another cell, erase the possibility of this digit here.
                }
                dig += 1;
                byte >>= 1;
            }
        }
    }
}
fn reduce(curr: &mut Sky) {
    for i in 0..N {
        filter(curr, (0..N).map(|j| j * N + i).collect::<Vec<_>>());
        filter(curr, (0..N).map(|j| i * N + j).collect::<Vec<_>>());
    }
}

fn checkrow(sky : &mut Sky, nrow: usize, mask: u8) -> bool {
    if mask != 254 { return true }
    let clue = sky.getrow(nrow);
    if clue == (0,0) { return true }
    let mut line:[u8;7] = [0;7];

    (0..N).for_each(|i| line[i] = sky.grid[nrow * N + i]);
    equals(&check_num(&line), &clue)
}
fn checkcol(sky : &mut Sky, ncol: usize, mask: u8) -> bool {
    if mask != 254 { return true }
    let clue = sky.getcol(ncol);
    if clue == (0,0) { return true }
    let mut line:[u8;7] = [0;7];

    (0..N).for_each(|i| line[i] = sky.grid[i * N + ncol]);
    equals(&check_num(&line), &clue)
}
fn backtrack(sky :&mut Sky, mut row: Vec<u8>, mut col: Vec<u8>, index: usize, hist: &Vec<usize>) -> bool {

    // print!("{:?}\n",index);
    if index == hist.len() { return true }
    let (x,y) = (hist[index] % N, hist[index] / N);
    let mut byte = sky.cell[hist[index]];
    let mut dig = 0;


    while byte != 0 {

        if (byte & 1) == 1 {

            if (row[y] >> dig &1) == 0 && (col[x] >> dig &1) == 0 {
            sky.grid[hist[index]] = dig;
            row[y] ^= 1 << dig;
            col[x] ^= 1 << dig;

            if checkrow(sky, y, row[y]) && checkcol(sky, x, col[x]) {
                if backtrack(sky, row.clone(), col.clone(), index + 1, hist) {
                    return true;
                }
            }

            col[x] ^= 1 << dig;
            row[y] ^= 1 << dig;
            sky.grid[hist[index]] = 0;
        }
        }
        dig += 1;
        byte >>= 1;
    }

    false
}

pub fn solve_puzzle(clues: &[u8; 28]) -> [[u8; 7]; 7] {
    let mut cycle = 6;
    let mut row = vec![0; N];
    let mut col = vec![0; N];
    let mut grid:[[u8; 7]; 7] = [[0;7];7];
    let mut tablex = Vec::new();
    let mut curr = Sky::new(&clues);
    curr.construct();

    while cycle != 0 {
        for i in 0..N {
            let horiz = curr.getrow(i);
            let verti = curr.getcol(i);
            let row = (0..N).map(|j| i * N + j).collect::<Vec<usize>>();
            let col = (0..N).map(|j| j * N + i).collect::<Vec<usize>>();

            reduce(&mut curr);
            search_comb(&mut curr, row, horiz);
            search_comb(&mut curr, col, verti);
        }

        cycle -= 1
    }
    // backtrack option for very hard puzzles
    for i in 0..(N * N) {
        if curr.grid[i] == 0 {
            tablex.push(i);
        } else {
            col[i % N] |= 1 << curr.grid[i];
            row[i / N] |= 1 << curr.grid[i];
        }
    }
    backtrack(&mut curr, row, col, 0, &tablex);
    // display(&curr);
    (0..N).for_each(|i| (0..N).for_each(|j| grid[i][j] = curr.grid[i * N + j]));
    grid
}

fn display(sky: &Sky) {

    print!(" ");
    for i in 0..N {
        print!(" ");
        if sky.clue[i] == 0 { print!(" "); } else { print!("{}", sky.clue[i]); }
        print!(" ");
    }
    print!("\n");

    for i in 0..N {
        let cli = sky.clue[((N * 4) - 1) - i];
        if cli == 0 { print!(" "); } else { print!("{}", cli); }

        for j in 0..N {

            if sky.grid[i * N + j] == 0 {
                print!("[ ]");
            } else {
                print!("[{}]", sky.grid[i * N + j])
            }
        }

        let cli = sky.clue[N + i];
        if cli == 0 { print!(" "); } else { print!("{}", cli); }
        print!("\n");
    }

    print!(" ");
    for i in 0..N {
        print!(" ");
        let cli = sky.clue[((N*4) -1) - i - N];
        if cli == 0 { print!(" "); } else { print!("{}", cli); }
        print!(" ");
    }
    print!("\n");
}
fn showbyte (mut byte: u8) {
    let mut dig = 0;

    while byte != 0 {
        if byte &1 == 1 { print!("{dig} ") };
        dig += 1;
        byte >>= 1;
    }
        print!("\n");

}


fn tests () {

const TESTS: [([u8; 28], [[u8; 7]; 7]); 3] = [
        (
            [
                7, 0, 0, 0, 2, 2, 3, 0, 0, 3, 0, 0, 0, 0, 3, 0, 3, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 4,
            ],
            [
                [1, 5, 6, 7, 4, 3, 2],
                [2, 7, 4, 5, 3, 1, 6],
                [3, 4, 5, 6, 7, 2, 1],
                [4, 6, 3, 1, 2, 7, 5],
                [5, 3, 1, 2, 6, 4, 7],
                [6, 2, 7, 3, 1, 5, 4],
                [7, 1, 2, 4, 5, 6, 3],
            ],
        ),
        (
            [
                0, 0, 5, 0, 0, 0, 6, 4, 0, 0, 2, 0, 2, 0, 0, 5, 2, 0, 0, 0, 5, 0, 3, 0, 5, 0, 0, 3,
            ],
            [
                [3, 4, 1, 7, 6, 5, 2],
                [7, 1, 2, 5, 4, 6, 3],
                [6, 3, 5, 2, 1, 7, 4],
                [1, 2, 3, 6, 7, 4, 5],
                [5, 7, 6, 4, 2, 3, 1],
                [4, 5, 7, 1, 3, 2, 6],
                [2, 6, 4, 3, 5, 1, 7],
            ],
        ),
        (
            [
                // for a _very_ hard puzzle, replace the last 7 values with zeroes
                0, 2, 3, 0, 2, 0, 0, 5, 0, 4, 5, 0, 4, 0, 0, 4, 2, 0, 0, 0, 6, 5, 2, 2, 2, 2, 4, 1,
            ],
            [
                [7, 6, 2, 1, 5, 4, 3],
                [1, 3, 5, 4, 2, 7, 6],
                [6, 5, 4, 7, 3, 2, 1],
                [5, 1, 7, 6, 4, 3, 2],
                [4, 2, 1, 3, 7, 6, 5],
                [3, 7, 6, 2, 1, 5, 4],
                [2, 4, 3, 5, 6, 1, 7],
            ],
        ),
        ];

    for idx in 0..TESTS.len() {
        let (clues, expected) = TESTS[idx];
        let actual = solve_puzzle(&clues);


        if actual != expected {

            print!("error {}\n", idx);
        }
    }
}


fn main () {

    let clues = [7,0,0,0,2,2,3, 0,0,3,0,0,0,0, 3,0,3,0,0,5,0, 0,0,0,0,5,0,4];
    let clues = [0,2,3,0,2,0,0, 5,0,4,5,0,4,0, 0,4,2,0,0,0,6, 0,0,0,0,0,0,0];

    solve_puzzle(&clues);
    // tests();

    print!("\nend\n");
}
