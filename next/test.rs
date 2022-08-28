use std::cmp::Ordering;
use std::collections::BinaryHeap;

#[derive(Copy, Clone, Eq, PartialEq)]
struct Vertex {
    dist: usize,
    pos: (usize,usize),
}

impl Ord for Vertex {
    fn cmp(&self, other: &Self) -> Ordering {
        other.dist.cmp(&self.dist)
            .then_with(|| self.pos.cmp(&other.pos))
    }
}

impl PartialOrd for Vertex {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}
fn isinside (grid: &Vec<Vec<i32>>, x: i32, y: i32) -> bool {
    x >= 0 && y >= 0 && y < grid.len() as i32 && x < grid[0].len() as i32
}
fn main() {

    let compas = [(0,-1),(1,0),(0,1),(-1,0)];
    let mut heap = BinaryHeap::new();
    let mut area: Vec<(usize,usize)> = Vec::new() ;

    heap.push(Vertex { dist: 0, pos: (1,1) });
    heap.push(Vertex { dist: 5, pos: (1,2) });
    heap.push(Vertex { dist: 3, pos: (1,5) });
    heap.push(Vertex { dist: 7, pos: (1,4) });
    /*
    */
    //print! ("{} ", heap.peek().unwrap());
    while let Some (Vertex { dist, pos }) = heap.pop() {

        for dir in compas {
            //let nxt = (nx as usize, ny as usize);

        }
        print! ("{} :: [{},{}]\n", dist, pos.0, pos.1);
    }

    print! ("\n");
}
