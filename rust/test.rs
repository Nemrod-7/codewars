#![allow(dead_code, unused)]
use std::collections::HashMap;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Element { C, H, O, B, Br, Cl, F, Mg, N, P, S, }
use Element::*;

#[derive(Hash, Clone)]
struct Atom {
    id:usize,
    element:Element,
    edge:Vec<(usize,usize)>,
}
struct Molecule {
    id:String,
    subs:Vec<Vec<Atom>>,
    lock:bool,
}
impl Molecule {
    fn tablex (&mut self) {

    }
    fn branch (&mut self, nxt: &[usize]) {

    }
}
fn main () {

    let nxt = [14,1,1];

    //let carbon = Atom{id: 0, element:C, edge:vec![] };
    //let hydrog = Atom{id: 0, element:H, edge:vec![] };

    let mut subs:Vec<i32> = vec![1,2,3,4];

    let formula = subs.iter().map(|x| format!("{x}")).collect::<String>();
    print!("{formula}\n");
}

