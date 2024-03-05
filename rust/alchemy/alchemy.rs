#![allow(dead_code, unused)]
// passed : 36

//////////////////////////////////////////preloaded////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////
use std::collections::HashMap;
use Element::*;

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

fn showbase(base: &Vec<Atom>) {
    print!("\n");
    for atom in base {
        if atom.element != H { print!("{atom} "); }
    }
    print!("\n");
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
    pub fn name(&self) -> &str { &self.id }
    pub fn branch(&mut self, _bs: &[usize]) -> ChemResult<&mut Self> {
        if self.lock == true { return Err(ChemError::LockedMolecule); }
        print!("branch : {:?}\n", _bs);

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

        if self.lock == true { return Err(ChemError::LockedMolecule); }
        print!("bond : {:?}\n", _poses);

        for it in _poses {
            let c1 = self.index[it.1][it.0];
            let c2 = self.index[it.3][it.2];

            if c1 == c2 {
                return Err(ChemError::InvalidBond);
            }
            self.link(c1,c2);
        }
        //showbase(&self.atoms);

        Ok(self)
    }
    pub fn mutate(&mut self, _ms: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
        // (&mut m).mutate(&[(nc, nb, elt), ...])

        if self.lock == true { return Err(ChemError::LockedMolecule); }
        print!("mutate : {:?}\n", _ms);
        //showbase(&self.atoms);

        for (nc, nb, elt) in _ms {
            let it = self.index[*nb][*nc];
            let mut curr = self.atoms[it].clone();

            if curr.edge.len() > valence(*elt) {
                print!("error\n");
                return Err(ChemError::InvalidBond);
            } else {
                curr.element = *elt;

                for edge in &curr.edge {
                    let nid = edge.id;

                    for link in &mut self.atoms[nid].edge {
                        if link.id == curr.id {
                            link.element = *elt;
                        }
                    }

                    self.atoms[nid].edge.sort_by(|a,b| if a.element == b.element { a.id.cmp(&b.id) } else { a.element.cmp(&b.element) });
                }
            }

            self.atoms[it] = curr;
        }

        Ok(self)
    }
    pub fn add(&mut self, _els: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
        if self.lock == true { return Err(ChemError::LockedMolecule); }
        print!("add : {:?}\n", _els);

        for (nc, nb, elt) in _els {
            let ix = self.index[*nb][*nc];
            let nx = self.atoms.len();
            let curr = self.atoms[ix].clone();

            if curr.edge.len() < valence(curr.element) {
                self.atoms.push(Atom {id: nx, element: *elt, edge:vec![]});
                self.link(ix,nx);
            } else {
                return Err(ChemError::InvalidBond);
            }
        }

        Ok(self)
    }
    pub fn add_chain(&mut self, nc: usize, nb: usize, _els: &[Element]) -> ChemResult<&mut Self> {
        print!("add_chain : {} {} {:?}\n", nc, nb, _els);
        let mut arm:Vec<usize> = Vec::new();
        let check = (0.._els.len()).map(|x| if x < _els.len()-1 {valence(_els[x]) as i32 - 2} else {valence(_els[x]) as i32 - 1}).collect::<Vec<i32>>();

        if self.lock == true { return Err(ChemError::LockedMolecule); }
        if check.iter().find(|&x| *x < 0).is_some() {
            return Err(ChemError::InvalidBond);
        }

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
        print!("close\n");
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

        //showbase(&self.atoms);
        Ok(self)
    }
    pub fn unlock(&mut self) -> ChemResult<&mut Self> {
        print!("unlock\n");
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

fn main () {

    //Should fail when chaining atoms after any monovalent atom
    let mut m = Molecule::from("Carbon 6");

    m.branch(&[3]);
    m.bond(&[]);
    m.add_chain(2,1, &[C,C,F,H]);
    m.close();

    showbase(&m.atoms);

    let _els = [C,C,F,H];
    //let check = (0.._els.len()).map(|x| if x < _els.len()-1 {valence(_els[x]) as i32 - 2} else {valence(_els[x]) as i32 - 1}).collect::<Vec<i32>>();
    let check = _els.iter().map(|x| valence(*x) as i32 - 2).collect::<Vec<i32>>();


    print!("{:?}\n", check);
        //let (0..check.len()).map(|x| if x < check.len()-1 {check[x]-2} else {check[x]-1}).collect::<Vec<i32>>();
        //*check.last_mut().unwrap() += 1;

    //print!(" {} => {:?} {:?}\n", m.name(), m.formula(), m.molecular_weight());

}
