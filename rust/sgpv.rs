#![allow(unused)]

fn tochar(nu: usize) -> char { (nu + 65) as u8 as char }
fn showarr(ve: &Vec<usize>) {
    for it in  ve {
        print!("{}", tochar(*it));
    }
    print!("\n");
}

fn mkgraph(line: &Vec<Vec<usize>>) -> Vec<Vec<usize>> {

    let n = line.len() * line[0].len();
    let mut graph = vec![vec![0;n];n]; 

    for i in 0..line.len() {
        for j in 0..line[i].len() {
            let cell = line[i][j];
            graph[cell][cell] = 1;

            for k in j + 1..line[i].len() {
                let a = line[i][j];
                let b = line[i][k];
                graph[a][b] = 1;
                graph[b][a] = 1;
            }
        }
    }

    graph
}

fn backup (n:usize, g:usize, d:usize) {
    let mut cube = vec![vec![vec![0;g]; n / g]; d];

    for i in 0..n {
        cube[0][i / g][i % g] = i ;
    }

    let mut graph = mkgraph(&cube[0]);

    let mut visit = vec![false;n];
    let mut cluster:Vec<usize> = Vec::new();
    let mut line:Vec<Vec<usize>> = Vec::new();

    for _ in 0..4 {
        for i in 0..n {
            if visit[i] == false && cluster.len() < g {
                if cluster.iter().find(|&x| graph[*x][i] == 1) == None {
                    cluster.push(i);
                }
            } 
        }

        if cluster.len() == g {
            for i in 0..cluster.len() {
                visit[cluster[i]] = true;

                for j in (i+1)..cluster.len() {
                    graph[cluster[i]][cluster[j]] = 1;
                    graph[cluster[j]][cluster[i]] = 1;
                }
            }

            line.push(cluster.clone());
            cluster.clear();
        }
    }


    print!("\n");
    for i in 0..line.len() {
        for j in 0..line[i].len() {
            print!("{}", tochar(line[i][j]));
        }

        print!(" ");
    }
}


fn validator (n:usize, g:usize, d:usize) {
    let mut cube = vec![vec![vec![0;g]; n / g]; d];

    for i in 0..n {
        cube[0][i / g][i % g] = i ;
    }

    let mut graph = mkgraph(&cube[0]);

    let mut stack = vec![ (
            0, 
            vec![vec![0;g]; n / g],
            vec![false;n],
            graph
    )];

    struct Vertex {
        index: usize,
        comb : Vec<Vec<usize>>,
        visit: Vec<usize>,
        hist : Vec<Vec<usize>>,
    }

    while let Some((index, mut cluster, visit, hist)) = stack.pop() {
        let x = index / g;
        let y = index % g;

        //print!("{x} {y} \n");
        if index == n  {
            for i in 0..cluster.len() {
                for j in 0..cluster[i].len() {
                    print!("{}", tochar(cluster[i][j]));
                }
                print!(" ");
            }
            print!("\n");
        } else {
            for num in 0..n {
                let mut valid = true;

                if visit[num] == false {
                    
                    for k in 0..y {
                        if hist[cluster[x][k]][num] == 1 {
                            valid = false;
                        }
                    }

                    if valid == true {
                        let mut nxc = cluster.to_vec();
                        let mut nxv = visit.to_vec();
                        let mut nxh = hist.to_vec();

                        nxv[num] = true; 
                        nxc[x][y] = num;

                        for k in 0..y {
                            nxh[cluster[x][k]][num] = 1;
                            nxh[num][cluster[x][k]] = 1;
                        }

                        stack.push((index + 1, nxc, nxv, nxh));
                    }
                }
            }
        }
    }

}

fn main () {

    [ "ABCD","EFGH","IJKL","MNOP","QRST" ];


    //validator(20,4,5);
}
