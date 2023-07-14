fn josephus<T:Clone+Copy + std::fmt::Debug>(mut items:Vec<T>, k:usize)-> Vec<T> {
    let mut index = 0;
    let mut seq = Vec::new();

    while items.len() != 0 {
        index = (index + (k - 1)) % items.len();
        // print!("{index} ");
        seq.push(items[index]);
        items.remove(index);
        print!("{:?} {}\n", items, k);
    }

    seq
}

fn main () {

    // josephus(vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 1);
    // josephus(vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10], 2);

    // josephus (vec![true, false, true, false, true, false, true, false, true], 9);
    //  right: `[true, true, true, false, false, true, false, true, false]`
    let mut index = 1;
    let mut items = vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10];


    print!("{:?}\n", items);

    // print!("{:?}\n", items);

}
