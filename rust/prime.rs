use std::time::{Instant};

const L1:usize = 32768;
const MAX:usize = 1e9 as usize;
const Wheel:[usize;48] = [2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10];

struct Seq {
    k:usize, t:usize, i:usize, u:usize, low:usize, high:usize,
    sieve:[bool; L1], cache:[bool; L1],
    hist:Vec<(usize,usize)>,
}

impl Seq {
    fn new() -> Self {
        Self {
            k:11, t:0, i:11, u:0, low:0, high:0,
            sieve:[true; L1], cache:[true; L1],
            hist:vec![],
        }
    }
}

impl Iterator for Seq {
    type Item = u32;

    fn next (&mut self) -> Option<Self::Item> {

        let mut prime = 0;

        loop {
            while self.k < self.high {
                if self.cache[self.k - self.low] == true {
                    prime = self.k as u32;
                }
                self.k += Wheel[self.t];
                self.t = if self.t == 47 { 0 } else { self.t + 1 };           

                if prime != 0 {
                    return Some (prime);
                }
            }

            if self.high == MAX {
                print! ("{} {}\n", self.k, self.high);
                return None;
            }
            if self.k >= self.high {
                self.high = std::cmp::min (self.high + L1, MAX);
                self.low = self.high - L1;

                for item in &mut self.cache { *item = true } 

                while self.i * self.i <= self.high { // update sieve
                    if self.sieve[self.i] == true {
                        let mut j = self.i * self.i;
                        let mut v = self.u;
                        self.hist.push ((self.i, self.i * self.i - self.low));
                        while j <= L1 {
                            self.sieve[j] = false;
                            j += self.i * Wheel[v];
                            v = if v == 47 { 0 } else { v + 1 }; 
                        }
                    }
                    self.i += Wheel[self.u];
                    self.u = if self.u == 47 { 0 } else { self.u + 1 };
                }
                for i in 0..self.hist.len() {
                    let k = self.hist[i].0 * 2;
                    let mut j = self.hist[i].1;

                    while j < L1 {
                        self.cache[j] = false;
                        j += self.hist[i].0 * 2;
                    }
                    self.hist[i].1 = j - L1;
                }
            }

        }
    }
}
fn stream () -> impl Iterator<Item = u32> { // Prime Streaming (NC-17)
    let prime = vec![2,3,5,7];
    return prime.into_iter().chain(Seq::new())
}


fn main () {

    let start = Instant::now();

    let limit = 1e6 as usize;
    let mut primes = stream();

    //let num = 100000001;

    //for i in 0..num {
    //    let num = primes.next();

    //    match num {
    //        None => break,
    //        Some(_) => (),
    //    }
    //}

    // 912088319

    //    println!("testing 1,000, streams up to 1,000");
    for _ in 0..1_000 {
        let segments: Vec<(u32, [u32; 10])> = vec![
            ( 9561, [ 99667, 99679, 99689, 99707, 99709, 99713, 99719, 99721, 99733, 99761, ],),
            ( 9428, [ 98143, 98179, 98207, 98213, 98221, 98227, 98251, 98257, 98269, 98297, ],),
            ( 9088, [ 94229, 94253, 94261, 94273, 94291, 94307, 94309, 94321, 94327, 94331, ],),
            ( 9560, [ 99661, 99667, 99679, 99689, 99707, 99709, 99713, 99719, 99721, 99733, ],),
        ];
        let chosen_varient = segments[0];
        test_segment(chosen_varient.0, chosen_varient.1);
    }

    //let segment: Vec<(u32, [u32; 10])>; 
    println!("testing a segment under 10 thousand");
    let segments: Vec<(u32, [u32; 10])> = vec![
        ( 9661, [ 100847, 100853, 100907, 100913, 100927, 100931, 100937, 100943, 100957, 100981, ],),
        ( 9025, [ 93481, 93487, 93491, 93493, 93497, 93503, 93523, 93529, 93553, 93557, ],),
        ( 9514, [ 99109, 99119, 99131, 99133, 99137, 99139, 99149, 99173, 99181, 99191, ],),
        ( 9068, [ 93983, 93997, 94007, 94009, 94033, 94049, 94057, 94063, 94079, 94099, ],),
    ];
    let chosen_varient = segments[0];
    test_segment(chosen_varient.0, chosen_varient.1);

    println!("testing a segment under 100 thousand");
    let segments: Vec<(u32, [u32; 10])> = vec![
        ( 98918, [ 1284433, 1284443, 1284467, 1284473, 1284487, 1284511, 1284523, 1284541, 1284551, 1284553, ],),
        ( 95154, [ 1231421, 1231423, 1231453, 1231457, 1231459, 1231469, 1231481, 1231487, 1231511, 1231513, ],),
        ( 97647, [ 1266389, 1266409, 1266413, 1266431, 1266451, 1266469, 1266487, 1266491, 1266493, 1266511, ],),
        ( 96062, [ 1244479, 1244483, 1244501, 1244521, 1244531, 1244533, 1244543, 1244567, 1244591, 1244603, ],),
    ];
    let chosen_varient = segments[0];
    test_segment(chosen_varient.0, chosen_varient.1);

    println!("testing a segment under 1 milion");
    let segments: Vec<(u32, [u32; 10])> = vec![
        ( 981067, [ 15170977, 15170993, 15171001, 15171017, 15171077, 15171103, 15171113, 15171119, 15171173, 15171199, ],),
        ( 976019, [ 15087967, 15088019, 15088039, 15088043, 15088049, 15088063, 15088079, 15088097, 15088111, 15088127, ],),
        ( 968915, [ 14970313, 14970323, 14970331, 14970337, 14970341, 14970343, 14970349, 14970359, 14970383, 14970419, ],),
        ( 962695, [ 14866543, 14866547, 14866571, 14866573, 14866613, 14866627, 14866633, 14866651, 14866673, 14866681, ],),
    ];
    let chosen_varient = segments[0];
    test_segment(chosen_varient.0, chosen_varient.1);

    let duration = start.elapsed();    
    println!("Duration: {:?}", duration);
}

fn test_segment(start: u32, numbers: [u32; 10]){
    let mut prime_iterator = stream();
    for _ in 0..start{
        prime_iterator.next();
    }
    for i in numbers{
        assert_eq!(Some(i), prime_iterator.next(),
        "\nYour result (left) did not match the expected output (right)");
    }
}
/*

   fn segsoe (limit: usize) -> Vec<u32> { 
   let mem = (limit as f64).sqrt().ceil() as usize;

   let mut i = 5;
   let mut n = 5;
   let mut t = 2;
   let mut u = 2;

   let mut vs = vec![2,3];
   let mut mul = vec![];
   let mut primes = vec![];
   let mut cache = vec![true; mem];
   let mut is_prime = vec![true; mem];

   for low in (0..= limit).step_by(mem) {
   let high = std::cmp::min (low + mem - 1, limit);
   for item in &mut cache { *item = true }
// generate sieving primes using simple cache of Eratosthenes
while i * i <= high {
if is_prime[i >> 1] == true {
primes.push(i);
mul.push(i * i - low);
let mut j = i * i;
let mut v = t;

while j <= mem {
is_prime[j >> 1] = false;
j += t * i;
}
}
i += t;
}
// cache the current segment
for p in 0..primes.len() {
let mut j = mul[p];
let mut k = primes[p] * 2;

while j < mem {
cache[j] = false;
j += k;
}
mul[p] = j - mem;
}

while n <= high {
if cache[n - low] == true {
vs.push (n as u32);
}

n += u;
u = 6 - u;
}
}

vs
}
use std::sync::Mutex;
static MEM:Mutex<Vec<u32>> = Mutex::new(Vec::new());

fn stream2 () -> impl Iterator<Item = u32> { // prime streaming (PG-13)
let mut seq = MEM.lock().unwrap();

if seq.len() == 0 {
let lim = 1e8 as usize;
let thd = (lim / 3) + 1;
let mut p = 5;
let mut step = 2;
let mut sieve = vec![true; thd];
 *seq = vec![2,3];

while p <= lim {
    if sieve[p / 3] == true {
        seq.push(p as u32);
        let mut j = p * p;
        let mut t = step;

        while j <= lim {
            sieve[j / 3] = false;
            j += p * t;
            t = 6 - t;
        }
    }
    p += step;
    step = 6 - step;
}
sieve.clear();
}

seq.clone().into_iter()
    }


struct Seq1 {
    k:u32,
    t:u32,
}
impl Seq1 {
    fn new() -> Self {
        Self {
            k:5,
            t:2,
        }
    }
}
impl Iterator for Seq1 {
    type Item = u32;

    fn next (&mut self) -> Option<Self::Item> {

        loop {
            self.k += self.t;
            self.t = 6 - self.t;

            if isprime(self.k) == true {
                break;
            }
        }

        Some(self.k)
    }
}
fn isprime (n: u32) -> bool {
    if n < 4 {
        n > 1
    } else if n % 2 == 0 || n % 3 == 0 {
        false
    } else {
        let end = (n as f64).sqrt().ceil() as u32;
        match (5..=end).step_by(6).find(|x| n % x == 0 || n % (x+2) == 0) {
            Some (_) => false,
            None => true,
        }   
    }
}
fn stream3 () -> impl Iterator<Item = u32> {
    let prime = vec![2,3,5];
    return prime.into_iter().chain(Seq1::new())
}
*/
