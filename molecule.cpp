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

// Symbol:        "H"     "B"   "C"   "N"   "O"   "F"   "Mg"  "P"   "S"   "Cl"  "Br"
// Valence number:   1     3     4     3     2     1     2     3     2     1     1
// Atomic weight:  1.0  10.8  12.0  14.0  16.0  19.0  24.3  31.0  32.1  35.5  80.0  (in g/mol)

int valence (const string &at) {
    vector<string> el = {"H","B","C","N","O","F","Mg", "P","S","Cl","Br"};
    vector<int> val = {1,3,4,3,2,1,2,3,2,1,1};

    for (int i = 0; i < el.size(); i++) {
        if (el[i] == at) {
            return val[i];
        }
    }

    return 0;
}
float weight (const string &at) {
    vector<string> el = {"H","B","C","N","O","F","Mg", "P","S","Cl","Br"};
    vector<float> we = {1.0,10.8,12.0,14.0,16.0,19.0,24.3,31.0,32.1,35.5,80.0};

    for (int i = 0; i < el.size(); i++) {
        if (el[i] == at) {
            return we[i];
        }
    }

    return 0.0;
}
Atom create(int id, const string el) { return {id, el}; }

class Molecule {
    private:
        string id, status;
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

        void link (int i1, int i2) {
            Atom a1 = base[i1], a2 = base[i2];
            base[i1].edge.push_back(a2);
            base[i2].edge.push_back(a1);

        }

        string formula() {
            if (base.size() == 1) throw::runtime_error ("empty molecule.");
            if (lock == false) throw::logic_error ("molecule unlocked.");

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
            if (base.size() == 1) throw::runtime_error ("empty molecule.");
            if (lock == false) throw::runtime_error ("molecule unlocked.");

            float sum = 0.0;

            for (auto &atom : base) {
                sum += weight (atom.element);
            }

            return sum;
        }

        void show() {
            for (auto &atom : base) {
                cout << atom.element << atom.id << ":";

                for (auto &edge : atom.edge) {
                    cout << edge.element << '.' << edge.id << ' ';
                }

                cout << '\n';
            }
        }

        Molecule &branch (const vector<int> &nc) {
            if (lock == true) throw::runtime_error ("molecule locked.");

            for (auto &carb : nc) {
                vector<int> arm = {0};

                for (int j = 1; j <= carb; j++) {
                    int id = base.size();

                    arm.push_back(id);
                    base.push_back({id, "C"});

                    if (j > 1) {
                        link(j-0, j-1);
                    }
                }
                index.push_back(arm);
            }

            return *this;
        }
        Molecule &bond (const vector<vector<int>> &bn) {
            if (lock == true) throw::runtime_error ("molecule locked.");

            for (auto &b : bn) {
                int c1 = b[0], b1 = b[1], c2 = b[2], b2 = b[3];

                if (index[b1][c1] == index[b2][c2]) {
                    break;
                }
                link(index[b1][c1], index[b2][c2]);
            }

            return *this;
        }

        Molecule &add (const vector<tuple<int,int,string>> &next) {
            if (lock == true) throw::runtime_error ("molecule locked.");

            for (auto &[nc, nb, elt] : next) {
                if (base[index[nb][nc]].edge.size() < valence (elt)) {
                    int id = base.size();

                    base.push_back(create(id, elt));
                    link(id, index[nb][nc]);
                } else {
                    throw::runtime_error ("invalid bond.");
                }
            }

            return *this;
        }
        Molecule &add_chain (int nc, int nb, vector<string> elt) {
            if (lock == true) throw::runtime_error ("molecule locked.");
            
            for (int i = 0; i < elt.size(); i++) {
                int val = valence(elt[i]) - ((i < elt.size() - 1) ? 2 : 1);

                if (val < 0) throw::runtime_error("Invalid Bond.");
            }

            vector<int> arm(elt.size());

            for (int i = 0; i < elt.size(); i++) {
                arm[i] = base.size();
                base.push_back(create(arm[i], elt[i]));

                if (i > 0) {
                    link(arm[i-1], arm[i-0]);
                }
            }


            link(index[nb][nc], arm[0]);

            return *this;
        }
        Molecule &mutate (const vector<tuple<int,int,string>> &next) {
            if (lock == true) throw::runtime_error ("molecule locked.");

            for (auto &[nc, nb, elt] : next) {
                Atom &curr = base[index[nb][nc]];
                // cout << curr.element << curr.id << " :: " << curr.edge.size() << " " << valence(elt) << "\n";
                if (curr.edge.size() <= valence (elt)) {
                    curr.element = elt;

                    for (auto &edg : curr.edge) {
                        for (auto &link : base[edg.id].edge) {
                            if (link.id == curr.id) {
                                link.element = elt;
                            }
                        }
                    }
                } else {
                    throw::runtime_error ("invalid bond.");
                }
            }

            return *this;
        }

        Molecule &close () {
            lock = true;

            for (int i = 1; i < base.size(); i++) {
                Atom atom = base[i];
                // int size = valence (atom.element);
                for (int j = 0; j < (valence (atom.element) - atom.edge.size()); j++) {
                    int id = base.size();

                    base.push_back(create(id, "H"));
                    link(id, i);
                }
            }
            // show();
            return *this;
        }
        Molecule &unlock () {
            lock = false;

            for (int i = 1; i < base.size(); i++) {
                Atom atom = base[i];
                vector<Atom> edge;

                for (auto &at2 : atom.edge) {
                    if (at2.element != "H") {
                        edge.push_back(at2);
                    }
                }

                base[i].edge = edge;
            }

            return *this;
        }
};

int main () {

    Molecule m ("biotin");

    m.branch({14,1,1});
    //m.bond({ {2,1,1,2},  {2,1,1,2}, {10,1,1,3}, {10,1,1,3}, {8,1,12,1}, {7,1,14,1} });
    //m.mutate({{1,1,"O"},{1,2,"O"}, {1,3,"O"}, {11,1,"N"}, {9,1,"N"}, {14,1,"S"}});

    //m.close();

    //cout << m.name() << " => " <<  m.formula() << " :: " << m.molecular_weight();
    //cout << "\nend\n";
}
