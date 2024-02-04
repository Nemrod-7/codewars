#![allow(dead_code, unused)]


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
    atoms:Vec<Atom>,
}

fn main () {

    let src = [14,1,1];

    let mut index = 0;
    let carbon = Atom{id: index, element:C, edge:vec![] };

    for i in 0..src.len() {
        let mut branch:Vec<Atom> = (0..src[i]).map(|_| Atom {id:0, element:C, edge:vec![]}).collect();

        for j in 0..branch.len() {
            branch[j].id = index;
            index += 1;
        }


        for j in 0..branch.len() - 1 {
            let a = j;
            let b = j + 1;
            let ida = branch[a].id;
            let idb = branch[b].id;

            branch[a].edge.push(idb);
            branch[b].edge.push(ida);

        }
    }
}
