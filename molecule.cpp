#include <iostream>
#include <vector>
#include <tuple>
#include <map>

using namespace std;

struct Atom {
    int id;
    string element;
    vector<Atom> edge;
};

// Symbol:        "H"     "B"     "C"     "N"     "O"     "F"    "Mg"     "P"     "S"    "Cl"    "Br"
// Valence number:   1     3     4     3     2     1     2     3     2     1     1
// Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  (in g/mol)

int valence (const Atom &at) {
    vector<string> el = {"H","B","C","N","O","F","Mg", "P","S","Cl","Br"};
    vector<int> val = {1,3,4,3,2,1,2,3,2,1,1};

    for (int i = 0; i < el.size(); i++) {
        if (el[i] == at.element) {
            return val[i];
        }
    }

    return 0;
}
float weight (const Atom &at) {
    vector<string> el = {"H","B","C","N","O","F","Mg", "P","S","Cl","Br"};
    vector<float> we = {1.0,10.8,12.0,14.0,16.0,19.0,24.3,31.0,32.1,35.5,80.0};

    for (int i = 0; i < el.size(); i++) {
        if (el[i] == at.element) {
            return we[i];
        }
    }

    return 0.0;
}
Atom create(int id, const string el) { return {id, el}; }

class Molecule {
    private:
        string id;
        vector<Atom> base;
        vector<vector<int>> index;
        bool lock;

    public:
        Molecule (string name = "") {
            id = name;
            base.push_back({});
            index.push_back({});
            lock = false;
        }
        string name () { return id; }

        string formula() {
            string os;
            map<string, int> freq;

            for (int i = 1; i < base.size(); i++) {
                freq[base[i].element]++;
            }

            for (auto &it : freq) {
                os += it.first + to_string(it.second);
            }

            return os;
        }
        float molecular_weight() {
            float sum = 0.0;

            for (auto &atom : base) {
                sum += weight (atom);
            }

            return sum;
        }

        void link (int i1, int i2) {
            Atom a1 = base[i1], a2 = base[i2];
            base[i1].edge.push_back(a2);
            base[i2].edge.push_back(a1);
        }
        Molecule &branch (const vector<int> &nc) {

            for (int i = 0; i < nc.size(); i++) {
                vector<int> arm = {0};

                for (int j = 0; j < nc[i]; j++) {
                    int id = base.size();

                    arm.push_back(id);
                    base.push_back({id, "C"}) ;
                }

                for (int j = 2; j <= nc[i]; j++) {
                    link (j-0, j-1);
                }

                index.push_back(arm);
            }

            return *this;
        }
        Molecule &bond (const vector<int> &nc) {

            for (int i = 0; i < nc.size(); i++) {
                // vector<int> arm = {0};
                //
                // for (int j = 0; j < nc[i]; j++) {
                //     int id = base.size();
                //
                //     arm.push_back(id);
                //     base.push_back({id, "C"}) ;
                // }
                // index.push_back(arm);
            }

            return *this;
        }

        Molecule &add (const vector<tuple<int,int,string>> &next) {

            for (auto &[nc, nb, elt] : next) {
                int id = base.size();

                base.push_back(create(id, elt));
                link(id, index[nb][nc]);
            }

            return *this;
        }
        Molecule &add_chain (int nc, int nb, vector<string> elt) {

            return *this;
        }
        Molecule &mutate (const vector<tuple<int,int,string>> &next) {

            for (auto &[nc, nb, elt] : next) {
                base[index[nb][nc]].element = elt;

                for (auto &edg : base[index[nb][nc]].edge) {
                    for (auto &link : base[edg.id].edge) {
                        if (link.id == base[index[nb][nc]].id) {
                            link.element = elt;
                        }
                    }
                }
            }

            return *this;
        }


        void close () {
            lock = true;

            for (int i = 1; i < base.size(); i++) {
                Atom atom = base[i];
                int size = valence (atom);

                for (int j = 1; j < size - atom.edge.size(); j++) {
                    int id = base.size();

                    base.push_back(create(id, "H"));
                    link(id, i);
                }
            }

        }

        void unlock () {
            lock = false;

            for (int i = 1; i < base.size(); i++) {
                Atom atom = base[i];

                for (auto &edge : atom.edge) {

                }
            }

        }
};

int main () {

    Molecule m ("octane");
    m.branch({4});
    m.close();

    cout << m.name() << " => " <<  m.formula();
    cout << "\nend\n";

}
