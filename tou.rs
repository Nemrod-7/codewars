#![allow(unused)]

fn build_matches_table (size: usize) -> Vec<Vec<(u32,u32)>>  {

    let mut graph = vec![vec![false; size]; size];
    let mut tour:Vec<Vec<(u32,u32)>> = Vec::new();

    for i in (0..size).step_by(2) {
        graph[i][i + 1] = true;
        graph[i + 1][i] = true;
        tour.push(vec![(i as u32, (i + 1) as u32)]);
    }

    loop {

        let mut visit = vec![false;size];
        let mut round:Vec<(u32,u32)> = Vec::new(); 

        for i in 0..size {
            for j in 0..size {
                if i != j && !visit[i] && !visit[j] {
                    if graph[i][j] == false {

                        graph[i][j] = true;
                        graph[j][i] = true;
                        visit[i] = true;
                        visit[j] = true;

                        round.push((i as u32, j as u32));
                    } 
                }
            }

        }

        if round.len() == 0 {
            break
        } else {
            print!("{:?}\n", round);
            tour.push(round);
        }
    }
tour    
}

fn main () {

    build_matches_table (4);
}
