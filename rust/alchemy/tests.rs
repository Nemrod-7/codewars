
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

macro_rules! assert_float_eq {
    ($x:expr, $y:expr, $eq1:ident <= $eps:expr, $txt:expr) => {

        if (($x - $y) as f64).abs() > $eps {
            print!("\nexpected : {} got : {}\n", $x, $y);
        }

        print!("\n");
    };
}

pub mod tests {
    // use float_eq::assert_float_eq;
    use crate::preloaded::{ChemResult, ChemError, Element::{self, *}};
    use Molecule;

    fn valence(a: &Element) -> usize {
        match a {
            H | Br | Cl | F => 1,
            O | Mg | S => 2,
            B | N | P => 3,
            C => 4,
        }
    }
    fn weight(a: &Element) -> f32 {
        match a {
            C => 12., H => 1., O => 16., B => 10.8, Br => 80., Cl => 35.5,
            F => 19., Mg => 24.3, N => 14., P => 31., S => 32.1,
        }
    }

    macro_rules! chem_assert {
        ($exp:expr, should be, $got:expr, $msg:expr) => {
            assert_eq!($got, $exp, "{}:\n\t{:?} should be {:?}", $msg, $got, $exp)
        };
        ($exp:expr, should be, $got:expr) => {
            assert_eq!($got, $exp, "{:?} should be {:?}", $got, $exp)
        };
        ($exp:expr, should all be, $got:expr $(, $stuff:expr)*) => {
            chem_assert!(&$exp[..], should be, $got $(, $stuff)*)
        };
        ($got:expr, should err with, $exp:expr, $msg:expr) => {
            if let Err(e) = $got {
                chem_assert!(e, should be, $exp, $msg)
            } else {
                panic!("Expected an error {:?} but got {:?} instead.\n\tDetails: {}\n", $exp, $got, $msg, )
            }
        };
    }

    fn atom_strs(m: &Molecule, with_h: bool) -> Vec<String> {
        m.atoms().into_iter()
            .filter(|a| with_h || a.element != Element::H)
            .map(|a| format!("{a}"))
            .collect::<Vec<String>>()
    }

    /// will panic if constructing the molecule fails with the given sequence
    /// of builder `funcs` supplied with `args`
    macro_rules! mol {
        ( @unpack $m:expr $(, $func:ident ( $( $arg:expr ),* ) )* ) => {
            {
                let mut m = $m;
                let _ = (&mut m)$(
                    .$func($($arg, )*)
                    .expect(&format!(
                            "{} failed",
                            stringify!($func)
                    )))*;
                m
            }
        };

        ($name:literal $(, $func:ident ( $( $arg:expr ),* ) )* ) => (
            mol!(@unpack Molecule::from($name) $(, $func ( $( $arg ),* ) )* )
        );

        ($($func:ident ( $( $arg:expr ),* ) ),* ) => (
            mol!("" $(, $func ( $( $arg ),* ) )* )
        );

        ($m:expr => $($func:ident ( $( $arg:expr ),* ) ),* ) => (
            mol!(@unpack $m $(, $func ( $( $arg ),* ) )* )
        );
    }


    /// will attempt to do each operation in the chain, stopping all operations
    /// when one fails
    macro_rules! mol_safe {
        ( @burrow $fst:expr, $h:ident ( $( $h_arg:expr ),* ), $( $tl:ident ( $( $tl_arg:expr ),* ) ),+ ) => (
            $fst.$h($($h_arg, )*).and_then(|m| mol_safe!(@burrow m, $($tl ( $($tl_arg),* )),+))
        );
        ( @burrow $fst:expr, $h:ident ( $( $h_arg:expr ),* ) ) => (
            $fst.$h($($h_arg, )*)
        );

        ( $m:expr => $($func:ident ( $( $arg:expr ),* ) ),* ) => (
            mol_safe!(@burrow $m, $($func ( $( $arg ),* ) ),*)
        );

        ( $($func:ident ( $( $arg:expr ),* ) ),* ) => {
            {
                let mut m = Molecule::from("");
                let compute = mol_safe!(@burrow &mut m, $($func ( $( $arg ),* ) ),*);
                match compute {
                    Err(e) => Err(e),
                    Ok(_) => Ok(m)
                }
            }
        };
    }


    pub mod basics {
        use super::*;

        pub fn constructors() {
            chem_assert!(String::default(), should be, Molecule::default().name());
            chem_assert!(mol!().name(), should be, "", "Empty name should be constructed properly.");
            chem_assert!(mol!("banana").name(), should be, "banana", "Name should be correct even if sweet.");
        }

        pub fn simple_carbohydrates() {
            let methane = mol!("methane", branch(&[1]), close());
            chem_assert!(&methane.formula().unwrap(), should be, "CH4", "Testing raw formula");
            assert_float_eq!(16., methane.molecular_weight().unwrap(), abs <= 0.00001, "Testing molecular weight");

            let octane = mol!("octane", branch(&[8]), close());
            chem_assert!(&octane.formula().unwrap(), should be, "C8H18", "Testing raw formula");
            assert_float_eq!(114., octane.molecular_weight().unwrap(), abs <= 0.00001, "Testing molecular weight");
        }
    }

    pub fn biotin() -> ChemResult<()> {
        println!("Build the biotin (the example at the beginning of the description. Just for fun)");

        println!("{}", r#"
        let ref mut biotin = Molecule::from("biotin");

        biotin.branch(&[14,1,1])?;
        biotin.bond(&[(2,1,1,2), (2,1,1,2),
                    (10,1,1,3), (10,1,1,3),
                    (8,1,12,1), (7,1,14,1)])?;
        biotin.mutate(&[(1,1,O),  (1,2,O), (1,3,O), (11,1,N), (9,1,N), (14,1,S)])?;
        biotin.close()?;

        Gives:
        "#);
        let ref mut biotin = Molecule::from("biotin");

        biotin.branch(&[14,1,1])?;
        biotin.bond(&[(2,1,1,2), (2,1,1,2),
        (10,1,1,3), (10,1,1,3),
        (8,1,12,1), (7,1,14,1)])?;
        biotin.mutate(&[(1,1,O),  (1,2,O), (1,3,O), (11,1,N), (9,1,N), (14,1,S)])?;
        biotin.close()?;

        let output = atom_strs(&biotin, false);
        println!("{output:?}");

        chem_assert!(output, should all be, ["Atom(O.1: C2,H)", "Atom(C.2: C3,O1,O15,O15)", "Atom(C.3: C2,C4,H,H)", "Atom(C.4: C3,C5,H,H)", "Atom(C.5: C4,C6,H,H)", "Atom(C.6: C5,C7,H,H)", "Atom(C.7: C6,C8,S14,H)", "Atom(C.8: C7,C12,N9,H)", "Atom(N.9: C8,C10,H)", "Atom(C.10: O16,O16,N9,N11)", "Atom(N.11: C10,C12,H)", "Atom(C.12: C8,C13,N11,H)", "Atom(C.13: C12,S14,H,H)", "Atom(S.14: C7,C13)", "Atom(O.15: C2,C2)", "Atom(O.16: C10,C10)"]);
        Ok(())
    }

    pub mod atom_spec {
        use super::*;

        pub fn atom_display() {
            let m = mol!("methane", branch(&[1]));
            let atms = m.atoms();
            chem_assert!(atms.len(), should be, 1);
            chem_assert!(format!("{}", atms.first().unwrap()), should be, "Atom(C.1)".to_string());
        }

        pub fn element_and_id() {
            let m = mol!("methane", branch(&[1]), close());
            let atoms = m.atoms();
            chem_assert!(atoms.len(), should be, 5);
            use Element::*;
            for (i, (elt, a)) in [C, H, H, H, H].iter().zip(atoms.into_iter()).enumerate() {
                chem_assert!(&a.element, should be, elt, format!("Wrong atom {} at index {i} in methane's atoms(), should be {elt}", a.element));
                chem_assert!(a.id, should be, i + 1, format!("Wrong id {} at index {i} in methane's atoms(), should be {}", a.id, i + 1));
            }
        }

        pub fn atom_display_with_bonds() {
            let m = mol!("methane", branch(&[1]), close());
            let atoms = m.atoms();
            assert!(atoms.len() > 4, "methane should have more than 4 atoms");
            chem_assert!(format!("{}", &atoms[0]), should be, "Atom(C.1: H,H,H,H)".to_string());
            chem_assert!(format!("{}", &atoms[4]), should be, "Atom(H.5: C1)".to_string());
        }

        pub fn atom_equals_only_uses_id() {
            let methane = mol!("methane", branch(&[1]), close());
            let m_atoms = methane.atoms();
            assert!(m_atoms.len() > 2, "methane should have more than two atoms");

            let octane = mol!("octane", branch(&[8]), close());
            let o_atoms = octane.atoms();
            assert!(o_atoms.len() > 3, "octane should have more than three atoms");

            assert_eq!(&m_atoms[1], &o_atoms[1], "Do not modify the PartialEq/Eq implementation");
            assert_ne!(&m_atoms[2], &o_atoms[3], "Do not modify the PartialEq/Eq implementation");
        }
    }
    pub mod create_and_bond_carbohydrates {
        use super::*;

        pub fn carbo_tests () {
            let carbon = [
                ( "cyclohexane", vec![6], vec![(1,1,6,1)], "C6H12", 84.,
                vec!["Atom(C.1: C2,C6,H,H)", "Atom(C.2: C1,C3,H,H)", "Atom(C.3: C2,C4,H,H)", "Atom(C.4: C3,C5,H,H)", "Atom(C.5: C4,C6,H,H)", "Atom(C.6: C1,C5,H,H)"]
                ), (
                "1,1-dimethyl-2-propylcyclohexane", vec![9,1,1], vec![(4,1,9,1), (5,1,1,2), (5,1,1,3)], "C11H22", 154.,
                vec!["Atom(C.1: C2,H,H,H)", "Atom(C.2: C1,C3,H,H)", "Atom(C.3: C2,C4,H,H)", "Atom(C.4: C3,C5,C9,H)", "Atom(C.5: C4,C6,C10,C11)", "Atom(C.6: C5,C7,H,H)", "Atom(C.7: C6,C8,H,H)", "Atom(C.8: C7,C9,H,H)", "Atom(C.9: C4,C8,H,H)", "Atom(C.10: C5,H,H,H)", "Atom(C.11: C5,H,H,H)"]
                ), (
                "cubane - one branch", vec![8], vec![(3,1,6,1), (2,1,7,1), (1,1,8,1), (4,1,1,1), (5,1,8,1)], "C8H8", 104.,
                vec!["Atom(C.1: C2,C4,C8,H)", "Atom(C.2: C1,C3,C7,H)", "Atom(C.3: C2,C4,C6,H)", "Atom(C.4: C1,C3,C5,H)", "Atom(C.5: C4,C6,C8,H)", "Atom(C.6: C3,C5,C7,H)", "Atom(C.7: C2,C6,C8,H)", "Atom(C.8: C1,C5,C7,H)"]
                ), (
                "cubane - two branches", vec![4,4], vec! [(1,1,4,1), (1,2,4,2), (1,1,1,2), (2,1,2,2), (3,1,3,2), (4,1,4,2)], "C8H8", 104.,
                vec!["Atom(C.1: C2,C4,C5,H)", "Atom(C.2: C1,C3,C6,H)", "Atom(C.3: C2,C4,C7,H)", "Atom(C.4: C1,C3,C8,H)", "Atom(C.5: C1,C6,C8,H)", "Atom(C.6: C2,C5,C7,H)", "Atom(C.7: C3,C6,C8,H)", "Atom(C.8: C4,C5,C7,H)"]
                ), (
                "benzene: double bonds",vec! [2,2,2], vec! [(1,1,2,1), (1,2,2,2), (1,3,2,3), (2,1,1,2), (2,2,1,3), (2,3,1,1)], "C6H6", 78.,
                vec!["Atom(C.1: C2,C2,C6,H)", "Atom(C.2: C1,C1,C3,H)", "Atom(C.3: C2,C4,C4,H)", "Atom(C.4: C3,C3,C5,H)", "Atom(C.5: C4,C6,C6,H)", "Atom(C.6: C1,C5,C5,H)"]
                ), (
                "acetylene: triple bonds", vec![2], vec![(1,1,2,1), (1,1,2,1)], "C2H2", 26.,
                vec! ["Atom(C.1: C2,C2,C2,H)", "Atom(C.2: C1,C1,C1,H)"]
                ) ];

            //println!("Create carbohydrates and bond them correctly (id tracking, raw formula and molecular weight tested)");

            for it in &carbon {
                let mut m = Molecule::from(it.0);
                m.branch(&it.1);
                m.bond(&it.2);
                m.close();

                chem_assert!(m.formula().unwrap(), should be, it.3, "Testing raw formula");
                chem_assert!(atom_strs(&m, false), should all be, &it.5, "Checking non-hydrogen bonds");
            }
        }
    }
    pub mod mutations_and_carbohydrates {
        use super::*;

        pub fn mutation_tests () {
            let mutation =  [
                ( "Furane: no additional hydrogens while closing after mutation",
                  vec! [5], vec![(5,1,1,1), (5,1,4,1), (2,1,3,1)], vec![(1,1,O)], "C4H4O", 68.,
                  vec!["Atom(O.1: C2,C5)", "Atom(C.2: C3,C3,O1,H)", "Atom(C.3: C2,C2,C4,H)", "Atom(C.4: C3,C5,C5,H)", "Atom(C.5: C4,C4,O1,H)"]
                ),
                ( "isopropylmagnesium bromide",
                  vec![4, 1], vec![(2,1,1,2)], vec![(3,1,Mg), (4,1,Br)], "C3H7BrMg", 147.3,
                  vec! ["Atom(C.1: C2,H,H,H)", "Atom(C.2: C1,C5,Mg3,H)", "Atom(Mg.3: C2,Br4)", "Atom(Br.4: Mg3)", "Atom(C.5: C2,H,H,H)"]
                )
            ];


            for it in mutation {
                let mut m = Molecule::from(it.0);

                m.branch(&it.1);
                m.bond(&it.2);
                m.mutate(&it.3);
                m.close();

                chem_assert!(m.formula().unwrap(), should be, it.4, "Testing raw formula");
                chem_assert!(atom_strs(&m, false), should all be, it.6, "Checking non-hydrogen bonds");
            }
        }
    }
    pub mod mutation_then_additions {
        use std::iter::repeat;

        use super::*;

        macro_rules! add_tests {
            ($( ( $test_name:ident, $neigh:expr ) ),+) => {
                $(
                    fn $test_name() {
                        let neigh = $neigh;
                        println!("Check all possible mutations and correct behavior of 'add' on the mutated atom: {}", neigh);
                        let m = mol!(branch(&[1]), mutate(&[(1, 1, neigh)]), close());
                        let mut raw_el_counts = std::collections::BTreeMap::new();
                        raw_el_counts.insert(neigh, 1);
                        let e = raw_el_counts.entry(H).or_default();
                        *e += valence(&neigh);
                        let expected_form: String = raw_el_counts.iter()
                            .map(|(e, i)| if *i > 1 { format!("{e}{i}") } else { format!("{e}") })
                            .collect();
                        let expected_mm = raw_el_counts.into_iter()
                            .map(|(e, i)| weight(&e) * (i as f32))
                            .sum::<f32>();

                        chem_assert!(m.formula().unwrap(), should be, expected_form, "Testing raw formula after mutation");
                        assert_float_eq!(m.molecular_weight().unwrap(), expected_mm, abs <= 0.00001, "Testing molecular weight");

                        let lst: Vec<_> = repeat((1, 1, Br)).take(valence(&neigh)).collect();
                        let m = mol!(branch(&[1]), mutate(&[(1, 1, neigh)]), add(lst.as_slice()), close());
                        let expected_form = match neigh {
                            H => "HBr".to_owned(),
                            O => "OBr2".to_owned(),
                            B => "BBr3".to_owned(),
                            Br => "Br2".to_owned(),
                            _ => expected_form.replace("H", "Br"),
                        };
                        chem_assert!(m.formula().unwrap(), should be, expected_form, "Testing raw formula after adding Br");
                    }
                )+
            };
        }

        add_tests! {
            (test_c, C),
            (test_h, H),
            (test_o, O),
            (test_b, B),
            (test_br, Br),
            (test_cl, Cl),
            (test_f, F),
            (test_mg, Mg),
            (test_n, N),
            (test_p, P),
            (test_s, S)
        }

        macro_rules! chain_adding_tests {
            ($( ( $test_name:ident, $name:expr, $branch:expr, $add_ch:expr, $formula:expr, $mm:expr, $carbToStr:expr ) ),+) => {
                $(
                    fn $test_name() {
                        println!("Check correct behavior of 'add_chaining' with {}", $name);
                        let (a, b, els) = $add_ch;
                        let m = mol!($name, branch(&$branch), add_chain(a, b, &els), close());

                        chem_assert!(m.formula().unwrap(), should be, $formula, "Testing raw formula");
                        assert_float_eq!(m.molecular_weight().unwrap(), $mm, abs <= 0.00001, "Testing molecular weight");
                        chem_assert!(atom_strs(&m, false), should all be, $carbToStr, "Checking non-hydrogen bonds");
                    }
                )+
            };
        }

        pub fn chain_adding () {

            chain_adding_tests! {
                (
                    adding_chain,
                    "isopropylmagnesium bromide - adding chain",
                    [3],
                    (2,1, [Mg, Br]),
                    "C3H7BrMg",
                    147.3,
                    ["Atom(C.1: C2,H,H,H)", "Atom(C.2: C1,C3,Mg4,H)", "Atom(C.3: C2,H,H,H)", "Atom(Mg.4: C2,Br5)", "Atom(Br.5: Mg4)"]
                )
            }
        }
    }

    fn chainable_builder_methods() -> ChemResult<()> {
        Molecule::from("")
            .branch(&[2])?
            .bond(&[(1, 1, 2, 1)])?
            .mutate(&[(1, 1, S)])?
            .add(&[(2, 1, Cl)])?
            .add_chain(2, 1, &[Cl])?
            .close()?
            .unlock()
            .map(|_| ())
    }

    pub mod failure {
        use super::*;

        pub mod basic_invalid_builds {
            use super::*;

            macro_rules! basic_failure_tests {
                ($( ( $test_name:ident, $message:expr, $branch:expr, $bonds:expr ) ),+) => {
                    $(
                        fn $test_name() {
                            chem_assert!(
                                mol_safe!(branch(&$branch), bond(&$bonds), close()),
                                should err with, ChemError::InvalidBond,
                                $message
                            );
                        }
                    )+
                };
            }

            pub fn run() {

                basic_failure_tests! {
                    (
                        invalid_self_bonding,
                        "No self-bonding",
                        [6],
                        [(1,1,1,1)]
                    ),
                    (
                        exceeding_valence_with_adding,
                        "Should fail when exceeding the valence number adding new alkyls to the same atom",
                        [3,1,1,1],
                        [(2,1,1,2), (2,1,1,3), (2,1,1,4)]
                    ),
                    (
                        exceeding_valence_with_bonding,
                        "Should fail when exceeding the valence number with multiple bonds",
                        [4],
                        [(2,1,3,1), (2,1,3,1), (2,1,3,1)]
                    )
                }
            }
        }

        pub mod invalid_mutation_and_addition {
            use super::*;

            /// the construction should succeed through the intermediate functions,
            /// but then fail with InvalidBond at final
            macro_rules! failure_tests {
                (
                    $(
                        ( $test_name:ident
                          , $message:expr
                          , $branch:expr
                          , $bonds:expr
                          $(, $intermediate:ident ( $i_arg:expr ) )*
                          =>
                          $final:ident ( $($f_arg:expr),* )
                        )
                    ),+
                ) => {
                    $(
                        fn $test_name() {
                            // note: this test only makes sense if the builder methods are (&mut self) -> &mut Self
                            // otherwise rust's ownership rules guarantee a chain of (self) -> Self would satisfy
                            // this test.
                            // note: std::command::Command uses the builder pattern with mutable borrows, feels
                            // like a good reason to use this pattern here.
                            println!($message);
                            let mut m = mol!(branch(&$branch), bond(&$bonds) $(, $intermediate($i_arg) )*);
                            let expected = atom_strs(&m, false);
                            chem_assert!(m.$final($($f_arg),*), should err with, ChemError::InvalidBond, format!("Failed at final step: {}", stringify!($final)));
                            chem_assert!(atom_strs(&m, false), should be, expected);
                        }
                    )+
                };
            }

            pub fn run() {

                failure_tests! {
                    (
                        mutate_full_carbon_1,
                        "Should fail when mutating a carbon with three atoms already linked to an oxygen",
                        [3,1],
                        [(2,1,1,2)]
                        =>
                        mutate(&[(2,1,O)])
                    ),
                    (
                        mutate_full_carbon_2,
                        "Should fail when mutating a carbon with two double bonds to nitrogen",
                        [3],
                        [(1,1,2,1), (3,1,2,1)]
                        =>
                        mutate(&[(2,1,N)])
                    ),
                    (
                        add_full_carbon,
                        "Should fail when adding a new hydrogen to a carbon with already 4 bonds",
                        [3],
                        [(1,1,2,1), (3,1,2,1)]
                        =>
                        add(&[(2,1,H)])
                    ),
                    (
                        overfilling_fails_after_mutating_element,
                        "Should fail when mutating an atom and then adding too many atoms on it",
                        [3],
                        [(1,1,2,1)],
                        mutate(&[(2,1,N)])
                        =>
                        add(&[(2,1,O)])
                    ),
                    (
                        chaining_monovalent_atom,
                        "Should fail when chaining atoms after any monovalent atom",
                        [3],
                        []
                        =>
                        add_chain(2, 1, &[C,C,F,H])
                    )
                }
            }
        }
    }

}
