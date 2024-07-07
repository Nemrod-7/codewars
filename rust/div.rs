
fn bouncy(num:u32) -> bool {

    let mut inc = true;
    let mut dec = true;
    let mut last = num % 10;

    while num > 0 {
        let mut curr = num % 10;

        if curr > last {
            dec = false;
        } else if curr < last {
            int = false;
        }

        last = curr; 
        num /= 10;
    }

    inc == false && dec == false
}
fn main () {

    let mut num = 123456;



}
