

fn show(mut num:u32) {
    let mut dig = 0;

    while num  > 0 {
        if num &1 == 1 {
            print!("{dig}");
        }

        dig += 1;
        num >>= 1;
    }

    print!("\n");
}


fn main () {

    let list = [
        vec![0, 1, 2],    // switch 0 controls lights 0, 1, 2
        vec![1, 2],       // switch 1 controls lights 1, 2
        vec![1, 2, 3, 4], // switch 2 controls lights 1, 2, 3, 4
        vec![1, 4]        // switch 3 controls lights 1, 4
    ];

    let mut comb = vec![0u32; list.len()];

    for i in 0..list.len() {
        for num in &list[i] {
            comb[i] |= 1 << num ;
        }
    }

    let a = comb[0];




    print!("\nend\n");

}
