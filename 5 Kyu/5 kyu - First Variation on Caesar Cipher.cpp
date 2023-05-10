#include <iostream>
#include <vector>

class CaesarCipher {
  private:
    static std::string ciphmodule (std::string src, int shift, char mode) {

      const int size = src.size();
      std::string os;

      for (auto &ch : src) {
        if (ispunct(ch) || isspace(ch) || isdigit(ch)) {
            os += ch;
        } else {
            #define OP (mode == 'd') ? (next < 97) : (next > 65)

            char case_mode = isupper (ch) ? 'A' : 'a';
            int next = (mode == 'd') ? (ch - shift) : (ch + shift);
            int crypt = (next - case_mode + 26 * size) % 26 + case_mode;

            os += (OP) ? crypt : next;
        }
        shift++;
      }

      return os;
    }
  public:
    static std::vector<std::string> movingShift (const std::string &src, int shift) {
        std::vector<std::string> ve;
        int size = src.size();
        int fst = 0, mean = size / 5;
        std::string str = ciphmodule (src, shift, 'e');

        if (mean * 5 < size)
          mean += 1;

        for (int i = 0; i < 5; i++) {
            ve.push_back(str.substr (fst, mean));
            fst += mean;
        }
        return ve;
    }
    static std::string demovingShift (std::vector<std::string> &src, int shift) {
        std::string os;

        for (int i = 0; i < 5; i++) {
            os += src[i];
        }
        return ciphmodule(os,shift,'d');
    }
};

int main () {

  std::string  src = "I should have known that you would have a perfect answer for me!!!";
  std::vector<std::string> sol = {"J vltasl rlhr ", "zdfog odxr ypw", " atasl rlhr p ", "gwkzzyq zntyhv", " lvz wp!!!"};

  std::string dec;
  auto crypt = CaesarCipher::movingShift(src, 1);
  std::cout << CaesarCipher::demovingShift(crypt,1);

    // CaesarCipher::movingShift(u, 1);
}
