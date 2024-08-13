#![allow(unused)]

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Element { C, H, O, B, Br, Cl, F, Mg, N, P, S, }

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ChemError {
    EmptyMolecule,
    LockedMolecule,
    InvalidBond,
    UnlockedMolecule,
}

pub type ChemResult<T> = Result<T, ChemError>;

use Element::*;

const DATA: &[(Element,(&str, usize, f32))] =
&[(H,("H",1,1.0)),(B,("B",3,10.8)),(C,("C",4,12.0)),(N,("N",3,14.0)),(O,("O",2,16.0)),(F,("F",1,19.0)),(Mg,("Mg",2,24.3)),(P,("P",3,31.0)),(S,("S",2,32.1)),(Cl,("Cl",1,35.5)),(Br,("Br",1,80.0))] ;

fn symbol (atom: Element) -> String { DATA.iter().find(|x| x.0 == atom).unwrap().1.0.to_string() }
fn valence (atom: Element) -> usize { DATA.iter().find(|x| x.0 == atom).unwrap().1.1 }
fn mass (atom: Element) -> f32 { DATA.iter().find(|x| x.0 == atom).unwrap().1.2 }

#[derive(Hash, Clone, Debug)]
struct Atom {
    id: usize,
    element:Element,
    edge: Vec<Option<Box<Atom>>>,
}
struct Node<'a> {
    edges: Vec<&'a Node<'a>>,
}

impl PartialEq for Atom {
    fn eq(&self, other: &Self) -> bool { 
      self.id == other.id 
    }
}
impl Eq for Atom {}

impl std::fmt::Display for Atom {
    fn fmt(&self, _f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut os = format!("Atom({}.{}", symbol(self.element), self.id);

        os += &")";
        write!(_f, "{}", os)
    }
}
impl Atom {
    fn from(atom: &Atom) -> Atom { Atom {id: atom.id, element: atom.element, edge: vec![]} }
    fn carbon(id:usize) -> Atom { Atom {id: id, element: C, edge: vec![]} }
    fn element(id:usize, elt:Element) -> Atom { Atom {id: id, element: elt, edge: vec![]} }
}

fn main () {

    Atom::carbon(0);
}
