
use num::BigUint;

struct Fib {
    prev:u32,
    curr:u32,
}
impl Fib {
    fn new () -> Self {
        Fib {
            prev:1,
            curr:1,
        }
    }
}

impl Iterator for Fib {
    type Item = u32;

    fn next (&mut self) -> Option<Self::Item> {

        let sum = self.prev + self.curr;
        self.prev = self.curr;
        self.curr = sum;

        Some (sum)
    }
}


fn stream () -> impl Iterator<Item = u32> {
    vec![1,1].into_iter().chain(Fib::new())
}

fn main () {

    let mut fib = stream();


    for _ in 0..10 {
        let num = fib.next().unwrap();
        print! ("{num} ");

    }
}
