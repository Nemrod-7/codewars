use std::cmp::Ordering;
use std::collections::BinaryHeap;

#[derive(Copy, Clone, Eq, PartialEq)]

struct Vertex {
    state: usize,
    index: usize,
    posit: ((usize,usize),(usize,usize)),
}
impl Ord for Vertex {
    fn cmp(&self, other: &Self) -> Ordering {
        other.state.cmp(&self.state)
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
fn direct (pos: ((usize,usize),(usize,usize)), x: i32, y:i32) {

}
fn blox_solver (level: Vec<&str>) {

    let width = level[0].len();
    let height = level.len();
    let compas = [(0,-1),(1,0),(0,1),(-1,0)];

    let mut exit:Vertex;
    let mut floor:Vec<Vec<char>> = Vec::new();
    let mut heap = BinaryHeap::new();

    for y in 0..height {
        floor.push (level[y].chars().collect::<Vec<char>>());
        for x in 0..width {
            match floor[y][x] {
                'X' => exit = Vertex { state: 1, index: 1, posit: ((x,y),(x,y)) },
                'B' => heap.push (Vertex { state: 1, index: 1, posit: ((x,y),(x,y)) }),
                _ => (),
            }
        }
    }

    while let Some (Vertex { state, index, posit }) = heap.pop() {

        for dir in compas {
            let x1 = dir.0 + posit.0.0 as i32;
            let y1 = dir.1 + posit.0.1 as i32;

            if state == 1 {
                let x2 = dir.0 * 2 + posit.0.0 as i32;
                let y2 = dir.1 * 2 + posit.0.1 as i32;

                if isinside (&floor, x2, y2) {
                    let x1 = x1 as usize; let y1 = y1 as usize;
                    let x2 = x2 as usize; let y2 = y2 as usize;

                    heap.push (Vertex { state: 2, index: index + 1, posit: ((x1, y1),(x2, y2)) }) ;
                }

            } else {
                let x2 = dir.0 + posit.1.0 as i32;
                let y2 = dir.1 + posit.1.1 as i32;


            }
        }
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


        let nh = vec![(0,1)];

        //let n2:Vertex = Vertex {state:0, posit:vec![(0,1)]};
        //let str:Vec<Vec<char>> =

     //blox_solver(level1);


}
