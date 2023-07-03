#include <iostream>
#include <thread> // this_thread::sleep_for() and thread objects
#include <chrono> // chrono::milliseconds()

bool running = true;


void WaitForKey() {
  while (running == true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    // delayms(1000);
  }
}



int main() {
  std::thread t1 (WaitForKey);

  while (running) {
    std::cout << "\e[1;31mWaiting for a key, Enter 'y' for yes\e[0m" << std::endl;





    if (std::cin.get() == 'y') {
        running = false;
    }
  }

  if (t1.joinable())
    t1.join();

  std::cout<<"Exit of Main function"<<std::endl;
  return 0;
}
