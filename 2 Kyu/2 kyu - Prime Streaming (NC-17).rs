use std::time::{Instant};

const L1:usize = 32768;
const MAX:usize = 1e9 as usize;
const WHEEL:[usize;48] = [2,4,2,4,6,2,6,4,2,4,6,6,2,6,4,2,6,4,6,8,4,2,4,2,4,8,6,4,6,2,4,6,2,6,6,4,2,4,6,2,6,4,2,4,2,10,2,10];

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
                self.k += WHEEL[self.t];
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
                            j += self.i * WHEEL[v];
                            v = if v == 47 { 0 } else { v + 1 };
                        }
                    }
                    self.i += WHEEL[self.u];
                    self.u = if self.u == 47 { 0 } else { self.u + 1 };
                }
                for i in 0..self.hist.len() {
                    // let k = self.hist[i].0 * 2;
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

    println!("testing a segment around 5 milion");
    let segments: Vec<(u32, [u32; 10])> = vec![
        ( 5132638, [ 88451611, 88451617, 88451641, 88451651, 88451683, 88451743, 88451747, 88451761, 88451777, 88451807, ], ),
        ( 5108684, [ 88015583, 88015597, 88015621, 88015649, 88015661, 88015679, 88015703, 88015721, 88015723, 88015751, ], ),
        ( 5841336, [ 101472037, 101472043, 101472101, 101472109, 101472121, 101472127, 101472143, 101472211, 101472229, 101472233, ], ),
        ( 4948709, [ 85091639, 85091641, 85091653, 85091689, 85091693, 85091701, 85091707, 85091717, 85091719, 85091723, ], ),
    ];
    let chosen_varient = segments[2];
    test_segment(chosen_varient.0, chosen_varient.1);

    println!("testing a segment under 50 milion");
     let segments: Vec<(u32, [u32; 10])> = vec![
         ( 46595985, [ 912088319, 912088327, 912088339, 912088349, 912088357, 912088361, 912088379, 912088423, 912088447, 912088477, ], ),
         ( 47592411, [ 932663491, 932663509, 932663527, 932663533, 932663561, 932663579, 932663603, 932663609, 932663617, 932663657, ], ),
         ( 45004712, [ 879286789, 879286817, 879286829, 879286909, 879286927, 879286957, 879286973, 879287021, 879287041, 879287053, ], ),
         ( 46067662, [ 901201573, 901201583, 901201619, 901201661, 901201667, 901201699, 901201703, 901201723, 901201751, 901201817, ], ),
     ];
     let chosen_varient = segments[1];
     test_segment(chosen_varient.0, chosen_varient.1);
 }

    let duration = start.elapsed();
    println!("Duration: {:?}", duration);
}

fn test_segment(start: u32, numbers: [u32; 10]) {
    let mut prime_iterator = stream();
    for _ in 0..start{
        prime_iterator.next();
    }
    for i in numbers{
        assert_eq!(Some(i), prime_iterator.next(),
        "\nYour result (left) did not match the expected output (right)");
    }
}
