mod = 4294967296
print('\n\n\n')

def hex (txt) : return [format(ha, '08X') for ha in txt]
def rotl(n, d): return ((n << d) | (n >> (32 - d))) & 0xffffffff

class SHA1(object) :
    def preprocess(txt) :
        end = (448 - ((len(txt) * 8 + 8) - 64)) // 8 - 1
        msg = [it for it in txt]

        msg.append(0x80)
        msg += [0] * end
        msg.append(len(txt) * 8)

        return msg

    def update(self, msg) :
        self.msg = msg

    def digest(self) :
        msg = SHA1.preprocess(self.msg)

        w = [0] * 80
        for i in range(16) :
            w[i] = msg[i * 4 + 0] << 24 | msg[i * 4 + 1] << 16 | msg[i * 4 + 2] <<  8 | msg[i * 4 + 3]

        for i in range(16,80) :
            w[i] = rotl(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);

        hash = [0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0]
        a,b,c,d,e = hash

        for i in range(80) :
            if i < 20 :
                f,k = (b & c) | (~b & d), 0x5A827999
            elif i < 40 :
                f,k = b ^ c ^ d, 0x6ED9EBA1
            elif i < 60 :
                f,k = (b & c) | (b & d) | (c & d), 0x8F1BBCDC
            else :
                f,k = b ^ c ^ d, 0xCA62C1D6

            s1 = (e + f) % mod
            s2 = (s1 + rotl(a,5)) % mod
            s3 = (s2 + w[i]) % mod
            s4 = (s3 + k) % mod

            e,d,c,b,a = d % mod, c % mod, rotl(b,30) % mod, a % mod, s4 % mod

            digest = [a,b,c,d,e]

        return ''.join([format(digest[i] + hash[i], 'x') for i in range(5)])



def binary(num) :
    for i in range(32) :
        bit = num >> i &1
        print(bit,end='')
    print()

sha = SHA1()
sha.update(b'abc')
actual = sha.digest()
expect = b'a9993e364706816aba3e25717850c26c9cd0d89d'

if actual != expect :
    print('actual : ', actual,  '\nexpect :', expect)
