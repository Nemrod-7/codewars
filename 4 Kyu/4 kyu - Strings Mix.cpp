#include <vector>
#include <algorithm>

using namespace std;

class Mix {
    private :
        static vector<int> getcnt (const string &src) {
            vector<int> hist (28);

            for (auto &it : src)
                if (islower(it)) hist[it - 'a']++;

            return hist;
        }
    public:
        static string mix (const string &s1, const string &s2) {

            cout << "[" << s1 << " " << s2 << "]" << endl;
            string os;
            vector<int> cnt1 = getcnt (s1), cnt2 = getcnt (s2);
            vector<string> hist;

            for (int i = 0; i < 26; i++) {

                int freq = max (cnt1[i], cnt2[i]);
                string line;

                if (freq > 1) {
                    if (cnt1[i] == cnt2[i]) {
                        line += "=:";
                    } else if (cnt1[i] == freq) {
                        line += "1:";
                    } else {
                        line += "2:";
                    }
                    line += string (freq, i + 'a');
                    hist.push_back (line);
                }
            }

            sort(hist.begin(), hist.end(), [](const string &s1, const string &s2) -> bool {
                return (s1.length() == s2.length()) ? s1 < s2 : s1.length() > s2.length();
            });

            for (auto &it : hist)
                os += it + "/";

            if (os.size()) os.pop_back();

            return os;
        }
};

int main () {
  
}
