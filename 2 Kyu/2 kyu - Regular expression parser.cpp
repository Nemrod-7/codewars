#include <iostream>
#include <vector>
#include <stack>

#include <cstdio>
#include <cstring>

//#include "/home/wintermute/code/templates/Assert.hpp"
////////////////////////////////////////////////////////////////////////////////

enum {norm, point, star, choi , fst, cat};

struct RegExp {

    int id;
    char op;
    RegExp *left, *right;

    RegExp (int type, RegExp *arg1, RegExp *arg2) : id (type), op (op), left (arg1), right (arg2) {}
    RegExp (int type, const char &op = 0) : id (type), op (op), left (nullptr), right (nullptr) {}
};

RegExp *parseRegExp (const char *);

RegExp *normal (char c) {                   // A character that is not in "()*|.
    return new RegExp (norm, c);
}
RegExp *any () {                            // Any character
    return new RegExp (point);
}
RegExp *zeroOrMore (RegExp *node) {         // Zero or more occurances of the same regexp
    return new RegExp (star, node, nullptr);
}
RegExp *orr (RegExp *left, RegExp *right) { // A choice between 2 regexps
    return new RegExp (choi , left, right);
}
RegExp *str (RegExp *node) {                // A sequence of regexps, first element
    return new RegExp (fst, node, nullptr);
}
RegExp *add (RegExp *str, RegExp *next) {   // A sequence of regexps, additional element
    return new RegExp (cat, str, next);
}

char *pretty (RegExp *node) {

  RegExp *root = node;
  char *os = new char[256];

  if (root) {

      switch (root->id) {
          case norm  : os[0] = root->op ; break;
          case point : os[0] = '.' ; break;
          case star  : os[0] = '*' ; break;
          case choi  : os[0] = '|' ; break;
          case fst   :               break;
          case cat   : os[0] = '(' ; break;
      }
      printf ("%c",os[0]);
      char *a = pretty (root->left);
      char *b = pretty (root->right);
  }

  return os;
}

bool equals (const RegExp &lhs, const RegExp &rhs) {
  return lhs.op == rhs.op && lhs.id == rhs.id && (lhs.left && rhs.left ? equals (*lhs.left, *rhs.left) : true) && (lhs.right && rhs.right ? equals (*lhs.right, *rhs.right) : true);
}
bool equals (const RegExp *lhs, const RegExp *rhs) { return equals (*lhs, *rhs); }
void shouldBe (const char *input, const char *expected) {
  char *result = pretty (parseRegExp (input));
  /*
  bool ok = !strcmp (result, expected);
  if (!ok) printf ("parse '%s' = '%s' shouldBe '%s'\n", input, result, expected);
  Assert::That (ok, Equals(true));
  */
}

std::string showtree (RegExp *node) {
    RegExp *root = node;
    std::string os;
    const std::vector<std::string> oper = {"normal", "any", "zeroOrMore", "orr", "str", "add"};

    if (root) {

        os = oper[root->id] + " (";
        if (root->id == norm) os += root->op;

        os += showtree (root->left) + showtree (root->right);
        os += ")";
    }

    return os;
}
////////////////////////////////////////////////////////////////////////////////
const char *oper = "().|*";
char isop (char c) { return (c == '(' || c == ')' || c =='*' || c == '|' || c == '.'); }
bool check (const char *input) {

    char *expr = strdup (input), *it = expr;
    const int size = strlen (expr);
    if (size == 0) return false;
    int par = 0;

    while (*it) {
        if (*it == '(')  par++;
        if (*it == ')')  par--;
        if (par < 0) return false;
        if ((it - expr) < size) it++;
    }

    if (par != 0) return false;

    return true;
}

template<class T> T getstack (std::stack<T> &stk) {
    //if (stk.empty()) return 0;
    T ex = stk.top();
    stk.pop();
    return ex;
}

int order (char op) {
    if (op == '*') return 2;
    if (op == '|') return 1;
    return 0;
}
void regroup (std::stack<RegExp *> &stk) {
    if (stk.size() > 1) {
        RegExp *right = getstack (stk), *left = str (getstack (stk));
        stk.push (add (left, right));
    }
}
void getnxt (std::stack<RegExp *> &tree, char op) {

    if (op == '|') {
        RegExp *right = getstack (tree), *left = getstack (tree);
        tree.push (orr (left, right));
    } else if (op == '.') {
        tree.push (any ());
    } else if (op == '*') {
        RegExp *act = getstack (tree);
        tree.push (zeroOrMore (act));
    }
}

RegExp *mktree (std::string code) {

    int index = 0;
    const int size = code.size();
    std::stack<RegExp *> tree;
    std::stack<char> ops;

    while (index < size) {
        char tile = code[index];
        RegExp *next;

        if (isalpha (tile)) {
            tree.push (normal (tile));
        } else if (tile == '(') {

            index++;
            int pile = 1;
            std::string sub;

            do {

                if (code[index] == '(') pile++;
                if (code[index] == ')') pile--;
                if (pile == 0) break;

                sub += code[index++];

            } while (index < size);

            next = mktree (sub);
            tree.push (next);
        } else {

            while (!ops.empty() && order (ops.top()) >= order (tile)) {
                char op = getstack (ops);
                getnxt (tree, op);
            }

            ops.push (tile);
        }
        index++;
    }

    while (!ops.empty()) {
        char op = getstack (ops);
        getnxt (tree, op);
    }

    if (tree.size() > 1) {
        RegExp *right = getstack (tree), *left = str (getstack (tree));
        tree.push (add (left, right));
    }
    //std::cout << showtree (tree.top()) << '\n';
    return !tree.empty() ? tree.top() : nullptr;
}

RegExp *parseRegExp (const char *code) {

    if (check (code) == false) return nullptr;
    int index = 0;
    const int size = strlen (code);
    std::stack<RegExp *> tree;
    std::stack<char> ops;

    while (index < size) {
        char tile = code[index];
        RegExp *next;

        if (isalpha (tile)) {
            tree.push (normal (tile));
        } else if (tile == '(') {

            index++;
            int pile = 1;
            char sub2[1024] = {}, *it = sub2;

            do {

                if (code[index] == '(') pile++;
                if (code[index] == ')') pile--;
                if (pile == 0) break;

                *it++ = code[index++];

            } while (index < size);

            next = parseRegExp (sub2);
            tree.push (next);
        } else {

            while (!ops.empty() && order (ops.top()) >= order (tile)) {
                char op = getstack (ops);
                getnxt (tree, op);
            }

            ops.push (tile);
        }
        index++;
    }

    while (!ops.empty()) {
        char op = getstack (ops);
        getnxt (tree, op);
    }

    if (tree.size() > 1) {
        RegExp *right = getstack (tree), *left = str (getstack (tree));
        tree.push (add (left, right));
    }
    //std::cout << showtree (tree.top()) << '\n';
    return !tree.empty() ? tree.top() : nullptr;
}

int main () {

  /*
  RegExp *tree = parseRegExp ("ab|c");
  std::cout << showtree (tree) << '\n';  orr (add (str (normal ('a')), normal ('b')), normal ('a'))
  orr (
       add (str (normal ('a')), normal ('b')),

       normal ('a'))
       */

  std::cout << showtree (parseRegExp ("a")) << '\n';   // normal ('a')
  std::cout << showtree (parseRegExp ("ab")) << '\n';  // add (str (normal ('a')), normal ('b'))
  std::cout << showtree (parseRegExp ("ab*")) << '\n'; // add (str (normal ('a')), zeroOrMore (normal ('b')))
  std::cout << showtree (parseRegExp ("a.*")) << '\n'; // add (str (normal ('a')), zeroOrMore (any ()))
  std::cout << showtree (parseRegExp ("(ab)*")) << '\n'; // zeroOrMore (add (str (normal ('a')), normal ('b')))

  std::cout << showtree (parseRegExp ("a|b*")) << '\n'; // orr (normal ('a'), zeroOrMore (normal ('b')))
  std::cout << showtree (parseRegExp ("(a|b)*")) << '\n'; // zeroOrMore (orr (normal ('a'), normal ('b')))
  std::cout << showtree (parseRegExp ("a(b|a)")) << '\n'; // add (str (normal ('a')), orr (normal ('b'), normal ('a')))
  std::cout << showtree (parseRegExp ("(a.*)|(bb)")) << '\n';
  /*

  "(a.*)|(bb)" // orr (add (str (normal ('a')), zeroOrMore (any ())) ,
                       add (str (normal ('b')), normal (b)))
  */

  printf ("\nend\n");
}

void Test () {

    //RegExp *l = str (normal ('a'));
    //puts (pretty (add (add (add (l, normal ('b')), orr (normal ('c'), normal ('d'))), zeroOrMore (normal ('e')))));

    shouldBe ("", "");
    shouldBe (".", ".");
    shouldBe ("a", "a");

    shouldBe ("a|b", "(a|b)");
    shouldBe ("a*", "a*");
    shouldBe ("(a)", "a");
    shouldBe ("(a)*", "a*");
    shouldBe ("(a|b)*", "(a|b)*");
    shouldBe ("a|b*", "(a|b*)");
    shouldBe ("abcd", "(abcd)");
    shouldBe ("ab|cd", "((ab)|(cd))");

    shouldBe ("ab*", "(ab*)");
    shouldBe ("(ab)*", "(ab)*");
    shouldBe ("ab|a", "((ab)|a)");
    shouldBe ("a(b|a)", "(a(b|a))");
    shouldBe ("a|b*", "(a|b*)");
    shouldBe ("(a|b)*", "(a|b)*");

    shouldBe ("a", "a");
    shouldBe ("ab", "(ab)");
    shouldBe ("a.*", "(a.*)");
    shouldBe ("(a.*)|(bb)", "((a.*)|(bb))");

    shouldBe ("", "");
    shouldBe ("(", "");
    shouldBe (")(", "");
    shouldBe ("*", "");
    shouldBe ("a**", "");
    shouldBe ("a(", "");
    shouldBe ("()", "");

    shouldBe ("((aa)|ab)*|a", "(((aa)|(ab))*|a)");
    shouldBe ("((a.)|.b)*|a", "(((a.)|(.b))*|a)");
};

RegExp *parseRegExp2 (const char *code) {

    if (check (code) == false) return nullptr;
    const int size = strlen (code);
    int index = 0;
    std::stack<RegExp *> tree;
    std::stack<char> ops;

    while (index < size) {
        char tile = code[index];
        RegExp *next;

        if (isalpha (tile)) {
            tree.push (normal (tile));
        } else if (tile == '(') {

            index++;
            int pile = 1;
            std::string sub;

            do {

                if (code[index] == '(') pile++;
                if (code[index] == ')') pile--;
                if (pile == 0) break;

                sub += code[index++];

            } while (index < size);

            next = mktree (sub);
            tree.push (next);
        } else {
            std::cout << tree.size() << " ";
            if (tile == '|') {
                RegExp *right = getstack (tree), *left = str (getstack (tree));
                next = add (left, right);

            } else if (tile == '.') {
                next = any ();

            } else if (tile == '*') {
                RegExp *act = getstack (tree);
                next = zeroOrMore (act);
            }
            tree.push (next);
        }

        index++;
    }

    regroup (tree);
    return !tree.empty() ? tree.top() : nullptr;
  }
