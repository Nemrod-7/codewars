int rgb(int r, int g, int b, char *output) {

  int colors[3] = {r,g,b};

  for (int i = 0; i < 3; ++i) {
    if (colors[i] > 255) colors[i] = 255;
    if (colors[i] < 0) colors[i] = 0;
    output += sprintf (output,"%02X",colors[i]);
  }
   return 0;
}
