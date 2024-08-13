#![allow(dead_code, unused)]


fn main () {
let  left = vec!["Atom(C.1: H)", "Atom(C.2: C3,Cl42)", "Atom(C.3: C2,C4)", "Atom(C.4: C3,Mg5)", "Atom(Mg.5: C4,C6)", "Atom(C.6: C7,Mg5)", "Atom(C.7: C6,H)", "Atom(C.9: C10)", "Atom(C.10: C9,C11,H)", "Atom(C.11: C10)", "Atom(C.12: C13)", "Atom(C.13: C12)", "Atom(C.14)", "Atom(F.15: C16)", "Atom(C.16: C17,F15)", "Atom(C.17: C16,C18)", "Atom(C.18: C17,C19,H)", "Atom(C.19: C18,C20)", "Atom(C.20: C19,B21)", "Atom(B.21: C20)", "Atom(C.22: C23,C23,P43)", "Atom(C.23: C22,C22,C24)", "Atom(C.24: C23,C25,Cl45)", "Atom(C.25: C24,C26)", "Atom(C.26: C25)", "Atom(C.27: C28,N40)", "Atom(C.28: C27,C29)", "Atom(C.29: C28,N30)", "Atom(N.30: C29,C31)", "Atom(C.31: Mg32,N30,N46)", "Atom(Mg.32: C31,C33)", "Atom(C.33: O41,Mg32)", "Atom(C.34: C35)", "Atom(C.35: C34,C36)", "Atom(C.36: C35,C37,H,B50)", "Atom(C.37: C36,C38)", "Atom(C.38: C37,C39)", "Atom(C.39: C38)", "Atom(N.40: C27)", "Atom(O.41: C33)", "Atom(Cl.42: C2)", "Atom(P.43: C22)", "Atom(Cl.45: C24)", "Atom(N.46: C31)", "Atom(B.50: C36)"];


let  right = vec!["Atom(C.1: H)", "Atom(C.2: C3,Cl42)", "Atom(C.3: C2,C4)", "Atom(C.4: C3,Mg5)", "Atom(Mg.5: C4,C6)", "Atom(C.6: C7,Mg5)", "Atom(C.7: C6,H)", "Atom(C.9: C10)", "Atom(C.10: C9,C11,H)", "Atom(C.11: C10)", "Atom(C.12: C13)", "Atom(C.13: C12)", "Atom(C.14)", "Atom(F.15: C16)", "Atom(C.16: C17,F15)", "Atom(C.17: C16,C18)", "Atom(C.18: C17,C19,H)", "Atom(C.19: C18,C20)", "Atom(C.20: C19,B21)", "Atom(B.21: C20)", "Atom(C.22: C23,C23,P43)", "Atom(C.23: C22,C22,C24)", "Atom(C.24: C23,C25,Cl45)", "Atom(C.25: C24,C26)", "Atom(C.26: C25)", "Atom(C.27: C28,N40)", "Atom(C.28: C27,C29)", "Atom(C.29: C28,N30)", "Atom(N.30: C29,C31)", "Atom(C.31: Mg32,N30,N46)", "Atom(Mg.32: C31,C33)", "Atom(C.33: O41,Mg32)", "Atom(C.34: C35)", "Atom(C.35: C34,C36)", "Atom(C.36: C35,C37,B50,H)", "Atom(C.37: C36,C38)", "Atom(C.38: C37,C39)", "Atom(C.39: C38)", "Atom(N.40: C27)", "Atom(O.41: C33)", "Atom(Cl.42: C2)", "Atom(P.43: C22)", "Atom(Cl.45: C24)", "Atom(N.46: C31)", "Atom(B.50: C36)"];




    print!("{} {}\n", left.len(), right.len());
    let size = std::cmp::min(left.len(), right.len());

    for i in 0..size {

        if left[i] != right[i] {
            print!("{} => {} {}\n",i, left[i], right[i]);
        }
    }


}
