use std::cmp::Ordering;
use std::collections::BinaryHeap;

#[derive(Copy, Clone, Eq, PartialEq)]
struct State {
    cost: usize,
    position: usize,
}

impl Ord for State {
    fn cmp(&self, other: &Self) -> Ordering {
       other.cost.cmp(&self.cost)
    }
}

impl PartialOrd for State {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

struct Edge {
    node: usize,
    cost: usize,
}
fn shortest_path(adj_list: &Vec<Vec<Edge>>, start: usize, goal: usize) -> Option<usize> {
    let mut dist: Vec<_> = (0..adj_list.len()).map(|_| usize::MAX).collect();
    let mut heap = BinaryHeap::new();

    dist[start] = 0;
    heap.push(State { cost: 0, position: start });

    while let Some(State { cost, position }) = heap.pop() {
        if position == goal { return Some(cost); }
        if cost > dist[position] { continue; }

        for edge in &adj_list[position] {
            let next = State { cost: cost + edge.cost, position: edge.node };

            if next.cost < dist[next.position] {
                heap.push(next);
                dist[next.position] = next.cost;
            }
        }
    }

    None
}

fn main() {


    let mut heap = BinaryHeap::new();

    heap.push(State { cost: 0, position: 1 });
    heap.push(State { cost: 4, position: 1 });
    heap.push(State { cost: 2, position: 1 });


    while let Some(State {cost, position}) = heap.pop() {
        print!("{cost}");
    }
}

