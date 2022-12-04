#include <iostream>
#include <vector>
#include <limits>

using namespace std;

class Assert {

		public:
				static void equals (int res, int exp, std::string str) {

						if (res != exp) {
								std::cout << str << " => ";
								std::cout << "got : " << res << " ";
								std::cout << "exp : " << exp << "\n";
						}
				}
};

class check {
		public :
			static void overflow (uint64_t a, double b) {
					uint64_t limit = numeric_limits<uint64_t>::max() - a;
					if (a > limit) throw overflow_error ("integer overflow\n");
			}
};

using point = pair<int,int>;
using rectangle = vector<int>;

int area (rectangle rec) { return abs (rec[3] - rec[0]) * abs (rec[4] - rec[1]); }
bool is_in_rect (int px, int py, rectangle r) { return (px >= r[0] && px <= r[3] && py >= r[1] && py <= r[4]); }
bool overlap (rectangle r1, rectangle r2) {
		return r1[0] <= r2[2] && r2[0] <= r1[2] && r1[1] <= r2[3] && r2[1] <= r1[3]; 
}

int overarea(rectangle r1, rectangle r2) {
		int width = min(r1[2], r2[2])- max(r1[0], r2[0]);
		int height = min(r1[3], r2[3]) - max(r1[1], r2[1]);
		if (width >= 0 && height >= 0) return width * height;
		return 0;
}
int calculate (vector<vector<int>> graph) {

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
									
		/*
								cout << i << ' ' << j << " => ";
								cout << over << " " ;

								cout << endl;
								
		*/
						}

				}
		}
		cout << total << ' ';

		return total;
}


int main () {

		rectangle r1 = {3,3,8,5}, r2 = {6,3,8,9}, r3 = {11,6,14,12};
		vector<rectangle> graph = {r1,r2,r3};

		uint64_t inf = std::numeric_limits<uint64_t>::max();

		/*
		(calculate({}), 0);
		//calculate (graph);
		(calculate({{0,0,1,1}}), 1);    
		(calculate({{0,4,11,6}}), 22);
		(calculate({{0,0,1,1},{1,1,2,2}}),2);
		(calculate({{0,0,1,1},{00,0,2,2}}),4);
		(calculate({{3,3,8,5},{6,3,8,9},{11,6,14,12}}),36);
*/

}

void Test () {
 vector<vector<int>> graph {{1, 4, 2, 7}, {1, 4, 2, 6}, {1, 4, 4, 5}, {2, 5, 6, 7}, {4, 3, 7, 6}}; // multiple intersections =>  1 point belongs to 3 rectanles at least.
																																																	 //
		Assert::equals(calculate({ { 1, 7, 3, 10 },  { 1, 8, 3, 9 }}), 6, "1 under 2");
		Assert::equals(calculate({ { 6, 7, 9, 10 },  { 7, 8, 8, 9 }}), 9, "nested");
		Assert::equals(calculate({ { 1, 7, 4, 10 },  { 2, 7, 4, 9 },  { 3, 7, 4, 9 }}), 9, "nested 2");
		Assert::equals(calculate({ { 1, 1, 4, 3 },  { 2, 2, 3, 4 }}), 7, "intersection up");
		Assert::equals(calculate({ { 5, 0, 7, 3 },  { 6, 1, 8, 2 }}), 7, "intersetion right");
		Assert::equals(calculate({ { 9, 0, 11, 2 },  { 10, 1, 12, 3 }}), 7, "intersection up right");
		Assert::equals(calculate({ { 13, 5, 15, 6 },  { 14, 4, 16, 7 }}), 7, "intersection of the entire right side");
		Assert::equals(calculate({ { 17, 1, 19, 3 },  { 18, 0, 20, 2 }}), 7, "intersection down right");
		Assert::equals(calculate({ { 13, 1, 16, 3 },  { 14, 0, 15, 2 }}), 7, "intersection down");
		Assert::equals(calculate({ { 1, 3, 4, 6 },  { 2, 1, 5, 4 },  { 3, 2, 6, 5 }}), 20, "intersection 3 rect");
		Assert::equals(calculate({{1,1,2,2},{2,1,3,2},{3,1,4,2},{1,2,2,3},{2,2,3,3},{3,2,4,3},{1,3,2,4},{2,3,3,4},{3,3,4,4}}), 9, "3*3");
		Assert::equals(calculate({{ 1, 1, 6, 6 },{ 1, 3, 4, 6 },{ 2, 3, 4, 6 },{ 2, 4, 5, 6 },{ 3, 5, 4, 6 }}), 25, "intersection");
		Assert::equals(calculate({{1,1,6,6},{2,1,6,6},{3,1,6,6},{4,1,6,6},{5,2,6,5}}), 25, "shift right");
		Assert::equals(calculate({{1,1,7,6},{2,2,8,7},{3,3,9,8},{4,4,10,9},{5,5,11,10}}), 70, "shift right down");
		Assert::equals(calculate({ { 1, 4, 5, 6 },  { 2, 5, 6, 7 },{ 3, 6, 7, 8 },{ 4, 7, 8, 9 },{ 2, 3, 6, 5 },{ 3, 2, 7, 4 },{ 4, 1, 8, 3 }}), 38, "wings");
		Assert::equals(calculate({ { 9, 5, 12, 6 },  { 10, 4, 11, 7 }}), 5, "intersection cross");
		Assert::equals(calculate({ { 7, 1, 11, 7 },  { 8, 0, 12, 3 },  { 8, 4, 13, 5 },  { 9, 5, 14, 8 },{ 10, 2, 15, 6 }}), 53, "intersection 2");
		Assert::equals(calculate({{1,2,6,6},{1,3,5,5},{1,1,7,7}}), 36, "pyramid");
		Assert::equals(calculate({{1,2,3,7},{2,1,7,3},{6,2,8,7},{2,6,7,8},{4,4,5,5}}), 37, "circle");
		Assert::equals(calculate({{1,1,2,2},{1,1,2,2},{1,1,2,2},{1,1,2,2},{1,1,2,2},{1,1,2,2}}), 1, "one");
		Assert::equals(calculate({{3,3,6,5},{4,4,6,6},{4,3,7,5},{4,2,8,5},{4,3,8,6},{9,0,11,4},{9,1,10,6},{9,0,12,2},{10,1,13,5},{12,4,15,6},{14,1,16,5},{12,1,17,2}}), 52, "very hard!");
		Assert::equals(calculate({{2, 2, 17, 2}, {2, 2, 17, 4}, {2, 2, 17, 6}, {2, 2, 17, 8}, {2, 2, 17, 10}, {2, 2, 17, 12}, {2, 2, 17, 14}, {2, 2, 17, 16}, {2, 2, 17, 18}, {2, 2, 17, 20}, {2, 2, 17, 22}, {2, 2, 17, 24}, {2, 2, 17, 26}, {2, 2, 17, 28}}), 390, "waterfall");


}
