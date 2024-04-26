#![allow(unused)]

fn show(arr: &Vec<u32>, nc: usize) {
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

fn tochar(x: u32) -> char { (x + 65) as u8 as char }

fn main () {

    let g = 4;
    let n = 20;
    let d = 5;

    let s2 = n / g;
    let mut res:Vec<String> = Vec::new();
    let mut arr:Vec<u32> = (2..n).map(|x| x as u32).collect();

    arr.push(1);

    for _rotation in 0..1 {
        arr.push(0);
        //show(&arr, s2);

        for i in 0..s2 {
            let mut round = Vec::new();

            for j in 0..(g / 2)  {
                let a = i * g + j;
                let b = n - a - 1;
                //print!("{}{}", tochar(arr[a]) , tochar(arr[b]));
                round.push(tochar(arr[a]));
                round.push(tochar(arr[b]));
            }

            round.sort();
            res.push(round.iter().collect::<String>());
        }
        arr.pop();
        arr.rotate_right(1);
    }


    print!("{:?}", res);





}

