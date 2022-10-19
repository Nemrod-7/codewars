fn get_pins(observed: &str) -> Vec<String> {

    let pad = ["08","124","2135","326","4157","52468","6359","748","80579","968"];

    let code:Vec<usize> = observed.chars().map(|x| x.to_string().parse::<usize>().unwrap()).collect();

    let mut index = code.iter().map(|&x| pad[x].len()).product::<usize>();
    let mut i = 0;
    let mut out:Vec<String> = vec!["".to_string(); index];

    while index > 0 {
        index -= 1;

        for dig in &code {
            let key = pad[*dig];

            out[index].push(key.chars().nth(i % key.len()).unwrap());

            i /= key.len();
        }
        i = index;
    }

    out
}
