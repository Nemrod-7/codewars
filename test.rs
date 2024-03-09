
fn decomp (mut num: u64) -> u64 {
    let mut sum = 0;

    while num > 1 {
        sum += num % 10;
        num /= 10;
    }
    sum
}
fn main () {


    let mut num =  81;



    for dig in 1..10 {
        let mut pw = 1;

        for it in 1..10 {
            pw *= dig;
            print!("{pw} ");
        }
        print!("\n");
    }


}
