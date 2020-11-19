#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

inline std::string to_brainfuck (const std::string& s_in) ;

int main () {

    auto start = chrono::high_resolution_clock::now();

    to_brainfuck ("Hello World!");


    auto end = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Process took " << elapsed.count()  << " ms" << endl;

    return 0;
}

inline std::string to_brainfuck (const std::string& s_in) {


    return std::string ();
}
