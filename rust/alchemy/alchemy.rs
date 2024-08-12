#![allow(dead_code, unused)]
// passed : 88 

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
    fn checkvalnce(&self, index:usize) -> bool {
        return self.atoms[index].edge.len() < valence(self.atoms[index].element)
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
        
        for &ncarb in _bs {
            let mut arm = vec![0];

            for _ in 0..ncarb {
                let nb = self.atoms.len();
                let atom = Atom { id:nb, element:C, edge: vec![] };

                arm.push(nb);
                self.atoms.push(atom);
            }

            for i in 2..=ncarb {
                self.link(arm[i-1], arm[i-0]);
            }

            self.index.push(arm);
        }

        Ok(self)
    }
    pub fn bond(&mut self, _poses: &[(usize, usize, usize, usize)]) -> ChemResult<&mut Self> {
        //(&mut m).bond(&[(c1, b1, c2, b2), ...])
        //creates new bonds between two atoms of existing branches : bond c1 of b1 with c2 of b2
        if self.lock == true { return Err(ChemError::LockedMolecule); }
        print!(".bond(&{:?})?\n", _poses);

        for &(c1,b1,c2,b2) in _poses {
            if self.inside(c1,b1) && self.inside(c2,b2) {
                let i1 = self.index[b1][c1];
                let i2 = self.index[b2][c2];

                if i1 == i2 { return Err(ChemError::InvalidBond) } // invalidate self bonding
                if !self.checkvalnce(i1) || !self.checkvalnce(i2) { return Err(ChemError::InvalidBond) }
                self.link(i1,i2);
            } 
            //else {
            //     return Err(ChemError::InvalidBond) 
            //}
        }

        Ok(self)
    }
    pub fn mutate(&mut self, _ms: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
        // (&mut m).mutate(&[(nc, nb, elt), ...])
        //mutate the carbon nc in the branch nb into the chemeical element elt
        if self.lock == true { return Err(ChemError::LockedMolecule); }
        print!(".mutate(&{:?})?\n", _ms);

        for &(nc, nb, elt) in _ms {
            if !self.inside(nc,nb) { return Err(ChemError::InvalidBond) }

            let i1 = self.index[nb][nc];
            let mut curr = self.atoms[i1].clone();

            if curr.edge.len() <= valence(elt) {
                curr.element = elt;

                for edge in &curr.edge {
                    let i2 = edge.id;

                    self.atoms[i2].edge.iter_mut().for_each (|link| if link.id == curr.id { link.element = elt });
                    self.atoms[i2].edge.sort_by(|a,b| if a.element == b.element { a.id.cmp(&b.id) } else { a.element.cmp(&b.element) });
                }

                self.atoms[i1] = curr;
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

            let i1 = self.index[nb][nc];
            let i2 = self.atoms.len();
            let curr = self.atoms[i1].clone();

            if curr.edge.len() < valence(curr.element) {
                self.atoms.push(Atom {id: i2, element: elt, edge:vec![]});
                self.link(i1,i2);
            } else {
                return Err(ChemError::InvalidBond);
            }

            self.atoms[i1].edge.sort_by(|a,b| if a.element == b.element { a.id.cmp(&b.id) } else { a.element.cmp(&b.element) });
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

        for &elt in _els {
            let ax = self.atoms.len();
            self.atoms.push(Atom {id: ax, element:elt , edge: vec![]});
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
        let atoms = self.atoms.clone();

        self.lock = false;

        self.index.iter_mut().for_each(|arm| arm.retain(|x| atoms[*x].element != H));
        self.index.retain(|x| x.len() != 1);

        self.atoms.retain(|atom| atom.element != H);
        self.atoms.iter_mut().for_each(|atom| atom.edge.retain(|edge| edge.element != H));

        (0..self.atoms.len()).for_each(|lv| {
            let curr = self.atoms[lv].clone();

            if curr.id != lv {
                self.atoms[lv].id = lv;
                self.index.iter_mut().for_each( |x| { x.into_iter().filter(|x| *x == &curr.id).for_each(|x| *x = lv) });
                self.atoms.iter_mut().for_each( |atom| atom.edge.iter_mut().for_each ( |edge| if edge.id == curr.id { edge.id = lv }));
            }

        });

        match self.atoms.len() {
            1 => Err(ChemError::EmptyMolecule),
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
fn biotin () -> ChemResult<Molecule> {

    let mut biotin = Molecule::from("biotin");

    biotin.branch(&[14,1,1])?;
    biotin.bond(&[(2,1,1,2), (2,1,1,2), (10,1,1,3), (10,1,1,3), (8,1,12,1), (7,1,14,1)])?;
    biotin.mutate(&[(1,1,O),  (1,2,O), (1,3,O), (11,1,N), (9,1,N), (14,1,S)])?;
    //biotin.close()?;

    print!("\n{:?} {:?}\n", biotin.index, biotin.formula());
    for atom in &biotin.atoms {
        print!("{atom}\n");
    }


    Ok(biotin)
}
fn test() -> ChemResult<Molecule> {

    let mut mol:Molecule = Molecule::default();
    mol

        .branch(&[3, 1, 8, 5])?
        .branch(&[5, 7, 9, 1])?
        .mutate(&[(7, 3, P)])?
        .add(&[(2, 1, F), (3, 3, C), (2, 3, C), (6, 3, F), (4, 7, O), (8, 3, Mg)])?
        .add(&[(2, 5, H), (3, 5, C), (6, 6, Cl)])?
        .add(&[(6, 7, P), (5, 3, F), (4, 7, H), (3, 6, F), (3, 5, Mg), (6, 3, Br)])?

        ;

    print!("\n{:?} {:?}\n", mol.index, mol.formula());
    for atom in &mol.atoms {
        //print!("{atom}\n");
    }

    Ok(mol)
}
fn main () {

    //assert();
    test();
    //biotin();


}
