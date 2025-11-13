

fn is_inside(x:i8, y:i8) -> bool { x >= 0 && y >= 0 && x < 4 && y < 4 }

fn mk_grid(src: &Vec<String>) -> Vec<Vec<String>> {
    let src:Vec<Vec<char>> = src.iter().map(|x| x.chars().collect()).collect::<Vec<_>>();
    let height = src.len();
    let width = src[0].len();

    let mut grid = vec![vec!["".to_string(); 4]; 4];

    for i in 0..height {
        for j in 0..width {
            let y = (i * 4) / height;
            let x = (j * 4) / width;

            match src[i][j] {
                'W' => (),
                'A' => (),
                'P' => (),
                'G' => (),
                'w' => (),
                's' => (),
                _ => continue,
            }

            if !grid[y][x].find(src[i][j]).is_some() {
                grid[y][x] += &format!("{}",src[i][j] ) ;
            }
        }
    }

    grid[0][0] = format!("");
    grid
}


fn next_dir(x:usize, y:usize) -> Vec<(usize,usize)> {
    let direction = [(0,1),(0,-1),(1,0),(-1,0)];
    let mut res = vec![];

    for (dx,dy) in direction.iter() {
        let (nx,ny) = (dx + x as i8, dy + y as i8);

        if is_inside(nx,ny) {
            res.push((nx as usize, ny as  usize));
        }
    }
    res
}

fn main () {

    let src = [
        "|------------|------------|------------|------------|",
        "|            |            |            |            |",
        "|            |            |            |            |",
        "|    Agent                                          |",
        "|            |            |            |  wwwwwwww  |",
        "|            |            |            |  wwwwwwww  |",
        "|-----  -----|-----  -----|-----  -----|-----  -----|",
        "|            |            |            |            |",
        "|            |            |         ww |            |",
        "|                                   ww       Pit    |",
        "|  ssssssss  |            |  wwwwwwwww |            |",
        "|  ssssssss  |            |  wwwwwwww  |            |",
        "|-----  -----|-----  -----|-----  -----|-----  -----|",
        "|            |            |            |  wwwwwwww  |",
        "|            | ss      ww |            | wwwwwwwww  |",
        "|   Wumpus     ss Gold ww       Pit      ww         |",
        "|            | ss      ww |            | ww         |",
        "|            |            |            |            |",
        "|-----  -----|-----  -----|-----  -----|-----  -----|",
        "|  ssssssss  |            |  wwwwwwww  |            |",
        "|  ssssssss  |            |  wwwwwwww  |            |",
        "|                                                   |",
        "|            |            |            |            |",
        "|            |            |            |            |",
        "|------------|------------|------------|------------|"];

    let src = src.iter().map(|x| format!("{x}")).collect::<Vec<String>>();

    let grid = mk_grid(&src);
    let mut pts = [[0; 4]; 4];

    (0..4).for_each(|y| 
        (0..4).for_each(|x|
        if grid[y][x].contains(|ch| ch == 's' || ch == 'w') {
            for (nx,ny) in next_dir(x,y) {
                pts[ny][nx] += 1; 
            }
        }
    ));

    (0..4).for_each(|y| 
        (0..4).for_each(|x|
        if grid[y][x] == "" {
            for (nx,ny) in next_dir(x,y) {
                pts[ny][nx] = 0; 
            }
        }
    ));

    for i in 0..4 {
        for j in 0..4 {
            print!("{:?} ", pts[i][j]);
        }
        print!("\n");
    }

    let mut visit = [[false; 4]; 4];
    let mut queue = vec![(0,0)];

    while let Some((x,y)) = queue.pop() {

        for (dx,dy) in direction.iter() {
            let (nx,ny) = (x + dx, y + dy);

            if is_inside(nx,ny) {
                let (nx,ny) = (nx as usize, ny as usize);

                if !visit[ny][nx] {
                    visit[ny][nx] = true;
                }

            }
        }
    }


}
