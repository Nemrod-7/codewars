#![allow(unused)]

fn showhash (arr: &Vec<u32>, nc: usize) {
    let s2 = arr.len() / nc;

    for i in 0..nc {
        for j in 0..s2 {
            let cell = arr[i * s2 + j];
            print!("{cell} ");
        }
        print!("\n");
    }

    print!("\n");
}
fn showhist (hist: &Vec<Vec<u32>>) {

    for i in 0..hist.len() {
        print!("{} : ", tochar(i as u32));

        for j in 0..hist[i].len() {
            if  hist[i][j] > 0 {
                print!("{}:{} ", tochar(j as u32),  hist[i][j]);
            }
        }
        print!("\n");
    }
}

fn tochar (x: u32) -> char { (x + 65) as u8 as char }
fn generate (n: usize, g:usize, arr: &mut Vec<u32>) -> Vec<Vec<u32>> {

    let s2 = n / g;
    let mut res:Vec<Vec<u32>> = Vec::new();

    for i in 0..s2 {
        let mut round = Vec::new();

        for j in 0..(g / 2)  {
            let a = i * g + j;
            let b = n - a - 1;
            round.push(arr[a]);
            round.push(arr[b]);
        }

        round.sort();
        res.push(round);
    }

    res
}
fn mkhist (res: &Vec<Vec<u32>>, hist: &mut Vec<Vec<u32>>) {
    for i in 0..res.len() {
        for j in 0..res[i].len() - 1 {
            for k in j + 1..res[i].len() {
                let a = res[i][j] as usize ;
                let b = res[i][k] as usize ;

                hist[a][b] += 1;
                hist[b][a] += 1;
            }
        }
    }
}

fn main () {

    let g = 4;
    let n = 20;
    let d = 5;

    let s2 = n / g;
    let mut arr:Vec<u32> = (2..n).map(|x| x as u32).collect();

    let mut hist = vec![vec![0; n]; n];

    arr.push(1);

    for _rotation in 0..2 {
        arr.push(0);
        let res = generate(n,g, &mut arr);
        //mkhist(&res, &mut hist);

        arr.pop();
        arr.rotate_right(g);
    }
    //showhash (&arr);
    //showhist(&hist);

}

