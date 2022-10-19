mod solution {
    pub fn range_extraction(args: &[i32]) -> String {

        let size = args.len();
        let mut minval:i32 = args[size - 1];
        let mut maxval:i32 = args[0];
        let mut cnt:i32 = 0;
        let mut nxt;
        let mut os = String::new();

        for i in 0..size {
            minval = std::cmp::min(minval, args[i]);
            maxval = std::cmp::max(maxval, args[i]);
            cnt += 1;
            if i < size - 1 { nxt = args[i+1] } else { nxt = 0 }

            if args[i] + 1 != nxt {

                if cnt < 2 {
                    os += &format! ("{minval}")
                } else if cnt == 2 {
                    os += &format! ("{minval},{maxval}")
                } else {
                    os += &format! ("{minval}-{maxval}")
                }

                if i + 1 < size { os += &format! (",") }

                minval = nxt;
                cnt = 0;
            }
        }

        os
    }
}
