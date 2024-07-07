

fn riddle (arr:&Vec<u8>) -> u8 {
    arr.iter().filter(|&&x| x == 3 || x == 5).fold(0,|sum,dig| sum + dig - 1)
}

fn main () {


    let res = riddle(&[1,2,3,4,5].to_vec());
    print!("{res}\n");

    let res = riddle(&[2,2,3,3].to_vec());
    print!("{res}\n");

    let res = riddle(&[6,6,4,4,1,3].to_vec());
    print!("{res}\n");
 
    let res = riddle(&[ 3, 5, 3, 5, 4, 2 ].to_vec());
    print!("{res}\n");

  }
