#![allow(unused)]

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Element { C, H, O, B, Br, Cl, F, Mg, N, P, S, }

impl std::fmt::Display for Element {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            C => write!(f, "C"),
            H => write!(f, "H"),
            O => write!(f, "O"),
            B => write!(f, "B"),
            F => write!(f, "F"),
            N => write!(f, "N"),
            P => write!(f, "P"),
            S => write!(f, "S"),
            Mg=> write!(f, "Mg"),
            Br=> write!(f, "Br"),
            Cl=> write!(f, "Cl"),
            _ => write!(f, ""), 
        }
    }
}
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
    edge: Vec<(Element, usize)>,
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

        if self.edge.len() > 0 {
            os += &": ";

            for &(element, id) in &self.edge {
                if element == H {
                    os += &format!("H,");
                } else {
                    os += &format!("{}{},", symbol(element), id);
                }
            }
            if self.edge.len() > 0 { os.pop(); }
        }
        os += &")";
        write!(_f, "{}", os)
    }
}
impl Atom {
    fn from(atom: &Atom) -> Atom { Atom {id: atom.id, element: atom.element, edge: vec![]} }
    fn carbon(id:usize) -> Atom { Atom {id: id, element: C, edge: vec![]} }
    fn element(id:usize, elt:Element) -> Atom { Atom {id: id, element: elt, edge: vec![]} }
    fn bond(&mut self, elt: Element, id: usize) -> &mut Self { 
        self.edge.push((elt, id));
        self
    }
}

struct Molecule {
    atoms: Vec<Atom>,
    branch: Vec<Vec<usize>>,
    locked: bool,
}

impl Molecule {
    fn link  (&mut self, a1: usize, a2: usize) {
        let e1 = self.atoms[a1].element;        
        let e2 = self.atoms[a2].element;

        self.atoms[a2].edge.push((e1,a1));
        self.atoms[a1].edge.push((e2,a2));
    }

    pub fn branch (&mut self, nbranch: &[(usize)]) {
        for &ncarb in nbranch {
            let mut chain = vec![0; ncarb + 1];

            for i in 1..ncarb {
                let id = self.atoms.len();
                self.atoms.push(Atom::carbon(id));

                if i > 1 {
                    self.atoms[id -1].edge.push((C, id -2));
                    self.atoms[id -2].edge.push((C, id -1));
                }
            }

            self.branch.push(chain);
        }

    }
}

fn main () {



    //atom.bond(H,2);


}
