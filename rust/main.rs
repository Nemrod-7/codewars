fn is_border (grid: &Vec<Vec<char>>, x: usize, y: usize) -> bool {

    let direc = vec![(0,1),(1,0),(0,-1),(-1,0)];
    let x = x as i32;
    let y = y as i32; 

    for (dx,dy) in &direc {
        let cell = grid[(y + dy) as usize][(x + dx) as usize]; 
        if cell == ' ' || cell == '-' { return true }
    }

    false
}
fn replace (grid: &mut Vec<Vec<char>>, x: usize, y: usize) {

    let direc = vec![(0,1),(1,0),(0,-1),(-1,0)];

    let height = grid.len() as i32 - 1;
    let width = grid[0].len() as i32 - 1;

    if grid[y][x] == '-' {
        for (dx,dy) in &direc {
            let nx = x as i32 + dx;
            let ny = y as i32 + dy;

            if nx >= 0 && nx <= width && ny >= 0 && ny <= height {
                if grid[ny as usize][nx as usize] == ' ' {
                    grid[ny as usize][nx as usize] = '-'; 
                }
            }
        }
    }
}

fn flood (grid: &mut Vec<Vec<char>>) {

    let height = grid.len() - 1;
    let width = grid[0].len() - 1;
    
    for _ in 0..2 {
        for y in 0..=height {
            for x in 0..=width {
                replace (grid, x, y);
                replace (grid,  x, height - y);
                replace (grid, width - x, height - y);
                replace (grid, width - x, y);
            }
        }
    }
}
fn dry_ground (isle: &[&str]) -> [u32;4] {
    
    if isle.len() == 0 || isle[0].len() == 0 { return [0;4] }

    let height = isle.len() - 1;
    let width = isle[0].len() - 1;
    let mut dr2 = [0;4];
    let mut grid = isle.iter().map(|x| x.chars().collect::<Vec<_>>() ).collect::<Vec<Vec<_>>>();
    
    for i in 0..4 {

        let cnt = grid.iter().map(|y| y.iter().filter(|&&x| x != '-').count()).sum::<usize>();
        dr2[i] = cnt as u32;
        flood (&mut grid);

        for y in 0..=height {
            for x in 0..=width {

                if grid[y][x] == '^' {
                    if x == 0 || y == 0 || x == width || y == height || is_border (&grid, x, y) { 
                        grid[y][x] = 'x' 
                    }
                }
            }
        }
        
        for y in 0..=height {
            for x in 0..=width {
                if grid[y][x] == 'x' { grid[y][x] = ' ' }
            }
        }
    }
    //print!("{:?}\n", &dr2);
    dr2
}


fn main () {

let mountain = [
"^^^^^^        ",
" ^^^^^^^^     ",
"  ^^^^^^^     ",
"  ^^^^^       ",
"  ^^^^^^^^^^^ ",
"  ^^^^^^      ",
"  ^^^^        "
];

let mountain = 
 ["^^^^^^^",
  "^^^^^^^",
  "^^^^^^^",
  "^^^ ^^^",
  "^^^^^^^",
  "^^^^^^^",
  "^^^^^^^"];


    let mountain = [
    "^^^^        ^^^^^   ",
    "^^^^        ^^^^^   ",
    "--------------------",
    "    ^^^^^^^^^^^^^   ",
    "    ^^^^^^^^^^^^^^  ",
    " ^^^^^^^^^^^^^^^^^^ ",
    " ^^^^ ^^^^^  ^^^^^  ",
    "^^^^^^^^^^^ ^^^^^^^ ",
    "^     ^^^^^^^^^^    ",
    "^^    ^^^^^^^^^^^   ",
    "         ^^^^^^^^   "];

    let mountain = [
"  ^^^^^^             ",       
"^^^^^^^^       ^^^   ",
"^^^^^^^  ^^^         ",
"^^^^^^^  ^^^         ",
"^^^^^^^  ^^^         ",
"---------------------",
"^^^^^                ",
"   ^^^^^^^^  ^^^^^^^ ",
"^^^^^^^^     ^     ^ ",
"^^^^^        ^^^^^^^ "];

    dry_ground (&mountain);
    let mountain = [
        "",
        "",
        ""
    ];


    let mountain = [

"^^^^^^^^^^^^^^^^^^^^^^^^^^^^",
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^",
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^",
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^",
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^",
"^^^^^^^^^^^^^-----^^^^^^^^^^",
"^^^^^^^^^^^^^-----^^^^^^^^^^",
"------------------^^^^^^^^^^",
"^^^^^^^^^^^^^-----^^^^^^^^^^",
"^^^^^^^^^^^^^-----^^^^^^^^^^",
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^",
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^",
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^",
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^",
"^^^^^^^^^^^^^^^^^^^^^^^^^^^^"];
// [382, 382, 260, 150]


        print!("\n");
}

fn display (grid: &Vec<Vec<char>>) {
    for y in grid {
        for x in y {
            print!("{x}");
        }
        print!("\n");
    }
        print!("\n");
}
fn peak_height (isle: &[&str]) -> u32 {

    
    let height = isle.len() - 1;
    let width = isle[0].len() - 1;
    let mut peak = 0;
    let mut running = true;
    let mut days:Vec<u32> = Vec::new();
    let mut grid:Vec<Vec<char>> = isle.iter().map(|x| x.chars().collect::<Vec<_>>() ).collect::<Vec<Vec<_>>>();

    while running == true {

        running = false;

        for y in 0..=height {
            for x in 0..=width {

                if grid[y][x] == '^' {
                    running = true;

                    if x == 0 || y == 0 || x == width || y == height || is_border (&grid, x, y) { 
                        grid[y][x] = 'x' 
                    }
                }
            }
        }
        
        if running {
            let mut dry = 0; 

            for y in 0..=height {
                for x in 0..=width {
                    if grid[y][x] == 'x' { grid[y][x] = ' ' }
                    if grid[y][x] != ' ' || grid[y][x] != '-' { dry += 1 }
                }
            }

            flood (&mut grid);

            days.push(dry);
            peak += 1;
        }
        //display (&grid);
   }
    //print!("{:?}\n", days);
    peak
}

