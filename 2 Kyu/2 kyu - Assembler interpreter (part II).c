#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BUFFSIZE 64

struct _line {
    char **com;
    size_t size;
};

const char endf = '\n',space = ' ', apos = 39;
enum {mov,inc,dec,add,sub,mul,divi,cmp,ret,msg,end,call,jmp,jne,je,jge,jg,jle,jl};

int count_char (const char *str, const char cmp) {
  size_t count = 0;
  while (*str) if (*str++ == cmp) count++;
  return count;
}
char *clean (char *src) {
  src[strlen(src) - 1] = '\0';
  src++;
  return src;
}
char *mk_msg (char *str, int reg[]) {

    int apostr;
    char *os = malloc (BUFFSIZE * sizeof (char)), *ptr = os;
    char *end = strpbrk (str, " "), *start;

    while (*end == space) end++;
    start = end - 1;

    while (end) {
        end = strstr (end + 1, ",");
        char buffer[BUFFSIZE] = {0}, *tmp = buffer;

        if (end == NULL)
            strcpy (buffer, start + 1);
        else
            memcpy (buffer, (start + 1), end - (start + 1));

        apostr = count_char (buffer, apos);

        if (apostr != 1) {
            while (*tmp == space) tmp++;

            if (apostr != 2)
                ptr += sprintf (ptr, "%i", get_val (reg, tmp));
            else
                ptr += sprintf (ptr, "%s", clean (tmp));

            start = end;
        }
    }
    return os;
}

struct _line format2 (const char *input) {
    int index = 0;
    size_t size = count_char(input, endf);

    struct _line new;
    new.com = malloc (size * sizeof (char*));

    char *code = strdup (input), *token = strtok (code, "\n");

    while (token) {

        while (!isalpha (*token)) token++;

        new.com[index++] = token;
        token = strtok (NULL, "\n");
    }

    new.size = index;
    return new;
}

int get_com (const char *input) {
  if (input == NULL) return -1;

  const char oper[19][5] = {"mov","inc","dec","add","sub","mul","div","cmp","ret","msg","end","call","jmp","jne","je","jge","jg","jle","jl"};

  for (int i = 0; i < 19; ++i)
  if (strcmp (input, oper[i]) == 0)
  return i;

  return -1;
}
int get_op (const char *input) { return (input && strlen (input) == 1) ? input[0] - 'a' : -1; }
int get_val (int reg[], const char *input) {
  if (input == NULL) return -1;
  return isalpha (input[0]) ? reg[input[0] - 'a'] : atoi (input);
}
int get_func (struct _line input, const char* func) {
  char buffer[BUFFSIZE] = {0};
  sprintf (buffer,"%s:",func);

  for (size_t i = 0; i < input.size; ++i) {
    char *code = strdup (input.com[i]), *str = strtok (code, " ");

    if (strcmp(str, buffer) == 0)
    return i;
  }

  return 0;
}

char* assembler_interpreter (const char* input) {

    struct _line prog = format2 (input);

    char *os, *label;
    bool jump;
    int reg[BUFFSIZE] = {0}, stack[BUFFSIZE], pos = 0;
    int command, key, val, comp = 0;
    size_t index = 0;

    while (index < prog.size) {

        char *code = strdup (prog.com[index]);

        command = get_com (strtok (code, " "));
        label = strtok (NULL, ", ");
        key = get_op(label);
        val = get_val (reg, strtok (NULL, ", "));

        jump = false;

        switch (command) {
            case mov : reg[key] = val; break;
            case inc : reg[key]++; break;
            case dec : reg[key]--; break;
            case add : reg[key] += val; break;
            case sub : reg[key] -= val; break;
            case mul : reg[key] *= val; break;
            case divi: reg[key] /= val; break;
            case cmp : comp = reg[key] - val; break;
            case ret : index = stack[--pos] - 1; break;
            case msg : os = mk_msg (prog.com[index], reg); break;
            case end : printf (">%s\n", os); return os; break; //
            case jmp : jump = true; break;
            case jne : if (comp != 0) jump = true; break;
            case je  : if (comp == 0) jump = true; break;
            case jge : if (comp >= 0) jump = true; break;
            case jg  : if (comp > 0) jump = true; break;
            case jle : if (comp <= 0) jump = true; break;
            case jl  : if (comp < 0) jump = true; break;
            case call:
                stack[pos++] = index + 1;
                jump = true;
                break;
            default : break;
        }
        if (jump == true) index = get_func (prog, label);

        index++;
    }

    return -1;
}

void Test () {

  const char * prog;
  char *actual;

{ // => ok
  prog = "msg 'Hello World!'\n\
  testlabel:\n\
  ; test comment\n\
  mov a,1\n\
  end";
    actual = assembler_interpreter (prog);
    free (actual);
}
{ // => ok
  prog = "\
; My first program\n\
mov  a, 5\n\
inc  a\n\
call function\n\
msg  '(5+1)/2 = ', a    ; output message\n\
end\n\
\n\
function:\n\
    div  a, 2\n\
    ret\n";

        actual = assembler_interpreter (prog);
        free (actual);
}
{ // => ok
prog = "\
mov   a, 5\n\
mov   b, a\n\
mov   c, a\n\
call  proc_fact\n\
call  print\n\
end\n\
\n\
proc_fact:\n\
    dec   b\n\
    mul   c, b\n\
    cmp   b, 1\n\
    jne   proc_fact\n\
    ret\n\
\n\
print:\n\
    msg   a, '! = ', c ; output text\n\
    ret\n\
" ;
   actual = assembler_interpreter (prog);
   free (actual);
}
{ // => ok
prog = "\
mov   a, 8            ; value\n\
mov   b, 0            ; next\n\
mov   c, 0            ; counter\n\
mov   d, 0            ; first\n\
mov   e, 1            ; second\n\
call  proc_fib\n\
call  print\n\
end\n\
\n\
proc_fib:\n\
    cmp   c, 2\n\
    jl    func_0\n\
    mov   b, d\n\
    add   b, e\n\
    mov   d, e\n\
    mov   e, b\n\
    inc   c\n\
    cmp   c, a\n\
    jle   proc_fib\n\
    ret\n\
\n\
func_0:\n\
    mov   b, c\n\
    inc   c\n\
    jmp   proc_fib\n\
\n\
print:\n\
    msg   'Term ', a, ' of Fibonacci series is: ', b        ; output text\n\
    ret\n\
";

    actual = assembler_interpreter (prog);
    free (actual);
}
{ // => ok
prog = "\
mov   a, 11           ; value1\n\
mov   b, 3            ; value2\n\
call  mod_func\n\
msg   'mod(', a, ', ', b, ') = ', d        ; output\n\
end\n\
\n\
; Mod function\n\
mod_func:\n\
    mov   c, a        ; temp1\n\
    div   c, b\n\
    mul   c, b\n\
    mov   d, a        ; temp2\n\
    sub   d, c\n\
    ret\n\
" ;

    actual = assembler_interpreter (prog);
    free (actual);
}
{ // => ok
prog = "\
mov   a, 81         ; value1\n\
mov   b, 153        ; value2\n\
call  init\n\
call  proc_gcd\n\
call  print\n\
end\n\
\n\
proc_gcd:\n\
    cmp   c, d\n\
    jne   loop\n\
    ret\n\
\n\
loop:\n\
    cmp   c, d\n\
    jg    a_bigger\n\
    jmp   b_bigger\n\
\n\
a_bigger:\n\
    sub   c, d\n\
    jmp   proc_gcd\n\
\n\
b_bigger:\n\
    sub   d, c\n\
    jmp   proc_gcd\n\
\n\
init:\n\
    cmp   a, 0\n\
    jl    a_abs\n\
    cmp   b, 0\n\
    jl    b_abs\n\
    mov   c, a            ; temp1\n\
    mov   d, b            ; temp2\n\
    ret\n\
\n\
a_abs:\n\
    mul   a, -1\n\
    jmp   init\n\
\n\
b_abs:\n\
    mul   b, -1\n\
    jmp   init\n\
\n\
print:\n\
    msg   'gcd(', a, ', ', b, ') = ', c\n\
    ret\n\
";

     actual = assembler_interpreter (prog);
     free (actual);
}
{ // => ok
prog = "\
call  func1\n\
call  print\n\
end\n\
\n\
func1:\n\
    call  func2\n\
    ret\n\
\n\
func2:\n\
    ret\n\
\n\
print:\n\
    msg 'This program should return -1'\n\
";

    actual = assembler_interpreter (prog);
    //free (actual);
}
{ // => ok
prog = "\
mov   a, 2            ; value1\n\
mov   b, 10           ; value2\n\
mov   c, a            ; temp1\n\
mov   d, b            ; temp2\n\
call  proc_func\n\
call  print\n\
end\n\
\n\
proc_func:\n\
    cmp   d, 1\n\
    je    continue\n\
    mul   c, a\n\
    dec   d\n\
    call  proc_func\n\
\n\
continue:\n\
    ret\n\
\n\
print:\n\
    msg a, '^', b, ' = ', c\n\
    ret\n\
";
     actual = assembler_interpreter (prog);
     free (actual);
}
/*
*/
prog =
"mov r, 2   ; instruction mov r, 2\n\
mov w, 7   ; instruction mov w, 7\n\
call func\n\
msg 'Random result: ', i\n\
end\n\
func:\n\
	cmp r, w\n\
	jg exit\n\
	mov i, r\n\
	sub i, w\n\
	ret\n\
; Do nothing\n\
exit:\n\
	msg 'Do nothing'";
  actual = assembler_interpreter (prog);
  //free (actual);
//====8<----
//Expected 'Random result: -5', but got no output

}

int main () {


  Test ();
  //printf ("%s\n", actual);


  printf ("\nprog end\n");
}
/*
bool is_valid (struct _line prog) {
    for (size_t i = 0; i < prog.size; ++i) {

        if (strcmp (prog.com[i], "end") == 0) {
            printf ("[%s]\n", prog.com[i]);
            return true;
        }
    }
    return 0;
}

int apostr (const char &in) { return count(in.begin(), in.end(), apos); }
void sanitize (string &str) { str.erase ( remove(str.begin(), str.end(), space), str.end()); }
vector<string> tokenize (const string &input) {
    vector<string> v;
    string sub;
    int start = 0, end = input.find (space);
    v.push_back(input.substr (start, end));

    while (end != string::npos && end < input.size()) {
        start = end + 1, end = input.find (comma, start);
        sub = input.substr (start, end - start); //cout << '[' << sub << ']';

        if (apostr (sub) == 1) {
            end = input.find (comma, end + 1);
            sub = input.substr (start, end - start);
        }
        if (apostr (sub) == 0) {
            sanitize (sub);
        }

        v.push_back(sub);
    }

    return v;
}
vector<string> format (string input) {

    istringstream iss (input);
    vector<string> v;
    string line;

    while (getline (iss, line)) {
        string sanit = line.substr (0, line.find (comment));
        if (sanit.size() != 0)
            v.push_back(sanit);
    }

    return v;
}
string mk_msg (const string &line) {
    int start = line.find (apos, 0) + 1, end = line.find (apos, start);
    return line.substr (start, (end - start));
}
struct _line tokenize (const char *input) {
struct _line line;
char **v = malloc (5 * sizeof (char*));
int it = 0;
char *code = strdup (input), *token = strtok (code, " ");

while (token) {
if (count_char (token, apos) != 2)
v[it++] = clean (token);
else
v[it++] = token;
//printf ("[%s]", v[it - 1]);
token = strtok (NULL, ",");
}
//printf ("\n");
line.com = v;
line.size = it;
return line;

}/*
struct _line format (const char *input) {
    struct _line new;
    size_t size = count_char(input, endf) - 1;
    new.com = malloc (size * sizeof (char*));

    char *code = strdup (input), *token = strtok (code, "\n");
    int index = 0, i;
    while (token) {
        char buffer[BUFFSIZE] = {0};

        for (i = 0; token[i] != NULL && token[i] != comment; ++i)
            buffer[i] = token[i];

        int sz = strlen (clean (buffer));
        if (sz != 0) {
            new.com[index] = malloc (128 * sizeof (char));
            strcpy (new.com[index], buffer);
            index++;
        }

        token = strtok (NULL, "\n");
    }
    new.size = index;
    return new;
}

struct _line tokenize2 (char * str) {

  int it = 1, apostr;
  struct _line new;
  new.com = malloc ((count_char(str, comma) + 1) * sizeof (char*));
  new.com[0] = malloc (BUFFSIZE * sizeof (char));

  while (*str == space) str++;

  char *end = strpbrk (str, " "), *start = str;

  if (end == NULL)
      new.com[0] = str;
  else
      memcpy (new.com[0], (start), end - start);

  start = end;
  while (end) {

      end = strstr (end + 1, ",");
      if (end == NULL) {
          new.com[it++] = clean (start + 1);
          break;
      }

      char buffer[BUFFSIZE] = {0};
      memcpy (buffer, (start + 1), end - (start + 1));
      apostr = count_char(buffer, apos);

      if (apostr != 1) {
          if (apostr != 2) {
              new.com[it++] = clean (buffer);
          } else {
              new.com[it] = malloc (BUFFSIZE * sizeof (char));
              strcpy (new.com[it++], buffer);
          }

          start = end;
      }
  }

  new.size = it;
  return new;
}
char *get_label (struct _line *input) {
    if (input->size > 1) {
        char *str = input->com[1];
        char *out = malloc (BUFFSIZE * sizeof (char));
        if (strlen (str) > 1 && count_char(str, apos) != 2) {
            sprintf (out, "%s:",str);
            return out;
        }
    }

    return "null";
}
char *clean (const char *str) {
    char *os = malloc (strlen(str) * sizeof (char));
    int i = 0;
    do {
      if (*str != space) os[i++] = *str;
    } while (*str++);
    os[i] = '\0';

    return os;
}
char *mk_msg (char *src) {
    int start = strchr (src, apos) - src + 1, end = strrchr (src, apos) - src;
    char *out = malloc (strlen(src) * sizeof (char));
    int j = 0;

    for (int i = start; i < end; ++i)
        out[j++] = src[i];

    out[j] = '\0';
    return out;
}
*/
