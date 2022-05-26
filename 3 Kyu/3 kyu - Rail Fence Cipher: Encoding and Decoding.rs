fn encode_rail_fence_cipher (text: &str, nrails: usize) -> String {
    
    let mut os = String::new();
    let h = (nrails - 1) * 2;
    
    for i in 0..nrails {
        let mut par = 0;
        let mut j =  i;
        
        while j < text.len() {
            let c = text.chars().nth(j).unwrap();
            os.push(c);
            
            if i == 0 || i == nrails - 1 {
                j += h;
            } else {
                if par % 2 == 0 { j += h - (i * 2); } else { j += i * 2; }
            }
            par += 1;
        }
    }
    
    os
}

fn decode_rail_fence_cipher (text: &str, nrails: usize) -> String {

    let mut buff = vec!['0'; text.len()];
    let mut nxt = text.chars();
    let h = (nrails - 1) * 2;
    
    for i in 0..nrails {
        let mut par = 0;
        let mut j = i;
        
        while j < text.len() {

            buff[j] = nxt.next().unwrap();

            if i == 0 || i == nrails - 1 {
                j += h;
            } else {
                if par % 2 == 0 { j += h - (i * 2); } else { j += i * 2; }
            }
            par += 1;
        }
    }
    
    buff.into_iter().collect()
}
