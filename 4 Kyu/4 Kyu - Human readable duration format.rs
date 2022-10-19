fn format_duration (mut seconds: u64) -> String {

    if seconds == 0 { return "now".to_string() }

    let base:Vec<(u64,&str)> = vec![(31536000,"year"),(86400,"day"),(3600,"hour"),(60,"minute"),(1,"second")];

    let mut count = 0;
    let mut period:Vec<u64> = Vec::new();
    let mut os = String::new();

    for it in &base {
        let dig = seconds / it.0;
        period.push(dig);

        if dig != 0 { count += 1 }
        seconds -= dig * it.0;
    }

    for i in 0..5 {
        if period[i] != 0 {
            count -= 1;
            os += &format! ("{} {}", period[i], base[i].1);

            if period[i] > 1 { os += &format!("s"); }
            if count > 1 { os += &format!(", "); }
            if count == 1 { os += &format!(" and "); }
        }
    }


    os
}
