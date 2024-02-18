#![allow(dead_code, unused)]

#[derive(Debug, PartialEq, Eq)]
enum Cons<T: Clone> {
    Cons(T, Box<Cons<T>>),
    Null
}

impl<T: Clone> Cons<T> {
    pub fn new(head: T, tail: Self) -> Self {
        Cons::Cons(head, Box::new(tail))
    }

    pub fn to_vec(&self) -> Vec<T> {
        match self {
            &Cons::Null => vec![],
            &Cons::Cons(ref head, ref tail) => {
                let mut head = vec![head.clone()];
                head.extend(tail.to_vec());
                head
            }
        }
    }

    pub fn from_iter<I: IntoIterator<Item = T>> (list: I) -> Cons<T> {
        let mut it = list.into_iter();

        match it.next() {
            Some(head) => Cons::new(head, Cons::from_iter(it)),
            None => Cons::Null,
        }
    }

    pub fn filter<F: Fn(&T) -> bool> (&self, f: F) -> Self {

        match self {
            &Cons::Null => Cons::Null,
            &Cons::Cons(ref head, ref tail) => {
                if f(head) == true {
                    Cons::new(head.clone(), tail.filter(f))
                } else {
                    tail.filter(f)
                }
            },
        }
    }

    pub fn map<F: Fn(&T) -> T> (&self, f: F) -> Self {

        match self {
            &Cons::Null => Cons::Null,
            &Cons::Cons(ref head, ref tail) => {
                Cons::new(f(head).clone(), tail.map(f))
            },
        }
    }

}


fn main () {

    let numbers = Cons::new(1, Cons::new(2, Cons::new(3, Cons::new(4, Cons::new(5, Cons::Null)))));
    let numbers = Cons::from_iter(vec![1,2,3,4,5]);

    //let res = numbers.filter(|x| x % 2 == 0).to_vec();  // yields [2,4]
    let digits = Cons::from_iter(vec!["1","2","3","4","5"]).to_vec();
    let res = numbers.map(|x| x * x).to_vec();  // yields [1,4,9,16,25]

    //print!("{:?}\n", res);

    // let ints = digits.map(str::parse::<i32>)
    //     .map(Result::unwrap)
    //     .filter(|&n| n > 3)
    //     .into_vec()  // yields [4,5]

    //    The static method Cons.fromArray (or from_array, from_iter) produces Cons(1, Cons(2, Cons(3, Cons 4, Cons 5, null))))).
    //    Above filter creates a new list: Cons(2, Cons(4, null)).
    //    So does above map: Cons(1, Cos(4, Cons(9, Cons(16, Cons(25, null))))).

}
