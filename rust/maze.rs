fn search () {

}


fn rotate (grid: &mut Vec<Vec<i32>>) {
    let height = grid.len();
    let width = grid[0].len();

    for y in 0..height {
        for x in 0..width {
            let cell = grid[y][x];

            if cell > 0 && cell < 15 {
                if cell < 8 {
                    grid[y][x] *= 2;
                } else {
                    grid[y][x] -= (15 - grid[y][x]);
                }
            }
        }
    }
}
fn maze_solver (grid: &Vec<Vec<i32>>) {

    let height = grid.len();
    let width = grid[0].len();
    let mut stack = vec![(0,0)];
    let mut graph = vec![vec![(0,vec![String::new()]);width];height];
    // graph: &Vec<Vec<(0,String)>>
    for y in 0..height {
        for x in 0..width {
            if grid[y][x] == -1 {
                stack.push((x,y));
            }
        }
    }

    let alp:Vec<&str> = vec!["E","S","W","N"];
    let dir:Vec<(i32,i32)> = vec![(1,0),(0,1),(-1,0),(0,-1)];

    while !stack.is_empty() {
        let pt = stack.pop().unwrap();
        let curr = grid[pt.1][pt.0];
        let path =  graph[pt.1][pt.0].1.to_vec();

        for i in 0..4 {

            let nx = pt.0 as i32 + dir[i].0;
            let ny = pt.1 as i32 + dir[i].1;

            if nx >= 0 && nx < width as i32 && ny >= 0 && ny < height as i32 {
                let nx = nx as usize;
                let ny = ny as usize;

                let nxc = grid[ny][nx];
                let node = &mut graph[ny][nx];
                let out = if curr < 0 { 0 } else { curr >> i&1 };
                let next = if nxc < 0 { 0 } else { nxc >> ((i + 2) % 4)&1 }; 

                if out == 0 && next == 0 && node.0 == 0 {
                    node.0 = 1;
                    node.1 = path.to_vec();

                    if let Some(nxdir) = node.1.last_mut() {
                        *nxdir += &format!("{}", alp[i]);
                    }
                    stack.push((nx, ny));
                }
            }
        }
    }

    let nd = &graph[0][1];

    print!("{:?}", nd.1);


    for y in 0..height {
        for x in 0..width {
            print!("{}", &graph[y][x].0);
        }
        print!("\n");
    }
    /*
    */

}

fn main () {

    let grid = vec![
        vec![  4,  2,  5,  4],
        vec![  4, 15, 11,  1],
        vec![ -1,  9,  6,  8],
        vec![ 12,  7,  7, -2]
    ];

  maze_solver(&grid); // ["NNE", "EE", "S", "SS"] <- one possible solution
          //let mut orin = vec![String];
        //orin[orin.len() - 1] += &format!("NE");


}
