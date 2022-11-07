
fn next_smaller_number (n: u64) -> Option<u64> {

  v  let mut num = n.to_string().chars().collect::<Vec<_>>();
    let len = num.len();
    let mut dig = len - 1;
    let mut end = len;

    loop {
        dig -= 1;
        if num[dig] > num[dig + 1] { break }
        if dig == 0 { return None }
    }

    loop {
        end -= 1;
        if num[end] < num[dig] { break }
    }

    num.swap(end,dig);
       
    for i in dig+1 ..len {
        for j in i+1 .. len {
            if num[i] < num[j] {
                num.swap(i,j)
            }
        }
    }

    match num[0] {
        '0' => return None,
         _  => {
              let res = num.iter().map(|x| x.to_string()).collect::<String>();
              Some(res.parse::<u64>().unwrap()) 
        },
    }
}
fn main () {

    
        assert_eq!(Some(12), next_smaller_number(21));
        assert_eq!(Some(513), next_smaller_number(531));
        assert_eq!(None, next_smaller_number(1027));
        assert_eq!(Some(414), next_smaller_number(441));

      
        imax" => 1,
        assert_eq!(Some(790), next_smaller_number(907));
        print!("\n");               
}
