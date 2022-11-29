#include <iostream>
#include <vector>

using namespace std;

using point = pair<int,int>;
struct rectangle {
		int x0,y0;
		int x1,y1;
};

int area (rectangle rec) {
		auto [x0,y0, x1,y1] = rec;
		return abs (x1 - x0) * abs (y1 - y0);
}
bool is_in_rect (int px, int py, rectangle r) {
		return (px >= r.x0 && px <= r.x1 && py >= r.y0 && py <= r.y1);
}
bool overlap (rectangle r1, rectangle r2) {
		if (r1.x0 == r1.x1 || r1.y0 == r1.y1) return false; // rectamgle 1 has area zero
		if (r2.x0 == r2.x1 || r2.y0 == r2.y1) return false; // rectamgle 2 has area zero

		if (r1.x0 > r2.x1 || r2.x0 > r1.x1) return false; // if one rectangle is on the left of the other
		if (r1.y1 > r2.y0 || r2.y1 > r1.y0) return false; // if one rectangle is obove other
		
		return true;
}
int main () {

// There are three rectangles: R1 = [3,3,8,5], R2 = [6,3,8,9], R3 = [11,6,14,12]
// S(R1) = 10, S(R2)= 12, S(R3) =  18
// S(R1 ∩ R2) = 4, S(R1 ∩ R3) = 0,  S(R2 ∩ R3) = 0
// S = S(R1) + S(R2) + S(R3) - S(R1 ∩ R2) - S(R1 ∩ R3) - S(R2 ∩ R3) = 36

		rectangle r1 = {3,3,8,5}, r2 = {6,3,8,9}, r3 = {11,6,14,12};
		vector<rectangle> graph = {r1,r2,r3};

		int total = 0;

		for (auto rec : graph) {
				auto [x0,y0, x1,y1] = rec;

				int h = abs(x0 - x1), l = abs(y0 - y1);
				int area = h * l;
				total += area;
		}

		for (int i = 0; i < graph.size(); i++) {
				auto [x0,y0, x1,y1] = graph[i];

				for (int j = 0; j < graph.size(); j++) {
						auto [x2,y2,x3,y3] = graph[j];
						if (i != j) {

								if (is_in_rect(x2,y2, graph[i])) {

										cout << i << ' ' << j << endl;
								}
						}
				}
		}

	//	cout << total << ' ';

}
