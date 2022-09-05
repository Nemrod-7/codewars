
fn display (skyscr: &Vec<Vec<usize>>, floor:usize) {

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
        if floor as usize == pos {
            print!("|<-\n");
        } else {
            print!("|\n");
        }
        if pos == 0 { break } else { pos -= 1 }
    }

}

fn progress (mat: &Vec<Vec<usize>>, lift: &Vec<usize>) -> bool {

    if lift.len() > 0 { return true }

    for floor in 0..mat.len() {
        for user in 0..mat[floor].len() {
            if mat[floor][user] != floor {
                return true
            }
        }
    }

    false
}
fn valid (user:usize, floor:usize, top: usize, direct:i32) ->bool {
    if user > floor && (direct == 1 || floor == 0) { return true }
    if user < floor && (direct == -1 || floor == top) { return true }
    false
}
fn the_lift (queue: &[Vec<u32>], capacity:usize) -> Vec<u32> {

    let top = queue.len() - 1;

    let mut floor = 0;
    let mut direct:i32 = 1;

    let mut lift = vec![0;0];
    let mut record = vec![0,1];
    let mut tower:Vec<Vec<u32>> = queue.clone().to_vec();


/*
    while progress (&tower, &lift) {

        let mut visited = false;

        if let Some(pos) = lift.iter().position(|x| *x == floor) { // disembark
            lift.remove(pos);
            visited = true;
        }

        let mut user = 0;
        while user != tower[floor].len() {                        // embark
            let embark = valid (tower[floor][user], floor, top, direct);

            if embark { visited = true }
            if embark && lift.len() < capacity {
                lift.push(tower[floor][user]);
                tower[floor].remove(user);
            } else {
                user += 1;
            }
        }

        if visited && *record.last().unwrap() != floor {
            record.push(floor)
        }
        //display (&tower, floor);
        print!("lift : {:?}\n", lift);
        if direct == 1 { floor += 1 } else { floor -= 1 }
        if floor == 0|| floor == top {
            direct = -direct
        }
    }
*/
    record
}

fn main() {

    let floor = 0;
    let mut lift = vec![1,2,0,5];

    let mut i = 0;

    let mut visited = false;
    let board:bool;

    let queue = vec![vec![], vec![], vec![5,2,5], vec![], vec![], vec![], vec![]] ;
    the_lift (&[vec![],vec![0],vec![],vec![],vec![2],vec![3],vec![]], 5);
    //the_lift (&queue, 5);

    print! ("\n");

}
