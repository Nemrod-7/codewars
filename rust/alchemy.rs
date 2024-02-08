#![allow(dead_code, unused)]

use std::collections::{HashMap,HashSet};
use std::fmt::Display;

///////////////////////////////////////////////////////////////////
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Element {
    C, H, O, B, Br, Cl, F, Mg, N, P, S,
}
use Element::*;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ChemError {
    EmptyMolecule,
    LockedMolecule,
    InvalidBond,
    UnlockedMolecule,
}

pub type ChemResult<T> = Result<T, ChemError>;

const DATA: &[ (Element,(&str, usize, f32)) ] =
&[(H,("H",2,1.0)),(B,("B",3,10.8)),(C,("C",4,12.0)),(N,("N",3,14.0)),(O,("O",2,16.0)),(F,("F",1,19.0)),(Mg,("Mg",2,24.3)),(P,("P",3,31.0)),(S,("S",2,21.1)),(Cl,("Cl",1,35.5)),(Br,("Br",1,80.0))] ;


fn symbol (atom: Element) -> String { DATA.iter().find(|x| x.0 == atom).unwrap().1.0.to_string() }
fn valence (atom: Element) -> usize { DATA.iter().find(|x| x.0 == atom).unwrap().1.1 }
fn mass (atom: Element) -> f32 { DATA.iter().find(|x| x.0 == atom).unwrap().1.2 }
///////////////////////////////////////////////////////////////////
#[derive(Hash, Clone)]
struct Atom {
    id:usize,
    element:Element,
    edge:Vec<(usize,usize)>,
}

impl PartialEq for Atom {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}

impl Eq for Atom { }

struct Molecule {
    id:String,
    subs:Vec<Vec<Atom>>,
    lock:bool,
}

impl Molecule {
    fn default () -> Molecule {
        Molecule {
            id:String::new(),
            subs:vec![vec![]],
            lock:false,
        }
    }
    fn from(src: &str) -> Molecule {
        Molecule {
            id:src.to_string(),
            subs:vec![vec![]],
            lock:false,
        }
    }


    fn name (&self) -> ChemResult<String> {

        if self.subs.len() == 0 {
            return Err(ChemError::EmptyMolecule);
        }

        Ok (self.id.to_string())
    }
    fn atoms(&self) -> ChemResult<String> {
        if self.subs.len() == 0 { return Err(ChemError::EmptyMolecule); }
        if self.lock == false { return Err(ChemError::UnlockedMolecule); }

        let mut os = String::new();

        for i in 1..self.subs.len() {
            for j in 1..self.subs[i].len() {
                let atom = &self.subs[i][j];

                if atom.element == H { continue } 
                let name = symbol(atom.element);
                os += &format!("Atom({}.{}:", name, atom.id);

                for i in 0..atom.edge.len() {
                    let pos = atom.edge[i];

                    let name = symbol(self.subs[pos.1][pos.0].element);
                    os += &format!("{}{},", name, pos.0);
                }
                if os.len() > 0 { os.pop(); }
                os += &")\n";
            }
            os += &"\n";
        }

        print!("{os}");
        Ok(os)
    }
    fn formula (&self) -> ChemResult<String> {
        if self.subs.len() == 0 { return Err(ChemError::EmptyMolecule); }
        if self.lock == false { return Err(ChemError::UnlockedMolecule); }

        let mut freq:HashMap<Element, usize> = HashMap::new();
        let mut res = String::new();

        for i in 1..self.subs.len() {
            for j in 1..self.subs[i].len() {
                let at = &self.subs[i][j];
                if let Some(val) = freq.get_mut(&at.element) {
                    *val += 1;
                } else {
                    freq.insert(at.element.clone(), 1);
                }
            }
        }


        let mut freq = freq.iter().collect::<Vec<_>>();
        freq.sort_by(|a,b| a.1.cmp(b.1));

        Ok (freq.iter().map(|atom| format!("{}{}", symbol(*atom.0), atom.1)).collect::<String>())
    }
    fn molecular_weight(&self) -> ChemResult<f32> {
        if self.subs.len() == 0 { return Err(ChemError::EmptyMolecule); }
        if self.lock == false { return Err(ChemError::UnlockedMolecule); }

        let mut sum = 0.0;

        for i in 1..self.subs.len() {
            for j in 1..self.subs[i].len() {
                let atom = &self.subs[i][j];

                sum += mass(atom.element); 
            }
        }

        Ok(sum)
    }

    fn bond (&mut self, bonds: &[(usize,usize,usize,usize)]) -> Result<usize,ChemError> {

        /*
           (&mut m).bond(&[(c1, b1, c2, b2), ...])

           The bond method...:

           Creates new bounds between two atoms of existing branches.
           Each argument is a tuple of four integers giving:
           c1 & b1: carbon and branch positions of the first atom
           c2 & b2: carbon and branch positions of the second atom

           All positions are 1-indexed, meaning (1,1,5,3) will bound the first carbon of the first branch with the fifth of the third branch.
           Only positive integers will be used.
           */
        if self.lock == true { return Err(ChemError::LockedMolecule); }

        for it in bonds {
            let nc1 = (0..self.subs[it.1].len()).filter(|x| self.subs[it.1][*x].element == C).collect::<Vec<_>>();
            let nc2 = (0..self.subs[it.3].len()).filter(|x| self.subs[it.3][*x].element == C).collect::<Vec<_>>();

            self.subs[it.1][nc1[it.0]].edge.push((it.2,it.3));
            self.subs[it.3][nc2[it.2]].edge.push((it.0,it.1));

        }

        Ok(0)
    }
    fn branch (&mut self, list: &[usize]) -> Result<usize,ChemError> {

        /*
           ie: (&mut m).branch(&[x, y, z, ...])

           In a Molecule instance, a "branch" represents a chain of atoms bounded together. When a branch is created, all of its atoms are carbons.
           Each "branch" of the Molecule is identifiable by a number that matches its creation order: first created branch as number 1, second as number 2, ...

           The branch method...:
           Can take any number of arguments (positive integers).
           Adds new "branches" to the current molecule.
           Each argument gives the number of carbons of the new branch.
           */
        if self.lock == true { return Err(ChemError::LockedMolecule); }

        for i in 0..list.len() {
            let nc = self.subs.len();
            let mut section:Vec<Atom> = (0..=list[i]).map(|ix| Atom{id: ix, element:C, edge: vec![]}).collect();

            for j in 1..list[i] {
                let a = j;
                let b = j + 1;
                section[a].edge.push((b, nc));
                section[b].edge.push((a, nc));
            }
            &self.subs.push(section);
        }

        Ok(0)
    }
    fn mutate (&mut self, list: &[(usize,usize,Element)]) -> Result<usize,ChemError> {
        /*
           (&mut m).mutate(&[(nc, nb, elt), ...])

           The mutate method...:

           Mutates the carbon nc in the branch nb to the chemical element elt(given as a string).
           Don't forget that carbons and branches are 1-indexed.
           This is mutation: the id number of the Atom instance stays the same. See the Atom class specs about that.
           */
        if self.lock == true { return Err(ChemError::LockedMolecule); }

        for it in list {
            let tablex = (0..self.subs[it.1].len()).filter(|x| self.subs[it.1][*x].element == C).collect::<Vec<_>>();
            let atom = &mut self.subs[it.1][tablex[it.0]];

            atom.element = it.2;
            if atom.edge.len() > valence(atom.element) { return Err(ChemError::InvalidBond); }
        }

        Ok(0)
    }
    fn add (&mut self, list: &[(usize,usize,String)]) -> Result<usize,ChemError> {

        /*
           (&mut m).add(&[(nc, nb, elt), ...])

           The add method...:

           Adds a new Atom of kind elt (string) on the carbon nc in the branch nb.
           Atoms added this way are not considered as being part of the branch they are bounded to and aren't considered a new branch of the molecule.
           */
        if self.lock == true { return Err(ChemError::LockedMolecule); }

        let symbol = HashMap::from([("H",H),("B",B),("C",C),("N",N),("O",O),("F",F),("Mg",Mg),("P",P),("S",S),("Cl",Cl),("Br",Br)]);

        for it in list {
            let nb = self.subs[0].len();
            let next = Atom{id: nb, element: *symbol.get(&it.2 as &str).unwrap(), edge:vec![(it.0,it.1)]};
            let tablex = (0..self.subs[it.1].len()).filter(|x| self.subs[it.1][*x].element == C).collect::<Vec<_>>();
            let atom = &mut self.subs[it.1][tablex[it.0]];

            if atom.edge.len() < valence(atom.element) {
                self.subs[it.1][tablex[it.0]].edge.push((0,nb));
                self.subs[0].push(next);
            } else {
                return Err(ChemError::InvalidBond);
            }
        }

        Ok(0)
    }
    fn add_chain (&mut self) -> Result<usize,ChemError> {
        /*
           (&mut m).add_chaining(nc, nb, &[elt, ...])

           The add_chaining method...:

           Adds on the carbon nc in the branch nb a chain with all the provided elements, in the specified order.
           Meaning: m.add_chaining(2, 5, "N", "C", "C", "Mg", "Br") will add the chain ...-N-C-C-Mg-Br to the atom number 2 in the branch 5.
           As for the add method, this chain is not considered a new branch of the molecule.
           */

        if self.lock == true { return Err(ChemError::LockedMolecule); }

        Ok(0)
    }

    fn close (&mut self) {
        // Finalizes the molecule instance, adding missing hydrogens everywhere and locking the object (see behaviours part below).
        self.lock = true;

        for i in 1..self.subs.len() {
            for j in 1..self.subs[i].len() {
                let val = valence(self.subs[i][j].element);
                let cur = self.subs[i][j].edge.len();
                let left = val - cur;

                for _ in 0..left {
                    let size = self.subs[i].len();
                    let hydrogen = Atom {id:size, element:H, edge:vec![]};

                    self.subs[i][j].edge.push((size, i));
                    self.subs[i].push(hydrogen);
                }
            }

        }
    }
    fn unlock(&mut self) {
        self.lock = false;
        let mut index:Vec<usize> = Vec::new();
        let molecule = self.subs.clone();

        /*
           Makes the molecule mutable again.
           Hydrogens should be removed, as well as any empty branch you might encounter during the process.
           After the molecule has been "unlocked", if by any (bad...) luck it does not have any branch left, throw an EmptyMolecule exception.
           The id numbers of the remaining atoms have to be continuous again (beginning at 1), keeping the order they had when the molecule was locked.
           After removing hydrogens, if you end up with some atoms that aren't connected in any way to the branches of the unlocked molecule, keep them anyway in the Molecule instance (for the sake of simplicity...).
           Once unlocked, the molecule has to be modifiable again, in any manner.
           */
        // for atom in &mut self.subs {
        //     atom.edge.retain(|&id| branch[id].element == Element::H);
        // }

        // self.subs.retain(|x| x.element == Element::H);

        // if self.subs.len() == 0 {
        //     //Err(ChemError::EmptyMolecule);
        // }

    }
}

fn main () {
    /*
       Methods that involve building molecule or mutating molecule objects have to be chainable (ex: let ref mut m = Molecule::from("octane"); m.branch(&[8])?.close()?;).

       Building a molecule consists in mutating the original object at each method call.
       An InvalidBond exception should be thrown each time you encounter a case where an atom exceeds its valence number or is bounded to itself (about the valence number, see additional information below).
       When a method throws an exception while it still has several arguments/atoms left to handle, the modifications resulting from the valid previous operations must be kept but all the arguments after the error are ignored.
       Special case with add_chaining: if an error occurs at any point when adding the chain, all its atoms have to be removed from the instance (even the valid ones).
       The whole molecule integrity should hold against any error, meaning that it must be possible to correctly work with a molecule object even after it threw an exception.
       The fields formula and molecular_weight or the associated getters (depending on your language) should throw an UnlockedMolecule exception if an user tries to access them while the molecule isn't locked
       (because we do not want the user to catch incomplete/invalid information).
       In a similar manner, attempts of modification of a molecule after it has been locked should throw a LockedMolecule exception (the closer method follows this behavior too).
       */

    let dictionary:HashMap<Element,(&str, usize, f32)> = HashMap::from(
        [
        (Element::H,("H",1,1.0)), (Element::B,("B",3,10.8)), (Element::C,("C",4,12.0)), (Element::N,("N",3,14.0)),
        (Element::O,("O",2,16.0)), (Element::F,("F",1,19.0)), (Element::Mg,("Mg",2,24.3)), (Element::P,("P",3,31.0)),
        (Element::S,("S",2,21.1)), (Element::Cl,("Cl",1,35.5)), (Element::Br,("Br",1,80.0))
        ]);


    let mole = HashMap::from([(H, 1.0),(B, 10.8),(C,12.0),(N,14.0),(O,16.0),(F,19.0),(Mg,24.3),(P,31.0),(S,21.1),(Cl,35.5),(Br,80.0)]);
    let symbol = HashMap::from([(H,"H"),(B,"B"),(C,"C"),(N,"N"),(O,"O"),(F,"F"),(Mg,"Mg"),(P,"P"),(S,"S"),(Cl,"Cl"),(Br,"Br")]);
    let valence = HashMap::from([(H, 1),(B, 3),(C,4),(N,3),(O,2),(F,1),(Mg,2),(P,3),(S,2),(Cl,1),(Br,1)]);

    let ref mut biotin = Molecule::from("biotin");

    biotin.branch(&[14,1,1]);
    //biotin.bond(&[(2,1,1,2),  (2,1,1,2), (10,1,1,3), (10,1,1,3), (8,1,12,1), (7,1,14,1)]);
    //biotin.mutate(&[(1,1,O),  (1,2,O), (1,3,O), (11,1,N), (9,1,N), (14,1,S)]);
    biotin.close();

    biotin.atoms();
    //print!("{:?} {:?}\n", biotin.name(), biotin.formula());


}
