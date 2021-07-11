#include <iostream>
#include <string>
#include <vector>

class RomanHelper{
      const std::vector<std::pair<int,std::string>> roman {{1000,"M"},{900,"CM"},{500,"D"},{400, "CD"},{100,"C"},
      {90,"XC"},{50,"L"}, {40,"XL"}, {10,"X"}, {9,"IX"}, {5,"V"},{4,"IV"},{1,"I"}};
    public:

      std::string to_roman (unsigned int n){
          std::string os;
          unsigned int index = 0, dec;
          while (n) {
              dec = roman[index].first;
              if (n >= dec) {
                  os += roman[index].second;
                  n -= dec;
              } else
                  index++;
          }

          return os;
      }
      int from_roman (std::string rn){
          int index = rn.size(), num = 0;
          short int dig, dec ,cen;
          dig = dec = cen = 1;

          while (index-->0) {
              char val = rn[index];
              switch (val) {
                case 'M': num += 1000;      cen = -1;       break;
                case 'D': num += 500;       dec = cen = -1; break;
                case 'C': num += 100 * cen; dig = dec = -1; break;
                case 'L': num += 50;        dig = dec = -1; break;
                case 'X': num += 10 * dec;  dig = -1;       break;
                case 'V': num += 5;         dig = -1;       break;
                case 'I': num += 1 * dig;                   break;
                }
          }
          return num;
      }
} RomanNumerals;

int main () {

}
