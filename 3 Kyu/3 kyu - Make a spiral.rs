fn spiralize(size: usize) -> Vec<Vec<i8>> {

    let mut grid:Vec<Vec<i8>> = vec![vec![0; size]; size];
    let mut x = 0;
    let mut y = 0;
    let mut dir = 0;
    let mut minx = 0;
    let mut miny = 0;
    let mut maxx = size - 1;
    let mut maxy = size - 1;

    loop {
        grid[y][x] = 1;

        match dir {
            0 => if x == maxx { miny += 2 ; dir = 1 } else { x += 1 },
            1 => if y == maxy { maxx -= 2 ; dir = 2 } else { y += 1 },
            2 => if x == minx { maxy -= 2 ; dir = 3 } else { x -= 1 },
            3 => if y == miny { minx += 2 ; dir = 0 } else { y -= 1 },
            _ => (),
        }
        if minx > maxx && miny > maxy { break }
    }

    grid
}
