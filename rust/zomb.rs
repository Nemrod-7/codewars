#[derive(Clone, Eq, PartialEq)]
enum Type {
    Grass,
    Plant,
    Multi,
    Zombe,
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
fn disperse (graph: &mut Vec<Vec<(Type,u32)>>, x:usize, y:usize) {

    let height = graph.len();
    let width = graph[0].len();

    for i in 1..width - x {
        let dx = x + i;
        let mut hit = [false; 3];

        for j in 0..3 {
            let dy = y as i32 + (i as i32 * (j as i32 - 1));

            if dy >= 0 && dy < height as i32 && hit[j] == false {
                let mut cell = &mut graph[dy as usize][dx];

                if cell.0 == Type::Zombe {
                    if cell.1 > 1 {
                        cell.1 -= 1
                    } else {
                        *cell = (Type::Grass, 0)
                    }
                    hit[j] = true;
                }
            }
        }
    }
}
fn plants_and_zombies (lawn: &Vec<&str>, zombies: &Vec<Vec<usize>>) -> Option<usize> {

    let mut cycle = 0;
    let height = lawn.len();
    let width = lawn[0].len();

    let mut graph = mkgraph (lawn);

    loop {

        let mut nplan = 0;
        let mut nzomb = 0;

        for zom in zombies {
            let mov = zom[0];
            let row = zom[1];
            let hp = zom[2] as u32;

            if mov == cycle {
                graph[row][width - 1] = (Type::Zombe, hp);
            }
        }

        display(&graph);

        for y in 0..height {
            let mut dfc = 0;
            for x in 0..width {
                let (id, hp) = &graph[y][x];

                match id {
                    Type::Grass => (),
                    Type::Multi => {
                        disperse (&mut graph, x, y);
                        nplan += 1;
                    },
                    Type::Plant => {
                        dfc += hp;
                        nplan += 1;
                    },
                    Type::Zombe => {

                        if dfc > *hp {
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
        if cycle == 15 { break  }
        
        for y in 0..graph.len() {
            for x in 1..graph[y].len() {
                if graph[y][x].0 == Type::Zombe {
                    if x == 1 { return Some(cycle) }
                    graph[y][x - 1] = (Type::Grass, 0);
                    graph[y].swap(x - 1, x);
                }
            }
        }

        cycle += 1;
    }

    None
}

fn main () {

    let lawn = vec![ "2       ", "  S     ", "21  S   ", "13      ", "2 3     " ];
    let zombies = vec![ vec![0,4,28], vec![1,1,6], vec![2,0,10], vec![2,4,15], vec![3,2,16], vec![3,3,13] ];

    plants_and_zombies (&lawn, &zombies);

    // assert_eq!(plants_and_zombies(&lawn, &zombies), 10);// OK

}
fn display (graph: &Vec<Vec<(Type,u32)>> ) {

    for y in 0..graph.len() {
        for x in 0..graph[y].len() {
            let cell = &graph[y][x];

            match cell.0 {
                Type::Grass => print!("[ ]"),
                Type::Multi => print!("[S]"),
                _ => print!("[{}]", cell.1),
            }
        }
        print!("\n");
    }
    print!("\n");

}

