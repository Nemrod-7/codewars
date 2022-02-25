#include <sstream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

class Stat {

    private :
        static vector<string> tokenize (const string &str, char lim) {
            vector<string> tok;
            string buff;

            for (auto &it : str) {
                if (it == lim) {
                    tok.push_back (buff);
                    buff.clear();
                } else if (it != ' ')
                    buff += it;
            }

            tok.push_back (buff);
            return tok;
        }
        static string format (int time) {
            ostringstream os;
            os << setfill ('0') << setw(2) << (time / 3600) % 60 << "|";
            os << setfill ('0') << setw(2) << (time / 60) % 60 << "|";
            os << setfill ('0') << setw(2) << time % 60;
            return os.str();
        }

    public:
        static string stat (const string &str) {

            int avg = 0, rng, med, mid;

            vector<string> tok = tokenize (str, ',');
            vector<int> hist;
            ostringstream os;

            for (auto &it : tok) {
                vector<string> time = tokenize (it, '|');
                hist.push_back (stoi (time[0]) * 3600 + stoi (time[1]) * 60 + stoi (time[2]));
            }
            sort (hist.begin(), hist.end());
          
            mid = hist.size() / 2;
            avg = accumulate (hist.begin(), hist.end(), 0) / hist.size();
            rng = hist.back() - hist.front();
            med = hist[mid];

            if (!(hist.size() % 2)) med = (med + hist[mid - 1]) / 2;

            os << "Range: "   << format (rng);
            os << " Average: " << format (avg);
            os << " Median: "  << format (med);

            return os.str();
        }
};
