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
    pub id:usize,
    pub element:Element,
    edge:Vec<(usize,usize)>,
}

impl PartialEq for Atom {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}

impl Eq for Atom { }

impl std::fmt::Display for Atom {
    fn fmt(&self, _f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        todo!()
    }
}

pub struct Molecule {
    id:String,
    subs:Vec<Vec<Atom>>,
    atoms: Vec<Atom>,
    lock:bool,
}
impl From<&'static str> for Molecule {
    fn from(_value: &'static str) -> Self {
        todo!()
    }
}

impl Molecule {
    fn default () -> Molecule {
        Molecule {
            id:String::new(),
            subs:vec![vec![]],
            atoms:vec![],
            lock:false,
        }
    }
    fn from(src: &str) -> Molecule {
        Molecule {
            id:src.to_string(),
            subs:vec![vec![]],
            atoms:vec![],
            lock:false,
        }
    }
    fn getcarbon (&mut self, nxt:&(usize, usize, Element)) -> &mut Atom {
        let arm = &mut self.subs[nxt.1];
        let carbon = (0..arm.len()).filter(|x| arm[*x].element == C).collect::<Vec<_>>();

        &mut arm[carbon[nxt.0]]
    }
    fn name (&self) -> &str { &self.id }

    fn atoms(&self) -> Vec<&Atom> {
        let mut atm = Vec::new();

        for i in 1..self.subs.len() {
            for j in 1..self.subs[i].len() {
                let atom = &self.subs[i][j];
                //let name = symbol(atom.element);
                //if atom.element == H { continue } 
                //os += &format!("Atom({}.{}:", name, atom.id);

                for k in 0..atom.edge.len() {
                    let pos = atom.edge[k];
                    let name = symbol(self.subs[pos.1][pos.0].element);
                    //os += &format!("{}{},", name, pos.0);
                }
                //if os.len() > 0 { os.pop(); }
                //os += &")\n";
                atm.push(atom);
            }
            //os += &"\n";
        }

        atm
    }
    fn formula (&self) -> ChemResult<String> {
        if self.subs.len() == 0 { return Err(ChemError::EmptyMolecule); }
        if self.lock == false { return Err(ChemError::UnlockedMolecule); }

        let mut freq:HashMap<Element, usize> = HashMap::new();
        let mut res = String::new();

        for i in 1..self.subs.len() {
            for j in 1..self.subs[i].len() {
                let atom = &self.subs[i][j];

                 *freq.entry(atom.element).or_insert(1) += 1;
            }
        }

        let mut freq = freq.iter().collect::<Vec<_>>();
        freq.sort_by(|a,b| a.0.cmp(b.0));

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

    fn bond (&mut self, bonds: &[(usize,usize,usize,usize)]) -> ChemResult<&mut Self> {

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
            self.getcarbon(&(it.0,it.1,H)).edge.push((it.2,it.3));
            self.getcarbon(&(it.2,it.3,H)).edge.push((it.0,it.1));
        }

        Ok(self)
    }
    fn branch (&mut self, list: &[usize]) -> ChemResult<&mut Self> { 

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
            let nb = self.subs.len();
            //let mut section = vec![Atom{id: 0, element:C, edge: vec![]}; list[i] + 1];
            let mut section:Vec<Atom> = (0..=list[i]).map(|ix| Atom{id: ix, element:C, edge: vec![]}).collect();

            for j in 1..list[i] {
                let a = j;
                let b = j + 1;
                section[a].edge.push((b, nb));
                section[b].edge.push((a, nb));
            }
            &self.subs.push(section);
        }

        Ok(self)
    }
    fn mutate (&mut self, list: &[(usize,usize,Element)]) -> ChemResult<&mut Self> {
        /*
           (&mut m).mutate(&[(nc, nb, elt), ...])

           The mutate method...:

           Mutates the carbon nc in the branch nb to the chemical element elt(given as a string).
           Don't forget that carbons and branches are 1-indexed.
           This is mutation: the id number of the Atom instance stays the same. See the Atom class specs about that.
           */
        if self.lock == true { return Err(ChemError::LockedMolecule); }

        for it in list {
            let atom = self.getcarbon(it);
            atom.element = it.2;
            
            if atom.edge.len() > valence(atom.element) { 
                return Err(ChemError::InvalidBond); 
            }
        }

        Ok(self)
    }
    fn add (&mut self, list: &[(usize,usize,Element)]) -> ChemResult<&mut Self> {

        /*
           (&mut m).add(&[(nc, nb, elt), ...])

           The add method...:

           Adds a new Atom of kind elt (string) on the carbon nc in the branch nb.
           Atoms added this way are not considered as being part of the branch they are bounded to and aren't considered a new branch of the molecule.
           */
        if self.lock == true { return Err(ChemError::LockedMolecule); }
        //let symbol = HashMap::from([("H",H),("B",B),("C",C),("N",N),("O",O),("F",F),("Mg",Mg),("P",P),("S",S),("Cl",Cl),("Br",Br)]);

        for it in list {
            let nb = self.subs[0].len();
            let atom = self.getcarbon(it);

            if atom.edge.len() < valence(atom.element) {
                atom.edge.push((0, nb));
                self.subs[0].push(Atom{id: nb, element: it.2, edge:vec![(it.0,it.1)]});
            } else {
                return Err(ChemError::InvalidBond);
            }
        }

        Ok(self)
    }
    fn add_chain (&mut self) -> ChemResult<&mut Self> {
        /*
           (&mut m).add_chaining(nc, nb, &[elt, ...])

           The add_chaining method...:

           Adds on the carbon nc in the branch nb a chain with all the provided elements, in the specified order.
           Meaning: m.add_chaining(2, 5, "N", "C", "C", "Mg", "Br") will add the chain ...-N-C-C-Mg-Br to the atom number 2 in the branch 5.
           As for the add method, this chain is not considered a new branch of the molecule.
           */

        if self.lock == true { return Err(ChemError::LockedMolecule); }

        Ok(self)
    }

    fn close (&mut self) -> ChemResult<&mut Self> {
        // Finalizes the molecule instance, adding missing hydrogens everywhere and locking the object (see behaviours part below).
        self.lock = true;

        for i in 1..self.subs.len() {
            for j in 1..self.subs[i].len() {
                let val = valence(self.subs[i][j].element);
                let cur = self.subs[i][j].edge.len();
                
                if val < cur {
                    return Err(ChemError::InvalidBond);
                }

                for _ in 0..(val - cur) {
                    let size = self.subs[i].len();
                    let hydrogen = Atom {id:size, element:H, edge:vec![]};

                    self.subs[i][j].edge.push((size, i));
                    self.subs[i].push(hydrogen);
                }
            }
        }

        Ok(self)
    }
    fn rebuild (&mut self) {
        let mut build:Vec<Vec<Atom>> = Vec::new();
        let prev = &self.subs;

        let mut build:Vec<Vec<Atom>> = Vec::new();

        for i in 0..prev.len() {
            let mut line:Vec<Atom> = Vec::new(); 

            for j in 0..prev[i].len() {
                let atom = &prev[i][j];

                if atom.element != H {
                    let edg = atom.edge.clone();
                    let next = Atom {id: atom.id, element:atom.element, edge: edg.into_iter().filter(|x| prev[x.1][x.0].element != H).collect::<Vec<_>>() };
                    line.push(next);
                }
            }
            build.push(line);
        } 
        self.subs = build;
    }
    fn unlock(&mut self) -> ChemResult<&mut Self> {
        self.lock = false;
        let mut build:Vec<Vec<Atom>> = Vec::new();
        let prev = &self.subs;
        /*
           Makes the molecule mutable again.
           Hydrogens should be removed, as well as any empty branch you might encounter during the process.
           After the molecule has been "unlocked", if by any (bad...) luck it does not have any branch left, throw an EmptyMolecule exception.
           The id numbers of the remaining atoms have to be continuous again (beginning at 1), keeping the order they had when the molecule was locked.
           After removing hydrogens, if you end up with some atoms that aren't connected in any way to the branches of the unlocked molecule, keep them anyway in the Molecule instance (for the sake of simplicity...).
           Once unlocked, the molecule has to be modifiable again, in any manner.
           */

        // remove hydrogen
        for i in 0..prev.len() {
            let mut line = vec![prev[i][0].clone()];

            for j in 1..prev[i].len() {
                let atom = &prev[i][j];

                if atom.element != H {
                    let edge = atom.edge.clone().into_iter().filter(|x| prev[x.1][x.0].element != H).collect::<Vec<_>>();
                    let next = Atom { id: atom.id, element:atom.element, edge:edge.clone(), };

                    if edge.len() == 0 && i > 0 {
                        build[0].push(next);
                    } else {
                        line.push(next);
                    }

                }
            }
            build.push(line);
        }

        self.subs = build;
        // make atoms id to be continuous
        // todo!


        self.subs.retain(|x| x.len() == 1); // remove empty branches

        if self.subs.len() == 0 {
            return Err(ChemError::EmptyMolecule);
        }

        Ok(self)
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

    let mut mol = Molecule::from("methane");
    mol.branch(&[1]);
    mol.close();

    let atoms = mol.atoms();

    for i in 0..atoms.len() {
        let name = symbol(atoms[i].element);

        print!("Atom({}.{}: ", name, atoms[i].id);



        print!(")\n");
    }
    //print!("{:?}\n", atoms);


}
