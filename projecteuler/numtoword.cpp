#include <iostream>
#include <vector>


using namespace std;

const vector<vector<string>> base = {
    {"zero","one","two","three","four","five","six","seven","eight","nine",
     "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"},
    {".",".","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"}};

const vector<string> mill = {"thousand","","hundred","million"};

string number_to_words (int src) {

    if (src < 20) {
        return base[0][src];
    }

    int buffer[5], index = 0;
    string os;

    do {
        buffer[index++] = src % 1000;
    } while (src /= 1000);

    while (index-->0) {
        int num = buffer[index];
        int cent = (num / 100) % 1000, dix = num % 100;

        if (cent) {
            os += base[0][cent] + " hundred";

            if (dix > 0) {
                os += " and ";
            }
        }

        if (dix < 20) {
            if (dix > 0) {
                os += base[0][dix] + " ";
            }

        } else {
            os += base[1][(dix / 10) % 10];

          if (dix % 10 != 0) {
              os += "-" + base[0][dix % 10];
          } else {
          //    os += " ";
          }
        }

        if (index == 1) os += "thousand ";
    }

    if (os.back() == ' ') {
        os.pop_back();
    }

    return os;
}

string project17 (int src) {

      if (src < 20) {
          return base[0][src];
      }

      int buffer[5], index = 0;
      string os;

      do {
          buffer[index++] = src % 1000;
      } while (src /= 1000);

      while (index-->0) {
          int num = buffer[index];
          int cent = (num / 100) % 1000, dix = num % 100;

          if (cent) {
              os += base[0][cent] + "hundred";

              if (dix > 0) {
                  os += "and";
              }
          }

          if (dix < 20) {
              if (dix > 0) {
                  os += base[0][dix];
              }

          } else {
              os += base[1][(dix / 10) % 10];

            if (dix % 10 != 0) {
                os +=  base[0][dix % 10];
            }
          }

          if (index == 1) os += "thousand";
      }

      if (os.back() == ' ') {
          os.pop_back();
      }

      return os;
}

int main () {

  int cnt = 0;
  //cout << number_to_words  (1022) ;// (three hundred and forty-two)
  for (int i = 1; i <= 1000; i++) {
    cnt += project17 (i).size();
    cout << i << " => [" << number_to_words(i) << "]" << endl;
    //cout << project17(i) << endl;

  }

  cout << cnt;
  /*
  */
		std::cout << "\nend\n";
}
