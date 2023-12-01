#![allow(dead_code, unused)]

mod flux_puzzle {
///////////// display component/////////////////////
    fn showgraph (graph: &Vec<Vec<(usize,Vec<String>)>>) {
        print!("\n");
        for y in 0..graph.len() {
            for x in 0..graph[y].len() {
                let cell = graph[y][x].0;
                if cell == 99 {
                    print!("[  ]");
                } else {
                    print!("[{:2}]", &graph[y][x].0);
                }
            }
            print!("\n");
        }
    }
    pub fn showgrid (grid: &Vec<Vec<i8>>) {

        for y in 0..grid.len() {
            for x in 0..grid[y].len() {
                print!("[{:2}]", grid[y][x]);
            }
            print!("\n");
        }
        print!("\n");
    }
///////////////////////////////////////////////////
    fn rotate (grid: &mut Vec<Vec<i8>>) {
        let height = grid.len();
        let width = grid[0].len();

        for y in 0..height {
            for x in 0..width {
                let cell = grid[y][x];

                if cell > 0 && cell < 15 {
                    if cell < 8 {
                        grid[y][x] *= 2;
                    } else {
                        grid[y][x] -= 15 - grid[y][x];
                    }
                }
            }
        }
    }
    fn update (grid: &Vec<Vec<i8>>, graph: &mut Vec<Vec<(usize,Vec<String>)>>, cycle: usize) -> bool {
        let alp:Vec<&str> = vec!["E","S","W","N"];
        let dir:Vec<(i32,i32)> = vec![(1,0),(0,1),(-1,0),(0,-1)];
        let height = grid.len();
        let width = grid[0].len();
        let mut stack = Vec::new();

        for y in 0..height {
            for x in 0..width {
                if graph[y][x].0 <= cycle {
                    stack.push((x,y));
                    graph[y][x].1.push("".to_string());
                }
            }
        }

        while let Some(pt) = stack.pop() {
            let cell = grid[pt.1][pt.0];
            let path =  graph[pt.1][pt.0].1.to_vec();

            if cell == -2 { return true; }

            for i in 0..4 {
                let nx = pt.0 as i32 + dir[i].0;
                let ny = pt.1 as i32 + dir[i].1;

                if nx >= 0 && nx < width as i32 && ny >= 0 && ny < height as i32 {
                    let nxc = grid[ny as usize][nx as usize];
                    let node = &mut graph[ny as usize][nx as usize];

                    let nxt = if nxc < 0 { 0 } else { nxc >> ((i + 2) % 4)&1 };
                    let out = if cell < 0 { 0 } else { cell >> i&1 };

                    if out == 0 && nxt == 0 && node.0 == 99 {
                        node.0 = cycle + 1;
                        node.1 = path.to_vec();

                        if let Some(nxdir) = node.1.last_mut() {
                            *nxdir += &format!("{}", alp[i]);
                        }
                        stack.push((nx as usize, ny as usize));
                    }
                }
            }
        }

        false
    }
    pub fn maze_solver (grid: &Vec<Vec<i8>>) -> Option<Vec<String>> {
        let height = grid.len();
        let width = grid[0].len();
        let mut exit = (0,0);
        let mut graph = vec![vec![(99,vec![]);width];height];
        let mut grid = grid.clone();

        for y in 0..height {
            for x in 0..width {
                match grid[y][x] {
                    -1 => graph[y][x].0 = 0,
                    -2 => exit = (x,y),
                     _ => (),
                }
            }
        }

        for cycle in 0..50 {
            if update (&grid, &mut graph, cycle) == true {
                return Some(graph[exit.1][exit.0].1.clone());
            }
            rotate (&mut grid);
        }

        None
    }
}


fn main () {

    let grid = vec![ vec![6,3,10,4,11], vec![8,10,4,8,5], vec![-1,14,11,3,-2], vec![15,3,4,14,15], vec![14,7,15,5,5]];

    let grid = vec![ 
        vec![ 8, 2, 7,15, 8,14,14, 1, 0], vec![ 0, 3, 6,11,14,14, 6,10, 1], vec![ 1,13, 8, 4,11, 1,13, 2, 0], vec![ 4, 0, 8,10, 0, 7, 2,15,14],
        vec![11, 7, 3, 2, 8, 2,15,14, 6], vec![-1, 7, 5,14,13,10, 8,13,-2], vec![ 7, 9, 3,12, 8,10,12,14, 7], vec![13,15,11, 6, 6,15, 1,14, 0],
        vec![14,10, 0, 8,13,13, 9, 7, 9], vec![ 6, 1,10, 1, 1, 0,13,13,12], vec![ 3, 9, 6, 9, 1,12, 5,11,10]];

        let grid = vec![
            vec![ 9,13, 0, 4, 4, 6, 5,15,12, 1,13,11,15, 6, 8,10,15, 7, 3, 2, 2,-2],
            vec![10, 4,15, 1,14, 0,11,15, 3,14, 9,10,15,14,12,13,14, 9, 1, 1, 8,12],
            vec![15, 3, 5, 0, 2, 5,13,12, 5, 9, 7, 3, 2, 4, 0,13, 3, 9,13, 8, 2, 3],
            vec![ 5,15,13, 4,13,14, 6, 0,15,11,11, 1, 0, 6,14, 4, 0, 6,11,13,11,12],
            vec![ 2, 0,11,13, 3, 0,13, 7, 3,12, 0,13,13, 2, 6, 8,12,14, 9,12,15, 4],
            vec![-1, 4, 2, 0,11,14,12,11, 8, 3, 0, 5, 4,12, 8,11,10, 1, 1, 9, 0,15],
            vec![10,11,14,12,14, 9,15,15,12, 6, 7, 9, 9,14, 2, 0, 0,14,15,12, 8, 6],
            vec![14,12, 5, 5,15,12, 1,13,11, 8, 3,14, 1, 7,13, 4, 8, 2, 8,11, 6, 4],
            vec![11, 9, 0, 6, 8, 9, 7, 4, 9, 3, 5, 9, 9,14,13, 2, 7, 4,15, 7,10,11],
            vec![13,14,11, 0,14, 6, 6, 3,14,15,11, 2,13,13,10, 6,11, 8,11, 7,11, 9],
            vec![ 7, 7, 8, 7,12,15, 8, 1, 6, 5, 7, 9, 7, 0,10, 4, 9, 4, 2, 9,15, 3],
            vec![ 6, 4,15, 0, 0, 8, 5, 1, 4,11, 2, 7, 8,14,15,15, 4, 2, 0, 1,14, 6],
            vec![14,10, 9, 2, 9,15, 8,11,15,12, 1, 9,13, 7, 5, 5, 0, 5,12,11,12,11],
            vec![ 3, 7, 9, 9, 3, 1, 5, 8,14,13, 6,11, 3, 5, 0,13, 8, 1, 3,14,10,13],
            vec![14, 5, 1, 1,13, 8, 9, 9, 6, 7,12, 2,12, 9, 6,13,12,15, 6, 4, 6,11],
            vec![14, 2,10,15, 2, 3,15,10, 6,13,13,11,14, 9,11, 9,11, 1,12, 0,14, 1],
            vec![ 0, 4,13,13,13,13, 1, 8, 0, 9,13, 6,13, 9,11, 5,15, 1, 3,15,14, 1],
            vec![ 0, 6,14, 2,13, 6, 1,12,12, 9, 2, 9, 9, 1, 7, 3, 4, 3, 4, 9, 8, 2],
            vec![ 5, 6,13,14, 1, 4, 1,11, 6,11, 3, 3, 2, 6,10, 4,12,15,13, 1,11,10]];


            if let Some(path) = flux_puzzle::maze_solver(&grid) {
                print!("{:?}\n", path);
            } else {
                print!("\nnot found\n");
            }


            print!("\nend\n");
}

fn example_tests() {

    let example_tests = vec![
        ( vec![ vec![4,2,5,4], vec![4,15,11,1], vec![-1,9,6,8], vec![12,7,7,-2], ], Some(vec!["NNE", "EE", "S", "SS"])),
        ( vec![ vec![6,3,10,4,11], vec![8,10,4,8,5], vec![-1,14,11,3,-2], vec![15,3,4,14,15], vec![14,7,15,5,5], ], Some(vec!["", "", "E", "", "E", "NESE"])),
        ( vec![ vec![9,1,9,0,13,0], vec![14,1,11,2,11,4], vec![-1,2,11,0,0,15], vec![4,3,9,6,3,-2], ], Some(vec!["E", "SE", "", "E", "E", "E"])),
        ( vec![ vec![-1,6,12,15,11], vec![8,7,15,7,10], vec![13,7,13,15,-2], vec![11,10,8,1,3], vec![12,6,9,14,7], ], None),
        ( vec![ vec![6,3,0,9,14,13,14], vec![-1,14,9,11,15,14,15], vec![2,15,0,12,6,15,-2], vec![4,10,7,6,15,5,3], vec![7,3,13,13,14,7,0], ], None),
    ];

    example_tests.iter().for_each(|(maze,sol)| {
        let refsol = sol.as_ref().map(|r| r.iter().map(|&s| String::from(s)).collect());
        test_helper::run_test(maze,flux_puzzle::maze_solver(maze),refsol)
    });
}

mod test_helper {
    use std::collections::HashSet;
    use std::collections::HashMap;

    pub fn run_test(r: &Vec<Vec<i8>>,_user: Option<Vec<String>>,_refsol: Option<Vec<String>>) {
        if let Some(user) = _user {
            let refsol = _refsol.unwrap();

            if user.join("").chars().any(|ch| !"WENS".contains(ch)) {
                return assert!(false,"Solution elements must only consist of the following characters: \"NWSE\"");}
            let ref_str = format!("Here is a valid solution:\n{}",sol_str(&refsol));
            let user_str = format!("Here is your solution:\n{}",sol_str(&user));
            if user.len() > refsol.len() {
                return assert!(false,"Your solution completes the task in {} iterations.\nThis test can be completed in {} iterations.\n{}\n{}",user.len(),refsol.len(),ref_str,user_str);}
            let dir_map: HashMap<u8,(i8,i8)> = vec![(78,(-1,0)),(87,(0,-1)),(83,(1,0)),(69,(0,1))].into_iter().collect();
            let dnum: HashMap<u8,usize> = vec![(78,0),(87,1),(83,2),(69,3)].into_iter().collect();
            let dword = ["north","west","south","east"];
            let grid: Vec<Vec<u8>> = r.iter().map(|row| row.iter().map(|&n| n.max(0) as u8).collect()).collect();
            let xl = r.len();
            let yl = r[0].len();
            let (mut px,mut py): (usize,usize) = (0,0);
            let mut dst: (usize,usize) = (0,0);
            for (i,row) in r.iter().enumerate() {
                for (j,cel) in row.iter().enumerate() {
                    if *cel < 0 {
                        if *cel == -1 { px = i; py = j; } else { dst = (i,j); }}}}
            let bad_move = |s: String| assert!(false,"Invalid move: {}\n{}",s,user_str);

            for (i,s) in user.iter().enumerate() {
                let mut visited: HashSet<(usize,usize)> = HashSet::new();
                for (j,b) in s.bytes().enumerate() {
                    let dq: usize = dnum[&b];
                    let (nx,ny): (i8,i8) = dir_map[&b];
                    let pos_str = format!("during move {} at iteration {}.\nLast valid position was [{}, {}].",j,i,px,py);
                    let _qx = nx + px as i8;
                    let _qy = ny + py as i8;
                    if _qx <  0 || _qx >= xl as i8 || _qy < 0 || _qy >= yl as i8 {
                        return bad_move(format!("Out of bounds {}",pos_str));}
                    let qx = _qx as usize;
                    let qy = _qy as usize;
                    let obstructs: Vec<(usize,u8)> = wall_check(grid[px][py],grid[qx][qy],dq,i%4);
                    if !obstructs.is_empty() {
                        let (celln,d) = obstructs[0];
                        let (xx,yy) = if d == 0 { (px,py) } else { (qx,qy) };
                        return bad_move(format!("Path obstructed by a wall on the {} side of [{}, {}] {}",dword[celln],xx,yy,pos_str));}
                    if visited.contains(&(qx,qy)) {
                        return bad_move(format!("Entered cell [{}, {}] a second time",qx,qy));}
                    px = qx;
                    py = qy;
                    visited.insert((qx,qy));}
            }

            if dst != (px,py) {
                return assert!(false,"The ball did not reach the destination. Its last position was [{}, {}]\n{}",px,py,user_str);}

            return assert!(true);
        } else {
            return assert!(_refsol.is_none(),"This puzzle has no solution");
        }
    }

    fn sol_str(r: &Vec<String>) -> String { format!("[ \"{}\" ]",r.join("\", \"")) }
    fn celrot(n: u8) -> u8 { let x = n << 1; x/16 + x%16 }
    fn get_celval(n: u8,c: usize) -> usize { (0..c).into_iter().fold(n, |z,_| celrot(z)) as usize }

    fn wall_check(fro: u8,too: u8,d: usize,c: usize) -> Vec<(usize,u8)> {
        let wall_fro = get_celval(fro,c) & (8 >> d) == 0;
        let wall_too = get_celval(too,c) & (8 >> (d+2)%4) == 0;
        if wall_fro && wall_too { vec![] } else if !wall_fro { vec![(d,0)] } else { vec![((d+2)%4,1)] }
    }
}
