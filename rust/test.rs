#![allow(dead_code, unused)]

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Element { C, H, O, B, Br, Cl, F, Mg, N, P, S, }
use Element::*;

const DATA: &[ (Element,(&str, usize, f32)) ] =
&[(H,("H",2,1.0)),(B,("B",3,10.8)),(C,("C",4,12.0)),(N,("N",3,14.0)),(O,("O",2,16.0)),(F,("F",1,19.0)),(Mg,("Mg",2,24.3)),(P,("P",3,31.0)),(S,("S",2,21.1)),(Cl,("Cl",1,35.5)),(Br,("Br",1,80.0))] ;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ChemError {
    EmptyMolecule,
    LockedMolecule,
    InvalidBond,
    UnlockedMolecule,
}

pub type ChemResult<T> = Result<T, ChemError>;

#[derive(Hash, Clone)]
struct Atom {
    id:usize,
    element:Element,
    edge:Vec<Atom>,
}

struct Molecule {
    id: String,
    index: Vec<Vec<usize>>,
    atoms: Vec<Atom>,
}

impl Molecule {
    fn new() -> Molecule {
        Molecule {
            id: String::new(),
            index: vec![],
            atoms: vec![],
        }
    }
    fn form(name: &str) -> Molecule {
        Molecule {
            id: name.to_string(),
            index:vec![],
            atoms:vec![],
        }
    }
}

fn main () {

    let carbon = Atom{id:0, element:C, edge:vec![]};
    let mut atoms: Vec<Atom> = vec![Atom{id:0, element:C, edge:vec![]}];


}
