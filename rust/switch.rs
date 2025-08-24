#![allow(warnings, unused)]

use std::time::{Instant};
use std::collections::{BinaryHeap, HashSet};

struct binary { }
impl binary {
    fn show(mut num:usize) {
        let mut dig = 0;

        while num  > 0 {
            if num &1 == 1 {
                print!("{dig}");
            }

            dig += 1;
            num >>= 1;
        }

        print!("\n");
    }

    fn itob(list: &Vec<usize>) -> usize {
        list.iter().fold(0, |bin, num| bin | 1 << num)
    }
}

struct LightController {
    max:usize,
    lim:usize,
    size:usize,
    comb:Vec<Vec<usize>>,
}
impl LightController {

    fn getmax(list: &[Vec<usize>]) -> usize {
        let max = list.iter().fold(None, |acc, x| std::cmp::max(acc, x.iter().max()) ).unwrap_or(&0usize);

        // let mut max = 0;
        //
        // for i in 0..list.len() {
        //     for nu in &list[i] {
        //         max = std::cmp::max(max, *nu);
        //     }
        // }
        //
        // print!("{:?}\n", max);
        *max
    }

    fn new (n: usize, list: &[Vec<usize>]) -> Self {
        let max = LightController::getmax(list); 
        let lim = (max >> 6) + 1;
        let mut comb = vec![vec![0; lim]; list.len()];

        for i in 0..list.len() {
            for nu in &list[i] {
                comb[i][nu >> 6] |= 1 << (nu&63);
            }
        }

        Self {
            max:max,
            lim:lim,
            size:n,
            comb: comb,
        }
    }

    fn solve(&self, lights: &Vec<usize>) -> Option<Vec<usize>> {
        let size = self.comb.len();
        let start = ( vec![0usize; self.lim], vec![]);
        let mut exit = vec![0; self.lim];
        let mut heap: BinaryHeap<(Vec<usize>, Vec<usize>)> = BinaryHeap::new();
        let mut hash: HashSet<Vec<usize>> = HashSet::new();

        for it in lights {
            if *it > self.max {
                return None
            } else {
                exit[it >> 6] |= 1 << (it&63);
            }
        }

        // binary::show(comb[i]);
        heap.push( start );

        while let Some ((state, hist)) = heap.pop() {

            if state == exit {
                return Some(hist);
            }

            for i in 0..size {
                let mut next = state.clone();

                for j in 0..self.lim {
                    next[j] ^= self.comb[i][j];
                }

                if hash.contains (&next) == false  {
                    let mut nxh = hist.clone();
                    nxh.push(i);

                    hash.insert(next.clone());
                    heap.push((next, nxh));
                }
            }
        }

        None
    }
}

fn main () {

    let start = Instant::now();

    let choice = vec![1,4,5];
    let list = [ vec![0, 1, 2], vec![1, 2], vec![1, 2, 3, 4], vec![1, 4] ];

    let controller = LightController::new(2, &list);


    fixed_tests::exhaustive_small_tests();


    print!("\n");
    let duration = start.elapsed();
    println!("Duration: {:?}", duration);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
mod fixed_tests {
    use super::LightController;

    pub fn exhaustive_small_tests() {
        let tests = vec![
            (2, vec![vec![0], vec![1]], vec![true, true, true, true]),
            (2, vec![vec![1, 0], vec![1]], vec![true, true, true, true]),
            (2, vec![vec![0, 1], vec![0, 1]], vec![   true, false, false, true],),
            (3, vec![vec![], vec![2], vec![0], vec![1, 0, 2]], vec![true, true, true, true, true, true, true, true],),
            (3, vec![vec![], vec![], vec![], vec![2, 1]], vec![true, false, false, false, false, false, true, false],),
            (0, vec![vec![]], vec![true]),
            (1, vec![], vec![true, false]),
            (0, vec![], vec![true]),
            (1, vec![vec![0]], vec![true, true]),
            (1, vec![vec![]], vec![true, false]),
            (4, vec![vec![0, 2, 3], vec![1, 2, 3], vec![1], vec![2], ], vec![true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true]),
            (4, vec![vec![3], vec![1], vec![0, 1, 2], vec![0, 1, 2, 3], ], vec![true, false, true, false, false, true, false, true, true, false, true, false, false, true, false, true]),
            (4, vec![vec![1, 2, 3], vec![0, 1, 3], vec![1, 2, 3], vec![], vec![3], ], vec![true, false, false, true, false, true, true, false, true, false, false, true, false, true, true, false]),
            (4, vec![vec![0, 1], vec![], vec![0, 1, 2, 3], ], vec![true, false, false, true, false, false, false, false, false, false, false, false, true, false, false, true]),
        ];

        for (n, comb, possible) in tests {
            let controller = LightController::new(n, &comb);
            for (choices, is_possible) in
                powerset(n).into_iter().zip(possible.into_iter()) {
                    test_controller( n, &comb, &controller, choices, is_possible,);
                }
        }
    }

    fn powerset(n: usize) -> Vec<Vec<usize>> {

        if n == 0 {
            vec![vec![]]
        } else {
            let mut s = powerset(n - 1);
            let mut s2 = s.clone();
            s2.iter_mut().for_each(|ss| ss.push(n - 1));
            s.append(&mut s2);
            s
        }
    }

    fn test_controller(n: usize, comb: &Vec<Vec<usize>>, controller: &LightController, choices: Vec<usize>, is_possible: bool,) {

        if let Some(index) = controller.solve(&choices) {
            // print!("n : {} comb : {:?} expect : {:?} got : {:?}\n", n, comb, choices, toggle_switches(comb, n, &index));
            assert_eq!( toggle_switches(comb, n, &index), choices,
            "controller for comb {:?} failed to provide a right set of switches: tried to turn on the lights {:?} with the switches {:?}", comb, choices, index)
        } else {
            assert!(!is_possible, "controller for comb {:?} returned None for the lights set {:?}", comb, choices);
        }
    }

    fn toggle_switches(comb: &Vec<Vec<usize>>, n: usize, index: &Vec<usize>,) -> Vec<usize> {

        let mut state = vec![false; n];

        for it in index {
            for light in &comb[*it] {
                state[*light] = !state[*light]
            }
        }
        (0..n).filter(|i| state[*i]).collect()
    }

}
