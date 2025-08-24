
mod = 4294967296

def rotl(n, d): return ((n << d) | (n >> (32 - d))) & 0xffffffff

class SHA1(object) :
    def __init__(self) -> None:
        self.msg = b''

    def update(self, msg) :
        self.msg += msg

    def preprocess(txt) :
        nzo = 64 - (len(txt) + 9) % 64

        msg = bytearray(txt)
        msg.append(0x80)
        msg += bytes(nzo)
        msg += (len(txt) * 8).to_bytes(8)

        return msg

    def compress(self, chunk) :
        w = [0] * 80
        hash = [0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0]
        a,b,c,d,e = hash

        for i in range(16) :
            w[i] = chunk[i * 4 + 0] << 24 | chunk[i * 4 + 1] << 16 | chunk[i * 4 + 2] <<  8 | chunk[i * 4 + 3]

        for i in range(16,80) :
            w[i] = rotl(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);

        for i in range(80) :
            if i < 20 :
                f,k = (b & c) | (~b & d), 0x5A827999
            elif i < 40 :
                f,k = b ^ c ^ d, 0x6ED9EBA1
            elif i < 60 :
                f,k = (b & c) | (b & d) | (c & d), 0x8F1BBCDC
            else :
                f,k = b ^ c ^ d, 0xCA62C1D6

            tmp = (rotl(a,5) +  e + f + k + w[i]) % mod
            e,d,c,b,a = d, c, rotl(b,30), a, tmp
            digest = [a,b,c,d,e]

        return digest

    def digest(self) :
        msg = SHA1.preprocess(self.msg)
        hash = [0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0]

        for i in range(0, len(msg), 64) :
            digest = self.compress(msg[i:i+64])
            hash = [(digest[i] + hash[i]) % mod for i in range(5)]
        hash = [format(hash[i], '08x') for i in range(5)]
        return bytes( ''.join(hash) , 'utf-8')


def assert_equals(actual, expect) :
    if actual != expect :
        print('actual :', actual,  '\nexpect :', expect)


# sha = SHA1()
# sha.update(b'abc')
# assert_equals(sha.digest(), b'a9993e364706816aba3e25717850c26c9cd0d89d')
#
# sha = SHA1()
# sha.update(b'hello this is a test')
# assert_equals(sha.digest(), b'f291f60cafb2ef2e0013f5a5889b1da5af4b4657')
#
# sha = SHA1()
# sha.update(b'codewars.com is awesome')
# assert_equals(sha.digest(), b'67f1d2416b4f0d24a22c9a79af1128bb40a808fb')

input = b'\x03\tiPKRp\xe6\x0f\xb7+\x03\xd2?Q\xfe*\xaa\xa1\xb2\xb5\xed\xa8s\x8fx\x07x\x10J\x12O\xc0\x80\xd3\xe7\xcf\x8e\xffY\xc7C\x9c\x97X\xf4d?^&9\xa6\x00\x83od\xc1\x17\xb1\xc7\xaeUl\xf5\xdf\x85G\xa0\xcf\t'
expect = b'6f9717b0646ae00c6895806a5bb0b4cac2affed9'

sha = SHA1()
sha.update(input)
actual = sha.digest()

print(actual)
