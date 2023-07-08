use std::time::{Instant};

use std::collections::{BinaryHeap, HashSet};

struct LightController {
    size:usize,
    switch:Vec<Vec<usize>>,
}
impl LightController {

    fn new (n: usize, list: &[Vec<usize>]) -> Self {
        //print!("{} {:?}\n", n, list);
        Self {
            size:n,
            switch:list.to_vec(),
        }
    }

    fn check(&self, lights: &Vec<usize>) -> bool {

        for pos in lights {
            if pos >= &self.size {
                return false;
            }
        }

        true
    }

    fn solve(&self, lights: &Vec<usize>) -> Option<Vec<usize>> {

        if self.check(lights) == false { return None; }

        let mut heap = BinaryHeap::new();
        let mut hash = HashSet::new();
        let mut mask = vec![0;(self.size >> 6) + 1];

        heap.push((mask.clone(), vec![]));

        for bit in lights {
            mask[bit >> 6] |= 1u64 << (bit &63);
        }

        while let Some ((state, hist)) = heap.pop() {

            if state == mask {
                return Some(hist);
            }

            for i in 0..self.switch.len() {
                let mut next:Vec<u64> = state.clone();

                for pos in &self.switch[i] {
                    next[pos >> 6] ^= 1u64 << (*pos &63);
                }

                if hash.contains (&next) == false  {
                    let mut nxp = hist.clone();
                    nxp.push(i);

                    hash.insert(next.clone());
                    heap.push((next, nxp));
                }
            }
        }

        None
    }
}

fn main () {

    let start = Instant::now();

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

        for (n, switch, possible) in tests {
            let controller = LightController::new(n, &switch);
            for (choices, is_possible) in
                powerset(n).into_iter().zip(possible.into_iter()) {
                    test_controller( n, &switch, &controller, choices, is_possible,);
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

    fn test_controller(n: usize, switch: &Vec<Vec<usize>>, controller: &LightController, choices: Vec<usize>, is_possible: bool,) {

        if let Some(index) = controller.solve(&choices) {
            assert_eq!( toggle_switches(switch, n, &index), choices,
                "controller for switch {:?} failed to provide a right set of switches: tried to turn on the lights {:?} with the switches {:?}",
                switch, choices, index)
        } else {
            assert!(!is_possible, "controller for switch {:?} returned None for the lights set {:?}", switch, choices);
        }
    }

    fn toggle_switches(switch: &Vec<Vec<usize>>, n: usize, index: &Vec<usize>,) -> Vec<usize> {

        let mut state = vec![false; n];

        for it in index {
            for light in &switch[*it] {
                state[*light] = !state[*light]
            }
        }
        (0..n).filter(|i| state[*i]).collect()
    }

}
