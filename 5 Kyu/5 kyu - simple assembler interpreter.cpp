#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include <iomanip>
#include <chrono>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
typedef std::unordered_map<std::string, int> t_type;

class Assert {
    public:
        static void That(t_type input, t_type expected) {
            if (input != expected)
                cout << "error"<< endl;
        }

};
t_type Equals(t_type entry) {return entry;}
void Test ();
////////////////////////////////////////////////////////////////////////////////

unordered_map<string, int> assembler(const vector<string>& program);
void display_map (unordered_map<string, int> input);
////////////////////////////////////////////////////////////////////////////////
int main () {
  auto start = chrono::high_resolution_clock::now();

  Test();
  assembler({"mov a -10", "mov b a", "inc a", "dec b", "jnz a -2"});

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  cout << "Process took " << elapsed.count()  << " ms" << endl;
  return EXIT_SUCCESS;
}

std::unordered_map<std::string, int> assembler(const std::vector<std::string>& input) {

    unordered_map<string,int> reg;
    vector<string>::const_iterator it(input.begin());
    //istringstream iss;
    string com, op, key;

    // auto val = [&](string ins)->int { return isalpha (ins[0]) ? reg[ins] : stoi (ins); };

    while (it != input.end()) {

        op  = it->substr (0,3);
        key = it->at (4);
        int pam1 = isdigit (key[0]) ? stoi (key) : reg[key];

        if (op == "mov") reg[key] = isalpha(it->at(6)) ? reg[{it->at(6)}] : stoi(it->substr(6));
        if (op == "inc") reg[key]++;
        if (op == "dec") reg[key]--;
        if (op == "jnz" && pam1 != 0) {
            int end = input.end() - it, offset = stoi(it->substr(6));
            it += end > offset ? offset : end;
        } else
            it++;
        //display_map (reg);
        cout << endl;
    }

    return reg;
}

void display_map (unordered_map<string, int> input) {
    for (auto now : input)
        cout<<"["<< now.first<<","<<setw(3)<<setfill(' ')<<now.second<<"]\n";
}
void Test () {
    //assembler({ "mov a 5", "inc a", "dec a", "dec a","jnz a -1", "inc a" });
    //assembler({"mov a -10", "mov b a", "inc a", "dec b", "jnz a -2"});

    {
    vector<string> program{"mov a 1","mov b 1" ,"mov c 0" ,"mov d 26" ,"jnz c 2" ,"jnz 1 5" ,
    "mov c 7","inc d" ,"dec c" ,"jnz c -2" ,"mov c a" ,"inc a" ,"dec b" ,"jnz b -2" ,"mov b c",
    "dec d" ,"jnz d -6" ,"mov c 18" ,"mov d 11" ,"inc a" ,"dec d" ,"jnz d -2" ,"dec c" ,"jnz c -5"};
    assembler (program);
    }


    {
      vector<string> program{"mov d 100", "dec d", "mov b d", "jnz b -2", "inc d",
              "mov a d", "jnz 5 10", "mov c a"};
      assembler (program);
    }


    {
      vector<string> program{"mov c 12", "mov b 0", "mov a 200", "dec a", "inc b", "jnz a -2",
            "dec c", "mov a b", "jnz c -5", "jnz 0 1", "mov c a"};
      assembler (program);
    }

}
