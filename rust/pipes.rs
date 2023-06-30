#![allow(dead_code,unused)]

fn isinside (map: &Vec<Vec<char>>, p: (i32,i32)) -> bool {
    p.0 >= 0 && p.1 >= 0 && (p.1 as usize) < map.len() && (p.0 as usize) < map[p.1 as usize].len()    
}
fn identify (cell:char) -> Vec<usize> {

    let mut comb= vec![];

    match cell {
        '━' => comb = vec![1,3],
        '┓' => comb = vec![2,3],
        '┃' => comb = vec![0,2],
        '┛' => comb = vec![0,3],
        '┏' => comb = vec![1,2],
        '┗' => comb = vec![0,1],
        '┳' => comb = vec![1,2,3],
        '┫' => comb = vec![0,2,3],
        '┣' => comb = vec![0,1,2],
        '┻' => comb = vec![0,1,3],
        '╋' => comb = vec![0,1,2,3],
            _  => (),
    }

    comb
}
fn search (grid: &Vec<Vec<char>>, visit: &Vec<Vec<bool>>) -> Option<(usize,usize)> {
    let ly = grid.len()-1;

    for x in 0..grid[0].len() {
        if visit[0][x] == false && grid[0][x] != '.' && grid[0][x] != ' ' {
            return Some ((x,0));
        }
    }

    for y in 0..grid.len() {
        if visit[y][0] == false && grid[y][0] != '.' && grid[y][0] != ' ' {
            return Some((0,y));
        }
    }

    for y in 0..grid.len() {
        let x = grid[y].len()-1;
        if visit[y][x] == false && grid[y][x] != '.' && grid[y][x] != ' ' {
            return Some((x,y));
        }
    }

    for x in 0..grid[ly].len() {
        if visit[ly][x] == false && grid[ly][x] != '.' && grid[ly][x] != ' ' {
            return Some((x,ly));
        }
    }


    None
}

fn check_pipe (map: &[&str]) -> bool {
    let direct = [[0,-1],[1,0],[0,1],[-1,0]]; // north,east,south,west
    let accept = [['╋','┃','┫','┣','┏','┓','┳'], ['╋','━','┓','┛','┫','┳','┻'] , ['╋','┃','┫','┣','┗','┛','┻'] , ['╋','━','┏','┗','┣','┳','┻']];

    let mut grid:Vec<Vec<_>> = vec![];

    for y in 0..map.len() {
        grid.push(map[y].chars().collect());
    }

    let height = grid.len();
    let width = grid[0].len();
    let mut visit = vec![vec![false; width]; height];

    while let Some(start) = search (&grid, &visit)  {
        let mut q1:Vec<(usize,usize)> = vec![start];

        while let Some(p) = q1.pop() {
            let cell = grid[p.1][p.0];
            let comb = identify (cell);

            visit[p.1][p.0] = true;

            if cell == '.' {
                return false;
            }
            //print!("[{cell}]");
            for i in comb {
                let [dx,dy] = direct[i];
                let nxt = (p.0 as i32 + dx, p.1 as i32 + dy);

                if isinside (&grid, nxt) {
                    let nxt = (nxt.0 as usize, nxt.1 as usize);

                    if accept[i].iter().find(|x| x == &&grid[nxt.1][nxt.0]) == None {
                        return false;
                    }

                    if visit[nxt.1][nxt.0] == false {
                        q1.push(nxt);
                    }
                }
            }
        } 
    }


    true
}


fn main () {
    let map = [
        "...┏", 
        "...┃", 
        "┓..┏"]; // this is not leaking
    dotest (&map);

    // let map = ["     +", "   + ╋━━┓", "     ┃..┃", "   + ┛..┣ +", "        +"];

    // let map = ["...┏", "...┃", "┛..┣"]; // this is leaking
    // dotest (&map);
    // 
    // let map = ["...┏", "...┃", "┓..┣"]; // this is not leaking
    // dotest (&map);

    // let map = ["....", ".┛┛.", "...." ];
    // dotest (&map);

    // let map = ["╋━━┓", "┃..┃", "┛..┣"];
    // dotest (&map);

    // let map = ["...┏", "┃..┃", "┛..┣"];
    // dotest (&map);

    //  const TEST_CASES: [([&str; 3], bool); 7] = [
    //      (["╋━━┓", "┃..┃", "┛..┣"], true),
    //      (["...┏", "┃..┃", "┛..┣"], false),
    //      (["...┏", "...┃", "┛..┣"], false),
    //      (["...┏", "...┃", "┓..┣"], true),
    //      (["╋", "╋", "╋"], true),
    //      (["╋....", "┃..┛.", "┃...."], false),
    //      (["....", ".┛┛.", "...."], true),
    //  ];
    //      for (pmap, answer) in &TEST_CASES {
    //          run_test(pmap, *answer);
    //      }

    print!("\n");

}
fn run_test(pmap: &[&str], answer: bool) {
    let test_result = check_pipe(pmap);
    assert!(
        test_result == answer,
        "Output: {}; expected value: {}; for input:\n{}\n",
        test_result,
        answer,
        pmap.join("\n")
        );
}
fn evaluate (map: &Vec<Vec<char>>, x:usize, y:usize) ->  bool {

    // north,east,south,west
    let direct = [[0,-1],[1,0],[0,1],[-1,0]];
    let accept = [['╋','┃','┫','┣','┏','┓','┳'], ['╋','━','┓','┛','┫','┳','┻'] , ['╋','┃','┫','┣','┗','┛','┻'] , ['╋','━','┏','┗','┣','┳','┻']];
    let cell = map[y][x];
    let comb = identify (cell);

    for i in 0..comb.len() {
        let dx = direct[comb[i]][0];
        let dy = direct[comb[i]][1];
        let p = (x as i32 + dx, y as i32 + dy); 

        if isinside (&map, p) {
            let next = map[p.1 as usize][p.0 as usize]; 

            match next {
                '.' => return false,
                ' ' => {
                    let p2 = (x as i32 + dx * 2, y as i32 + dy * 2);
                    if isinside (&map, p2) {
                        print! ("[{}]", map[p2.1 as usize][p2.0 as usize]);
                    }
                },

                _ => {
                    if accept[i].iter().find(|x| x == &&next) == None {
                        return false;
                    }

                },
            }
            print! ("[{}]", next);
        } 
    }

    true
}
fn dotest (map: &[&str]) {

    for y in 0..map.len() {
        print! ("{}\n", map[y]);
    }
    let res = check_pipe (map);
    print! ("\n => {}\n", res);
}
