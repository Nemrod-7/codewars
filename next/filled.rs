use std::collections::HashMap;

fn getmin (grid: &Vec<Vec<i32>>) -> i32 {
    *grid.iter().flatten().min().unwrap_or(&i32::MAX)
}
fn getmax (grid: &Vec<Vec<i32>>) -> i32 {
    *grid.iter().flatten().max().unwrap_or(&i32::MAX)
}

fn getpos (grid: &Vec<Vec<i32>>, val: i32) -> (usize,usize) {

    for y in 0..grid.len() {
        for x in 0..grid[0].len() {
            if grid[y][x] == val { return (x,y) }
        }
    }

    (0,0)
}
fn isinside (grid: &Vec<Vec<i32>>, x: i32, y: i32) -> bool {
    x >= 0 && y >= 0 && y < grid.len() as i32 && x < grid[0].len() as i32
}

fn scan_area (grid: &Vec<Vec<i32>>, src: (usize,usize)) -> usize {

    let compas = [(0,-1),(1,0),(0,1),(-1,0)];

    let sum = 0;
    let depth = grid[src.1][src.0];
    let mut minv = i32::MAX;
    let mut valid = true;
    let mut stack = vec![src];
    let mut area: Vec<(usize,usize)> = Vec::new() ;

    while !stack.is_empty() {

        let u = *stack.last().unwrap();
        stack.pop();

        if !area.contains (&u) { area.push(u) }

        for pos in compas {
            let nx = pos.0 + u.0 as i32;
            let ny = pos.1 + u.1 as i32;

            if isinside (grid, nx, ny) {
                let nxt = (nx as usize, ny as usize);
                let cell = grid[nxt.1][nxt.0];

                if cell == depth {
                    if !area.contains (&nxt) {        
                        stack.push (nxt);
                    }
                } else  {
                    minv = std::cmp::min (minv, cell);
                }
            } else {
                valid = false;
            }
        }
    }

    print! ("{}\n", minv);
    for p in area {

        //let cell = grid[nxt.1][nxt.0];
        print! ("{},{}\n", p.0, p.1);
    }
    sum
}
fn volume (grid: &Vec<Vec<i32>>) -> usize {

    let sum = 0;

    let sweep = getmin (grid);
    scan_area (grid, getpos(grid, sweep));
    //print! ("{}", top);
    for row in grid {
        for cell in row {
            //print! ("{}", cell);
        }
        //print! ("\n");
    }


    sum
}

fn main () {

    let grid =
    vec![vec![8,8,8,8,6,6,6,6],
        vec![8,0,0,8,6,0,0,6],
        vec![8,0,0,8,6,0,0,6],
        vec![8,8,8,8,6,6,6,0]];

    let array = vec![1,8,2,8,5,6,6,7];

    volume (&grid);
}
