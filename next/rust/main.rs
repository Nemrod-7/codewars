
extern crate rand;

fn rnd_walk (hist: &Vec<usize>, val: usize) -> usize {

    let mut idx:Vec<usize> = Vec::new();
    let rn = rand::random::<usize>();

    for pos in 0..hist.len() {
        if hist[pos] == val {
            idx.push(pos)
        }
    }
    idx[rn % idx.len()]
}
fn reset (board: &Vec<usize>, rows: &Vec<usize>, d1: &Vec<usize>, d2: &Vec<usize>, size: usize) {

}
fn min_conflict (size: usize, pos: (usize,usize)) {

    let mut board = vec![0; size];
    let mut hist = vec![0; size];

    ///////Qdata///////
    let rows = vec![0; size];
    let d1 = vec![0; 2 * size - 1];
    let d2 = vec![0; 2 * size - 1];

    board[pos.0] = pos.1;


    let mut cnt = 0;
    let mut sum = 0;
    let mut val = 0;

    for x in 0..size {

        if x == pos.0 {
            cnt = 0;
        } else {
            let y = board[x];
            cnt = rows[y] + d1[size + x - y - 1] + d2[x + y];
        }

        hist[x] = cnt;
        val = std::cmp::max (val,cnt);
        sum += cnt;
    }

    let x = rnd_walk (&hist, val);
    val = size;

    for y in 0..size {
        cnt = rows[y] + d1[size + x - y - 1] + d2[x + y];

        val = std::cmp::min (val,cnt);
        hist[y] = cnt;
    }
    board[x] = rnd_walk (&hist, val);



}
fn solveNQueens (size: usize, dot: (usize,usize)) {

}
fn main() {

    let size = 8;
    let pos = (2,3);

    let mut board = vec![7,4,3,4,0,1,0,5];

    for i in 0..size { board[i] = 0 }

    print!("{:?}", board);


    print!("\nend\n");
}
