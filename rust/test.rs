#![allow(dead_code, unused)]
use std::collections::HashMap;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Element { C, H, O, B, Br, Cl, F, Mg, N, P, S, }
use Element::*;

#[derive(Hash, Clone)]
struct Atom {
    id:usize,
    element:Element,
    edge:Vec<usize>,
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
    fn bond (&mut self, )
}
fn main () {

    let nxt = [14,1,1];

    let carbon = Atom{id: 0, element:C, edge:vec![] };
    let hydrog = Atom{id: 0, element:H, edge:vec![] };

    let mut subs:Vec<Vec<Atom>> = Vec::new();

     for _ in 0..nxt.len() {
         let mut atoms:Vec<Atom> = (0..=src[i]).map(|ix| Atom {id:ix, element:C, edge:vec![]}).collect();

    //     for j in 0..atoms.len() - 1 {
    //         let a = j;
    //         let b = j + 1;
    //         let ida = atoms[a].id;
    //         let idb = atoms[b].id;
    //
    //         atoms[a].edge.push(idb);
    //         atoms[b].edge.push(ida);
    //
    //     }
     }
}
