fn count_patterns(dot: char, length: u8) -> u64 {

    if length > 9 { return 0; }

    let fiv:[u64; 10] = [0,1,5,31,154,684,2516,7104,13792,13792];
    let sev:[u64; 10] = [0,1,7,37,188,816,2926,8118,15564,15564];
    let eig:[u64; 10] = [0,1,8,48,256,1152,4248,12024,23280,23280];

    if dot == 'E' { return eig[length as usize]; }
    if (dot as u32  - 'A' as u32) % 2 == 0 { return fiv[length as usize]; }
    return sev[length as usize];
}
