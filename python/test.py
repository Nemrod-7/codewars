H0 = [0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0]
K = [0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6]

def F1(B, C, D) : return (B & C) | ((~B) & D)
def F2(B, C, D) : return B ^ C ^ D
def F3(B, C, D) : return (B & C) | (B & D) | (C & D)
def F4(B, C, D) : return B ^ C ^ D


def preprocess(txt) :
    end = (448 - ((len(txt) * 8 + 8) - 64)) // 8 - 1

    txt += chr(0x01)

    for i in range(end) :
        txt += chr(0x00)

    txt += chr(len(txt) * 8)
    return txt


msg = preprocess('test')

w = [0] * 90

for i in range(16) :
    w[i] = msg[i * 4 + 0] << 24 | msg[i * 4 + 1] << 16 | msg[i * 4 + 2] <<  8 | msg[i * 4 + 3]

