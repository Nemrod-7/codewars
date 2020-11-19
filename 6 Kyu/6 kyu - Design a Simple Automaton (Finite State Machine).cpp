#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

class Automaton {
    enum {q1,q2,q3};

    public:
      bool read_commands(const std::vector<char>& commands) {
            int state = q1;

            for (auto &c : commands)
                switch (state) {
                    case q1 : if (c == '1') state = q2; break;
                    case q2 : if (c == '0') state = q3; break;
                    case q3 : state = q2; break;
                }

            return state == q2;
      }
};
