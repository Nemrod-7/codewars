
fn backtrack (light: Vec<usize>, switch:&Vec<Vec<usize>>) -> bool {
    if light.iter().filter(|x| x ==&&1).count() == light.len() {
        return true;
    }

    for curr in switch {
        let comb = &curr;
        let next = light.clone();

        for i in comb {
            next[*i] ^= 1;
        }

    }

    false 
}
fn main () {

    let switch:Vec<Vec<usize>> = vec![vec![0,1,2],vec![1,2],vec![1,2,3,4],vec![1,4]];
    let N = 5;
    //using namespace std;
    let mut light = vec![0;N];
    let N = switch.len();

    let mut cnt = light.iter().filter(|x| x == &&1).count();

    let curr = &switch[0];

    //print!("{:?} {}\n", light, cnt);
}
