#![allow(dead_code, unused)]

use std::collections::HashMap;

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

fn symbol (atom: Element) -> String { DATA.iter().find(|x| x.0 == atom).unwrap().1.0.to_string() }
fn valence (atom: Element) -> usize { DATA.iter().find(|x| x.0 == atom).unwrap().1.1 }
fn mass (atom: Element) -> f32 { DATA.iter().find(|x| x.0 == atom).unwrap().1.2 }

pub type ChemResult<T> = Result<T, ChemError>;

#[derive(Hash, Clone)]
struct Atom {
    id:usize,
    element:Element,
    edge:Vec<Atom>,
}

impl std::fmt::Display for Atom {
    fn fmt(&self, _f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        todo!()
    }
}
struct Molecule {
    id: String,
    index: Vec<Vec<usize>>,
    atoms: Vec<Atom>,
    lock: bool,
}

impl From<&'static str> for Molecule {
    fn from(_value: &'static str) -> Self {
        todo!()
    }
}

impl Molecule {
    fn new() -> Molecule {
        Molecule {
            id: String::new(),
            index: vec![vec![]],
            atoms: vec![Atom{id:0, element:C,edge: vec![]}],
            lock: false,
        }
    }
    fn from(name: &str) -> Molecule {
        Molecule {
            id: name.to_string(),
            index:vec![vec![]],
            atoms: vec![Atom{id:0, element:C,edge: vec![]}],
            lock:false,
        }
    }
     pub fn branch(&mut self, _bs: &[usize]) -> ChemResult<&mut Self> {

         if self.lock == true { return Err(ChemError::LockedMolecule); }

         for ncarb in _bs {
             let mut arm = vec![0];

             for _ in 0..*ncarb {
                 let nb = self.atoms.len();
                 let atom = Atom {id:nb, element:C, edge: vec![] };

                 arm.push(nb);
                 self.atoms.push(atom);
             }

             for i in 2..*ncarb {
                 let c1 = arm[i-1];
                 let c2 = arm[i-0];

                 let at1 = self.atoms[c1].clone();
                 let at2 = self.atoms[c2].clone();

                 self.atoms[c1].edge.push(at2);
                 self.atoms[c2].edge.push(at1);
             }

             self.index.push(arm);
         }

         Ok(self)
     }

     fn getcarbon (&mut self, nc: usize, nb: usize) -> &mut Atom {
         let carbon = (0..self.index[nb].len()).filter(|x| self.atoms[self.index[nb][*x]].element == C).collect::<Vec<_>>();

         &mut self.atoms[self.index[nb][carbon[nc]]]
     }
     pub fn mutate(&mut self, _ms: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
         if self.lock == true { return Err(ChemError::LockedMolecule); }

         for (nc, nb, elt) in _ms {
             let atom = self.getcarbon(*nc,*nb);
             atom.element = *elt;

             if atom.edge.len() > valence(atom.element) {
                 return Err(ChemError::InvalidBond);
             } 
         }

         Ok(self)
     }

     pub fn bond(&mut self, _poses: &[(usize, usize, usize, usize)]) -> ChemResult<&mut Self> {
         if self.lock == true { return Err(ChemError::LockedMolecule); }
        
         for it in _poses {
             self.getcarbon(it.0,it.1);//.edge.push((it.2,it.3));
             
         }
Ok(self)
     }

     pub fn add(&mut self, _els: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {

         if self.lock == true { return Err(ChemError::LockedMolecule); }
         let mut nid = self.atoms.len();

         for (nc, nb, elt) in _els {
             let atom = self.getcarbon(*nc,*nb);
             let curr = atom.clone();

             if curr.edge.len() < valence(curr.element) {
                 let mut next = Atom{id: nid, element: *elt, edge:vec![]};

                 atom.edge.push(next.clone());
                 next.edge.push(curr.clone());

                 self.atoms.push(next);
                 nid += 1;
             } else {
                 return Err(ChemError::InvalidBond);
             }
         }

         Ok(self)
     }

     pub fn add_chain(&mut self, _nc: usize, _nb: usize, _els: &[Element]) -> ChemResult<&mut Self> {
         todo!()
     }

     pub fn close(&mut self) -> ChemResult<&mut Self> {
         self.lock = true;

         for i in 1..self.index.len() {
             for j in 1..self.index[i].len() {
                 let mut atom = self.atoms[self.index[i][j]].clone();
                 let size = valence (atom.element) as i32 - atom.edge.len() as i32;

                 let id1 = self.index[i][j];

                 if size < 0 {
                     return Err(ChemError::InvalidBond);
                 } else {
                     for _ in 0..size {
                         let hydrogen = Atom {id: self.atoms.len(), element:H, edge:vec![atom.clone()]};

                         atom.edge.push(hydrogen.clone());
                         self.atoms.push(hydrogen);
                     }
                 }

                 self.atoms[self.index[i][j]] = atom;
             }
         }

         Ok(self)
     }

     pub fn unlock(&mut self) -> ChemResult<&mut Self> {
         self.lock = false;

         Ok(self)
     }

     pub fn formula(&self) -> ChemResult<String> {
         if self.atoms.len() == 1 { return Err(ChemError::EmptyMolecule); }
         if self.lock == false { return Err(ChemError::UnlockedMolecule); }

         let mut freq:HashMap<Element, usize> = HashMap::new();
         let mut res = String::new();

         for i in 1..self.atoms.len() {
             let atom = &self.atoms[i];

             *freq.entry(atom.element).or_insert(0) += 1;
         }

         let mut freq = freq.iter().collect::<Vec<_>>();
         freq.sort_by(|a,b| a.0.cmp(b.0));

         Ok (freq.iter().map(|atom| format!("{}{}", symbol(*atom.0), atom.1)).collect::<String>())
     }

     pub fn molecular_weight(&self) -> ChemResult<f32> {
         if self.atoms.len() == 1 { return Err(ChemError::EmptyMolecule); }
         if self.lock == false { return Err(ChemError::UnlockedMolecule); }

         let mut sum = 0.0;

         for i in 1..self.atoms.len() {
             let atom = &self.atoms[i];
             sum += mass(atom.element); 
         }

         Ok(sum)
     }

     pub fn atoms(&self) -> Vec<&Atom> {
         let mut atm = Vec::new();

         for i in 1..self.atoms.len() {
             atm.push(&self.atoms[i]);
         }

         atm
     }

     pub fn name(&self) -> &str { &self.id }   
}


fn main () {

    //let carbon = Atom{id:0, element:C, edge:vec![]};
    //let mut atoms: Vec<Atom> = vec![Atom{id:0, element:C, edge:vec![]}];
    //let mut index: Vec<Vec<usize>> = vec![];

    let mut m = Molecule::from("methane");
    m.branch(&[1]);

    m.close();

    let atoms = m.atoms();

    for i in 0..atoms.len() {
        let name = symbol(atoms[i].element);

        print!("Atom({}.{}: ", name, atoms[i].id);


        print!(")\n");
    }

    print!("{} {:?}\n", m.name(), m.formula());


}
