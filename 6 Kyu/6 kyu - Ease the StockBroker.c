#include <stdio.h>
#include <stdlib.h>

#define IS_UPPER(x) (((x) >= 'A') && ((x) <= 'Z'))
#define IS_LOWER(x) (((x) >= 'a') && ((x) <= 'z'))
#define IS_ALPHA(x) (IS_UPPER(x) || IS_LOWER(x))
#define IS_DIGIT(x) (((x) >= '0') && ((x) <= '9' ))
#define IS_POINT(x) ((x) == '.')
#define IS_SPACE(x) ((x) == ' ')

#define EDGE ','
#define TOKEN 'x'
#define BUFSIZE 100

enum kBool {FALSE,TRUE};
enum setflag {OFF,ON};

typedef unsigned short int uint16_t;
typedef struct {
  char name[10];
  unsigned int qty;
  double price;
  char status;
} order_t;

typedef struct {
    double buy;
    double sell;
    char bad[BUFSIZE];
    int num;
} stock_t;

char* balanceStatements(char *);
stock_t dispatch (char *);
stock_t convert (char *, int );
order_t scan (char *);
void copy (char *, char *);
void reset (stock_t *);

char* balanceStatements(char* lst) {

  char *buffer = malloc (BUFSIZE * sizeof(char));

  if (lst[0] == 0) {
    snprintf (buffer,100,"Buy: 0 Sell: 0");
    return buffer;
  }

  stock_t final = dispatch (lst);

  if (final.num < 1)
   snprintf (buffer,100,"Buy: %.0lf Sell: %.0lf",
                   final.buy,final.sell);
  else
  snprintf (buffer,100,"Buy: %.0lf Sell: %.0lf; Badly formed %d: %s",
                    final.buy,final.sell,final.num,final.bad);

  return buffer;
}

stock_t dispatch (char *src) {

    uint16_t i = 0;
    char res[BUFSIZE];

    do {
      res[i++] = *src;
        if (*src == EDGE) {
          res[i - 1] = TOKEN;
          res[i] = '\0';
          convert(res,i);
          i = 0;
        }
    }
    while (*src++);

    return convert(res,i);
}

stock_t convert (char *src, int lim) {

  order_t order;
  static stock_t next;
  static _Bool flag = OFF;

  if (flag == ON) {
    reset (&next);
    flag = OFF;
  }
  if (src[lim - 1] != TOKEN) {
    flag = ON;
  }

  order = scan (src);

   switch (order.status) {
      case 'B': next.buy += order.qty * order.price; break;
      case 'S': next.sell += order.qty * order.price; break;
      default : copy (src, next.bad), next.num++; break;
  }

  return next;
}

void copy (char *from, char *to) {

  while (*to != '\0')
    *to++;

  while (*to++ = *from++)
    if (*from == TOKEN) {
      *from = ' ';
      *(from + 1) = ';';
      *(from + 2) = '\0';
    }
}

void reset (stock_t *src) {
  src->buy = 0, src->sell = 0;
  src->bad[0] = '\0', src->num = 0;
}

order_t scan (char *src) {

  uint16_t i, power = 1;
  float val = 0.0;
  order_t now;
  _Bool isfloat = FALSE, isint = FALSE;

  now.name[0] = '0', now.qty = 0, now.price = 0.0, now.status = '\0';
  for (i = 0; !IS_SPACE(src[i]); ++i)
    now.name[i] = src[i];
    now.name[i] = '\0';

  for (++i; IS_DIGIT(src[i]); ++i)
      now.qty = now.qty * 10 + (src[i] - '0');
  if (!IS_POINT(src[i])) isint = TRUE;

  for (++i; IS_DIGIT(src[i]); ++i)
      val = val * 10.0 + (src[i] - '0');
      
  if (IS_POINT(src[i])) isfloat = TRUE;
  for (++i; IS_DIGIT(src[i]); ++i) {
      val = val * 10.0 + (src[i] - '0');
      power *= 10;
  }
  now.price = val / power;
  now.status = (isint == TRUE && isfloat == TRUE) ? src[++i] : 'E';

  return now;
}
