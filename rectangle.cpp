#include <iostream>
#include <vector>

using namespace std;

using point = pair<int,int>;
struct rectangle { int x1,y1, x2,y2; };

int area (rectangle rec) { return abs (rec.x2 - rec.x1) * abs (rec.y2 - rec.y1); }
bool is_in_rect (int px, int py, rectangle r) { return (px >= r.x1 && px <= r.x2 && py >= r.y1 && py <= r.y2); }
bool overlap (rectangle r1, rectangle r2) { return r1.x1 <= r2.x2 && r2.x1 <= r1.x2 && r1.y1 <= r2.y2 && r2.y1 <= r1.y2; }

int overarea(rectangle r1, rectangle r2) {
		int x_dist = min(r1.x2, r2.x2)- max(r1.x1, r2.x1);
		int y_dist = min(r1.y2, r2.y2) - max(r1.y1, r2.y1);
		if (x_dist >= 0 && y_dist >= 0) return x_dist * y_dist;
		return 0;
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
				for (int j = i + 1; j < graph.size(); j++) {


						if (overlap (graph[i], graph[j]) == true) {
								int over = overarea (graph[i], graph[j]);
								total -= over;
								/*
								cout << i << ' ' << j << " => ";
								cout << over << " " ;

								cout << endl;
								*/
						}

				}
		}

		cout << total << ' ';

}
