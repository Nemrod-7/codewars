fn factorial (n:u32) -> String {
    let mut fact = String::new();
    let mut carry = 0;
    let mut ndigit = 1;
    let mut buffer = [0;1024];
    buffer[0] = 1;

    for x in 2..=n {
        let mut i = 0;

        while i < ndigit {
            let tmp = buffer[i] * x;
            buffer[i] = (tmp + carry) % 10;
            carry = (tmp + carry) / 10;

            if i == (ndigit - 1) && (carry > 0) {
                ndigit += 1;
            }

            i += 1;
        }
    }

    while ndigit > 0 {
        ndigit -= 1;
        fact += &format! ("{}", buffer[ndigit]);
    }

    fact
}

fn main () {

}
