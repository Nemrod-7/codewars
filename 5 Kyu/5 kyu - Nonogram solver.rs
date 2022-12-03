fn mkcomb (num: u8) -> Vec<u8> {

    let mut comb:Vec<u8> = Vec::new();
    let mut acc = 0;
    
    for i in 0..5 {
        let bit =  num >> i &1;
        acc += bit;

        if (bit == 0 || i == 4) && acc != 0 {
            comb.push(acc);
            acc = 0;
        }
    }

    comb
}
fn backtrack (grid: &mut [[u8;5];5], (top, left): (&Vec<Vec<u8>>, &Vec<Vec<u8>>), y:usize) -> bool { 

    if y == 5 {

        for x in 0..5 {
            let mut num = 0;

            for i in 0..5 {
                num |= grid[i][x] << i;
            }
           
            if top[x].iter().position(|&p| p == num) == None { return false; }
        }

        return true;
    }

    for mask in &left[y] {
        for x in 0..5 {
            grid[y][x] = mask >> x &1; 
        }

        if backtrack (grid, (top,left), y + 1) == true {
            return true;
        }
    }
 
    return false;
}
fn solve_nonogram ((top, left): ([&[u8]; 5], [&[u8]; 5])) -> [[u8; 5]; 5] {


    let mut grid = [[0;5];5];
    let mut lateral = vec![vec![];5];
    let mut vertical = vec![vec![];5];

    for num in 0..32 {
        let comb = mkcomb (num);

        for i in 0..5 {
            if comb == left[i] { lateral[i].push(num); }
            if comb == top[i] { vertical[i].push(num); }
        }
    }

    backtrack (&mut grid, (&vertical,&lateral), 0);

    grid
}



fn main () {

    let n = 5;

    solve_nonogram(([&[1, 1], &[4], &[1, 1, 1], &[3], &[1]], [&[1], &[2], &[3], &[2, 1], &[4]]));
   
    //csp.retain(|&num| (num&1 << x) != 1);

        /*
 [1,1,1], [1,3] [2,2], [3,1],[5]
   
    grid = 
[[0, 0, 1, 0, 0],
 [1, 1, 0, 0, 0],
 [0, 1, 1, 1, 0],
 [1, 1, 0, 1, 0],
 [0, 1, 1, 1, 1]];

*/

}
////////////////////////////////////////Arkive///////////////////////////////////////////////
fn getline (grid:&[[u8;5];5], y:usize) -> Vec<u8> {

    let mut acc = 0;
    let mut line = Vec::new();

    for i in 0..5 {
        let cell = grid[y][i];
        acc += cell;
        
        if (cell == 0 || i == 4) && acc != 0 {
            line.push(acc);
            acc = 0;
        }
    }    

    line
}
fn getcol (grid:&[[u8;5];5], x:usize) -> Vec<u8> {

    let mut acc = 0;
    let mut line = Vec::new();

    for i in 0..5 {
        let cell = grid[i][x];
        acc += cell;
        
        if (cell == 0 || i == 4) && acc != 0 {
            line.push(acc);
            acc = 0;
        }
    }    

    line
}
fn isvalid (grid:&[[u8;5];5], (top, left): ([&[u8]; 5], [&[u8]; 5])) -> bool {

    for i in 0..5 {
        if getline (&grid, i) != left[i] || getcol (&grid, i)  != top[i] {
            return false
        }
    }

    true
}
fn reduce (grid:&mut [[u8;5];5], (vertical, lateral): (&Vec<Vec<u8>>, &Vec<Vec<u8>>)) {

    for y in 0..5 {
        if vertical[y].len() == 1 {
            let num = vertical[y][0];

            for x in 0..5 {
                grid[x][y] = num >> x &1; // if (num&1 << x) != 0 { 1 } else { 0 };
            }
        }
        if lateral[y].len() == 1 {
            let num = lateral[y][0];

            for x in 0..5 {
                grid[y][x] = num >> x &1;
            }
        }
    }
 }
fn filter (grid:&mut [[u8;5];5], (vertical, lateral): (&Vec<Vec<u8>>, &Vec<Vec<u8>>)) {
/*
    for y in 0..5 {
        for x in 0..5 {
            if grid[y][x] == 1 {
                lateral[y] = lateral[y].iter().filter(|&&cmb| (cmb&1 << x) != 0).cloned().collect::<Vec<_>>();
                vertical[x] = vertical[x].iter().filter(|&&cmb| (cmb&1 << y) != 0).cloned().collect::<Vec<_>>();
            }
        }
    }
    */
}


