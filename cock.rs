

fn cockroaches(room: &[&str]) -> [u32; 10] {
    
    let grid = room.iter().map(|x| x.chars().map(|x| x).collect::<Vec<char>>()).collect::<Vec<Vec<char>>>();
    let height = grid.len();
    let width = grid[0].len();
    let mut freq = [0u32;10];
    let mut roaches = Vec::new();

    for y in 0..room.len() {
        let line = room[y].chars().map(|x| x).collect::<Vec<char>>();
        
        for x in 0..line.len() {
            if line[x].is_alphabetic() {
                roaches.push( (line[x], (x,y)) );
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

        loop {
            let mut nx = x;
            let mut ny = y;
            let mut lx = x;
            let mut ly = y;

            match dir {
                'U' => { ny -= 1; lx += 1 },
                'L' => { nx -= 1; ly -= 1 },
                'R' => { nx += 1; lx -= 1 },
                'D' => { ny += 1; ly += 1 },
                _ => (),
            }

            if grid[ly][lx].is_digit(10) {
                freq[grid[ly][lx] as usize - 48] += 1;
                break;
            }
            /*
            if (isdigit (grid[ly][lx])) {
                freq[grid[ly][lx] - '0']++;
                break;
            }

            if (isdigit (grid[ny][lx])) {
                freq[grid[ny][lx] - '0']++;
                break;
            } else if (isdigit(grid[ly][nx])) {
                freq[grid[ly][nx] - '0']++;
                break;
            }
*/

            if grid[ly][lx] == '|' {
                if dir == 'R' { dir = 'U' }
                if dir == 'L' { dir = 'D' }
            } else if grid[ly][ly] == '-' {
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



}
