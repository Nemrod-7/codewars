

fn main () {

    let max = 100;
    let  mut sieve = vec![0u32; max];
    let mut i = 3;

    while i < max {
        if (sieve[i >> 5] >> (i &31) &1u32) == 0 {
            print!("{} ", i);

            let mut j = i * i;


            while j < max {
                sieve[j >> 5] |= 1u32 << (j &31);
                j += i;
            }
        }

        i += 2;
    }


  }
