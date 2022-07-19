

fn SieveOfEratosthenes (num: u64) -> Vec<u64> {

    let end = (num as f64).sqrt();
    let mut primes = vec![0; num];
    let mut sieve = vec![0; num];

    for p in 0..=end {
        if primes[p] == false {
            let mut i = p * p;

            while i <= num {
                primes[i] = true;
                i += p;
            }
        }
    }

    for i in 2..=num {
        if prime[i] == 0 {
            sieve.push(i);
        }
    }

    return sieve;
}
/*
unsigned maxnum (int* list,int size) {

    unsigned max = 0;
    while (size-->0)
        max = MAX (max, abs (NUM));

    return max;
}
char *sumOfDivided (int *list, int l) {

    int sum = 0, max = maxnum (list,l) + 1;
    char *output = malloc (BUFFSIZE * sizeof(char)), *out = output;
    unsigned *primes = SieveOfEratosthenes (max);
    bool divided = false;
    *output = '\0';

    for (int i = 1; i < primes[0]; ++i) {
        for (int size = 0; size < l; ++size)
            if (abs (NUM) % primes[i] == 0)
                sum += NUM, divided = true;

        if (divided == true)
            out += sprintf (out,"(%i %i)", primes[i], sum);

        sum = 0, divided = false;
    }
    return output;
}
*/
fn main() {
    // Statements here are executed when the compiled binary is called

    // Print text to the console
    println!("Hello World!");
}
