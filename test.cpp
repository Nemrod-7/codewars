// #pragma once

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main () {

  double amp = 1.0; // Amplitude of the sine wave
  double freq = 1.0; // Frequency of the sine wave
  double phase = 0.0; // Phase of the sine wave

  for (double x = 0; x < 40; x += 1) {
      double y = amp * sin (freq * x + phase);
      std::cout << x << " " << y << std::endl;

  }

  for(x = 0; x < getmaxx(); x+=3) {
       /* calculate y value given x */
       y = 50*sin(angle*3.141/180);
       y = getmaxy()/2 - y;

       /* color a pixel at the given position */
    putpixel(x, y, 15);
    delay(100);

    /* increment angle */
    angle+=5;
   }
  cout << "\nend\n";
  return 0;
}
