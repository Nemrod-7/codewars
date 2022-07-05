#include <iostream>
#include <vector>
#include <stack>
#include <cstdio>
#include <cstring>

#include "regexparser.hpp"
//#include "/home/wintermute/code/templates/Assert.hpp"

template<class T> T getfront (std::deque<T> &dq) {
  //if (dq.empty()) return 0;
  T ex = dq.front();
  dq.pop_front();
  return ex;
}
template<class T> T getback (std::deque<T> &dq) {
  //if (dq.empty()) return 0;
  T ex = dq.back();
  dq.pop_back();
  return ex;
}

RegExp *getstack (std::stack<RegExp *> &s1) {
    RegExp *ex = s1.top();
    s1.pop();
    return ex;
}
std::stack<RegExp *> reverse (std::stack<RegExp *> s1) {
  std::stack<RegExp *> s2;

  while (!s1.empty()) s2.push (getstack (s1));

  return s2;
}
void reduce (std::stack<RegExp *> &tree) {
    tree = reverse (tree);
    if (tree.size() > 1) tree.top() = str (tree.top());

    while (tree.size() > 1) {
        RegExp *left = getstack (tree), *right = getstack (tree);
        tree.push (add (left, right));
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
    std::stack<RegExp *> tree, ops;

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
            tree.push (next);
        } else if (tile == ')') {
        } else if (tile == '.') {
            tree.push(any());
        } else if (tile == '*') {
            RegExp *back = getstack (tree);
            tree.push (zeroOrMore (back));
        } else if (tile == '|') {
            //std::cout << showtree (tree.top());
            reduce (tree);
            ops.push (getstack(tree));
        } else {
            tree.push (normal (tile));
        }
        index++;
    }

    while (!ops.empty()) {
        reduce (tree);
        RegExp *right = getstack (tree), *left = getstack (ops);
        tree.push (orr (left, right));
    }

    tree = reverse (tree);
    tree.push (str (getstack (tree)));

    while (tree.size() > 1) {
        RegExp *left = getstack (tree), *right = getstack (tree);
        tree.push (add (left, right));
    }

    return !tree.empty() ? tree.top() : nullptr;
}

int main () {

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
  /*

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
