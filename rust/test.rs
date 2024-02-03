#![allow(dead_code, unused)]

use std::collections::HashMap;
use std::fmt::Display;

///////////////////////////////////////////////////////////////////
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Element {
    C, H, O, B, Br, Cl, F, Mg, N, P, S,
}


#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ChemError {
    EmptyMolecule,
    LockedMolecule,
    InvalidBond,
    UnlockedMolecule,
}

pub type ChemResult<T> = Result<T, ChemError>;
///////////////////////////////////////////////////////////////////

struct Atom {
    id:i32,
    nb:i32,
    element:String,
    edge:Vec<Atom>,
}
struct Molecule {
    id:String,
    atoms:Vec<Atom>,
    lock:bool,
}

impl PartialEq for Atom {
    fn eq(&self, other: &Self) -> bool { 
      self.id == other.id 
    }
}

impl Eq for Atom {

}
impl Atom {
    fn new() -> Atom {
        Atom {
            id:0,
            nb:1,
            element: String::new(),
            edge:Vec::new(),
        }
    }
}
fn valence (sym: &str) -> i32 {
    let dict = HashMap::from([("H", (1,1.0)), ("B", (3,10.8)), ("C",(4,12.0)), ("N",(3,14.0)), ("O", (2,16.0)), ("F", (1,19.0)), ("Mg", (2,24.3)), ("P",(3,31.0)),("S",(2,21.1)),("Cl",(1,35.5)),("Br",(1,80.0))]);

    if let Some(val) = dict.get(sym) {
       return val.0; 
    } 

    return 0;
}

impl Molecule {
    fn default () -> Molecule {
        Molecule {
            id:String::new(),
            atoms:Vec::new(),
            lock:true,
        }
    }
    
    fn from(src: &str) -> Molecule {
        Molecule {
            id:src.to_string(),
            atoms:Vec::new(),
            lock:true,
        }
    }

    fn formula (&self) -> Result<String, ChemError> {
        let mut res = String::new();

        if self.atoms.len() == 0 { return Err(ChemError::EmptyMolecule); }
        if self.lock == false { return Err(ChemError::UnlockedMolecule); }

        for at in &self.atoms {
            res += &format!("{}{}", at.element, at.nb);
        }

        Ok(res)
    }

    fn molecular_weight(&self) -> Result<f32, ChemError> {
        let dict = HashMap::from([("H", (1,1.0)), ("B", (3,10.8)), ("C",(4,12.0)), ("N",(3,14.0)), ("O", (2,16.0)), ("F", (1,19.0)), ("Mg", (2,24.3)), ("P",(3,31.0)),("S",(2,21.1)),("Cl",(1,35.5)),("Br",(1,80.0))]);
        if self.atoms.len() == 0 { return Err(ChemError::EmptyMolecule); }
        if self.lock == false { return Err(ChemError::UnlockedMolecule); }
        let mut weight = 0.0;

        for at in &self.atoms {
            let key = &at.element;
            let nb = &at.nb;

            if let Some(val) = dict.get(key as &str) {
                weight += val.1 * (*nb as f32);
            }
        }

        Ok(weight)
    }

    fn atoms(&self) -> Result<String, ChemError> {
        if self.atoms.len() == 0 { return Err(ChemError::EmptyMolecule); }
        if self.lock == false { return Err(ChemError::UnlockedMolecule); }
        let mut res = String::new();

        for at in &self.atoms {

        }

        Ok(res)
    }

    fn name (&self) -> Result<String,ChemError> {

        if self.atoms.len() == 0 {
            return Err(ChemError::EmptyMolecule);
        }

        Ok (self.id.to_string())
    }

    fn bond (&mut self, src: &Vec<i32>) {

    }
    fn branch (&mut self, src: &[i32]) {

    }
    fn mutate (&mut self) {

    }
    fn add (&mut self) {

    }
    fn add_chain (&mut self) {

    }

    fn close (&mut self) {

    }

    fn unlock(&mut self) {
        self.lock = false;
    }
}

fn main () {
    let dict = HashMap::from([("H", (1,1.0)), ("B", (3,10.8)), ("C",(4,12.0)), ("N",(3,14.0)), ("O", (2,16.0)), ("F", (1,19.0)), ("Mg", (2,24.3)), ("P",(3,31.0)),("S",(2,21.1)),("Cl",(1,35.5)),("Br",(1,80.0))]);

    let ref mut biotin = Molecule::from("biotin");


    /*
       biotin.branch(&[14,1,1])?;
       biotin.bond(&[(2,1,1,2), (2,1,1,2), (10,1,1,3), (10,1,1,3), (8,1,12,1), (7,1,14,1)])?;
       biotin.mutate(&[(1,1,O), (1,2,O), (1,3,O), (11,1,N), (9,1,N), (14,1,S)])?;
       biotin.close()?;
       */

}
