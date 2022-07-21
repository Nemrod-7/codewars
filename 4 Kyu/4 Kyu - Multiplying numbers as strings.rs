fn reverse (src: &str) -> Vec<u8> {
    src.bytes().rev().collect::<Vec<u8>>()
}
fn multiply(a: &str, b: &str) -> String {

    if a == "0" || b == "0" { return "0".to_string() }

    let a = reverse (a);
    let b = reverse (b);
    let mut os:String = String::new();
    let mut ans = [0; 1000];
    let mut size = 0;

    for i in 0..a.len() {
        for j in 0..b.len() {
            ans[i + j] += (a[i] - 0x30) * (b[j] - 0x30);
            ans[i + j + 1] = ans[i + j + 1] + ans[i + j] / 10;
            ans[i + j] %= 10;
            size = i + j + 1;
        }
    }
    while ans[size] == 0 { size -= 1 }

    loop {
        os.push ((ans[size] + 0x30) as char);
        if size == 0 { return os }
        size -= 1;
    }
}
