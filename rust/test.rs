
#![allow(dead_code, unused)]

use std::collections::LinkedList;

fn main() {

    let mut list = LinkedList::from([1,2,3,4,5,6,7,8,9]);
    let mut iter = list.iter();

    if let Some(removed) = list.remove(1) {
        println!("Removed element at index 1: {}", removed);
    }
    
    while let Some(num) = iter.next() {
        print!("{}", num);
    }
    print!("\n");

    /*
    */
    //show(getvar(t1));
}
