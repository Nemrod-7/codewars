#![allow(dead_code, unused)]

use std::collections::HashMap;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Element { C, H, O, B, Br, Cl, F, Mg, N, P, S, }
use Element::*;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ChemError {
    EmptyMolecule,
    LockedMolecule,
    InvalidBond,
    UnlockedMolecule,
}

pub type ChemResult<T> = Result<T, ChemError>;

const DATA: &[(Element,(&str, usize, f32))] =
&[(H,("H",2,1.0)),(B,("B",3,10.8)),(C,("C",4,12.0)),(N,("N",3,14.0)),(O,("O",2,16.0)),(F,("F",1,19.0)),(Mg,("Mg",2,24.3)),(P,("P",3,31.0)),(S,("S",2,21.1)),(Cl,("Cl",1,35.5)),(Br,("Br",1,80.0))] ;

fn symbol (atom: Element) -> String { DATA.iter().find(|x| x.0 == atom).unwrap().1.0.to_string() }
fn valence (atom: Element) -> usize { DATA.iter().find(|x| x.0 == atom).unwrap().1.1 }
fn mass (atom: Element) -> f32 { DATA.iter().find(|x| x.0 == atom).unwrap().1.2 }

#[derive(Hash, Clone)]
struct Atom {
    id:usize,
    element:Element,
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
    fn link(&mut self, c1: usize, c2: usize) {
        let at1 = self.atoms[c1].clone();
        let at2 = self.atoms[c2].clone();

        self.atoms[c1].edge.push(at2);
        self.atoms[c2].edge.push(at1);

        self.atoms[c1].edge.sort_by(|a,b| a.id.cmp(&b.id));
        self.atoms[c2].edge.sort_by(|a,b| a.id.cmp(&b.id));
    }
    /*
    fn ncarb(&self, nc: usize, nb: usize) -> usize {
        let carbon = (0..self.index[nb].len()).filter(|x| self.atoms[self.index[nb][*x]].element == C).collect::<Vec<_>>();
        carbon[nc]
    }
    */
    pub fn name(&self) -> &str { &self.id }

    fn carbindex(&self) -> Vec<Vec<usize>> {
        (0..self.index.len()) .map(|i| (0..i) .filter(|j| self.atoms[self.index[i][*j]].element == C).collect::<Vec<_>>()).collect::<Vec<_>>()
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

            for i in 2..=*ncarb {
                self.link(arm[i-1], arm[i-0]);
            }

            self.index.push(arm);
        }

        Ok(self)
    }

    pub fn bond(&mut self, _poses: &[(usize, usize, usize, usize)]) -> ChemResult<&mut Self> {
        if self.lock == true { return Err(ChemError::LockedMolecule); }

        for it in _poses {
            let c1 = self.index[it.1][it.0];
            let c2 = self.index[it.3][it.2];
            self.link(c1,c2);
        }

        Ok(self)
    }

    pub fn mutate(&mut self, _ms: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
        if self.lock == true { return Err(ChemError::LockedMolecule); }

        for (nc, nb, elt) in _ms {
            let it = self.index[*nb][*nc]; 
            let mut curr = self.atoms[it].clone();

            if curr.edge.len() > valence(curr.element) {
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

                    self.atoms[nid].edge.sort_by(|a,b| a.element.cmp(&b.element));
                }
            }

            self.atoms[it] = curr;
        }

        Ok(self)
    }

    pub fn add(&mut self, _els: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
        if self.lock == true { return Err(ChemError::LockedMolecule); }

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
        self.add(&_els.iter().map(|elt| (nc,nb,*elt)).collect::<Vec<_>>())
    }

    pub fn close(&mut self) -> ChemResult<&mut Self> {
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

    let it = ( "isopropylmagnesium bromide - adding chain",
               [3],
               (2,1, [Mg, Br]),
               "C3H7BrMg",
               147.3,
               ["Atom(C.1: C2,H,H,H)", "Atom(C.2: C1,C3,Mg4,H)", "Atom(C.3: C2,H,H,H)", "Atom(Mg.4: C2,Br5)", "Atom(Br.5: Mg4)"]
             );


    let mut m = Molecule::from(it.0);
    m.branch(&it.1);
    m.add_chain(it.2.0, it.2.1, &it.2.2);

    m.close();

    print!("{:?} {:?}\n", m.formula(), m.molecular_weight());

}
