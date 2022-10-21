
fn simple_cases() {
    assert_eq!(interpreter("*e*e*e*es*es*ws*ws*w*w*w*n*n*n*ssss*s*s*s*", 0, 6, 9), "000000\r\n000000\r\n000000\r\n000000\r\n000000\r\n000000\r\n000000\r\n000000\r\n000000", "Your interpreter should initialize all cells in the datagrid to 0");
    assert_eq!(interpreter("*e*e*e*es*es*ws*ws*w*w*w*n*n*n*ssss*s*s*s*", 7, 6, 9), "111100\r\n000000\r\n000000\r\n000000\r\n000000\r\n000000\r\n000000\r\n000000\r\n000000", "Your interpreter should adhere to the number of iterations specified");
    assert_eq!(interpreter("*e*e*e*es*es*ws*ws*w*w*w*n*n*n*ssss*s*s*s*", 19, 6, 9), "111100\r\n000010\r\n000001\r\n000010\r\n000100\r\n000000\r\n000000\r\n000000\r\n000000", "Your interpreter should traverse the 2D datagrid correctly");
    assert_eq!(interpreter("*e*e*e*es*es*ws*ws*w*w*w*n*n*n*ssss*s*s*s*", 42, 6, 9), "111100\r\n100010\r\n100001\r\n100010\r\n111100\r\n100000\r\n100000\r\n100000\r\n100000", "Your interpreter should traverse the 2D datagrid correctly for all of the \"n\", \"e\", \"s\" and \"w\" commands");
    assert_eq!(interpreter("*e*e*e*es*es*ws*ws*w*w*w*n*n*n*ssss*s*s*s*", 100, 6, 9), "111100\r\n100010\r\n100001\r\n100010\r\n111100\r\n100000\r\n100000\r\n100000\r\n100000", "Your interpreter should terminate normally and return a representation of the final state of the 2D datagrid when all commands have been considered from left to right even if the number of iterations specified have not been fully performed");
}

fn pretty_print(datagrid: &str) -> &str {
    let rows = datagrid.split("\r\n");
    let mut output = String::new();
    output += "<pre>";
    for row in rows {
        for cell in row.chars() {
            output += "<span style=\"color:";
            output += if cell == '0' { "black" } else { "white" };
            output += ";background-color:";
            output += if cell == '0' { "black" } else { "white" };
            output += "\">xx</span>";
        }
        output += "<br />";
    }
    output += "</pre>";
    //println!("{}", output);
    datagrid
}
fn display_actual(actual: &str) -> &str {
    println!("You returned:");
    pretty_print(actual)
}
fn display_expected(expected: &str) -> &str {
    println!("Expected final state of data grid:");
    pretty_print(expected)
}

fn find_next (code: &Vec<char>, pos:usize) -> usize {

    let size = code.len();

    let mut index = pos;
    let mut pile = 0;
    let mut fwrd:bool = true;
    if code[index] == ']' { fwrd = false };

    while index < size {
        if code[index] == '[' { pile += 1 }
        if code[index] == ']' { pile -= 1 }

        if pile == 0 { return index }
        if fwrd == true { index +=1 } else { index -= 1 }
    }
    return index;
}
fn interpreter (code: &str, mut iters: usize, width: usize, height: usize) -> String {

    let code = code.chars().collect::<Vec<_>>();

    let mut index = 0;
    let mut p = (0, 0);
    let mut os = String::new();
    let mut grid = vec![vec![0;width];height];

    while iters > 0 && index < code.len() {

        match code[index] {
            '*' => { grid[p.1][p.0] ^= 1 },
            'e' => if p.0 < width - 1 { p.0 += 1 } else { p.0 = 0 },
            'w' => if p.0 > 0 { p.0 -= 1 } else { p.0 = width - 1 },
            'n' => if p.1 > 0 { p.1 -= 1 } else { p.1 = height - 1 },
            's' => if p.1 < height - 1 { p.1 += 1 } else { p.1 = 0 },
            '[' => if grid[p.1][p.0] == 0 { index = find_next (&code, index) },
            ']' => if grid[p.1][p.0] != 0 { index = find_next (&code, index) },
             _  => iters += 1,
        }

        iters -= 1;
        index += 1;
    }

    for row in grid {
        let row = row.iter().map(|x| x.to_string() ).collect::<String>();
        os += &format!("{row}\r\n");
    }
    os.pop();
    os.pop();

    os
}

fn main() {

    let res = interpreter("*[es*]",37,5,6);

    print!("{res}");

    print!("\nend\n");

}
