#include <iostream>
#include <vector>
#include <stack>
#include <cstdio>
#include <cstring>

#include "regexparser.hpp"
//#include "/home/wintermute/code/templates/Assert.hpp"

RegExp *getfront (std::deque<RegExp*> &dq) {
    if (dq.empty()) return nullptr;
    RegExp *ex = dq.front();
    dq.pop_front();
    return ex;
}
RegExp *getback (std::deque<RegExp*> &dq) {
    if (dq.empty()) return nullptr;
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
    int par = 0, op = 0, var = 0, index = 0;
    if (size == 0) return false;

      while (index < size) {
        char prev = index > 0 ? input[index - 1]: 0;
        char curr = input[index];
        if (prev == '*' && curr == '*') return false;

        switch (curr) {
            case '(' : par++; break;
            case ')' : par--; break;
            case '*' : op++ ; break;
            default  : var++; break;
        }

        if (par < 0) return false;
        index++;
    }

    if (op > var) return false;
    if (par != 0) return false;

    return true;
}
RegExp *parseRegExp (const char *code) {

<<<<<<< HEAD
    static int cycle = 0;
    cycle++;
    //std::cout << cycle << " -> [ " << code << " ]" << "\n\n";
=======
>>>>>>> f93690170342421b022abdcea3458bec7b404f00
    if (check (code) == false) return nullptr;

    const int size = strlen (code);
    int index = 0;
    std::deque<RegExp *> tree, ops;


    while (index < size) {
        char curr = code[index];
        RegExp *next;

        if (curr == '(') {

            index++;
            int pile = 1;
            char sub[2048] = {}, *it = sub;

            do {

                if (code[index] == '(') pile++;
                if (code[index] == ')') pile--;
                if (pile == 0) break;

                *it++ = code[index++];

            } while (index < size);

            next = parseRegExp (sub);
            tree.push_back (next);

        } else if (curr == '.') {
            tree.push_back (any());
        } else if (curr == '*') {
            RegExp *back = getback (tree);
            tree.push_back (zeroOrMore (back));
        } else if (curr == '|') {

            reduce (tree);
            ops.push_back (getback(tree));
        } else if (curr != ')') {
            tree.push_back (normal (curr));
        }
        index++;
    }

    while (!ops.empty()) {
        reduce (tree);
        RegExp *right = getback (tree), *left = getback (ops);
        //std::cout << showtree (left) << " " << showtree (right) << "\n";
        tree.push_back (orr (left, right));
    }
    std::cout << tree.size();
    reduce (tree);
    return !tree.empty() ? tree.back() : nullptr;
}

char *pretty2 (RegExp *node) {

  RegExp *root = node;
  char *os = new char[10000], *ptr = os;

  if (root) {
      char c;

      switch (root->id) {
        case norm  : c = root->op ; break;
        case point : c = '.' ; break;
        case star  : c = '*' ; break;
        case choi  : c = '|' ; break;
        case fst   : c = '(' ; break;
        case cat   : c = 1 ; break;
        //default : c = 0; break;
      }

      ptr += sprintf (ptr, "%s", pretty2 (root->left));
      /*
      */
      ptr += sprintf (ptr, "%c", c);
      ptr += sprintf (ptr, "%s", pretty2 (root->right));

  }

  return os;
}
int main () {


const char *parse = "(u(mj(p|(w.v(jxgpubqtoengmvillcf(fujft.saewudv(y|t)ri.trnroqsc)hvtjyzl)rw.))jp(u|f)der((s|p)|(y|o))m)wc*(h|m)(m*(y|(z|b))(j|z*)u*nw(d|n*)s)p(l*|z*)(m|.)*u(.|((towljnt(i|c)mjue.n.(owc*gcswlni.)oj*r(d.zxcukadicwaxgmtgfwjivewidbm)h)scx.ha(vne.)))l(g*|r)(b|(p|(c|v)))y*x.bun(q|(r(((c|c)|(i|v))|b)xy.imo(k|v)c*ay(mawidwri(.w)wnyuqux*)*zna*pc*zx)))";

/*
shouldBe
'(u(mj(p|(w.v(jxgpubqtoengmvillcf(fujft.saewudv(y|t)ri.trnroqsc)hvtjyzl)rw.))jp(u|f)der((s|p)|(y|o))m)wc*(h|m)(m*(y|(z|b))(j|z*)u*nw(d|n*)s)p(l*|z*)(m|.)*u(.|((towljnt(i|c)mjue.n.(owc*gcswlni.)oj*r(d.zxcukadicwaxgmtgfwjivewidbm)h)scx.ha(vne.)))l(g*|r)(b|(p|(c|v)))y*x.bun(q|(r(((c|c)|(i|v))|b)xy.imo(k|v)c*ay(mawidwri(.w)wnyuqux*)*zna*pc*zx)))'

 =
'(u(mj(p|(w.v(jxgpubqtoengmvillcf(fujft.saewudv(y|t)ri.trnroqsc)hvtjyzl)rw.))jp(u|f)der((s|p)|(y|o))m)wc*(h|m)(m*(y|(z|b))(j|z*)u*nw(d|n*)s)p(m|.)*u(.|((towljnt(i|c)mjue.n.(owc*gcswlni.)oj*r(d.zxcukadicwaxgmtgfwjivewidbm)h)scx.ha(vne.)))l(g*|r)(b|(p|(c|v)))y*x.bun(q|(r(((c|c)|(i|v))|b)xy.imo(k|v)c*ay(mawidwri(.w)wnyuqux*)*zna*pc*zx)))'

  */

<<<<<<< HEAD
  RegExp *tree = parseRegExp ("l*|r*");
  std::cout << showtree (tree) << '\n';

  //std::cout << pretty2 (tree);

=======
>>>>>>> f93690170342421b022abdcea3458bec7b404f00
  /*
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

  RegExp *tree = parseRegExp ("ab|c");
  std::cout << showtree (tree) << '\n';  orr (add (str (normal ('a')), normal ('b')), normal ('a'))
  orr (
    add (str (normal ('a')), normal ('b')),

    normal ('a'))
    */

  //std::cout << pretty ("((o|.)|.)");


  //std::cout << check ("a*|b") << '\n';
  /*
  std::cout << showtree (parseRegExp ("a**"));

  "(a.*)|(bb)" // orr (add (str (normal ('a')), zeroOrMore (any ())) ,
                       add (str (normal ('b')), normal (b)))
  */

  printf ("\nend\n");
}
