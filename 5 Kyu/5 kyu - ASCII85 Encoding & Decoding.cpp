#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>

#include <chrono>

typedef struct {
    const char *data;
    int n;
  } block;

#define IS_ASCII(x) (((x) >= 33) && ((x) <= 117))

#define TEXT *(input.data + index)
#define BUFFSIZE 2048

#define E(e) (e), (sizeof (e)-1)
#define B(e) { E(e) }
#define len(a) (sizeof(a)/sizeof(*a))

block decoded[] = {
  B ("easy"),
  B ("somewhat difficult"),
};

const char *encoded[] = {
  "<~ARTY*~>",
  "<~F)Po,GA(E,+Co1uAnbatCif~>",
};

void Test();

int main () {
  auto start = std::chrono::steady_clock::now();

  Test ();
  //cout <<fixed << 3392780147 / 870 /28;

  auto end = std::chrono::steady_clock::now();
  std::cout << "Process took "
            << std::chrono::duration<double, std::milli>(end - start).count()
            << " ms" << std::endl;
}

block sanitize (const char *src, unsigned size) {

    block actual;
    int next = 0, i = 0;
    char *out = new char[BUFFSIZE];

    if (size >= 4)
        if ((src[0] == '<') && (src[1] == '~') &&
           (src[size - 1] == '>') && (src[size - 2] == '~'))
                i = 2, size -= 2;

    for (; i < size; ++i) {
        if (src[i] == 'z') size = i;
        if (IS_ASCII (src[i]))  out[next++] = src[i];

    }
    out[next] = '\0';
    actual.data = out, actual.n = next;
    return actual;
}
char *toAscii85 (block input) {
    int size = strlen (input.data) , index = 0, next, track = 0, now;
    unsigned byte = 0;
    char buffer[BUFFSIZE] = {0}, *output = new char[BUFFSIZE];
    //std::cout << "size " << input.n << " str " << input.data << '\n';
    if (input.n > 0)
        while (true) {
            byte <<= 8;
            byte |= static_cast<uint8_t>(input.data[index]);

            next = (index + 1) % 4;

            if (next == 0)
                if (byte == 0 && index < input.n) {
                    buffer[track++] = 'z';
                } else {
                    now = track += 5;

                    while (now-->track - 5) {
                        buffer[now] = byte % 85 + 33;
                        byte /= 85;
                    }
                }

            index++;

            if (index >= input.n && next == 0)
                break;
        }
        now = track - (index - input.n);
        buffer[now] = '\0';

    sprintf (output,"<~%s~>\0", buffer);
    //std::cout << output << '\n';
    return output;
}
block fromAscii85 (const char *source) {

    int index = 0, size = strlen(source), next, track = 0, now;
    unsigned byte = 0;
    block input = sanitize (source, size), output = {0};

    if (input.n == 0) return input;
    char *buffer = new char[BUFFSIZE], ascii;

    while (true) {
        next = (index + 1) % 5;
        ascii = (index < input.n ? input.data[index] : 'u') - 33;
        byte += ascii * pow (85, 4 - index % 5);

        if (next == 0) {
            now = (track += 4);

            while (now-->track - 4) {
                buffer[now] = byte % 256;
                byte >>= 8;
            }
        }
        index++;

        if (index >= input.n && next == 0)
            break;
    }
    now = track - (index - input.n), buffer[now] = '\0';

    output.data = buffer, output.n = now;
    return output;
}

void dotest (block b) {

  char *encode = toAscii85 (b);
  block decode = fromAscii85 (encode);
  std::cout << encode << " => " << decode.data << "\n";
  //printf ("%s => %s\n",encode, decode.data);
}
void Test () {
  { block tst = {"Man ",4}; dotest(tst); }
  { block tst = {"easy",4}; dotest(tst); }
  { block tst = {"moderate",8}; dotest(tst); }
  { block tst = {"somewhat difficult",18}; dotest(tst); }
  { block tst = {"whitespace test",15}; dotest (tst); }
  { block tst = {"Now is the time for all good men to come to the aid of their country",68};dotest (tst);}
  /*
  { block tst = {"",0}; dotest (tst); }
  { block tst = {"",1}; dotest (tst); }
  { block tst = {"",2}; dotest (tst); }
  { block tst = {"",3}; dotest (tst); }
  { block tst = {"",4}; dotest (tst); }
  { block tst = {"",17}; dotest (tst); }
  { block tst = {"",5}; dotest (tst); }
  { block tst = {"",6}; dotest (tst); }
  { block tst = {"",7}; dotest (tst); }
  { block tst = {"",8}; dotest (tst); }
  { block tst = {"",9}; dotest (tst); }
  { block tst = {"",10}; dotest (tst); }
  { block tst = {"Some ",54}; dotest (tst); }
  */
  /*
*/
    //{ block tst = {"bart",16}; dotest (tst); }

    // '<~@UX;z!!-;jkE-#r8~>'  <= received
    // '<~@UX;!z!-;jkE-#r8~>'  <= expected

    /* -----> results <-------
    easy                <~ARTY*~>
    moderate            <~D/WrrEaa'$~>
    somewhat difficult  <~F)Po,GA(E,+Co1uAnbatCif~>

    <~GA(]4ATMg!@	q?d)ATMr91B~>
    <~GA(]4ATMg !@q?d)ATMq~>


    ---=--- string ---=---::
    <~GA(]4ATMg !@q?d)ATMq~>
    ---=--- result ---=---::
    whitesp`+Z

    ---=--- string ---=---::
    <~GA(]4A\nTMg!@q\n?d)ATM\nr91&~>
    ---=--- result ---=---::
    whitb+H

    ---=--- string ---=---::
    <~GA(]4ATMg!@	q?d)ATMr91B~>
    ---=--- result ---=---::
    whitespa_KF
    */

}
