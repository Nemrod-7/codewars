#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <regex>
#include <algorithm>

#include <iomanip>
#include <chrono>

using namespace std;

vector<string> format3 (const string &input) {
    regex form ("[;|\\w].+");
    sregex_token_iterator it (input.begin(), input.end(), form);
    return vector<string> (it, sregex_token_iterator());
}
vector<string> tokenize (string line) {
    line = line.substr (0, line.find (";"));
    regex tok ("->|_?\\w+|[{}();,]");
    sregex_token_iterator it (line.begin(), line.end(), tok);

    return vector<string> (it, sregex_token_iterator());
}

string mkmsg (map<string, int>& reg, vector<string> &line) {

    string os;
    regex quote ("\".+\"|\'.+\'");

    for (size_t i = 1; i < line.size(); ++i) {
        if (regex_match (line[i], quote)) {
            os += line[i].substr (1, line[i].size() - 2);
        } else {
            os += to_string (reg[line[i]]);
        }
    }

    return os;
}
int get_val (map<string, int>& reg, string op) { return isalpha (op.at(0)) ? reg[op] : stoi (op); }

string assembler_interpreter (string input) {

    bool jump, running = true;
    int val, cmp = 0;
    string com, key, os;

    map<string, int> reg;
    vector<string> code, prog = format3 (input);
    vector<string>::iterator line = prog.begin();
    stack<vector<string>::iterator> pos;
    int cycle = 0;
    while (running) {

          cycle++;

          cout << cycle << " " << *line << endl;
          code = tokenize (*line);
          jump = false;

          com = code.size() > 0 ? code[0] : "";
          key = code.size() > 1 ? code[1] : "";
          val = (code.size() > 2 && com != "msg") ? get_val (reg, code[2]) : 0;

          if (com == "end") return os;
          if (com == "inc") reg[key]++;
          if (com == "dec") reg[key]--;
          if (com == "mov") reg[key] = val;
          if (com == "add") reg[key] += val;
          if (com == "sub") reg[key] -= val;
          if (com == "mul") reg[key] *= val;
          if (com == "div") reg[key] /= val;
          if (com == "cmp") cmp = reg[key] - val;
          if (com == "jmp") jump = true;
          if (com == "jne" && cmp != 0) jump = true;
          if (com == "je"  && cmp == 0) jump = true;
          if (com == "jge" && cmp >= 0) jump = true;
          if (com == "jg"  && cmp  > 0) jump = true;
          if (com == "jle" && cmp <= 0) jump = true;
          if (com == "jl"  && cmp  < 0) jump = true;
          if (com == "call") {
              pos.push (line + 1);
              jump = true;
          }
          if (com == "ret") {
              line = pos.top() - 1;
              pos.pop();
          }
          if (com == "msg") os += mkmsg (reg, code);

          if (jump == true) line = find (prog.begin(), prog.end(), code[1] + ':');

          line++;
          if (line == prog.end()) running = false;
    }

    return "-1";
}
////////////////////////////////////////////////////////////////////////////////
typedef string t_type;

class Assert {
    public:
        static void That (t_type input, t_type expected) {
            static int cycle;

            if (input != expected)
                cout << "error" << " " << cycle << endl;
            cycle++;
        }

};
t_type Equals(t_type entry) {return entry;}

////////////////////////////////////////////////////////////////////////////////
void Test () {
  std::string program;

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

}

int main () {
  auto start = chrono::high_resolution_clock::now();

  string     program = R"(
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

  cout << assembler_interpreter (program);
  //Test ();

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  cout << "\nProcess took " << elapsed.count()  << " ms" << endl;

  return EXIT_SUCCESS;
}
