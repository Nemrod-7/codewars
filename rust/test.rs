#![allow(warnings)]

use std::collections::{LinkedList, HashMap};
type Train = (char,usize, LinkedList<(i32,i32)>);

fn process (iter: &mut Iterator<Item = (i32,i32)>)  {
    let loco = iter.next().unwrap();

    print!("{:?} \n", loco);

    for v in iter {
        print!("{:?} ", v);
    }
}

fn collision (a: &LinkedList<i32>, b: &LinkedList<i32>) -> bool {
    let mut map:HashMap<i32, usize> = HashMap::new();

    for &it in a.iter() { *map.entry(it).or_insert(0) += 1; }
    for &it in b.iter() { *map.entry(it).or_insert(0) += 1; }

    for it in map {
        print!("{} {}\n", it.0, it.1);
    }

    false
}


fn main() {

    let mut train:LinkedList<i32> = LinkedList::new();

    train.push_back(4);
    train.push_back(3);
    train.push_back(5);
    train.push_back(4);

    // let loco = train.front().unwrap();

    collision(&train, &train);

    let mut wagon = train.iter();
    let mut loco = wagon.next().unwrap();



    // while let Some(node) = wagon.next() {
    //     print!("{} ", node);
    //     // loco = loco.iter().next();
    // }



}
