#include <iostream>
#include <vector>

using namespace std;

using point = pair<int,int>;
using rectangle = vector<int>;

int area (rectangle rec) { return abs (rec[3] - rec[0]) * abs (rec[4] - rec[1]); }
bool is_in_rect (int px, int py, rectangle r) { return (px >= r[0] && px <= r[3] && py >= r[1] && py <= r[4]); }
bool overlap (rectangle r1, rectangle r2) { return r1[0] <= r2[2] && r2[0] <= r1[2] && r1[1] <= r2[3] && r2[1] <= r1[3]; }

int overarea(rectangle r1, rectangle r2) {
		int width = min(r1[2], r2[2])- max(r1[0], r2[0]);
		int height = min(r1[3], r2[3]) - max(r1[1], r2[1]);
		if (width >= 0 && height >= 0) return width * height;
		return 0;
}

int main () {

		rectangle r1 = {3,3,8,5}, r2 = {6,3,8,9}, r3 = {11,6,14,12};
		vector<rectangle> graph = {r1,r2,r3};
		int total = 0;

		for (auto rec : graph) {
				int h = abs(rec[0] - rec[2]), l = abs(rec[1] - rec[3]);
				int area = h * l;
				total += area;

		}

		
		for (int i = 0; i < graph.size(); i++) {
				for (int j = i + 1; j < graph.size(); j++) {


						if (overlap (graph[i], graph[j]) == true) {
								int over = overarea (graph[i], graph[j]);
								total -= over;
								
								cout << i << ' ' << j << " => ";
								cout << over << " " ;

								cout << endl;
								
						}

				}
		}
		/*
		*/
		cout << total << ' ';

}
