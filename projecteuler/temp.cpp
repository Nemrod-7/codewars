#include <iostream>
#include <vector>
#include <deque>

#include <chrono>

using namespace std;

int main () {

    chrono::steady_clock::time_point alpha = chrono::steady_clock::now (), end;
    chrono::duration<double> elapsed;

    // Problem 149 : Searching for a maximum-sum subsequence
    // Finally, find the greatest sum of (any number of) adjacent entries in any direction (horizontal, vertical, diagonal or anti-diagonal).

    // Initialize char for printing
    // loading bar
    char a = 177, b = 219;

    printf("\n\n\n\n");
    printf("\n\n\n\n\t\t\t\t\tLoading...\n\n");
    printf("\t\t\t\t\t");

    // Print initial loading bar
    for (int i = 0; i < 26; i++)
        printf("%c", a);

    // Set the cursor again starting
    // point of loading bar
    printf("\r");
    printf("\t\t\t\t\t");

    // Print loading bar progress
    for (int i = 0; i < 26; i++) {
        printf("%c", b);

        // Sleep for 1 second
        Sleep(1000);
    }


    end = chrono::steady_clock::now (), elapsed = end - alpha;
    std::cout << "\nDuration " <<fixed<< elapsed.count() << " ms" << std::endl;
}
