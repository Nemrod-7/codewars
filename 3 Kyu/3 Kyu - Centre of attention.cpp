#include <iostream>
#include <vector>

using namespace std;

struct Image {
 unsigned *pixels;
 unsigned width, height;

 vector<unsigned> central_pixels(unsigned colour) const;
};

vector<unsigned> find_max (unsigned val, vector<unsigned> w) {

    vector<unsigned> out;
    unsigned i = w.size();

    if (val > 0)
        while (i-->0)
            if (w[i] == val)
                out.push_back(i);

    return out;
}
vector<unsigned> Image::central_pixels(unsigned color) const {

    unsigned size = height * width, val ,maxval = 0;
    vector<unsigned> w(size);

    for (unsigned i = 0; i < size ; ++i) {

        if (pixels[i] == color) {
            if ((i % width == 0) || (i < width))
                w[i] = 1;
            else
                w[i] = min (w[i - width], w[i - 1]) + 1;
        }
    }

    for (unsigned i = size; i-->0 ; ) {

        if (pixels[i] == color) {
            if (((i + 1) % width == 0) || ((i + width) >= size)) val = 1;
            else val = min (w [i + 1],w [i + width]) + 1;

            w[i] = min (w[i], val);
            maxval = max (maxval, w[i]);
        }
    }

    return find_max (maxval, w);
}

int main () {

}
