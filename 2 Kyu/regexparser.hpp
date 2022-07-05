#include <vector>

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
  char *os = new char[256], *ptr = os;

  if (root) {

      ptr += sprintf (ptr, "%s", pretty (root->left));
      char c;

      switch (root->id) {
        case norm  : c = root->op ; break;
        case point : c = '.' ; break;
        case star  : c = '*' ; break;
        case choi  : c = '|' ; break;
        case fst   :               break;
        case cat   : c = '(' ; break;
      }

      ptr += sprintf (ptr, "%c", c);
      ptr += sprintf (ptr, "%s", pretty (root->right));
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

        os = oper[root->id];
        os += " (";

        if (root->id == norm) os += root->op;
        os += showtree (root->left);

        os += showtree (root->right);
        os += ")";
    }

    return os;
}
////////////////////////////////////////////////////////////////////////////////

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
