use std::collections::LinkedList;


fn recover_secret(tri: Vec<[char; 3]>) -> String {

    let size = tri.len();
    let mut visit = vec![0;size];
    let mut list = LinkedList::from([tri[0], tri[1], tri[2]]);
    
    visit[0] = 1;

    


    return "";
}

fn main () {

assert_ eq!(recover_secret(vec![ ['t','u','p'], ['w','h','i'], ['t','s','u'], ['a','t','s'], ['h','a','p'], ['t','i','s'], ['w','h','s']]) , "whatisup");

/*
    t u p
    t s u p
    a t s u p
    a t i s u p
    h a t i s u p
    w h a t i s u p
*/

}
