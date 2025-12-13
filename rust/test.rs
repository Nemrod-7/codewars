#![allow(warnings)]

use std::collections::LinkedList;
type Train = (char,usize, LinkedList<(i32,i32)>);

fn process (iter: &mut Iterator<Item = (i32,i32)>)  {
    let loco = iter.next().unwrap();

    print!("{:?} \n", loco);

    for v in iter {
        print!("{:?} ", v);
    }
}

fn collision (a: &LinkedList<i32>, b: &LinkedList<i32>) -> bool {
    let mut tr_a = a.iter();
    let mut tr_b = b.iter();
    let loco_a = tr_a.next().unwrap();
    let loco_b = tr_b.next().unwrap();

    while let Some(wagon) = tr_b.next() {
        if loco_a == wagon || loco_b == wagon { return true }
    }

    while let Some(wagon) = tr_a.next() {
        if loco_a == wagon || loco_b == wagon { return true }
    }

    false
}


fn main() {

    let mut train:LinkedList<i32> = LinkedList::new();

    train.push_back(4);
    train.push_back(3);
    train.push_back(5);

    // let loco = train.front().unwrap();

    collision(&train, &train);

    let mut wagon = train.iter();
    let mut loco = wagon.next().unwrap();



    // while let Some(node) = wagon.next() {
    //     print!("{} ", node);
    //     // loco = loco.iter().next();
    // }



}
