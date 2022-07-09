#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
enum {norm, point, star, choi , fst, cat};

typedef struct RegExp {
  int id;
  char op;
  struct RegExp *left, *right;
} RegExp;

RegExp *new (int type) {
    RegExp *nxt = malloc (sizeof (RegExp));
    nxt->id = type;
    nxt->op = 0;
    nxt->left = nxt->right = NULL;
    return nxt;
}
RegExp *normal (char c) {                   // A character that is not in "()*|.
    RegExp *nxt = new (norm);
    nxt->op = c;
    return nxt;
}
RegExp *any () {                            // Any character
    return new (point);
}
RegExp *zeroOrMore (RegExp *node) {         // Zero or more occurances of the same regexp
    RegExp *nxt = new (star);
    nxt->left = node;
    return nxt;
}
RegExp *or (RegExp *left, RegExp *right) { // A choice between 2 regexps
    RegExp *nxt = new (choi);
    nxt->left = left;
    nxt->right = right;
    return nxt;
}
RegExp *str (RegExp *node) {                // A sequence of regexps, first element
    RegExp *nxt = new (fst);
    nxt->left = node;
    return nxt;
}
RegExp *add (RegExp *str, RegExp *next) {   // A sequence of regexps, additional element
    RegExp *nxt = new (cat);
    nxt->left = str;
    nxt->right = next;
    return nxt;
}

void showtree (RegExp *node) {
    RegExp *root = node;
    const char *oper[6] = {"normal", "any", "zeroOrMore", "orr", "str", "add"};

    if (root) {
        printf ("%s (", oper[root->id]);

        if (root->id == norm) printf ("%c", root->op);
        showtree (root->left);
        showtree (root->right);

        printf (")");
    }
}
///////////////////////////////////////////////////////////////////
typedef struct _node {
    RegExp *data;
    struct _node *prev, *next;
} Node;

typedef struct {
    int size;
    Node *front, *back;
} Deque;

bool is_empty (const Deque *deque) { return (!deque->front || !deque->back); }
RegExp *peek_front (const Deque *deque) {
    return !is_empty (deque) ? deque->front->data : NULL;
}
RegExp *peek_back (const Deque *deque) {
    return !is_empty (deque) ? deque->back->data : NULL;
}

void push_front (Deque *deque, RegExp *data) {

    Node *cell = malloc (sizeof (Node));
    cell->data = data;
    cell->prev = cell->next = NULL;

    if (is_empty(deque)) {
        deque->front = deque->back = cell;
    } else {
        cell->next = deque->front;
    }

    deque->front = deque->front->prev = cell;
    deque->size++;
}
RegExp *pop_front (Deque *deque) {

    RegExp *value = peek_front (deque);
    Node *p = deque->front;
    deque->front = p->next;

    if (deque->front) {
        deque->front->prev = NULL;
    } else {
        deque->back = deque->front = NULL;
    }

    deque->size--;

    free(p);
    return value;
}
void push_back (Deque *deque, RegExp *data) {

    Node *cell = malloc (sizeof (Node));
    cell->data = data;
    cell->prev = cell->next = NULL;

    if (is_empty (deque)) {
        deque->front = deque->back = cell;
    } else {
        cell->prev = deque->back;
    }

    deque->back = deque->back->next = cell;
    deque->size++;
}
RegExp *pop_back (Deque *deque) {

    RegExp *value = peek_back (deque);
    Node *p = deque->back;
    deque->back = p->prev;

    if (deque->back){
        deque->back->next = NULL;
    } else {
        deque->back = deque->front = NULL;
    }

    deque->size--;

    free (p);
    return value;
}

Deque *newdeque () {
    Deque *nxt = malloc (sizeof (Deque));
    nxt->size = 0;
    nxt->front = nxt->back = NULL;
    return nxt;
}
void delete_deque (Deque *deque) {
  Node *nd = deque->front;
  while (nd) {
    Node *tmp = nd->next;
    free(nd);
    nd = tmp;
  }
  free(deque);
}
///////////////////////////////////////////////////////////////////
bool check (const char *input) {

    const int size = strlen (input);
    char *expr = strdup (input), *it = expr;
    int par = 0, op = 0, var = 0;
    if (size == 0) return false;

    while (*it) {
      char prev = it - expr > 0 ? *(it - 1) : 0;

      if (prev == '*' && *it == '*') return false;
      switch (*it) {
        case '(' : par++; break;
        case ')' : par--; break;
        case '|' : op++ ; break;
        case '*' : op++ ; break;
        case '.' : var++; break;
        default  : var++;
        break;
      }

      if (par < 0) return false;
      if ((it - expr) < size) it++;
    }

    if (op > var) return false;
    if (par != 0) return false;

    return true;
}
void reduce (Deque *tree) {

    if (tree->size > 1) {
        push_front (tree, str (pop_front(tree)));

        while (tree->size > 1) {
            RegExp *left = pop_front (tree), *right = pop_front (tree);
            push_front (tree, add (left, right));
        }
    }
}
RegExp *parseRegExp (char *input) {

    if (check (input) == false) return NULL;
    const int size = strlen (input);
    int index = 0;
    Deque *tree = newdeque(), *ops = newdeque();
    //printf ("%s\n\n", input);

    while (index < size) {
        char tile = input[index];
        RegExp *next;

        if (tile == '(') {

            index++;
            int pile = 1;
            char sub[2048] = {}, *it = sub;

            do {

                if (input[index] == '(') pile++;
                if (input[index] == ')') pile--;
                if (pile == 0) break;

                *it++ = input[index++];

            } while (index < size);

            next = parseRegExp (sub);

            push_back (tree, next);
        } else if (tile == '.') {
            push_back (tree, any());
        } else if (tile == '*') {
            next = pop_back (tree);
            push_back (tree, zeroOrMore (next));
        } else if (tile == '|') {

            reduce (tree);
            push_back (ops, pop_back (tree));
            tree->front = tree->back = NULL;
        } else if (tile != ')') {
            push_back (tree, normal (tile));
        }

        index++;
    }

    if (!is_empty(ops)) {
        reduce (tree);
        RegExp *right = pop_back (tree), *left = pop_back (ops);
        push_back (tree, or (left, right));
    }

    reduce (tree);
    //showtree (peek_front (tree));
    return !is_empty (tree) ? peek_back (tree) : NULL;
}
///////////////////////////////////////////////////////////////////
int main () {
  RegExp *tree;

  const char *parse = "(k|(zk*(wn*ux*y.b(rxw((clmlekr*pai.xj.yt(o.nlxwgde)qaxj(pigiifrqqti)(z|t)ta*.ikp)(awt.l.ept.jkjesfl.jeuprqlkxt)hbo(k|k)uzhfuw(r|h)dsgmmzgi)l(t|v)btm((p|o)|o)pk(m*gbvvfcj.((dabwtaco(v|b).(cw.zkaay)zlwfmcipcgn)|r).myiusomjfotef(rualk(v|s)n(k|e)aamyduuyms)x)foa(pega.(zbgv*.floaz(cu.pecdjg*tiekfs.s.gm)y*z*fi(.|.).ltuuowlmblxp)gnx*wivhv..ik*hagy)(x(ozqz)gxmonmayggp(j|v)wafilgaffuyos)wal.s)gd((w|v)|g)((c|g)x))o(xi((wxgktia*kl(tpfqc.ibcnuko..vkuxzmjvkksejj)mj(f|h)o(k|.)u*w(holrkwnnzzioiwnysukcst)kx)|.)kx*e*fvrlkkunaakf(e|.)nxtii(q.)r)y(i|h)cn*(b|.)c(m|w)r.ql*jy))";

  tree = parseRegExp("(.|.)");
  showtree (tree);

  /*
  shouldBe '(k|(zk*(wn*ux*y.b(rxw((clmlekr*pai.xj.yt(o.nlxwgde)qaxj(pigiifrqqti)(z|t)ta*.ikp)(awt.l.ept.jkjesfl.jeuprqlkxt)hbo(k|k)uzhfuw(r|h)dsgmmzgi)l(t|v)btm((p|o)|o)pk(m*gbvvfcj.((dabwtaco(v|b).(cw.zkaay)zlwfmcipcgn)|r).myiusomjfotef(rualk(v|s)n(k|e)aamyduuyms)x)foa(pega.(zbgv*.floaz(cu.pecdjg*tiekfs.s.gm)y*z*fi(.|.).ltuuowlmblxp)gnx*wivhv..ik*hagy)(x(ozqz)gxmonmayggp(j|v)wafilgaffuyos)wal.s)gd((w|v)|g)((c|g)x))o(xi((wxgktia*kl(tpfqc.ibcnuko..vkuxzmjvkksejj)mj(f|h)o(k|.)u*w(holrkwnnzzioiwnysukcst)kx)|.)kx*e*fvrlkkunaakf(e|.)nxtii(q.)r)y(i|h)cn*(b|.)c(m|w)r.ql*jy))'

  = '(k|(zk*(wn*ux*y.b(rxw((clmlekr*pai.xj.yt(o.nlxwgde)qaxj(pigiifrqqti)(z|t)ta*.ikp)(awt.l.ept.jkjesfl.jeuprqlkxt)hbo(k|k)uzhfuw(r|h)dsgmmzgi)l(t|v)btm((p|o)|o)pk(m*gbvvfcj.((dabwtaco(v|b).(cw.zkaay)zlwfmcipcgn)|r).myiusomjfotef(rualk(v|s)n(k|e)aamyduuyms)x)foa(pega.(zbgv*.floaz(cu.pecdjg*tiekfs.s.gm)y*z*fi.ltuuowlmblxp)gnx*wivhv..ik*hagy)(x(ozqz)gxmonmayggp(j|v)wafilgaffuyos)wal.s)gd((w|v)|g)((c|g)x))o(xi((wxgktia*kl(tpfqc.ibcnuko..vkuxzmjvkksejj)mj(f|h)o(k|.)u*w(holrkwnnzzioiwnysukcst)kx)|.)kx*e*fvrlkkunaakf(e|.)nxtii(q.)r)y(i|h)cn*(b|.)c(m|w)r.ql*jy))'

  */

  // shouldBe ("ab|cd", "((ab)|(cd))");
 //shouldBe ("ab|a", "((ab)|a)");

  /*
  tree = parseRegExp ("(a.*)|(bb)");

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
  */
}
