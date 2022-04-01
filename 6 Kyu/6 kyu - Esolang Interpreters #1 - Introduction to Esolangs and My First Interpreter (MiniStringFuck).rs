fn my_first_interpreter(code: &str) -> String {
    
    let mut os = String::new();
    let mut mem = 0u8;
    
    for c in code.chars() {  
        match c {            
            '+' => mem = mem.wrapping_add (1),        
            '.' => os.push (mem as char),
             _  => ()       
        }      
    }
    
    os
}
