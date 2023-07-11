#include <iostream>
#include <string>

using namespace std;

uint64_t factorial (int n) {
  uint64_t mult = 1;
  for (int i = 1; i <= n ; ++i)
      mult *= i;

  return mult;
}



int main () {


		string alpha = "1234";
		string result = "2314";


		char ch;
		int i = 0, selection, n = result.size(), k;

		ch = result[i];
		selection = alpha.find(ch);
		k = selection * factorial (n - 1) + 1;
		alpha.erase(selection,1);
		i++;
		cout << k;

		ch = result[i], n = alpha.size();

		selection = alpha.find(ch);
		k = selection * factorial (n - 1) + 1;
		alpha.erase(selection,1);
		i++;

		cout << k;

}
