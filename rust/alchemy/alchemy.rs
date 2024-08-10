#![allow(dead_code, unused)]
// passed : 36

mod tests; 
use tests::*;
use tests::tests::*;

use preloaded::{ChemResult, ChemError, Element::{self, *}};
use std::collections::HashMap;

const DATA: &[(Element,(&str, usize, f32))] =
&[(H,("H",1,1.0)),(B,("B",3,10.8)),(C,("C",4,12.0)),(N,("N",3,14.0)),(O,("O",2,16.0)),(F,("F",1,19.0)),(Mg,("Mg",2,24.3)),(P,("P",3,31.0)),(S,("S",2,32.1)),(Cl,("Cl",1,35.5)),(Br,("Br",1,80.0))] ;

fn symbol (atom: Element) -> String { DATA.iter().find(|x| x.0 == atom).unwrap().1.0.to_string() }
fn valence (atom: Element) -> usize { DATA.iter().find(|x| x.0 == atom).unwrap().1.1 }
fn mass (atom: Element) -> f32 { DATA.iter().find(|x| x.0 == atom).unwrap().1.2 }

#[derive(Hash, Clone, Debug)]
pub struct Atom {
    pub id:usize,
    pub element:Element,
    edge:Vec<Atom>,
}

impl Eq for Atom {}

impl PartialEq for Atom {
    fn eq(&self, other: &Self) -> bool { self.id == other.id }
}
impl std::fmt::Display for Atom {
    fn fmt(&self, _f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut os = format!("Atom({}.{}", symbol(self.element), self.id);

        if self.edge.len() > 0 {
            os += &": ";

            for ed in &self.edge {
                if ed.element == H {
                    os += &format!("H,");
                } else {
                    os += &format!("{}{},", symbol(ed.element), ed.id);
                }
            }
            if self.edge.len() > 0 { os.pop(); }
        }
        os += &")";
        write!(_f, "{}", os)
    }
}

#[derive(Debug, Default)]
struct Molecule {
    id: String,
    index: Vec<Vec<usize>>,
    atoms: Vec<Atom>,
    lock: bool,
}

impl From<&'static str> for Molecule {
    fn from(_value: &'static str) -> Self {
        Molecule {
            id: _value.to_string(),
            index:vec![vec![]],
            atoms: vec![Atom{id:0, element:C,edge: vec![]}],
            lock:false,
        }
    }
}
impl Molecule {
     fn carbindex(&self) -> Vec<Vec<usize>> {
         (0..self.index.len()) .map(|i| (0..i).filter(|j| self.atoms[self.index[i][*j]].element == C).collect::<Vec<_>>()).collect::<Vec<_>>()
     }
    fn link(&mut self, c1: usize, c2: usize) {
        let at1 = self.atoms[c1].clone();
        let at2 = self.atoms[c2].clone();

        self.atoms[c1].edge.push(at2);
        self.atoms[c2].edge.push(at1);

        self.atoms[c1].edge.sort_by( |a,b| if a.element == b.element { a.id.cmp(&b.id) } else { a.element.cmp(&b.element) });
        self.atoms[c2].edge.sort_by( |a,b| if a.element == b.element { a.id.cmp(&b.id) } else { a.element.cmp(&b.element) });
    }
    fn inside(&self, nc:usize, nb:usize) -> bool { return nb < self.index.len() && nc < self.index[nb].len() }

    pub fn name(&self) -> &str { &self.id }

    pub fn branch(&mut self, _bs: &[usize]) -> ChemResult<&mut Self> {
        // add new branches with x carbon to the molecule
        if self.lock == true { return Err(ChemError::LockedMolecule); }
        if self.atoms.len() == 0 { *self = Molecule::from(""); }
        
        print!(".branch(&{:?})?\n", _bs);
        //print!("name {:?}\nindex {:?}\natoms {:?}\n", self.id, self.index, self.atoms);
        for ncarb in _bs {
            let mut arm = vec![0];

              for _ in 0..*ncarb {
                let nb = self.atoms.len();
                let atom = Atom {id:nb, element:C, edge: vec![] };

                arm.push(nb);
                self.atoms.push(atom);
            }

            for i in 2..=*ncarb {
                self.link(arm[i-1], arm[i-0]);
            }

            self.index.push(arm);
        }

        Ok(self)
    }
    pub fn bond(&mut self, _poses: &[(usize, usize, usize, usize)]) -> ChemResult<&mut Self> {
        // (&mut m).bond(&[(c1, b1, c2, b2), ...])
        //creates new bonds between two atoms of existing branches : bond c1 of b1 with c2 of b2
        if self.lock == true { return Err(ChemError::LockedMolecule); }
        let n_brs = self.index.len();
        print!(".bond(&{:?})?\n", _poses);

        for &(c1,b1,c2,b2) in _poses {
            if self.inside(c1,b1) && self.inside(c2,b2) {
                let a1 = self.index[b1][c1];
                let a2 = self.index[b2][c2];

                if a1 == a2 {
                    return Err(ChemError::InvalidBond);
                }
                self.link(a1,a2);
            } else {
                //return Err(ChemError::InvalidBond) 
            }
        }

        Ok(self)
    }
    pub fn mutate(&mut self, _ms: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
        // (&mut m).mutate(&[(nc, nb, elt), ...])
        //mutate the carbon nc in the branch nb into the chemeical element elt
        if self.lock == true { return Err(ChemError::LockedMolecule); }
        print!(".mutate(&{:?})?\n", _ms);

        for &(nc, nb, elt) in _ms {
            if self.inside(nc,nb) {
                let it = self.index[nb][nc];
                let mut curr = self.atoms[it].clone();

                if curr.edge.len() > valence(elt) {
                    return Err(ChemError::InvalidBond);
                } else {
                    curr.element = elt;

                    for edge in &curr.edge {
                        let nid = edge.id;

                        for link in &mut self.atoms[nid].edge {
                            if link.id == curr.id {
                                link.element = elt;
                            }
                        }

                        self.atoms[nid].edge.sort_by(|a,b| if a.element == b.element { a.id.cmp(&b.id) } else { a.element.cmp(&b.element) });
                    }
                }
                self.atoms[it] = curr;
            }

        }

        Ok(self)
    }
    pub fn add(&mut self, _els: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
        // add a new atom of kind elt on the carbon nc in the branch nb
        if self.lock == true { return Err(ChemError::LockedMolecule); }
        print!(".add(&{:?})?\n", _els);

        for &(nc, nb, elt) in _els {
            if !self.inside(nc,nb) { return Err(ChemError::InvalidBond) }

            let ix = self.index[nb][nc];
            let nx = self.atoms.len();
            let curr = self.atoms[ix].clone();

            if curr.edge.len() < valence(curr.element) {
                self.atoms.push(Atom {id: nx, element: elt, edge:vec![]});
                self.link(ix,nx);
            } else {
                return Err(ChemError::InvalidBond);
            }
        }

        Ok(self)
    }
    pub fn add_chain(&mut self, nc: usize, nb: usize, _els: &[Element]) -> ChemResult<&mut Self> {
        //adds on the carbon nc in the branch nb a chain with all the provided elements
        print!(".add_chain({}, {}, &{:?})?\n", nc, nb, _els);
        let mut arm:Vec<usize> = Vec::new();
        let check = (0.._els.len())
            .map(
                |x| if x < _els.len() - 1 { valence(_els[x]) as i32 - 2 } else { valence(_els[x]) as i32 - 1 }
            )
            .collect::<Vec<i32>>();

        if self.lock == true { return Err(ChemError::LockedMolecule); }
        if check.iter().find(|&x| *x < 0).is_some() { return Err(ChemError::InvalidBond); }

        for elt in _els {
            let ax = self.atoms.len();
            self.atoms.push(Atom {id: ax, element:*elt , edge: vec![]});
            arm.push(ax);
        }

        for i in 1..arm.len() {
            self.link(arm[i-0], arm[i-1]);
        }

        self.link(self.atoms[self.index[nb][nc]].id, arm[0]);

        Ok(self)
    }
    pub fn close(&mut self) -> ChemResult<&mut Self> {
        print!(".close()?\n");
        if self.lock == true { return Err(ChemError::LockedMolecule); }
        self.lock = true;

        for i in 1..self.atoms.len() {
            let mut atom = self.atoms[i].clone();
            let size = valence (atom.element) as i32 - atom.edge.len() as i32;

            if size < 0 {
                return Err(ChemError::InvalidBond);
            } else {
                for _ in 0..size {
                    let hydrogen = Atom {id: self.atoms.len(), element:H, edge:vec![atom.clone()]};
                    atom.edge.push(hydrogen.clone());
                    self.atoms.push(hydrogen);
                }
            }

            self.atoms[i] = atom;
        }

        Ok(self)
    }
    pub fn unlock(&mut self) -> ChemResult<&mut Self> {
        print!(".unlock()?\n");

        if self.lock == false { return Err(ChemError::UnlockedMolecule) }

        self.lock = false;
        self.atoms.retain(|x| x.element != H);

        for i in 1..self.atoms.len() {
            self.atoms[i].edge.retain(|x| x.element != H);
        }

        self.index.retain(|x| x.len() != 1);

        (0..self.atoms.len()).for_each(|lv| {
            let id = self.atoms[lv].id;

            if id != lv {
                self.index.iter_mut().for_each(|x| { x.into_iter().filter(|x| *x == &id).for_each(|x| *x = lv); });
            }
        });

        match self.atoms.len() {
            1 => Err(ChemError::EmptyMolecule) ,
            _ => Ok(self),
        }
    }

    pub fn formula(&self) -> ChemResult<String> {
        if self.atoms.len() == 1 { return Err(ChemError::EmptyMolecule); }
        if self.lock == false { return Err(ChemError::UnlockedMolecule); }

        let mut freq:HashMap<Element, usize> = HashMap::new();

        for i in 1..self.atoms.len() {
            let atom = &self.atoms[i];
            *freq.entry(atom.element).or_insert(0) += 1;
        }

        let mut freq = freq.iter().collect::<Vec<_>>();
        freq.sort_by(|a,b| a.0.cmp(b.0));

        Ok(freq.iter().map(|atom| if atom.1 > &1 { format!("{}{}", symbol(*atom.0), atom.1) } else { format!("{}", symbol(*atom.0)) }).collect::<String>())
    }
    pub fn molecular_weight(&self) -> ChemResult<f32> {
        if self.atoms.len() == 1 { return Err(ChemError::EmptyMolecule); }
        if self.lock == false { return Err(ChemError::UnlockedMolecule); }

        let sum = (1..self.atoms.len()).fold(0.0, |sum, x| sum + mass(self.atoms[x].element) );

        Ok(sum)
    }
    pub fn atoms(&self) -> Vec<&Atom> {

        (1..self.atoms.len()).map(|x| &self.atoms[x]).collect::<Vec<_>>()
    }
}

fn showbase(base: &Vec<Atom>) {
    print!("\n");
    for atom in base {
        if atom.element != H { print!("{atom} "); }
    }
    print!("\n");
}

fn assert() {

    basics::constructors();
    basics::simple_carbohydrates();

    atom_spec::atom_display();
    atom_spec::element_and_id();
    atom_spec::atom_display_with_bonds();
    atom_spec::atom_equals_only_uses_id();

    create_and_bond_carbohydrates::carbo_tests();

    mutations_and_carbohydrates::mutation_tests();
    mutation_then_additions::chain_adding();

    failure::basic_invalid_builds::run();
    failure::invalid_mutation_and_addition::run();
}

fn fun() -> ChemResult<Molecule> {

    let mut mol:Molecule = Molecule::default();
    mol

        .branch(&[1, 6, 4])?
        .add_chain(1, 1, &[C, Br])?
        .mutate(&[(1, 1, H)])?
        .close()?
        .unlock()?

        .bond(&[(5, 1, 3, 2)])?

        ;

    Ok(mol)
}
fn main () {

    //assert();
    fun();

    /*

       tests::failure::unlocking::manages_atoms_in_branches
       Correct management of the atoms that should be in the branches or not

       .branch(&[1, 6, 4])?
       .add_chain(&1 1 [C, Br])?
       .mutate(&[(1, 1, H)])?
       .close()?
       .unlock()?
       .bond(&[(5, 1, 3, 2)])?

       tests::failure::unlocking::remove_empty_branches_after_unlocking
       .branch(&[1, 5])?
       .bond(&[(2, 2, 5, 2), (4, 2, 1, 1)])?
       .mutate(&[(1, 1, H)])?
       .branch(&[3])?
       .bond(&[(2, 3, 1, 3), (2, 3, 3, 3)])?
       .close()?
       .unlock()?
       .add(&[(2, 2, P)])?
       .add(&[(2, 2, P)])?

       Got error Err(InvalidBond) but expected InvalidBond.
       Details: Should remove any empty branch: m.add(&[(2,2,P)]) shouldn't work, now, since the targeted carbon (previously third on the branch) cannot accept new bonds.

       -------------------------------------------------------------
       tests::failure::unlocking::remove_hydrogens_and_update_id_numbers
       Properly unlock a molecule, removing hydrogens and updating id numbers)

       .branch(&[3])?
       .add(&[(2, 1, H)])?
       .branch(&[1])?
       .bond(&[(2, 1, 1, 2)])?
       .close()?
       .unlock()?

       assertion failed: `(left == right)`
       left: `["Atom(C.1: C2)", "Atom(C.2: C1,C3,C5)", "Atom(C.3: C2)", "Atom(C.5: C2)"]`,
       right: `["Atom(C.1: C2)", "Atom(C.2: C1,C3,C4)", "Atom(C.3: C2)", "Atom(C.4: C2)"]`: *All* hydrogens should be removed after unlocking and carbond id modified accordingly:
       ["Atom(C.1: C2)", "Atom(C.2: C1,C3,C5)", "Atom(C.3: C2)", "Atom(C.5: C2)"] should be ["Atom(C.1: C2)", "Atom(C.2: C1,C3,C4)", "Atom(C.3: C2)", "Atom(C.4: C2)"]

       ---------------------------------------------------
       tests::failure::unlocking::unlocking_after_mutation_to_h
       Handle unlocking while a carbon from the 'branches' has been mutated to an hydrogen before

       .branch(&[8, 5])?
       .bond(&[(2, 2, 5, 2), (4, 2, 2, 1)])?
       .mutate(&[(1, 2, H)])?
       .bond(&[(3, 2, 4, 1)])?
       .close()?
       .unlock()?
       assertion failed: `(left == right)`
       left: `["Atom(C.1: C2)", "Atom(C.2: C1,C3,C12)", "Atom(C.3: C2,C4)", "Atom(C.4: C3,C5,C11)", "Atom(C.5: C4,C6)", "Atom(C.6: C5,C7)", "Atom(C.7: C6,C8)", "Atom(C.8: C7)", "Atom(C.10: C11,C13)", "Atom(C.11: C4,C10,C12)", "Atom(C.12: C2,C11,C13)", "Atom(C.13: C10,C12)"]`,

       right: `["Atom(C.1: C2)", "Atom(C.2: C1,C3,C11)", "Atom(C.3: C2,C4)", "Atom(C.4: C3,C5,C10)", "Atom(C.5: C4,C6)", "Atom(C.6: C5,C7)", "Atom(C.7: C6,C8)", "Atom(C.8: C7)", "Atom(C.9: C10,C12)", "Atom(C.10: C4,C9,C11)", "Atom(C.11: C2,C10,C12)", "Atom(C.12: C9,C11)"]`: Should remove all the hydrogens after unlocking and affect new id numbers when needed:

       ["Atom(C.1: C2)", "Atom(C.2: C1,C3,C12)", "Atom(C.3: C2,C4)", "Atom(C.4: C3,C5,C11)", "Atom(C.5: C4,C6)", "Atom(C.6: C5,C7)", "Atom(C.7: C6,C8)", "Atom(C.8: C7)", "Atom(C.10: C11,C13)", "Atom(C.11: C4,C10,C12)", "Atom(C.12: C2,C11,C13)", "Atom(C.13: C10,C12)"] should be ["Atom(C.1: C2)", "Atom(C.2: C1,C3,C11)", "Atom(C.3: C2,C4)", "Atom(C.4: C3,C5,C10)", "Atom(C.5: C4,C6)", "Atom(C.6: C5,C7)", "Atom(C.7: C6,C8)", "Atom(C.8: C7)", "Atom(C.9: C10,C12)", "Atom(C.10: C4,C9,C11)", "Atom(C.11: C2,C10,C12)", "Atom(C.12: C9,C11)"]

*/

}
