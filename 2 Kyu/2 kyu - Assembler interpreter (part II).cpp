#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <stack>

#include <iomanip>
#include <chrono>

using namespace std;

const char endf = '\n', comment = ';', space = ' ', apos = 39 , colon = ':', comma = ',';

int apostr (const string &in) { return count (in.begin(), in.end(), apos); }
void sanitize (string &str) { str.erase ( remove (str.begin(), str.end(), space), str.end()); }
vector<string> tokenize (const string &input) {
    vector<string> v;
    string sub;
    int start = 0, end = input.find (space), apost;
    v.push_back(input.substr (start, end));

    while (end != string::npos && end < input.size()) {
        start = end + 1, end = input.find (comma, start);
        sub = input.substr (start, end - start);
        apost = apostr (sub);

        if (apost == 1) {
            end = input.find (comma, end + 1);
            sub = input.substr (start, end - start);
        }

        if (apost == 0)
            sanitize (sub);

        v.push_back(sub);
    }

    return v;
}
vector<string> format2 (const string &input) {

    istringstream iss (input);
    vector<string> v;
    string line;
    int pos, end;
    while (getline (iss, line)) {
        pos = 0;
        while ((line[pos]) == space) pos++;

        string sanit = line.substr (pos, line.find (';') - pos);
        if (sanit.size() != 0) {
            cout << sanit << endl;
            v.push_back (sanit);
        }
    }

    return v;
}
string mk_msg (const string &line) {
    int start = line.find (apos, 0) + 1, end = line.find (apos, start);
    return line.substr (start, (end - start));
}

static int get_val (map<string, int>& reg, string op) { return isalpha (op.at(0)) ? reg[op] : stoi (op); }
std::string assembler_interpreter (std::string input) {
    int val, cmp = 0;
    bool jump;
    string com, key, label;
    string os;

    map<string, int> reg;
    vector<string> op, prog = format2 (input);
    vector<string>::iterator it = prog.begin();
    stack<vector<string>::iterator> pos;

    while (it != prog.end()) {

          op = tokenize (*it);

          com = op[0];
          if (op.size() > 1)
              key = op[1];

          val = (op.size() > 2 && com != "msg") ? get_val (reg, op[2]) : 0;
          jump = false;

          if (com == "mov") reg[key] = val;
          if (com == "inc") reg[key]++;
          if (com == "dec") reg[key]--;
          if (com == "add") reg[key] += val;
          if (com == "sub") reg[key] -= val;
          if (com == "mul") reg[key] *= val;
          if (com == "div") reg[key] /= val;
          if (com == "cmp") cmp = reg[key] - val;

          if (com == "msg")
              for (size_t i = 1; i < op.size(); ++i)
                  os += apostr(op[i]) == 2 ? mk_msg (op[i]) : to_string (reg[op[i]]);

          if (com == "end")
              return os;

          if (com == "call") {
              pos.push (it + 1);
              jump = true;
          }
          if (com == "ret") {
              it = pos.top() - 1;
              pos.pop();
          }
          if (com == "jmp") jump = true;
          if (com == "jne" && cmp != 0) jump = true;
          if (com == "je"  && cmp == 0) jump = true;
          if (com == "jge" && cmp >= 0) jump = true;
          if (com == "jg"  && cmp  > 0) jump = true;
          if (com == "jle" && cmp <= 0) jump = true;
          if (com == "jl"  && cmp  < 0) jump = true;

          if (jump == true)
              it = find (prog.begin(), prog.end(), op[1] + ':');

          it++;
    }

    return "-1";
}
////////////////////////////////////////////////////////////////////////////////
typedef string t_type;

class Assert {
    public:
        static void That(t_type input, t_type expected) {
            if (input != expected)
                cout << "error"<< endl;
        }

};
t_type Equals(t_type entry) {return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////

int main () {
  auto start = chrono::high_resolution_clock::now();

  //assembler({"mov a -10", "mov b a", "inc a", "dec b", "jnz a -2"});
  string program = R"(
; My first program
mov  a, 5
inc  a
call function
msg  '(5+1)/2 = ', a    ; output message
end

function:
div  a, 2
ret)";



  Test ();

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  cout << "\nProcess took " << elapsed.count()  << " ms" << endl;
  return EXIT_SUCCESS;
}
void display_code (vector<string> &code) {
    for (auto it : code) {
        cout << it << endl;
    }
}
void display_map (map<string, int> input) {
    for (auto now : input)
        cout<<"["<< now.first<<","<<setw(3)<<setfill(' ')<<now.second<<"]\n";
}
void Test () {
  std::string program;
  /*
{ // => ok
  program = R"(
; My first program
mov  a, 5
inc  a
call function
msg  '(5+1)/2 = ', a    ; output message
end

function:
    div  a, 2
    ret)";
    Assert::That(assembler_interpreter(program), Equals("(5+1)/2 = 3"));
}
*/
{ // => ok

  program = R"(
mov   a, 5
mov   b, a
mov   c, a
call  proc_fact
call  print
end

proc_fact:
    dec   b
    mul   c, b
    cmp   b, 1
    jne   proc_fact
    ret

print:
    msg   a, '! = ', c ; output text
    ret
)";
  Assert::That(assembler_interpreter(program), Equals("5! = 120"));
}
/*
{ // => ok
  program = R"(
mov   a, 8            ; value
mov   b, 0            ; next
mov   c, 0            ; counter
mov   d, 0            ; first
mov   e, 1            ; second
call  proc_fib
call  print
end

proc_fib:
cmp   c, 2
jl    func_0
mov   b, d
add   b, e
mov   d, e
mov   e, b
inc   c
cmp   c, a
jle   proc_fib
ret

func_0:
mov   b, c
inc   c
jmp   proc_fib

print:
msg   'Term ', a, ' of Fibonacci series is: ', b        ; output text
ret)";
     Assert::That(assembler_interpreter(program), Equals("Term 8 of Fibonacci series is: 21"));
}
{ // => ok
  program = R"(
mov   a, 11           ; value1
mov   b, 3            ; value2
call  mod_func
msg   'mod(', a, ', ', b, ') = ', d        ; output
end

; Mod function
mod_func:
mov   c, a        ; temp1
div   c, b
mul   c, b
mov   d, a        ; temp2
sub   d, c
ret)";
   Assert::That(assembler_interpreter(program), Equals("mod(11, 3) = 2"));
}
{ // => ok
  program = R"(
mov   a, 81         ; value1
mov   b, 153        ; value2
call  init
call  proc_gcd
call  print
end

proc_gcd:
cmp   c, d
jne   loop
ret

loop:
cmp   c, d
jg    a_bigger
jmp   b_bigger

a_bigger:
sub   c, d
jmp   proc_gcd

b_bigger:
sub   d, c
jmp   proc_gcd

init:
cmp   a, 0
jl    a_abs
cmp   b, 0
jl    b_abs
mov   c, a            ; temp1
mov   d, b            ; temp2
ret

a_abs:
mul   a, -1
jmp   init

b_abs:
mul   b, -1
jmp   init

print:
msg   'gcd(', a, ', ', b, ') = ', c
ret)";
   Assert::That(assembler_interpreter(program), Equals("gcd(81, 153) = 9"));
}
{ // => ok
  program = R"(
call  func1
call  print
end

func1:
call  func2
ret

func2:
ret

print:
msg 'This program should return -1')";
   Assert::That(assembler_interpreter(program), Equals("-1"));
}
{ // => ok
  program = R"(
mov   a, 2            ; value1
mov   b, 10           ; value2
mov   c, a            ; temp1
mov   d, b            ; temp2
call  proc_func
call  print
end

proc_func:
cmp   d, 1
je    continue
mul   c, a
dec   d
call  proc_func

continue:
ret

print:
msg a, '^', b, ' = ', c
ret)";
  Assert::That(assembler_interpreter(program), Equals("2^10 = 1024"));
  }

  */
}
//////////////////////////////// arkive ////////////////////////////////////////

bool is_msg (const string &in) { return count(in.begin(), in.end(), apos) == 2; }
string getcom (const string &str, int pos) {
  int ini = pos, nxt = str.find (space, pos + 1);
  int len = nxt - pos;
  return str.substr (ini, len);
}
string get_line2 (const string &input, string::iterator &it) {
    string code;
    bool cpy = true;
    //if (input[it] == endf) it++;

    for (; it < input.end(); it++) {
        if (*it == endf) break;
        if (*it == comment) cpy = false;
        if (cpy == true) code += *it;
    }

    return code;
}
string get_line (const string &input, int &it) {
    string code;
    bool cpy = true;
    if (input[it] == endf) it++;
    for (; it < input.size(); it++) {
        if (input[it] == endf) break;
        if (input[it] == comment) cpy = false;
        if (cpy == true) code += input[it];
    }
    return code;
  }
  /*
  vector<vector<string>> mk_pgm (const string &program) {
      istringstream iss (program);
      string line, sub;
      vector<vector<string>> code;

      while (getline (iss, line)) {
          string sanit = line.substr (0, line.find (comment));
          if (sanit.size() != 0) {
              vector<string> v;
              int start = 0, end = line.find (space);
              v.push_back(sanit.substr (start, end));  //cout << '[' << sanit.substr (start, end) << ']';

              while (end != string::npos && end < sanit.size()) {
                  start = end + 1, end = sanit.find (comma, start);
                  sub = sanit.substr (start, end - start); //cout << '[' << sub << ']';

                  if (count(sub.begin(), sub.end(), apos) == 2)
                      v.push_back(sub);
                  else
                      v.push_back(sanitize(sub));
              }
              code.push_back(v); //cout << endl;
          }
      }
      return code;
  }
  */
std::map<std::string, int> assembler(const std::vector<std::string>& input) {

    map<string,int> reg;
    vector<string>::const_iterator it(input.begin());
    //istringstream iss;
    string com, op, key;

    // auto val = [&](string ins)->int { return isalpha (ins[0]) ? reg[ins] : stoi (ins); };

    while (it != input.end()) {

        op  = it->substr (0,3);
        key = it->at (4);
        int pam1 = isdigit (key[0]) ? stoi (key) : reg[key];

        if (op == "mov") reg[key] = isalpha(it->at(6)) ? reg[{it->at(6)}] : stoi(it->substr(6));
        if (op == "inc") reg[key]++;
        if (op == "dec") reg[key]--;
        if (op == "jnz" && pam1 != 0) {
            int end = input.end() - it, offset = stoi(it->substr(6));
            it += end > offset ? offset : end;
        } else
            it++;
        display_map (reg);
        cout << endl;
    }

    return reg;
}
/*
*/
