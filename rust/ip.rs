
fn is_inside (p: (i32,i32)) -> bool { p.0 >= 0 && p.1 >=0 && p.0 < 8 && p.1 < 8 }
fn search (grid: &[[char;8];8]) -> (i32,i32) {
    for y in 0..8 {
        for x in 0..8 {
            if grid[x][y] == '♔' {
                return (x as i32,y as i32)
            }
        }
    }

    (0,0)
}
fn check (grid: &[[char;8];8], p: (i32,i32), dir: (i32,i32), dist: i32) -> char {

    for i in 1..dist {
        let nx = p.0 + dir.0 * i;
        let ny = p.1 + dir.1 * i;
        if is_inside ((nx,ny)) {
            if grid[nx as usize][ny as usize] != ' ' {
                return grid[nx as usize][ny as usize];
            }
        }
    }

    ' '
}
fn king_in_check (grid: &[[char;8];8]) -> bool {
    let knight = [(1,-2),(-1,-2),(1,2),(-1,2),(2,-1),(-2,-1),(2,1),(-2,1)]  ;
    let cross = [(0,1),(0,-1),(1,0),(-1,0)];
    let diag = [(-1,-1),(-1,1),(1,1),(1,-1)];
    let k = search(grid);

    for i in 0..4 {
        let corner = check (grid, k, diag[i], 8);
        let line = check (grid, k, cross[i], 8);
        let near = check (grid, k, diag[i], 1);

        if near == '♟' { return true; }
        if line == '♛' || line == '♜' { return true; }
        if corner == '♛' || corner == '♝' { return true; }
    }

    for i in 0..8 {
        let nx = k.0 + knight[i].0;
        let ny = k.1 + knight[i].1;

        if is_inside ((nx,ny)) {
            if grid[nx as usize][ny as usize] == '♞' {
                  return true;
            }
        }
    }

    false
}

fn main () {

    /*

'♔' for the black King;
'♛' for a white Queen;
'♝' for a white Bishop;
'♜' for a white Rook;
'♟' for a white Pawn;

'♞' for a white Knight;

*/

    let res = king_in_check (
    &[[' ',' ',' ',' ',' ',' ',' ',' '],
     [' ',' ',' ',' ',' ',' ','♛',' '],
     ['♞',' ',' ',' ',' ',' ',' ',' '],
     [' ',' ',' ',' ',' ','♟',' ',' '],
     [' ','♝',' ',' ',' ','♔',' ',' '],
     [' ',' ',' ',' ','♟',' ',' ',' '],
     [' ',' ',' ',' ',' ',' ',' ',' '],
     [' ',' ',' ',' ',' ',' ',' ',' '] ]);


     print!("{res}\n");

}
