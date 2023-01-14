
fn search (tri: &Vec<Vec<char>>, letter: char) -> bool {

	for i in 0..tri.len() {
        let end = tri[i].len();
        if end > 0 {
            for j in 0..end-1 {
                if tri[i][j] == letter {
                    return true;
                }
            }
        }
	}
	return false;
}
fn recover_secret  (tri: Vec<[char; 3]>) -> String {

    let size = tri.len();
    let mut pass = String::new();
    let mut tri:Vec<Vec<char>> = tri.iter().map(|x| x.to_vec()).collect();

    loop {
        let mut running = false;

        for i in 0..size {
            let levsz = tri[i].len();

            if levsz > 0 {
                let last = tri[i][levsz - 1];
                running = true;

                if search (&tri, last) == false {
                    pass.push(last);

                    for k in 0..size {
                        if let Some (val) = tri[k].last() {
                            if val == &last {
                                tri[k].pop();
                            }
                        }
                    }
                    break;
                }

            }
        }
        if running == false {
            break;
        }
    }

    pass.chars().rev().collect::<String>()
}
fn _display (tri: &Vec<Vec<char>>) {

    for i in 0..tri.len() {
        let end = tri[i].len();
        print!("[");
        for j in 0..end {
            print!("{}", tri[i][j]);
        }
        print!("]\n");
    }
    print! ("\n");
}

fn main () {

/*
let pass = recover_secret(vec![
['t','u','p'],
['w','h','i'],
['t','s','u'],
['a','t','s'],
['h','a','p'],
['t','i','s'],
['w','h','s']]);
*/


let pass = recover_secret (vec![
['t','s','f'],
['a','s','u'],
['m','a','f'],
['a','i','n'],
['s','u','n'],
['m','f','u'],
['a','t','h'],
['t','h','i'],
['h','i','f'],
['m','h','f'],
['a','u','n'],
['m','a','t'],
['f','u','n'],
['h','s','n'],
['a','i','s'],
['m','s','n'],
['m','s','u']
]);

print!("{pass}\n");


/*
assert_eq!(recover_secret(vec![ ['t','u','p'], ['w','h','i'], ['t','s','u'], ['a','t','s'], ['h','a','p'], ['t','i','s'], ['w','h','s']]) , "whatisup");
    t u p
    t s u p
    a t s u p
    a t i s u p
    h a t i s u p
    w h a t i s u p
*/

}
