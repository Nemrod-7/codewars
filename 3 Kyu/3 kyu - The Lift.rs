
fn display (skyscr: &Vec<Vec<u32>>, lift:&Vec<u32>, floor:usize) {

    let mut pos = skyscr.len() - 1;
    let maxv = skyscr.iter().max().unwrap().len();

    print!("\n.");
    for i in 0..maxv {
        print!("_");
    }
    print!(".\n");

    loop {
        print!("|");
        for i in 0..maxv {
            if skyscr[pos].len() > i && skyscr[pos][i] > 0 {
                print! ("{}",skyscr[pos][i]);
            } else {
                print!("_");
            }
        }
        if floor == pos {
            print!("|<-|");
            for pp in lift {
                print! ("{}",pp);
            }
        }
        print!("|\n");
        if pos == 0 { break } else { pos -= 1 }
    }

}

fn progress (mat: &Vec<Vec<u32>>, lift: &Vec<u32>) -> bool {

    if lift.len() > 0 { return true }

    for floor in 0..mat.len() {
        for user in 0..mat[floor].len() {
            if mat[floor][user] != floor as u32 {
                return true
            }
        }
    }

    false
}
fn valid (user:usize, floor:usize, top: usize, direct:i32) ->bool {
    if user > floor && (direct ==  1 || floor ==  0 ) { return true }
    if user < floor && (direct == -1 || floor == top) { return true }
    false
}
fn getlast (record: &Vec<u32>, floor:usize) -> bool {
    *record.last().unwrap() != floor as u32
}
fn the_lift (queue: &[Vec<u32>], capacity:u32) -> Vec<u32> {

    let top = queue.len() - 1;
    let capacity = capacity as usize;
    let mut floor:usize = 0;
    let mut direct:i32 = 1;

    let mut record = vec![0;1];
    let mut lift:Vec<u32> = vec![0;0];
    let mut tower:Vec<Vec<u32>> = queue.to_vec();

    while progress (&tower, &lift) {

        let mut visited = false;

        while let Some(pos) = lift.iter().position(|x| *x == floor as u32) { // disembark
            lift.remove(pos);
            visited = true;
        }

        while let Some(pos) = tower[floor].iter().position(|x| valid (*x as usize, floor, top, direct)) { // disembark
            visited = true;

            if lift.len() < capacity {
                lift.push(tower[floor][pos]);
                tower[floor].remove(pos);
            } else {
                break
            }
        }

        if visited && getlast (&record, floor) {
            record.push(floor as u32)
        }

        if direct == 1 {
            floor += 1
        } else {
            floor -= 1
        }
        if floor == 0|| floor == top {
            direct = -direct
        }

    }

    if getlast (&record, 0) { record.push(0) }

    record
}

fn main() {

    let queue = vec![vec![], vec![], vec![5,2,5], vec![], vec![], vec![], vec![]] ;

    print! ("end\n");
}


/*
let mut user = 0;
while user != tower[floor].len() {          // embark
    let go = tower[floor][user] as usize;

    let embark = valid (go, floor, top, direct);

    if embark { visited = true }
    if embark && lift.len() < capacity {
        lift.push(tower[floor][user]);
        tower[floor].remove(user);
    } else {
        user += 1;
    }
}
*/
