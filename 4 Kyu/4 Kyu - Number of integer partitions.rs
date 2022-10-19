fn partitions(num: u32) -> u32 {

    let mut cluster:Vec<u32> = vec![0;num as usize];
    let mut index:usize = 0;
    let mut prev:u32 = 0;
    let mut cnt:u32 = 0;

    cluster[index] = num + 1;

    while cluster[0] > 1 {

        let mut digit = cluster[index] - 1;
        let mut next = index;
        let mut rem = prev;

        while rem < num {
            if num - rem >= digit {
                if digit > 1 {
                    index = next;
                    prev = rem;
                }

                cluster[next] = digit;
                rem += digit;
                next += 1;
            } else {
                digit -= 1;
            }
        }

        cnt += 1;

        if index > 0 && cluster[index] == 1 {
            index -= 1;
            prev -= cluster[index];
        }
    }

    cnt
}
