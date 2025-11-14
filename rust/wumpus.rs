

fn is_inside(x:i8, y:i8) -> bool { x >= 0 && y >= 0 && x < 4 && y < 4 }

fn mk_grid(src: &Vec<String>) -> Vec<Vec<String>> {
    let src:Vec<Vec<char>> = src.iter().map(|x| x.chars().collect()).collect::<Vec<_>>();
    let height = src.len();
    let width = src[0].len();

    let mut grid = vec![vec!["".to_string(); 4]; 4];

    for i in 0..height {
        for j in 0..width {
            let (x,y) = ((j * 4) / width, (i * 4) / height) ;

            if !"WAPGws".contains(src[i][j]) {
                continue;
            }

            if !grid[y][x].contains(src[i][j]) {
                grid[y][x] += &format!("{}",src[i][j] ) ;
            }
        }
    }

    grid[0][0] = format!("");
    grid
}


fn next_dir(x:usize, y:usize) -> Vec<(usize,usize)> {
    [(0,1),(0,-1),(1,0),(-1,0)] 
        .iter() 
        .map(|(dx,dy)| (dx + x as i8, dy + y as i8) ) 
        .filter(|&(nx,ny)| is_inside(nx, ny)) 
        .map(|(nx,ny)| (nx as usize, ny as usize))
        .collect::<Vec<_>>()
}
fn solve (src: &Vec<String>) -> bool {

    let direction = [(0,1),(0,-1),(1,0),(-1,0)];
    let grid = mk_grid(&src);
    let mut exit = (0,0);
    let mut dmz = [[0; 4]; 4];

    (0..4).for_each(|y| 
        (0..4).for_each(|x| {
            if grid[y][x].contains(|ch| ch == 'G') {
                exit = (x as usize,y as usize);
            }
            if grid[y][x].contains(|ch| ch == 's' || ch == 'w') {
                next_dir(x,y).iter().for_each(|&(nx,ny)| dmz[ny][nx] += 1)
            }
        }
        ));

    (0..4).for_each(|y| 
        (0..4).for_each(|x|
            if grid[y][x] == "" {
                next_dir(x,y).iter().for_each(|&(nx,ny)| dmz[ny][nx] = 0)
            }
        ));


    let mut visit = [[0; 4]; 4];
    let mut queue = vec![(0,0)];

    while let Some((x,y)) = queue.pop() {

        for (dx,dy) in direction.iter() {
            let (nx,ny) = (x + dx, y + dy);

            if is_inside(nx,ny) {
                let (nx,ny) = (nx as usize, ny as usize);

                if visit[ny][nx] == 0 && dmz[ny][nx] == 0 {
                    visit[ny][nx] = 1;
                    queue.push( (nx as i8, ny as i8) );
                }
            }

        }
    }

    for i in 0..4 {
        for j in 0..4 {
            print!("{:?} ", visit[i][j]);
        }
        print!("\n");
    }

    // print!("{} {}\n", exit.0, exit.1);
    if visit[exit.1][exit.0] == 1 { return true } else { return false }
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

    let res = solve(&src);
    print!("{res}\n");

}
