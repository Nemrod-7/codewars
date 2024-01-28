
fn main () {
    let arr = [66, 36, 49, 40, 73, 12, 77, 78, 76, 8, 50, 20, 85, 22, 24, 68, 26, 59, 92, 93, 30];


    let _size = arr.len();
    //let _cnt = arr.iter().filter(|&&x| sieve[x as usize] == 0).count();
    let mut i; 
    let mut nprime = 0;
    let mut maxv = 0;
    let mut res = 0;

    for num in arr {
        //print!("{num} => ");
        let mut ndiv = 0;
        i = 1;

        while i * i <= num {
            if num % i == 0 {

                if num / i == i {
                    ndiv += 1;
                } else {
                    ndiv += 2;
                }
                //print!("{} ", i);
            }

            i += 1;
        }

        if ndiv == 2 {
            nprime += 1;
        } else if ndiv > maxv {
            maxv = ndiv;
            res = num;
        }
        //print!("\n");
        //print!(":: {}\n", ndiv);
    }

    print!("{} {} {} {}\n", arr.len(),  );
    //print!("size : {} ", arr1.len());
}
