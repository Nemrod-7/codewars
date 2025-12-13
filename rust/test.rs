#![allow(warnings)]

use std::collections::LinkedList;

fn process (iter: &mut Iterator<Item = i32>)  {
    let loco = iter.next().unwrap();

    print!("{} \n", loco);

    for v in iter {
        print!("{} ", v);
    }
}

fn main() {

    let mut train:LinkedList<i32> = LinkedList::new();

    train.push_back(4);
    train.push_back(3);
    train.push_back(5);

    // let loco = train.front().unwrap();

    // process(&mut train.iter().copied());

    let mut wagon = train.iter();
    let mut loco = wagon.next().unwrap();



    // while let Some(node) = wagon.next() {
    //     print!("{} ", node);
    //     // loco = loco.iter().next();
    // }



}
