extern crate regex;
use regex::Regex;

fn tokenize (src: &str) -> Vec<String> {
    let token = Regex::new("->|_?\\w+|[{}();,]").unwrap();
    token.captures_iter(src).map(|x| x[0].to_string()).collect::<Vec<_>>()
}
fn valid_expres (expr: &str) -> bool {

    // a() or {}() or {}{} or a(){} or {}(){}
    if Regex::new(r"^[\s\n]*(\w+|(\{[^\{\}]*\}))[\s\n]*(\([^\(\)]*\))?[\s\n]*(\{[^\{\}]*\})?[\s\n]*$").unwrap().find(expr) == None { return false };
    if Regex::new(r"^[\s\n]*\{[^\{\}]*\}[\s\n]*$").unwrap().find(expr).is_some() { return false }   // rejects {...} :  single lambda

    if Regex::new(r"^\s*\(|[^\)\}\s\n]$|^[\s\n]*$").unwrap().find(expr).is_some() { return false }  // the expression begins with '(' or ends with any char exept '}' or ')'
    if Regex::new(r"([^\w]|^)[0-9]+[A-z]+|[^,_\w\{\}\s\n->]").unwrap().find(expr).is_some() { return false }  // bad name : 1a or bad character

    if Regex::new(r"->[^\}]*,[^\}]*\}").unwrap().find(expr).is_some() { return false }              // rejects  -> x , y }
    if Regex::new(r"\{[^\}]$|^[^\{]*\}|^[^\(]*\)|\([^\)]$").unwrap().find(expr).is_some() { return false }

    if Regex::new(r"[\{\(]\s*[,-]|,\s*[\}\)\-]").unwrap().find(expr).is_some() { return false }       // rejects (-> or {-> or (, or {,
    if Regex::new(r"\{[^\{]*\w+\s+\w+[^\}]*->").unwrap().find(expr).is_some() { return false }      // rejects { x y ->

    if Regex::new(r"\{[^>\(]*\w+[\s\n]*,[\s\n]*\w+[^\}-]*\}|,\s+,").unwrap().find(expr).is_some() { return false }   // rejects {a,b ,c} or , ,
    if Regex::new(r"\([^\{]*(\w+|(\{[^\{\}]*\}))[\s\n]+(\w+|(\{[^\{\}]*\}))[^\}]*\)").unwrap().find(expr).is_some() { return false }  // rejects ( a b c )

    true
}
fn valid_braces (expr: &str) -> bool {
    let mut brc = Vec::new();

    for index in 0.. expr.len() {
        let ch = expr.chars().nth(index).unwrap();

        match ch {
            '(' => brc.push(')'),
            '[' => brc.push(']'),
            '{' => brc.push('}'),
            ')' => if Some(ch) != brc.pop() { return false },
            '}' => if Some(ch) != brc.pop() { return false },
            ']' => if Some(ch) != brc.pop() { return false },
            '-' => if expr.chars().nth(index+1) != Some('>') { return false } ,
            '>' => if expr.chars().nth(index-1) != Some('-') { return false } ,
             _  => (),
        }
    }

    brc.is_empty()
}

fn lambda (code :&Vec<String>, end: &mut usize) -> String {
    *end += 1;
    if code[*end] == "}" { return "(){}".to_string() }

    let mut index = *end;
    let mut mid = index;
    let mut os = format!("(");
    let start = index;

    while code[index] != "}" {
        if code[index] == "->" { mid = index; }
        index += 1;
    }

    for it in start..index {
        if code[it] != "," {
            if it == mid {
                os += &format!("){{");
            }

            if it - start < mid - start {
                os += &format!("{}", code[it]);
                if it - start < mid - start - 1 {
                    os += &format!(",")
                }
            } else if code[it] != "->" {
                os += &format!("{};", code[it]);
            }
        }
    }

    *end = index;
    os + "}"
}
fn transpile (expr: &str) -> Result<String, String> {

    if !valid_braces (expr) || !valid_expres (expr)  { return Err (format! ("Hugh?")) }
    let code = tokenize (expr);
    let size = code.len();
    let mut index = 0;
    let mut os = String::new();

    while index != size {
        let tok = &code[index];

        if tok == "(" {

            while code[index] != ")" {

                if code[index] == "{" {
                    os += &lambda (&code, &mut index)
                } else {
                    os += &code[index].to_string()
                }
                index += 1;
            }
            // if os.chars().last() == Some(',') { os.pop(); }
        } else if tok == "{" {
            if index >= 2 {
                if code[index-1] == "}" { os += "(" }
                if code[index-1] == ")" && code[index-2] != "(" { os += "," }
            }
            os += &lambda (&code, &mut index);
        } else {
            os += &format! ("{tok}");

            if code[index+1] != "(" {
                os += "(";
            }
        }

        index += 1;
    }

    let mut cnt = 0;
    for it in os.chars() {
        match it {
            '(' => cnt += 1,
            ')' => cnt -= 1,
            _ => (),
        }
    }

    if cnt > 0 { os += &format!(")") }
    // print!(" => {os}\n");
    return Ok (os)
}

fn main () {

    let expr = "{xJ31FzZujAp   ,32012626  -> vESH9h3M1JG   12044     C5lbSZczACi     9949}  ({jv0hXeAkz5d,   48556321 ,  8790025,   P3arIy90UVg ,   ry5zPoPoKRX   ->   OQMRuNsK74A  8    35572    7360604   94 }   , {  dmvU7SP2wED   }  VcGxpePfPZZ ,  {4,  QJMRpMyp5Wr  ,  25529-> 386954  dufIAW0SLjD   PX1JSLustbu   }  , {416736   ,12144234  ,  DDlVf5hhIHU,  91231263  ,   627 ->  91428207    yZDfdOC6Keu    6031   5} ){3283->  wU5MXXc3SkQ   }";

    let res = transpile (expr);
    // print!("{:?}\n", res);

    test();
/*
    (?<=-)
*/

}
fn form (src: &str) -> Vec<String> {
    let token = Regex::new(r"^[\s\n]*(\w+|(\{[^\{\}]*\}))[\s\n]*(\([^\(\)]*\))?[\s\n]*(\{[^\{\}]*\})?[\s\n]*$").unwrap();
    token.captures_iter(src).map(|x| x[0].to_string()).collect::<Vec<_>>()
}

fn accepts(expr: &str, expected: &str) { do_test(expr, Ok(expected.to_string())); }
fn rejects(expr: &str) { do_test(expr, Err("Hugh?".to_string())); }
fn do_test(expr: &str, expected: Result<String,String>) {
    assert_eq!(transpile(expr), expected, "\nYour result (left) did not match expected output (right) for the expression:\n{expr:?}");
}
fn test() {


        accepts("042()", "042()");
        accepts("call()", "call()");
        accepts("\n \n  1(  \n )\n", "1()");
        accepts("_call(a,b)", "_call(a,b)");

        accepts("call   (    jim ,      my )", "call(jim,my)");

        accepts("{}(x)", "(){}(x)");
        accepts("call({\n})", "call((){})");
        accepts("{p,  5 ->}()", "(p,5){}()");
        accepts("call{666}", "call((){666;})");
        accepts("call(a,b){}", "call(a,b,(){})");


        accepts("{}{}", "(){}((){})");
        accepts("{p ->}()", "(p){}()");
        accepts("call(a, b, {})", "call(a,b,(){})");
        accepts("{a b c}{d->e\nf}", "(){a;b;c;}((d){e;f;})");
        accepts("{x\n->y}(666){4,2->0}", "(x){y;}(666,(4,2){0;})");
        accepts("call({a,b,c->d e}){\n}", "call((a,b,c){d;e;},(){})");

        rejects("abc9_(");
        rejects("f(42a)");

        rejects("name%1&*");

        rejects("f({)");
        rejects("f(){");
        rejects("f(})");

        rejects("call");
        rejects("{}{1a 2}");

        rejects("{u,w,v -> x,y}{}");
        rejects("f({a b -> c})");

        rejects("f({p,t,})");

        rejects("f()()");

        rejects("{}");
        rejects("{a,b->c}");

        rejects ("%^&*(");
        rejects ("x9x92xb29xub29bx120()!(");

        rejects ("f(a,)");
        rejects ("i(,{})");
        rejects ("f (,a)");

        rejects ("a b c");

        rejects ("1a()");       // func name with number ?
        rejects ("f({1a->a})");
        rejects ("call(1a)");
        rejects ("(12,ab)");    // param without expression
        rejects ("f(12,ab)c");

        rejects ("run{->a}");
        rejects ("f(->)");

        rejects ("{a->a}(cde,y,z){x y d -> stuff}");
        rejects ("{a->a}(cde,y,z){x y,d -> stuff}");
        rejects ("{a->a}(cde,y,z){x,    y,d ->stuff,g,h}");


        rejects ("f( a v)");

        rejects ("{a}(cde,y,z){x,y,d jj}");
        rejects ("{a o , p}(cde,y,z){x,y,d,jj}");

        rejects ("");
        rejects ("$call()");
        rejects ("{}{}{}");
        rejects("{JyWcROvDBOn  ,   99-> 84326   18716553 }  {4 ,  161593   66  1358   715749   BurWnFKgHIR   4153  53246599 }");
        rejects ("QRq2xe2XmpC {BMtnT2rwhVF   , 1 ,  ,  wz4uxTyEx72 ->IOrVlTAZ7IT D0ycvp65akz dBRjS6qzodu  67794030 DSexeNW0zGm KCGBKbn0P95  }");
        reject
        s("tAE6l69bP16(9107 , jWZUMmX80uA  ,   84029796   ){5110817   ,  nuXj244xThu  ,  ->FOKLA28f1XC 30020877   H8DcfachlMa cHwHGdOymdQ DBVyPVOsg4m   8   }");

        accepts ("call({},{},{},  {},{}  ,{})","call((){},(){},(){},(){},(){},(){})");
        accepts ("GVGYpa6ob62   {pTGlOqfgQnY,  YOMDes44si3,   rKPntLrcs1V  ,vDFfY49Zmx2,   AJrByI3Cpky   ->  2930370  }","GVGYpa6ob62((pTGlOqfgQnY,YOMDes44si3,rKPntLrcs1V,vDFfY49Zmx2,AJrByI3Cpky){2930370;})");
        accepts ("{653601   -> hZAn2HSnx22    efMH581zDFN      824745      4    3063301 } ({ixpM9l1r7ak, hfynVkvPxtL ->  3     25038    7395 I1gHgvAw2HE }   ){BFEBdI7WFXf    SRSMZmOBDbd    6   }", "(653601){hZAn2HSnx22;efMH581zDFN;824745;4;3063301;}((ixpM9l1r7ak,hfynVkvPxtL){3;25038;7395;I1gHgvAw2HE;},(){BFEBdI7WFXf;SRSMZmOBDbd;6;})");
        rejects("{ 5    19160432    26852}   ({76086094  ,   nG4o570a11t->   J0gZWBZfV9I  vDA88FDTeru }   74  ){89694 ,   ESAf6DQbcnr   , ouACbv83b4C  ,4114008  ->   LWwRqYkP5Um   67087    KQHNqO8cBEt    22  fANQUNM9Kq3}");

        /*

        */
}
