#include <iostream>
#include <vector>

#include <cstdio>
#include <cstring>

#include "/home/wintermute/code/templates/Assert.hpp"

struct RegExp;

RegExp *normal (char);                     // ^ A character that is not in "()*|."
RegExp *any ();                            // ^ Any character
RegExp *zeroOrMore (RegExp *starred);      // ^ Zero or more occurances of the same regexp
RegExp *orr (RegExp *left, RegExp *right); // ^ A choice between 2 regexps
RegExp *str (RegExp *first);               // ^ A sequence of regexps, first element
RegExp *add (RegExp *str, RegExp *next);   // ^ A sequence of regexps, additional element

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

RegExp *parseRegExp (const char *input) {

    return nullptr;
}

int main () {

}

char *pretty (RegExp *r) {

    return nullptr;
}

void shouldBe (const char *input, const char *expected) {
  char *result = pretty (parseRegExp (input));
  bool ok = !strcmp (result, expected);
  if (!ok) printf ("parse '%s' = '%s' shouldBe '%s'\n", input, result, expected);
  Assert::That (ok, Equals(true));
  /*
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
