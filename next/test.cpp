#include <iostream>
#include <vector>

using namespace std;

template<class T> T getval (vector<T> &vec) {
    if (vec.empty()) return 0;
    T val = vec.back();
    vec.pop_back();
    return val;
}
vector<string> tokenize (string src) {

    vector<string> vect;

    for (int i = 0; i < src.size(); i++) {
        while (src[i] == ' ') i++;
        string buff;

        while (i < src.size() && src[i] != ' ') buff += src[i++];

        vect.push_back(buff);
    }

    return vect;
}
int calc (vector<string> equ) {

    int it = 0;
    vector<int> vals;
    vector<string> op;

    while (it < equ.size()) {

        if (equ[it] == "x") {
            vals.push_back(0);
        } else if (equ[it] == "+" || equ[it] == "-") {
            if (vals.size() > 1 && op.size() > 0) {
                string oper = getval(op);
                int b = getval (vals), a = getval (vals);
                vals.push_back ((oper == "-" ? a - b : a + b));
            }
            op.push_back(equ[it]);
        } else {
            vals.push_back(stoi(equ[it]));
        }

        it++;
    }

    while (op.size()) {
        string oper = getval(op);
        int b = getval (vals), a = getval (vals);

        vals.push_back ((oper == "-" ? a - b : a + b));
    }

    return vals.back();
}
int solve (const string &equ) {

    int res = 0;
    int equ = equ.find("="), ex = equ.find ("x");

    vector<string> first = tokenize (equ.substr (0, equ));
    vector<string> secnd = tokenize (equ.substr (equ + 1));

    if (first[0] == "-" && secnd[0] == "-") {
        first[0] = "+", secnd[0] = "+";
    }

    int left = calc(first), right = calc (secnd);

    if (ex < equ) {
        res = right - left;
    } else {
        res = left - right;
    }

    /*
    cout << left << "=" << right << " => " << res << "\n";
    */
    return res;
}

int main () {
    /*
    solve ("x + 1 = 9 - 2"); // 6
    solve ("- 10 = x");      // -10
    solve ("x - 2 + 3 = 2"); // 1
    solve ("- x = - 1");     // 1
    */
    int num = -0;


    cout << num;
    //Display::vect (first);

}



class Display {
  public :
  static void vect (vector<string> V) {
    for (auto &it : V) {
      cout << "["<<it<<"]";
    }
    cout << endl;
  }
};
