#include <iostream>
#include <vector>
#include <stack>

#include <cstdio>
#include <cstring>

//#include "/home/wintermute/code/templates/Assert.hpp"
////////////////////////////////////////////////////////////////////////////////

enum {norm, point, star, choice, fst, cat};

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
    return new RegExp (choice, left, right);
}
RegExp *str (RegExp *node) {                // A sequence of regexps, first element
    return new RegExp (fst, node, nullptr);
}
RegExp *add (RegExp *str, RegExp *next) {   // A sequence of regexps, additional element
    return new RegExp (cat, str, next);
}

char *pretty (RegExp *re) {
  RegExp *root = re;
  char *os = new char[8];

  if (root) {

      switch (root->id) {
          case norm  : os[0] = root->op ; break;
          case point : os[0] = '.' ;  break;
          case star  : os[0] = '*' ; break;
          case choice: os[0] = '|' ; break;
          case fst   :  break;
          case cat   : os[0] = '(' ; break;
      }
      pretty (root->left);
      pretty (root->right);

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

std::string shownode (RegExp *node) {

    const std::vector<std::string> oper = {"normal", "any", "zeroOrMore", "orr", "str", "add"};
    std::string os;

    if (node) {
      os += oper[node->id] + " (";
      if (node->id == norm) os += node->op;
    }

    return os;
}
std::string showtree (RegExp *node) {
    RegExp *root = node;
    std::string os;
    const std::vector<std::string> oper = {"normal", "any", "zeroOrMore", "orr", "str", "add"};

    if (root) {

        os = oper[node->id] + " (";
        if (node->id == norm) os += node->op;

        os += showtree (root->left);
        os += showtree (root->right);
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

RegExp *getstack (std::stack<RegExp *> &stk) {
    if (stk.empty()) return nullptr;
    RegExp *ex = stk.top();
    stk.pop();
    return ex;
}
void regroup (std::stack<RegExp *> &stk) {

    if (stk.size() > 1) {
        RegExp *right = getstack (stk), *left = getstack (stk);
        stk.push (add (left, right));
    }
}

RegExp *mktree (std::string code) {

    std::string::iterator it = code.begin();
    int index = 0;
    std::stack<RegExp *> tree, ops;

    while (index < code.size()) {

        char tile = code[index];

        if (isalpha (tile)) {
            RegExp *now = normal(tile);

            if (tree.size() > 0) {
                RegExp *left = str (getstack (tree));
                tree.push (left);
                //now = add (left, now);
            }

            tree.push (now);
        } else {

            if (tile == '|') {
                RegExp *right = getstack (tree), *left = getstack (tree);
                tree.push (orr (left, right));

            } else if (tile == '.') {
                ops.push (any ());
            } else if (tile == '*') {
                RegExp *act = getstack (tree);
                tree.push (zeroOrMore (act));

            } else if (tile == '(') {

                index++;
                int pile = 1;
                std::string sub;

                do {

                    if (code[index] == '(') pile++;
                    if (code[index] == ')') pile--;
                    if (pile == 0) break;

                    sub += code[index++];

                } while (index < code.size());

                RegExp *now = mktree (sub);
                tree.push (now);

            }
        }

        index++;
    }

    regroup (tree);
    //std::cout << showtree (tree.top()) << '\n';
    return !tree.empty() ? tree.top() : nullptr;
}
RegExp *parseRegExp (const char *code) {

    if (check (code) == false) return nullptr;

    return mktree (code);
}


int main () {

  /*

  "ab*"     -> add (str (normal ('a')), zeroOrMore (normal ('b')))
  "(ab)*"   -> zeroOrMore (add (str (normal ('a')), normal ('b')))

  "ab|a"    -> orr (add (str (normal ('a')), normal ('b')), normal ('a'))

  "a(b|a)"  -> add (str (normal ('a')), orr (normal ('b'), normal ('a')))

  "a|b*"    -> orr (normal ('a'), zeroOrMore (normal ('b')))

  "(a|b)*"  -> zeroOrMore (orr (normal ('a'), normal ('b')))

  "a"          -> normal ('a')
  "ab"         -> add (str (normal ('a')), normal ('b'))
  "a.*"        -> add (str (normal ('a')), zeroOrMore (any ()))

  "(a.*)|(bb)" -> orr (add (str (normal ('a')), zeroOrMore (any ())) ,
  add (str (normal ('b')), normal (b)))

  */

  std::cout << showtree (parseRegExp ("a")) << '\n' ;
  std::cout << showtree (parseRegExp ("ab")) << '\n' ;
  std::cout << showtree (parseRegExp ("ab*")) << '\n' ;
  std::cout << showtree (parseRegExp ("(ab)*")) << '\n' ;
  //"(ab)*"   // zeroOrMore (add (str (normal ('a')), normal ('b')))

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
