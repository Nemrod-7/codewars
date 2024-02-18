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
    fn default() -> Molecule {
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
    fn link (&mut self, c1: usize, c2: usize) {
        let at1 = self.atoms[c1].clone();
        let at2 = self.atoms[c2].clone();

        self.atoms[c1].edge.push(at2);
        self.atoms[c2].edge.push(at1);

        self.atoms[c1].edge.sort_by(|a,b| a.id.cmp(&b.id));
        self.atoms[c2].edge.sort_by(|a,b| a.id.cmp(&b.id));
    }
    fn ncarb (&self, nc: usize, nb: usize) -> usize {
        let carbon = (0..self.index[nb].len()).filter(|x| self.atoms[self.index[nb][*x]].element == C).collect::<Vec<_>>();
        carbon[nc]
    }
    pub fn name(&self) -> &str { &self.id }

    fn carbindex (&self) -> Vec<Vec<usize>> {
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
            let c1 = self.index[it.1][it.0];//let c1 = self.index[it.1][self.ncarb(it.0,it.1)];
            let c2 = self.index[it.3][it.2];//let c2 = self.index[it.3][self.ncarb(it.2,it.3)];
            self.link(c1,c2);
        }

        Ok(self)
    }

    pub fn mutate(&mut self, _ms: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
        if self.lock == true { return Err(ChemError::LockedMolecule); }

        for (nc, nb, elt) in _ms {
            let it = self.index[*nb][*nc]; 
            let mut curr = self.atoms[it].clone();
            curr.element = *elt;

            if curr.edge.len() > valence(curr.element) {
                return Err(ChemError::InvalidBond);
            } else {

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

       // for i in 1..self.atoms.len() {
       //     print!("{}\n", self.atoms[i]);
       // }
        Ok(self)
    }

    pub fn add(&mut self, _els: &[(usize, usize, Element)]) -> ChemResult<&mut Self> {
        if self.lock == true { return Err(ChemError::LockedMolecule); }

        for (nc, nb, elt) in _els {
            let it1 = self.index[*nb][*nc];
            let curr = self.atoms[it1].clone();
            let nod = self.atoms.len();

            if curr.edge.len() < valence(curr.element) {
                self.atoms.push(Atom {id: nod, element: *elt, edge:vec![]});
                self.link(it1,nod);
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

        for i in 1..self.index.len() {
            for j in 1..self.index[i].len() {
                let ix = self.index[i][j];

                print!("{} ", ix);
            }
            print!("\n");
        }

        Ok(self)
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

        let res = freq.iter().map(|atom| if atom.1 > &1 { format!("{}{}", symbol(*atom.0), atom.1) } else { format!("{}", symbol(*atom.0)) }).collect::<String>();
        //print!("res:: {res}\n");
        Ok (res)
    }

    pub fn molecular_weight(&self) -> ChemResult<f32> {
        if self.atoms.len() == 1 { return Err(ChemError::EmptyMolecule); }
        if self.lock == false { return Err(ChemError::UnlockedMolecule); }

        let sum = (1..self.atoms.len()).fold(0.0, |sum, x| sum + mass(self.atoms[x].element) );

        Ok(sum)
    }

    pub fn atoms(&self) -> Vec<&Atom> {
        (1..self.atoms.len()).map(|x| &self.atoms[x]).collect::<Vec<_>>()
            /*
               let mut atm = Vec::new();

               for i in 1..self.index.len() {
               for j in 1..self.index[i].len() {
               atm.push(&self.atoms[self.index[i][j]]);
               }
               }
               print!("{}\n", atm.len());

               atm
               */
    }

}


fn main () {
    ( "isopropylmagnesium bromide - adding chain",
      [3],
      (2,1, [Mg, Br]),
      "C3H7BrMg",
      147.3,
      ["Atom(C.1: C2,H,H,H)", "Atom(C.2: C1,C3,Mg4,H)", "Atom(C.3: C2,H,H,H)", "Atom(Mg.4: C2,Br5)", "Atom(Br.5: Mg4)"]
    );


    let mutation =  [
        ( "Furane: no additional hydrogens while closing after mutation",
          vec! [5], vec![(5,1,1,1), (5,1,4,1), (2,1,3,1)], vec![(1,1,O)], "C4H4O", 68.,
          vec!["Atom(O.1: C2,C5)", "Atom(C.2: C3,C3,O1,H)", "Atom(C.3: C2,C2,C4,H)", "Atom(C.4: C3,C5,C5,H)", "Atom(C.5: C4,C4,O1,H)"]
        ),
    ];


    for it in mutation {
        let mut m = Molecule::from(it.0);
        m.branch(&it.1);
        m.bond(&it.2);
        m.mutate(&it.3);
        m.close();

        m.unlock();
    }


}
