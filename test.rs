
use std::collections::BTreeMap;

fn smaller(xs: &[i32]) -> Vec<usize> {
    let mut res = vec![0;xs.len()];
    let mut sweep:BTreeMap<i32,usize> = BTreeMap::new();

    for i in (0..xs.len()).rev() {
        let mut cnt = 0;

        for nc in sweep.iter() {
            if *nc.0 < arr[i] {
                cnt += nc.1;
            } else {
                break
            }
        }

        res[i] = cnt;
        *sweep.entry(xs[i]).or_insert(0) += 1;
    }

    res
}

fn main () {

    let arr = [945, -763, -141, 520, -963, -2, -387, -322, -427, 467, -883, 609, 262, 974, 482, 732, -872, -611, -537];

    let mut sweep:BTreeMap<i32,usize> = BTreeMap::new();

    for i in (0..arr.len()).rev() {
        let mut cnt = 0;

        for nc in sweep.iter() {
            if *nc.0 >= arr[i] {
                break
            } else {
                cnt += nc.1;
            }
        }
        *sweep.entry(arr[i]).or_insert(0) += 1;
    }

    //smaller(&[5, 4, 7, 9, 2, 4, 1, 4, 5, 6]);
    //smaller(&act);
}

