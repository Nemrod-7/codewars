extern crate rand;

fn reset (board: &Vec<usize>, rows: &mut Vec<usize>, d1: &mut Vec<usize>, d2: &mut Vec<usize>, n: usize) {

    let diagsz = 2 * n - 1;

    for i in 0..n {
        rows[i] = 0
    }
    for i in 0..diagsz {
        d1[i] = 0;
        d2[i] = 0;
    }

    for i in 0..n {

        rows[board[i]] += 1;
        d1[n + i - board[i] - 1] += 1;
        d2[i + board[i]] += 1;
    }
}
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
fn min_conflict (size: usize, pos: (usize,usize)) -> Option<Vec<usize>> {

    let mut board = vec![0; size];

    let mut rows = vec![0; size];
    let mut d1 = vec![0; 2 * size - 1];
    let mut d2 = vec![0; 2 * size - 1];

    board[pos.0] = pos.1;
    reset (&board, &mut rows, &mut d1, &mut d2, size);

    let mut iter = size * 50;

    while iter > 0 {

        let mut hist = vec![0; size];
        let mut sum = 0;
        let mut val = 0;
        let mut cnt;

        for x in 0..size {

            if x == pos.0 {
                cnt = 0;
            } else {
                let y = board[x];
                cnt = rows[y] + d1[size + x - y - 1] + d2[x + y] - 3;
            }

            hist[x] = cnt;
            val = std::cmp::max (val,cnt) ;
            sum += cnt;
        }

        if sum == 0 { return Some (board) }
        let x = rnd_walk (&hist, val);
        val = size;

        for y in 0..size {
            cnt = rows[y] + d1[size + x - y - 1] + d2[x + y];

            val = std::cmp::min (val,cnt);
            hist[y] = cnt;
        }

        board[x] = rnd_walk (&hist, val);
        reset (&board, &mut rows, &mut d1, &mut d2, size);
        iter -= 1;
    }

    None
}
pub fn solve_n_queens (size: usize, pos: (usize, usize)) -> Option<String> {

    if size == 1 { return Some ("Q\n".to_string()); }
    if size <= 3 { return None }

    for _cycle in 0..1 {
        if let Some (board) = min_conflict (size, pos) {

            let mut os = String::new();

            for y in 0..size {
                for x in 0..size {
                    if board[x] == y { os.push ('Q') } else { os.push ('.') }
                }
                os.push ('\n')
            }
            return Some (os);
        }
    }

    None
}

fn main() {

    print!("\nend\n");
}
