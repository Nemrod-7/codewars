
fn calc_special(last: u8, base: u8) -> String {
    let N = 100000;
    let mut num = 11;
    let pow = base;
    
    /*
    while num < N {

 //       if num > pow * base { pow * base }
        let dig = num % base;
        let trunc = num % base;
        let nxt = dig * pow + trunc;

        if dig > 0 {
            if /* dig != nxt % base && */  dig * num == nxt {
                print!("{num} * {mul} == {nxt}\n");
            }
        }
        num += 1;
    }
*/
    
    "".to_string()
}


fn parasitic (mul: u8, base: u8) -> String {
    let N:usize = 1000000;
    let mut pow:usize = 10;
    let mut hist = Vec::new();
    
    hist.push(vec![1,2,3,4,5,6,7,8,9]);

    while pow < N {
        let mut line = Vec::new();

        for part in hist.last().unwrap() {
            for dig in 0..10 {
                let fst = part * 10 + dig;
                let scd = dig * pow + part;

                if mul as usize * fst == scd {
                    print!("{mul} * {fst} == {scd}\n");
                }

                line.push(fst);
            }
        }
        print!("\n");

        hist.push(line);
        pow *= 10;
    }


    "".to_string()
}

fn main() {


    parasitic(4, 10);

}
