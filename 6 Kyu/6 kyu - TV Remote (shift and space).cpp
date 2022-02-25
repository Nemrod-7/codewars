using namespace std;
using point = pair<int,int>;
const string pad = "abcde123fghij456klmno789pqrst.@0uvwxyz_/| ";

int distance (const point a, const point b) { 
    return abs (a.first - b.first) + abs(a.second - b.second); 
}
bool shift (const char tile, const bool maj) { return ((isupper (tile) && !maj) || (islower(tile) && maj)); }
point getpos (char curr) {
    int idx = pad.find (curr);
    return {idx % 8, idx / 8};
}
int tv_remote(const string &src) {
    int dist = 0;
    bool maj = false;
    point shft = getpos(tolower ('|')), prev = {0,0}, now;

    for (auto &it : src) {
        now = getpos (tolower (it));

        if (shift (it, maj)) {
            dist += distance (prev, shft) + distance (shft, now) + 2;
            maj ^= 1;
        } else {
            dist += distance (prev, now) + 1;
        }

        prev = now;
    }

    return dist;
}
