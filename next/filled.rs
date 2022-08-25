
fn getmin (grid: &Vec<Vec<i32>>) -> i32 {
    let mut vmin = i32::MAX;

    for row in grid {
        for cell in row {
            vmin = std::cmp::min (vmin, *cell);
        }
    }

    vmin
}
fn getmax (grid: &Vec<Vec<i32>>) -> i32 {
    let mut vmax = i32::MIN;

    for row in grid {
        for cell in row {
            vmax = std::cmp::max (vmax, *cell);
        }
    }

    vmax
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

    let sum = 0;
    let depth = grid[src.1][src.0];
    let mut minv = i32::MAX;
    let mut valid = true;
    let mut stack = vec![src];

    let u = *stack.last().unwrap();
    stack.pop();

    let compas = [(0,-1),(1,0),(0,1),(-1,0)];

    for pos in compas {
        let nx = pos.0 + u.0;
        let ny = pos.1 + u.1;
        //print! ("{} {}\n", nx, ny);
        if isinside (grid, nx, ny) {
            let cell = grid[ny as usize][nx as usize];
            if cell == depth {
                stack.push ((nx, ny));
            } else {
                minv = std::cmp::min (minv, cell);
            }
        } else {
            valid = false;
        }


    }


    sum
}
fn volume (grid: &Vec<Vec<i32>>) -> usize {

    let sum = 0;

    let sweep = getmin (grid);
    scan_area (grid, (0,0));

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

    volume (&grid);
}
