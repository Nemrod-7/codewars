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
    int equal = equ.find("="), ex = equ.find ("x");

    vector<string> first = tokenize (equ.substr (0, equal));
    vector<string> secnd = tokenize (equ.substr (equal + 1));

    if (first[0] == "-" && secnd[0] == "-") {
        first[0] = "+", secnd[0] = "+";
    }

    int left = calc(first), right = calc (secnd);

    if (ex < equal) {
        res = right - left;
    } else {
        res = left - right;
    }

    /*
    cout << left << "=" << right << " => " << res << "\n";
    */
    return res;
}

enum dir {up,down,left,right};

struct instr {
    static void move (enum dir) {

    }
    void jump () {

    }
};


int main () {
    /*
    solve ("x + 1 = 9 - 2"); // 6
    solve ("- 10 = x");      // -10
    solve ("x - 2 + 3 = 2"); // 1
    solve ("- x = - 1");     // 1
    */

/*

'n' => Some(Instr::Move(Dir::Up)),
's' => Some(Instr::Move(Dir::Down)),
'e' => Some(Instr::Move(Dir::Right)),
'w' => Some(Instr::Move(Dir::Left)),
'*' => Some(Instr::Flip),

*/
    instr::move (up);


    cout << "\nend\n";
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
