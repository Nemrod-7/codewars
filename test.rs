
fn main () {

    let grid:Vec<Vec<char>> = vec![
        vec!['+','-','+'],
        vec!['|','U','|'],
        vec!['+','-','+']
    ];


        let res = (0..grid.len())
            .map( |y| (0..grid[y].len()) 
                  .filter(|x| grid[y][*x].is_alphabetic())
                  .map(|x| (grid[y][x], (x,y)))
                  .collect::<Vec<_>>()
                )
            .into_iter()
            .flatten()
            .collect::<Vec<_>>() ;

        print!("{:?}\n", res);

        //su.extend(grid[y].clone());

        //print!("{res}\n")
}
