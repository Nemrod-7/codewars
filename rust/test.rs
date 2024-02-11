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
    fn from(name: &str) -> Molecule {
        Molecule {
            id: name.to_string(),
            index:vec![],
            atoms: vec![Atom{id:0, element:C, edge: vec![]}],
        }
    }

}

fn carbon_test (name: &str, chain: Vec<usize>, bond: Vec<(usize,usize,usize,usize)>, mass: f32) {

}

fn main () {

    let carbon = Atom{id:0, element:C, edge:vec![]};
    //let mut atoms: Vec<Atom> = vec![Atom{id:0, element:C, edge:vec![]}];

    let carbon = [
            ( "cyclohexane", vec![6], vec![(1,1,6,1)], "C6H12", 84.,
                vec!["Atom(C.1: C2,C6,H,H)", "Atom(C.2: C1,C3,H,H)", "Atom(C.3: C2,C4,H,H)", "Atom(C.4: C3,C5,H,H)", "Atom(C.5: C4,C6,H,H)", "Atom(C.6: C1,C5,H,H)"]
            ), ( 
                "1,1-dimethyl-2-propylcyclohexane", vec![9,1,1], vec![(4,1,9,1), (5,1,1,2), (5,1,1,3)], "C11H22", 154.,
                vec!["Atom(C.1: C2,H,H,H)", "Atom(C.2: C1,C3,H,H)", "Atom(C.3: C2,C4,H,H)", "Atom(C.4: C3,C5,C9,H)", "Atom(C.5: C4,C6,C10,C11)", "Atom(C.6: C5,C7,H,H)", "Atom(C.7: C6,C8,H,H)", "Atom(C.8: C7,C9,H,H)", "Atom(C.9: C4,C8,H,H)", "Atom(C.10: C5,H,H,H)", "Atom(C.11: C5,H,H,H)"]
            ), (
                "cubane - one branch", vec![8], vec![(3,1,6,1), (2,1,7,1), (1,1,8,1), (4,1,1,1), (5,1,8,1)], "C8H8", 104.,
               vec!["Atom(C.1: C2,C4,C8,H)", "Atom(C.2: C1,C3,C7,H)", "Atom(C.3: C2,C4,C6,H)", "Atom(C.4: C1,C3,C5,H)", "Atom(C.5: C4,C6,C8,H)", "Atom(C.6: C3,C5,C7,H)", "Atom(C.7: C2,C6,C8,H)", "Atom(C.8: C1,C5,C7,H)"]
            ), (
                "cubane - two branches", vec![4,4], vec! [(1,1,4,1), (1,2,4,2), (1,1,1,2), (2,1,2,2), (3,1,3,2), (4,1,4,2)], "C8H8", 104.,
                vec!["Atom(C.1: C2,C4,C5,H)", "Atom(C.2: C1,C3,C6,H)", "Atom(C.3: C2,C4,C7,H)", "Atom(C.4: C1,C3,C8,H)", "Atom(C.5: C1,C6,C8,H)", "Atom(C.6: C2,C5,C7,H)", "Atom(C.7: C3,C6,C8,H)", "Atom(C.8: C4,C5,C7,H)"]
            ), (
                "benzene: double bonds",vec! [2,2,2], vec! [(1,1,2,1), (1,2,2,2), (1,3,2,3), (2,1,1,2), (2,2,1,3), (2,3,1,1)], "C6H6", 78.,
                vec!["Atom(C.1: C2,C2,C6,H)", "Atom(C.2: C1,C1,C3,H)", "Atom(C.3: C2,C4,C4,H)", "Atom(C.4: C3,C3,C5,H)", "Atom(C.5: C4,C6,C6,H)", "Atom(C.6: C1,C5,C5,H)"]
            ), (
                "acetylene: triple bonds", vec![2], vec![(1,1,2,1), (1,1,2,1)], "C2H2", 26.,
               vec! ["Atom(C.1: C2,C2,C2,H)", "Atom(C.2: C1,C1,C1,H)"]
            ) ];

    for it in &carbon {

    }
}
