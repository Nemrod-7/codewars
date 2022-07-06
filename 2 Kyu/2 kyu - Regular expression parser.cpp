#include <iostream>
#include <vector>
#include <stack>
#include <cstdio>
#include <cstring>

#include "regexparser.hpp"
//#include "/home/wintermute/code/templates/Assert.hpp"

RegExp *getfront (std::deque<RegExp*> &dq) {
  RegExp *ex = dq.front();
  dq.pop_front();
  return ex;
}
RegExp *getback (std::deque<RegExp*> &dq) {
  RegExp *ex = dq.back();
  dq.pop_back();
  return ex;
}

void reduce (std::deque<RegExp *> &tree) {

    if (tree.size() > 1) {
        tree.front() = str (tree.front());

        while (tree.size() > 1) {
            RegExp *left = getfront (tree), *right = getfront (tree);
            tree.push_front (add (left, right));
        }
    }
}
bool check (const char *input) {

    const int size = strlen (input);
    char *expr = strdup (input), *it = expr;
    int par = 0, op = 0, var = 0;
    if (size == 0) return false;

    while (*it) {

        switch (*it) {
            case '(' : par++; break;
            case ')' : par--; break;
            case '|' : op++ ; break;
            case '*' : op++ ; break;
            case '.' :      ; break;
            default  : var++; break;
        }

        if (par < 0) return false;
        if ((it - expr) < size) it++;
    }

    if (op > var) return false;
    if (par != 0) return false;

    return true;
}

RegExp *parseRegExp (const char *code) {

    if (check (code) == false) return nullptr;
    const int size = strlen (code);
    int index = 0;
    std::deque<RegExp *> tree, ops;

    while (index < size) {
        char tile = code[index];
        RegExp *next;

        if (tile == '(') {

            index++;
            int pile = 1;
            char sub[1024] = {}, *it = sub;

            do {

                if (code[index] == '(') pile++;
                if (code[index] == ')') pile--;
                if (pile == 0) break;

                *it++ = code[index++];

            } while (index < size);

            next = parseRegExp (sub);
            tree.push_back (next);

        } else if (tile == '.') {
            tree.push_back (any());
        } else if (tile == '*') {
            RegExp *back = getback (tree);
            tree.push_back (zeroOrMore (back));
        } else if (tile == '|') {

            reduce (tree);
            ops.push_back (getback(tree));
        } else if (tile != ')') {
            tree.push_back (normal (tile));
        }
        index++;
    }

    while (!ops.empty()) {
        reduce (tree);
        RegExp *right = getback (tree), *left = getback (ops);
        tree.push_back (orr (left, right));
    }

    reduce (tree);

    return !tree.empty() ? tree.back() : nullptr;
}

int main () {

  shouldBe ("((aa)|ab)*|a", "(((aa)|(ab))*|a)");
  shouldBe ("((a.)|.b)*|a", "(((a.)|(.b))*|a)");
  /*
  RegExp *tree = parseRegExp ("(aa)|ab");
  //std::cout << showtree (tree) << '\n';

  std::cout << showtree (parseRegExp ("a")) << '\n';      // normal ('a')
  std::cout << showtree (parseRegExp ("ab")) << '\n';     // add (str (normal ('a')), normal ('b'))
  std::cout << showtree (parseRegExp ("a.*")) << '\n';    // add (str (normal ('a')), zeroOrMore (any ()))

  std::cout << showtree (parseRegExp ("ab*")) << '\n';    // add (str (normal ('a')), zeroOrMore (normal ('b')))
  std::cout << showtree (parseRegExp ("(ab)*")) << '\n';  // zeroOrMore (add (str (normal ('a')), normal ('b')))

  std::cout << showtree (parseRegExp ("a|b*")) << '\n';   // orr (normal ('a'), zeroOrMore (normal ('b')))
  std::cout << showtree (parseRegExp ("(a|b)*")) << '\n'; // zeroOrMore (orr (normal ('a'), normal ('b')))
  std::cout << showtree (parseRegExp ("a(b|a)")) << '\n'; // add (str (normal ('a')), orr (normal ('b'), normal ('a')))
  std::cout << showtree (parseRegExp ("(a.*)|(bb)")) << '\n';


  std::cout << showtree (parseRegExp (""));
  std::cout << showtree (parseRegExp ("("));
  std::cout << showtree (parseRegExp (")("));
  std::cout << showtree (parseRegExp ("a("));
  std::cout << showtree (parseRegExp ("()"));


  /*
  RegExp *tree = parseRegExp ("ab|c");
  std::cout << showtree (tree) << '\n';  orr (add (str (normal ('a')), normal ('b')), normal ('a'))
  orr (
    add (str (normal ('a')), normal ('b')),

    normal ('a'))
    */
  //shouldBe ("a.*", "(a.*)");
//shouldBe ("(a.*)|(bb)", "((a.*)|(bb))");

  //std::cout << pretty (tree);


  //std::cout << check ("a*|b") << '\n';
  /*
  std::cout << showtree (parseRegExp ("a**"));

  "(a.*)|(bb)" // orr (add (str (normal ('a')), zeroOrMore (any ())) ,
                       add (str (normal ('b')), normal (b)))
  */

  printf ("\nend\n");
}

template<class T> T getstack (std::stack<T> &stk) {
  T ex = stk.top();
  stk.pop();
  return ex;
}
std::stack<RegExp *> reverse (std::stack<RegExp *> s1) {
  std::stack<RegExp *> s2;

  while (!s1.empty()) s2.push (getstack (s1));

  return s2;
}
