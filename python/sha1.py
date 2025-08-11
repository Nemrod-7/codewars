H0 = [0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0]
K = [0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6]

def F1(B, C, D) : return (B & C) | ((~B) & D)
def F2(B, C, D) : return B ^ C ^ D
def F3(B, C, D) : return (B & C) | (B & D) | (C & D)
def F4(B, C, D) : return B ^ C ^ D

def rotate(val, n) :
    return (val << n) ^ (val >> (32-n));

def add (a,b) :
    return (a + b) % 4294967296

def preprocess(txt) :
    end = (448 - ((len(txt) * 8 + 8) - 64)) // 8 - 1

    txt += chr(0x01)

    for i in range(end) :
        txt += chr(0x00)

    txt += chr(len(txt) * 8)
    return txt


txt = 'Hello'
msg = [ord(it) for it in preprocess(txt)]
# compression

w = [0] * 90
for i in range(16) :
    w[i] = msg[i * 4 + 0] << 24 | msg[i * 4 + 1] << 16 | msg[i * 4 + 2] <<  8 | msg[i * 4 + 3]

for i in range(16,80) :
    w[i] = rotate(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);

hash = H0.copy()
a,b,c,d,e = H0

for i in range(80) :
    if i < 20 :
        f,k = F1(b,c,d), K[0]
    elif i < 40 :
        f,k = F2(b,c,d), K[1]
    elif i < 60 :
        f,k = F3(b,c,d), K[2]
    else :
        f,k = F4(b,c,d), K[3]
    # e + f + k + rotate(a,5) + w[i]
    # s1 = add(e , f);
    # s2 = add(s1 , rotate(a,5));
    # s3 = add(s2 , w[i]);
    # s4 = add(s3 , k);
    e = d;
    d = c;
    c = rotate(b,30);
    b = a;
    a = e + f + k + rotate(a,5) + w[i]

digest = [a,b,c,d,e]
hash = [add(digest[i] , hash[i]) for i in range(5)]

print(msg)
# hash = [format(ha, '08x') for ha in hash]
