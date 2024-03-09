fn cockroaches(room: &[&str]) -> [u32; 10] {

    let grid = room.iter().map(|x| x.chars().map(|x| x).collect::<Vec<char>>()).collect::<Vec<Vec<char>>>();
    let height = grid.len();
    let width = grid[0].len();
    let mut freq = [0u32;10];
    let mut search;
    let mut roaches = Vec::new();

    for y in 0..grid.len() {
        for x in 0..grid[y].len() {
            if grid[y][x].is_alphabetic() {
                roaches.push( (grid[y][x], (x,y)) );
            }
        }
    }
    for (mut dir, (mut x,mut y)) in &roaches {

        match dir {
            'U' => { while y > 1 { y -= 1 } ; dir = 'L' },
            'L' => { while x > 1 { x -= 1 } ; dir = 'D' },
            'R' => { while x < width - 2 { x += 1 } ; dir = 'U' },
            'D' => { while y < height - 2 { y += 1 } ; dir = 'R' } ,
            _ => (),
        }

        search = true;

        while search == true {
            let mut nx = x;
            let mut ny = y;
            let mut lx = x;
            let mut ly = y;

            match dir {
                'U' => { ny -= 1; lx += 1 },
                'L' => { nx -= 1; ly -= 1 },
                'R' => { nx += 1; ly += 1 },
                'D' => { ny += 1; lx -= 1 },
                _ => (),
            }

            for nxt in [(nx,ny),(lx,ly),(nx,ly),(lx,ny)] {
                if grid[nxt.1][nxt.0].is_digit(10) {
                    freq[grid[nxt.1][nxt.0] as usize - 48] += 1;
                    search = false;
                    break;
                }
            }

            if grid[ny][nx] == '|' {
                if dir == 'R' { dir = 'U' }
                if dir == 'L' { dir = 'D' }
            } else if grid[ny][nx] == '-' {
                if dir == 'U' { dir = 'L' }
                if dir == 'D' { dir = 'R'  }
            } else {
                x = nx; y = ny;
            }
        }
    }

    freq
}

fn main () {

    cockroaches ( &[
"+-----------+",
"0           |",
"2           |",
"3           |",
"|    L      |",
"+-----------+" ]);

}
