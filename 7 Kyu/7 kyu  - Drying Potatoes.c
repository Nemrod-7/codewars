int potatoes(int p0, int w0, int p1) {
float w1 = 0;

    w1 = ((w0 * 100) - (w0 * p0)) / (100 - p1);

return (int)w1;
}
