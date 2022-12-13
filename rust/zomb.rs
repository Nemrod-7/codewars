
mod pnz {

#[derive(Clone, Eq, PartialEq)]
    enum Type {
        Grass,
        Plant,
        Multi,
        Zombe,
    }

    fn display (graph: &Vec<Vec<(Type,u32)>> ) {

        print!("\n");
        for y in 0..graph.len() {
            for x in 0..graph[y].len() {
                let cell = &graph[y][x];

                match cell.0 {
                    Type::Grass => print!("[  ]"),
                    Type::Multi => print!("[ S]"),
                    _ => print!("[{:2}]", cell.1),
                }
            }
            print!("\n");
        }
        print!("\n");

    }

    fn disperse (graph: &mut Vec<Vec<(Type,u32)>>, x:usize, y:usize) {

        let height = graph.len();
        let width = graph[0].len();
        let mut hit = [false; 3];

        for i in 1..width - x {
            let dx = x + i;

            for j in 0..3 {
                let dy = y as i32 + (i as i32 * (j as i32 - 1));

                if dy >= 0 && dy < height as i32 && hit[j] == false {
                    let mut cell = &mut graph[dy as usize][dx];

                    if cell.0 == Type::Zombe {
                        if cell.1 == 1 {
                            *cell = (Type::Grass, 0)
                        } else {
                            cell.1 -= 1
                        }
                        hit[j] = true;
                    }
                }
            }
        }
    }
    fn mkgraph (lawn: &Vec<&str>) -> Vec<Vec<(Type,u32)>> {

        let height = lawn.len();
        let width = lawn[0].len();
        let mut graph = vec![vec![(Type::Grass,0);width]; height];

        for y in 0..height {
            let line = lawn[y].chars().collect::<Vec<char>>();

            for x in 0..width {
                if line[x] == 'S' {
                    graph[y][x] = (Type::Multi, 1);
                } else if line[x] != ' ' {
                    let pt = line[x].to_digit(10).unwrap();
                    graph[y][x] = (Type::Plant, pt);
                }
            }
        }

        graph
    }

    fn showtest(lawn: &Vec<&str>, zombies: &Vec<Vec<usize>>) {

        print!("let lawn = vec![");
        for line in lawn {
            print!("\"{}\",",line);
        }
        print!("];\nlet zombies = vec![");
        for line in zombies {
            print!("vec![");
            for dig in line {
                print!("{dig},");
            }
            print!("],");
        } 
        print!("];\n");
    }

    fn showforces(graph: &Vec<Vec<(Type,u32)>>, zombies: &Vec<Vec<usize>>) {
        
        print!("width {} \n", graph[0].len());
        for y in 0..graph.len() {
            for x in 0..graph[y].len() {
                let cell = &graph[y][x];
                match cell.0 {
                    Type::Multi => print!("S"),
                    _ => print!("{}", cell.1),
                }
            }
            print!(" => ");
            for it in zombies {
                let row = it[1];
                if row == y {
                    print! ("{} ", it[2]);
                }
            }
            print!("\n")
        }
    }
    pub fn plants_and_zombies (lawn: &Vec<&str>, zombies: &Vec<Vec<usize>>) -> usize {

        let height = lawn.len();
        let width = lawn[0].len();
        let mut graph = mkgraph (lawn);
        let mut cycle = zombies.iter().map(|x| x[0]).min().unwrap();
        //print!("{maxx}\n");
        //showtest (lawn, zombies);
        //showforces (&graph, zombies);
            display(&graph);
        loop {
            //print! ("{cycle} => \n");
            let mut nzomb = 0;
            for zomb in zombies {
                if zomb[0] == cycle {
                    graph[zomb[1]][width - 1] = (Type::Zombe, zomb[2] as u32);
                }
            }
            //display(&graph);
            for y in 0..height {
                let mut dfc = 0;
                for x in 0..width {
                    let (id, hp) = &graph[y][x];

                    match id {
                        Type::Grass => {},
                        Type::Multi => { disperse (&mut graph, x, y); },
                        Type::Plant => { dfc += hp; },
                        Type::Zombe => {
                            if x == 0 { return cycle + 1 }
                            if dfc >= *hp {
                                dfc -= *hp;
                                graph[y][x] = (Type::Grass, 0);
                            } else {
                                graph[y][x].1 -= dfc;
                                dfc = 0;
                                nzomb += 1;
                            }
                        },
                    }
                }
            }

            if nzomb == 0 { break }

            for y in 0..height {
                for x in 1..width {
                    if graph[y][x].0 == Type::Zombe {
                        graph[y][x - 1] = (Type::Grass, 0);
                        graph[y].swap(x - 1, x);
                    }
                }
            }

            cycle += 1;
        }

        0
    }

}
fn main () {

    let lawn = vec![ "2       ", "  S     ", "21  S   ", "13      ", "2 3     " ];
    let zombies = vec![ vec![0,4,28], vec![1,1,6], vec![2,0,10], vec![2,4,15], vec![3,2,16], vec![3,3,13] ];

    //let res = plants_and_zombies (&lawn, &zombies);
    let lawn = vec![ "1         ", "SS        ", "SSS       ", "SSS       ", "SS        ", "1         "];
    let zombies  = vec![ vec![0,2,16], vec![1,3,19], vec![2,0,18], vec![4,2,21], vec![6,3,20], vec![7,5,17], vec![8,1,21], vec![8,2,11], vec![9,0,10], vec![11,4,23], vec![12,1,15], vec![13,3,22]];
    //assert_eq!(pnz::plants_and_zombies(&lawn, &zombies), 0);// OK




let lawn = vec!["3S1 S         ","61            ","22 SS         ","2111          ","S 3 31        ","1 32          ","2             ","4             "];
let zombies = vec![vec![0,0,52],vec![0,1,60],vec![0,2,52],vec![0,4,69],vec![0,5,52],vec![0,6,17],vec![0,7,34],vec![1,3,46],vec![3,0,27],vec![3,1,32],vec![3,4,36],vec![3,5,27],vec![3,6,9],vec![4,3,24],vec![4,7,21],vec![6,2,34],vec![11,0,29],vec![11,1,34],vec![11,4,39],vec![13,5,32],vec![13,6,11],vec![14,3,27],vec![14,7,22],vec![16,2,39],vec![17,0,29],vec![17,1,34],vec![17,4,39],vec![17,5,27],vec![17,6,9],vec![18,3,23],vec![19,2,26],vec![19,7,21],vec![21,0,26],vec![21,1,31],vec![21,4,35],vec![22,5,29],vec![22,6,9],vec![23,3,24],vec![25,7,23],vec![26,1,33],vec![26,4,38],vec![26,5,26],vec![26,6,9],vec![27,0,32],vec![29,2,44],vec![29,3,25],vec![31,7,21],vec![32,0,27],vec![32,1,34],vec![32,2,27],vec![33,3,22],vec![33,4,44],vec![33,5,32],vec![33,6,11],vec![37,1,31],vec![37,7,21]];
//assertion failed: `(left == right)` left: `31`, right: `25`
assert_eq!(pnz::plants_and_zombies(&lawn, &zombies), 25);


    //example_tests();
    print!("\n");
}
fn example_tests() {
    let example_tests: Vec<(Vec<&str>,Vec<Vec<usize>>,usize)> = vec![
        (
            vec![ "2       ", "  S     ", "21  S   ", "13      ", "2 3     "],
            vec![ vec![0,4,28], vec![1,1,6], vec![2,0,10], vec![2,4,15], vec![3,2,16], vec![3,3,13]],
            10
        ),
        (
            vec![ "11      ", " 2S     ", "11S     ", "3       ", "13      "],
            vec![ vec![0,3,16], vec![2,2,15], vec![2,1,16], vec![4,4,30], vec![4,2,12], vec![5,0,14], vec![7,3,16], vec![7,0,13]],
            12
        ),
        (
            vec![ "12        ", "3S        ", "2S        ", "1S        ", "2         ", "3         "],
            vec![ vec![0,0,18], vec![2,3,12], vec![2,5,25], vec![4,2,21], vec![6,1,35], vec![6,4,9], vec![8,0,22], vec![8,1,8], vec![8,2,17], vec![10,3,18], vec![11,0,15], vec![12,4,21]],
            20
        ),
        (
            vec![ "12      ", "2S      ", "1S      ", "2S      ", "3       "],
            vec![ vec![0,0,15], vec![1,1,18], vec![2,2,14], vec![3,3,15], vec![4,4,13], vec![5,0,12], vec![6,1,19], vec![7,2,11], vec![8,3,17], vec![9,4,18], vec![10,0,15], vec![11,4,14]],
            19
        ),
        (
            vec![ "1         ", "SS        ", "SSS       ", "SSS       ", "SS        ", "1         "],
            vec![ vec![0,2,16], vec![1,3,19], vec![2,0,18], vec![4,2,21], vec![6,3,20], vec![7,5,17], vec![8,1,21], vec![8,2,11], vec![9,0,10], vec![11,4,23], vec![12,1,15], vec![13,3,22]],
            0
        )
            ];

    example_tests.into_iter().for_each(|(grid,zqueue,sol)| assert_eq!(pnz::plants_and_zombies(&grid,&zqueue),sol));

let lawn = vec![" 42         ","14          ","3SS         ","51          ","4           ","5  2        "];
let zombies = vec![vec![2,0,32],vec![2,1,27],vec![2,3,32],vec![2,4,21],vec![2,5,37],vec![3,0,20],vec![3,3,20],vec![3,4,13],vec![4,1,19],vec![4,5,27],vec![5,0,17],vec![5,3,17],vec![8,1,15],vec![8,2,36],vec![9,2,22],vec![9,4,15],vec![9,5,24],vec![10,0,17],vec![10,3,17],vec![11,1,15],vec![11,2,16],vec![11,4,12],vec![12,5,23],vec![13,0,18],vec![13,2,13],vec![13,3,18],vec![15,1,17],vec![15,4,13],vec![15,5,22],vec![16,0,18],vec![16,2,14],vec![17,1,14],vec![17,4,11],vec![18,5,22]];
    assert_eq!(pnz::plants_and_zombies(&lawn, &zombies), 21);

let lawn = vec!["31SSSS               ","114SS                ","41    S              "," 31S 11S 1           ","1111S                ","133SS                "," S34S                ","32                   ","7 SS                 ","S31   S              ","231SS   S            ","S 4S                 ","4S12S S              ","11  S                ","211 SSSS             ","61SSSS               ","121SSS               "];
let zombies = vec![vec![1,2,93],vec![1,3,139],vec![1,5,139],vec![1,6,139],vec![1,7,77],vec![1,8,139],vec![1,9,93],vec![1,10,139],vec![1,13,46],vec![1,15,170],vec![1,16,108],vec![2,0,130],vec![2,4,81],vec![2,11,97],vec![2,12,162],vec![2,14,130],vec![7,1,153],vec![7,3,70],vec![7,5,70],vec![7,6,70],vec![7,7,39],vec![7,8,70],vec![7,13,23],vec![7,15,85],vec![7,16,54],vec![8,0,63],vec![8,9,51],vec![8,10,76],vec![8,11,48],vec![8,12,79],vec![8,14,63],vec![9,4,43],vec![12,1,64],vec![13,2,68],vec![16,3,71],vec![16,5,71],vec![16,15,88],vec![16,16,56],vec![17,0,64],vec![17,6,78],vec![17,7,43],vec![17,8,78],vec![17,9,48],vec![17,11,48],vec![18,10,80],vec![18,12,88],vec![18,13,28],vec![18,14,70],vec![20,1,64],vec![20,2,49],vec![20,4,48],vec![26,3,72],vec![26,5,72],vec![26,15,88],vec![28,6,74],vec![28,7,41],vec![28,8,74],vec![28,13,23],vec![29,0,70],vec![29,9,53],vec![29,12,82],vec![29,16,66],vec![30,4,39],vec![30,10,81],vec![30,14,71],vec![31,2,48],vec![33,1,70],vec![34,11,70],vec![40,5,72],vec![40,15,88],vec![41,3,79],vec![41,7,40],vec![41,13,24],vec![43,0,65],vec![43,8,79],vec![43,16,59]];
// assertion failed: `(left == right)` left: `0`, right: `33`
assert_eq!(pnz::plants_and_zombies(&lawn, &zombies), 33);


}


