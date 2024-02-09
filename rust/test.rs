#![allow(dead_code, unused)]

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

pub type ChemResult<T> = Result<T, ChemError>;

struct Atom {
    id:usize,
    element:Element,
}

struct Molecule {

}

impl Molecule {

    fn new() -> Molecule {
        Molecule {

        }
    }

    fn first(&mut self) -> ChemResult<&mut Molecule> {
        
        Ok(self)
    }

    fn next(&mut self) -> ChemResult<&mut Molecule> {

        Ok(self)
    }

}



fn construct() -> ChemResult<Molecule> {

    let mut mole:Molecule = Molecule::new();

    mole.first()?.next()?;

    Ok(mole)
}
fn main () {

    //let mole = HashMap::from([(H, 1.0),(B, 10.8),(C,12.0),(N,14.0),(O,16.0),(F,19.0),(Mg,24.3),(P,31.0),(S,21.1),(Cl,35.5),(Br,80.0)]);
    //let symbol = HashMap::from([(H,"H"),(B,"B"),(C,"C"),(N,"N"),(O,"O"),(F,"F"),(Mg,"Mg"),(P,"P"),(S,"S"),(Cl,"Cl"),(Br,"Br")]);
    //let valence = HashMap::from([(H, 1),(B, 3),(C,4),(N,3),(O,2),(F,1),(Mg,2),(P,3),(S,2),(Cl,1),(Br,1)]);

    let dictionary:Vec<(Element,(&str, usize, f32))> =
        vec![(H,("H",2,1.0)),(B,("B",3,10.8)),(C,("C",4,12.0)),(N,("N",3,14.0)),(O,("O",2,16.0)),(F,("F",1,19.0)),(Mg,("Mg",2,24.3)),(P,("P",3,31.0)),(S,("S",2,21.1)),(Cl,("Cl",1,35.5)),(Br,("Br",1,80.0))] ;


    let line:Vec<Atom> = Vec::new();

    for atom in line.iter() {
        if atom.element == H {

        }
    }
}
