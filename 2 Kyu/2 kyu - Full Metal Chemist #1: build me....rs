#![allow(dead_code, unused)]
// passed : 102 

//mod tests; 
//use tests::*;
//use tests::tests::*;

pub mod preloaded {

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

    impl std::fmt::Display for Element {
        fn fmt(&self, _f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
            let mut os = String::new();

            match self {
                Element::C  => os += &format!("Carbon"),
                Element::H  => os += &format!("Hydrogen"),
                Element::O  => os += &format!("Oxygen"),
                Element::B  => os += &format!("Bore"),
                Element::Br => os += &format!("Brome"),
                Element::Cl => os += &format!("Chlore"),
                Element::F  => os += &format!("Fluor"),
                Element::Mg => os += &format!("Magnesium"),
                Element::N  => os += &format!("Azote"),
                Element::P  => os += &format!("Phosphore"),
                Element::S  => os += &format!("Sulfur"),
                _ => (),
            }
            write!(_f, "{}", os)
        }
    }
}

//Symbol:           H     B     C     N     O     F    Mg     P     S    Cl    Br
//Valence number:   1     3     4     3     2     1     2     3     2     1     1
//Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  (in g/mol)

use preloaded::{ChemResult, ChemError, Element::{self, *}};
use std::collections::HashMap;

const DATA: &[(Element,(&str, usize, f32))] =
&[(H,("H",1,1.0)),(B,("B",3,10.8)),(C,("C",4,12.0)),(N,("N",3,14.0)),(O,("O",2,16.0)),(F,("F",1,19.0)),(Mg,("Mg",2,24.3)),(P,("P",3,31.0)),(S,("S",2,32.1)),(Cl,("Cl",1,35.5)),(Br,("Br",1,80.0))] ;

fn mass (atom: Element) -> f32 { DATA.iter().find(|x| x.0 == atom).unwrap().1.2 }
fn valence (atom: Element) -> usize { DATA.iter().find(|x| x.0 == atom).unwrap().1.1 }
fn symbol (atom: Element) -> String { DATA.iter().find(|x| x.0 == atom).unwrap().1.0.to_string() }
fn comp (atom:&Atom) -> usize { 
    if atom.element == H { 
        std::usize::MAX 
    } else { 
        atom.id 
    } 
}

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

#[derive(Debug)]
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
    fn default() -> Molecule {
        Molecule {
            id: format!(""),
            index:vec![vec![]],
            atoms: vec![Atom{id:0, element:C,edge: vec![]}],
            lock:false,
        }

    }
    fn checkvalence(&self, index:usize) -> bool {
        return self.atoms[index].edge.len() < valence(self.atoms[index].element)
    }
    fn sortedge(&mut self, index: usize) {
        self.atoms[index].edge.sort_by(|a,b| a.id.cmp(&b.id));
        self.atoms[index].edge.sort_by( |a,b| if a.element != H && b.element != H { a.element.cmp(&b.element) } else { comp(a).cmp(&(comp(b))) });
    }
    fn link(&mut self, c1: usize, c2: usize) {
        let at1 = self.atoms[c1].clone();
        let at2 = self.atoms[c2].clone();

        self.atoms[c1].edge.push(at2);
        self.atoms[c2].edge.push(at1);

        self.sortedge(c1);
        self.sortedge(c2);
    }
    fn inside(&self, nc:usize, nb:usize) -> bool { 
        return nb < self.index.len() && nc < self.index[nb].len() 
    }

    pub fn branch(&mut self, _bs: &[usize]) -> ChemResult<&mut Self> {
        // add new branches with x carbon to the molecule
        if self.lock == true { return Err(ChemError::LockedMolecule) }
        print!(".branch(&{:?})?\n", _bs);

        for &ncarb in _bs {
            let mut arm = vec![0];

            for _ in 0..ncarb {
                let nb = self.atoms.len();
                self.atoms.push(Atom { id:nb, element:C, edge: vec![] });
                arm.push(nb);
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
        if self.lock == true { return Err(ChemError::LockedMolecule) }
        print!(".bond(&{:?})?\n", _poses);

        for &(c1,b1,c2,b2) in _poses {
            if self.inside(c1,b1) && self.inside(c2,b2) {
                let a1 = self.index[b1][c1];
                let a2 = self.index[b2][c2];

                if a1 == a2 { return Err(ChemError::InvalidBond) } // invalidate self bonding
                if !self.checkvalence(a1) || !self.checkvalence(a2) { return Err(ChemError::InvalidBond) }
                self.link(a1,a2);
            } else {
                 return Err(ChemError::InvalidBond) 
            }
        }

        Ok(self)
    }
    pub fn mutate(&mut self, _ms: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
        // (&mut m).mutate(&[(nc, nb, elt), ...])
        //mutate the carbon nc in the branch nb into the chemeical element elt
        if self.lock == true { return Err(ChemError::LockedMolecule) }
        print!(".mutate(&{:?})?\n", _ms);

        for &(nc, nb, elt) in _ms {
            if !self.inside(nc,nb) { return Err(ChemError::InvalidBond) }

            let it = self.index[nb][nc];
            let mut curr = self.atoms[it].clone();

            if curr.edge.len() <= valence(elt) {
                curr.element = elt;

                for edge in &curr.edge {
                    let nid = edge.id;

                    self.atoms[nid].edge.iter_mut().for_each (
                        |link| if link.id == curr.id {
                            link.element = elt
                        }
                    );
                    self.sortedge(nid);
                }
            } else {
                return Err(ChemError::InvalidBond)
            }

            self.atoms[it] = curr;
        }

        Ok(self)
    }
    pub fn add(&mut self, _els: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
        // add a new atom of kind elt on the carbon nc in the branch nb
        if self.lock == true { return Err(ChemError::LockedMolecule) }
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
        }

        //print!("\n{:?} {:?}\n", self.index, self.formula());
        Ok(self)
    }
    pub fn add_chain(&mut self, nc: usize, nb: usize, _els: &[Element]) -> ChemResult<&mut Self> {
        //adds on the carbon nc in the branch nb a chain with all the provided elements
        print!(".add_chain({}, {}, &{:?})?\n", nc, nb, _els);

        let check = (0.._els.len()) .map(|x| valence(_els[x]) as i32 - if x < _els.len() - 1 { 2 } else { 1 }).collect::<Vec<i32>>(); // check for monovalent atom

        if self.lock == true { return Err(ChemError::LockedMolecule) }
        if !self.inside(nc, nb) { return Err(ChemError::InvalidBond) }
        if !self.checkvalence(self.index[nb][nc]) { return Err(ChemError::InvalidBond) }
        if check.iter().find(|&x| *x < 0).is_some() { return Err(ChemError::InvalidBond) }

        let mut chain:Vec<usize> = Vec::new();
        //print!("\n{atom} -> {} :: {}\n", atom.element, valence(atom.element));

        for &elt in _els {
            //print!("{} {}\n", elt, valence(elt));
            let id = self.atoms.len();
            self.atoms.push(Atom {id: id, element:elt , edge: vec![]});
            chain.push(id);
        }
        
        for i in 1..chain.len() {
            self.link(chain[i-0], chain[i-1]);
        }

        self.link(self.index[nb][nc], chain[0]);

        Ok(self)
    }
    pub fn close(&mut self) -> ChemResult<&mut Self> {
        print!(".close()?\n");
        if self.lock == true { return Err(ChemError::LockedMolecule) }
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

    pub fn name(&self) -> &str { &self.id }
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
    biotin.close()?;

    print!("\n{:?} {:?}\n", biotin.index, biotin.formula());
    for atom in &biotin.atoms {
        print!("{atom}\n");
    }

    let result = vec!["Atom(O.1: C2,H)", "Atom(C.2: C3,O1,O15,O15)", "Atom(C.3: C2,C4,H,H)", "Atom(C.4: C3,C5,H,H)", "Atom(C.5: C4,C6,H,H)", "Atom(C.6: C5,C7,H,H)", "Atom(C.7: C6,C8,S14,H)", "Atom(C.8: C7,C12,N9,H)", "Atom(N.9: C8,C10,H)", "Atom(C.10: O16,O16,N9,N11)", "Atom(N.11: C10,C12,H)", "Atom(C.12: C8,C13,N11,H)", "Atom(C.13: C12,S14,H,H)", "Atom(S.14: C7,C13)", "Atom(O.15: C2,C2)", "Atom(O.16: C10,C10)"];

    Ok(biotin)
}
fn test() -> ChemResult<Molecule> {

    let mut mol:Molecule = Molecule::default();

    mol.branch(&[8]);
    mol.branch(&[3, 9, 8, 5, 7, 5, 2]);
    mol.mutate(&[(6, 6, Cl), (2, 3, C), (2, 8, C), (1, 8, H), (7, 6, C), (7, 4, N), (5, 6, B)]);
    mol.mutate(&[(3, 6, O), (5, 1, S), (1, 7, N), (4, 3, B), (1, 6, O), (3, 4, O), (2, 5, C), (1, 4, F), (3, 2, H), (8, 3, Br)]);
    mol.add(&[(5, 4, H), (1, 6, Mg), (5, 5, P), (1, 3, O), (3, 3, N)]);
    mol.add_chain(5, 1, &[B, F]);


    print!("\n");
    for i in 1..mol.atoms.len() {
        if mol.atoms[i].element != H {
            print!("{} ", mol.atoms[i]);
        }
    }

    Ok(mol)
}

fn main () {


    // left: `["Atom(C.1: C2,H,H,H)", "Atom(C.2: C1,C3,Mg4,H)", "Atom(C.3: C2,H,H,H)", "Atom(Mg.4: C2,Br5)", "Atom(Br.5: Mg4)"];
    //right: `["Atom(C.1: C2,H,H,H)", "Atom(C.2: C1,C3,Mg4,H)", "Atom(C.3: C2,H,H,H)", "Atom(Mg.4: Br5,C2)", "Atom(Br.5: Mg4)"];
    // order Mg Br C B H
    //assert();
    let result = test();

    //    print!("{:?}", result);
    //    biotin();


}
