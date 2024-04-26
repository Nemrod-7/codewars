#![allow(unused)]

fn show(curr: &Vec<Vec<u32>>) {

    for i in 0..curr.len() {
        for j in 0..curr[i].len() {
            print!("{} ", curr[i][j]);
        }
        print!("\n");
    }
    print!("\n");
}

fn rotation(curr: &Vec<Vec<u32>>) -> Vec<Vec<u32>> {
    let s2 = curr[0].len();
    let mut next = vec![vec![0; s2]; curr.len()];

    next[0][0] = curr[0][0];
    next[1][s2-1] = curr[0][s2-1];

    if s2 > 1 {
        next[0][1] = curr[1][0];
        next[1][0] = curr[1][1];
    }

    for i in 1..s2-1 {
        next[0][i+1] = curr[0][i];
        next[1][i] = curr[1][i+1];
    }

    next
}

fn build_matches_table (size: u32) -> Vec<Vec<(u32,u32)>>  {
    let size = size as usize;
    let s2 = size / 2 ;
    let mut tour: Vec<Vec<(u32,u32)>> = Vec::new();
    let mut curr:Vec<Vec<u32>> = vec![vec![0; s2]; 2];

    if size % 2 != 0 { return tour; }

    for i in 0..size {
        curr[i % 2][i / 2] = i as u32 + 1;
     }

    for _ in 0..(size - 1) {
        let mut round = Vec::new();

        for i in 0..s2 {
            round.push((curr[0][i] , curr[1][i] ));
        }

        tour.push(round);
        curr = rotation(&curr);
    }
    tour    
}

fn main () {


    build_matches_table(4);
}

