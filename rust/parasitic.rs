#![allow(dead_code, unused)]


fn parasitic (dig:u8, base:u8) -> String {
    let mut num = vec![dig];

    for _ in 0..23 {
        let mut size = num.len() + 1;
        let mut ans = vec![0; size + 1];

        num = num.into_iter().rev().collect();

        for j in 0..num.len() {
            ans[j] += dig * num[j];
            ans[j + 1] = ans[j + 1] + ans[j] / base;
            ans[j] %= base;
        }

        while ans[size] == 0 { size -= 1 }
        if ans[size] < dig { size -= 1 }

        num = (0..=size).rev().map(|x| ans[x]).collect();
        num.push(dig);

        if ans[size] == 0 {
            let res = "1".to_string() + &num.iter().map(|x| (x + 0x30) as char ).collect::<String>();
            // print!("res : [{}]\n", res);
            return res;
        }

        // print!("{} {:?}\n", size, num);
    }

    "".to_string()
}
fn main() {

/*
1 => []
2 => [105263157894736842]
3 => [10689655172413793]
4 => [102564]
5 => []
6 => [10677966]
7 => [105797]
8 => [1012658227848]
9 => [10224719]

let dyson = vec![0, 1, 105263157894736842, 1034482758620689655172413793, 102564, 142857,
 	1016949152542372881355932203389830508474576271186440677966,
 	1014492753623188405797, 1012658227848,
 	10112359550561797752808988764044943820224719]
*/

    let dig = 3;
    let base = 10;


    for dig in 1..10 {

        let res = parasitic(dig,10);
        print!("{dig} => [{res}]\n");
    }

}
