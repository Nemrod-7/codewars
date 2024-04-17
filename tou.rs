#![allow(unused)]

fn build_matches_table (size: usize) {

    let  mut graph = vec![vec![0; size]; size];
    
    for i in (0..size).step_by(2) {
        graph[i][i + 1] = 1;
        graph[i + 1][i] = 1;
    }

    let a = 0;
    let b = 1;

    let mut visit = vec![false;size];

    if !visit[a] && !visit[b] {
        if graph[a][b] == 0 {
            graph[a][b] = 1;
            graph[b][a] = 1;

            let pair = (a,b);
        } else {

        }
    }

    //print!("{:?}", start);


}

fn main () {

    build_matches_table (4);
}
