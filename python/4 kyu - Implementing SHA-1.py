
class SHA1(object) :
    def __init__(self) -> None:
        self.msg = b''
        self.hash = [0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0]

    def update(self, msg) :
        self.msg += msg

    def rotl(n, d): return ((n << d) | (n >> (32 - d))) & 0xffffffff

    def preprocess(txt) :
        msg = bytearray(txt)
        msg.append(0x80)
        msg += bytes( 64 - (len(txt) + 9) % 64 )
        msg += (len(txt) * 8).to_bytes(8)

        return [msg[i:i+64] for i in range(0,len(msg), 64)]

    def compress(self, block) :
        a,b,c,d,e = self.hash
        w = [0] * 80

        for i in range(16) :
            w[i] = block[i * 4 + 0] << 24 | block[i * 4 + 1] << 16 | block[i * 4 + 2] <<  8 | block[i * 4 + 3]

        for i in range(16,80) :
            w[i] = SHA1.rotl(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);

        for i in range(80) :
            if i < 20 :
                f,k = (b & c) | (~b & d), 0x5A827999
            elif i < 40 :
                f,k = b ^ c ^ d, 0x6ED9EBA1
            elif i < 60 :
                f,k = (b & c) | (b & d) | (c & d), 0x8F1BBCDC
            else :
                f,k = b ^ c ^ d, 0xCA62C1D6

            tmp = (SHA1.rotl(a,5) +  e + f + k + w[i]) &0xFFFFFFFF
            e,d,c,b,a = d, c, SHA1.rotl(b,30), a, tmp

        return [a,b,c,d,e]

    def digest(self) :
        msg = SHA1.preprocess(self.msg)
        for block in msg :
            digest = self.compress(block)
            self.hash = [(digest[i] + self.hash[i]) &0xFFFFFFFF for i in range(5)]

        self.hash = [ format(self.hash[i], '08x') for i in range(5) ]
        return bytes( ''.join(self.hash) , 'utf-8')


def assert_equals(actual, expect) :
    if actual != expect :
        print('actual :', actual,  '\nexpect :', expect)


sha = SHA1()
sha.update(b'abc')
assert_equals(sha.digest(), b'a9993e364706816aba3e25717850c26c9cd0d89d')

sha = SHA1()
sha.update(b'hello this is a test')
assert_equals(sha.digest(), b'f291f60cafb2ef2e0013f5a5889b1da5af4b4657')

sha = SHA1()
sha.update(b'codewars.com is awesome')
assert_equals(sha.digest(), b'67f1d2416b4f0d24a22c9a79af1128bb40a808fb')


sha = SHA1()
input = b'\x03\tiPKRp\xe6\x0f\xb7+\x03\xd2?Q\xfe*\xaa\xa1\xb2\xb5\xed\xa8s\x8fx\x07x\x10J\x12O\xc0\x80\xd3\xe7\xcf\x8e\xffY\xc7C\x9c\x97X\xf4d?^&9\xa6\x00\x83od\xc1\x17\xb1\xc7\xaeUl\xf5\xdf\x85G\xa0\xcf\t'
sha.update(input)
assert_equals(sha.digest(), b'6f9717b0646ae00c6895806a5bb0b4cac2affed9')

sha = SHA1()
input = b'F\xfd\xf9r\x85\x14\x86"\xd3\xb1\xd3\x99\xcc\xc8ZMQ\x92\x93\xdb\x8a\x06\xf8\x92\x1b.\n\x18\x15\x1b\xa1\xa9\n\xe1y"\x94\xe0\x1a\x16`\x01\x0f\x87.\xc4\x130\x86z\xb1\xb0\xb0Y\x08\x14D\xce\xcfl\xa8\xbc&_'
expect = b'3bae1bf6bf50c789467f5b8ae78f909453988e98'
sha.update(input)
assert_equals(sha.digest(), expect)


# print(input)
