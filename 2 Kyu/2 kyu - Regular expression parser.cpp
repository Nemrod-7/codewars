#include <iostream>
#include <vector>
#include <stack>

#include <cstdio>
#include <cstring>

//#include "/home/wintermute/code/templates/Assert.hpp"
enum {norm, point, star, choice};

struct RegExp {

    int id;
    char op;
    RegExp *left, *right;

    RegExp (int type, RegExp *arg1, RegExp *arg2) : id (0), op (op), left (arg1), right (arg2) {}
    RegExp (int type = 0, const char &op = 0) : id (type), op (op), left (nullptr), right (nullptr) {}
};

RegExp *normal (char c) { // ^ A character that is not in "()*|.
    return new RegExp (norm, c);
}
RegExp *any () { // ^ Any character
    return new RegExp (point);
}
RegExp *zeroOrMore (RegExp *starred) {     // ^ Zero or more occurances of the same regexp
    return new RegExp (star, starred, nullptr);
}
RegExp *orr (RegExp *left, RegExp *right) { // ^ A choice between 2 regexps
    return new RegExp (choice, left, right);
}
RegExp *str (RegExp *first) {               // ^ A sequence of regexps, first element

}
RegExp *add (RegExp *str, RegExp *next) {   // ^ A sequence of regexps, additional element

}

char *pretty (RegExp *re) {

    return nullptr;
}

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
"(a.*)|(bb)" -> orr (add (str (normal ('a')), zeroOrMore (any ())), add (str (normal ('b')), normal (b)))

*/
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
int getsub (char *it) {
    int pile = 0, index = -1;
    int size = strlen (it);

    do {
        if (*it == '(') pile++;
        if (*it == ')') pile--;
        if (pile == 0) return index;
        index++;
    } while (*it++ != '\0');

    return size;
}
RegExp *getstack (std::stack<RegExp *> &stk) {
    if (stk.empty()) return nullptr;
    RegExp *ex = stk.top();
    stk.pop();
    return ex;
}

RegExp *parseRegExp (const char *code) {

    if (check (code) == false) return nullptr;

    const int size = strlen (code);
    int index = 0;

    std::stack<RegExp *> tree, ops;

    while (index < size) {

        char tile = code[index];

            if (isalpha (tile)) {
            //tree.push (normal (tile));
            printf ("[%c]", tile);
        } else if (tile == '|') {
            /*
            RegExp *right = getstack (tree);
            RegExp *left = getstack (tree);
            tree.push (orr (left, right));
            */
        } else if (tile == '.') {
            //tree.push (any ());
        } else if (tile == '*') {
          /*
            RegExp * act = tree.top();
            tree.pop();
            tree.push (zeroOrMore(act));
          */
        } else if (tile == '(') {

            index++;
            int pile = 1, it = 0;
            char buffer[1024];

            do {

                if (code[index] == '(') pile++;
                if (code[index] == ')') pile--;
                if (pile == 0) break;
                
                buffer[it++] = code[index++];

            } while (index < size);

            buffer[it] = '\0';

            printf ("%c", code[index]);
        } else {

        }

        index++;
    }

    return !tree.empty() ? tree.top() : nullptr;
}

int main () {
  /*
    parseRegExp ("");
    parseRegExp ("(");
    parseRegExp (")(");
    parseRegExp ("a(");

    parseRegExp ("*");
    parseRegExp ("a**");
    */
    parseRegExp ("ab(cde)fgh");

}

void shouldBe (const char *input, const char *expected) {
  char *result = pretty (parseRegExp (input));
  bool ok = !strcmp (result, expected);
  if (!ok) printf ("parse '%s' = '%s' shouldBe '%s'\n", input, result, expected);
  /*
  Assert::That (ok, Equals(true));
  */
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
