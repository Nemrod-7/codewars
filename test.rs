use std::sync::Mutex;
static MEM:Mutex<Vec<u32>> = Mutex::new(Vec::new());

fn stream() -> impl Iterator<Item = u32> {
    let mut seq = MEM.lock().unwrap();
    print!("size {} \n", seq.len());

    if seq.len() == 0 {
        let lim = 1e6 as usize;
        let mut sieve = vec![0u64; (lim >> 6) + 1];
        *seq = vec![2];

        (3..lim)
            .step_by(2)
            .for_each(
                |i| if (sieve[i >> 6] >> (i&63)&1) == 0 {
                    seq.push(i as u32);

                    (i*i..lim)
                        .step_by(2 * i)
                        .for_each( |j| sieve[j >> 6] |= 1u64 << (j &63))
                }
                );

        sieve.clear();
    }

    seq.clone().into_iter()
}

fn main () {

    let lim = 1e2 as usize;
    let mut sieve = vec![0; lim + 1];

    (2..(lim as f64).sqrt() as usize)
        .step_by(1)
        .for_each( |i| if sieve[i] == 0 {
            (i*i..lim).step_by(i).for_each( |j| sieve[j] = 1 )
        }
        );


    for i in 2..lim - 2 {
        if sieve[i] == 0 && sieve[i+2] == 0 {
            print!("({} {})", i, i+2);
        }
    }



}
