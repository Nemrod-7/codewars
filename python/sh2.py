
class SHA1:
    def __init__(self):
        self.data = b''
        self.h = [ 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 ]

    def update(self, data):
        self.data += data

    def rotate(self, n, b):
        return ((n << b) | (n >> (32 - b))) & 0xFFFFFFFF

    def to_big_endian(self, value, length):
        """Convert an integer to big-endian bytes."""
        result = []
        for i in range(length - 1, -1, -1):
            result.append((value >> (i * 8)) & 0xFF)
        return bytes(result)

    def from_big_endian(self, data):
        """Convert big-endian bytes to an integer."""
        result = 0
        for byte in data:
            result = (result << 8) | byte
        return result

    def padding(self):
        padding = b"\x80" + b"\x00" * (63 - (len(self.data) + 8) % 64)
        length = len(self.data) * 8
        length_bytes = self.to_big_endian(length, 8)
        return self.data + padding + length_bytes

    def split_blocks(self):
        return [
            self.padded_data[i:i + 64]
            for i in range(0, len(self.padded_data), 64)
        ]

    def expand_block(self, block):
        w = [self.from_big_endian(block[i:i + 4]) for i in range(0, 64, 4)] + [0] * 64
        for i in range(16, 80):
            w[i] = self.rotate(
                w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1
            )
        return w

    def digest(self):
        self.padded_data = self.padding()
        self.blocks = self.split_blocks()
        
        for block in self.blocks:
            print(block)
            exp_block = self.expand_block(block)
            a, b, c, d, e = self.h
            for i in range(80):
                if 0 <= i < 20:
                    f = (b & c) | ((~b) & d)
                    k = 0x5A827999
                elif 20 <= i < 40:
                    f = b ^ c ^ d
                    k = 0x6ED9EBA1
                elif 40 <= i < 60:
                    f = (b & c) | (b & d) | (c & d)
                    k = 0x8F1BBCDC
                elif 60 <= i < 80:
                    f = b ^ c ^ d
                    k = 0xCA62C1D6
                a, b, c, d, e = (
                    self.rotate(a, 5) + f + e + k + exp_block[i] & 0xFFFFFFFF,
                    a,
                    self.rotate(b, 30),
                    c,
                    d,
                )
            self.h = (
                self.h[0] + a & 0xFFFFFFFF,
                self.h[1] + b & 0xFFFFFFFF,
                self.h[2] + c & 0xFFFFFFFF,
                self.h[3] + d & 0xFFFFFFFF,
                self.h[4] + e & 0xFFFFFFFF,
            )
        return ("{:08x}" * 5).format(*self.h).encode('ascii')

sha = SHA1()
input = b"\xc3sO\x13\xe3_\xfb\x86\x19\xa8.\x9a\xc0\x01.w6J\xc2\xff\xb7\xe2\xea\x88\xb5#\x19\xde\x1e\xf2\x00\xa0\x95\xee\xd9\xd2\xa8\x97\xc7=\xf5d\x08\xfd\xe9c5\xa7\x93US\xdd\x80\xa8 1\x01\xd6g\x19k\xf7\xa9P\x7fGG;\xe2r\xa2T\x1bM\xd1\xc9\xfd]\x86\xec\x98\x9f\xaa\x0f \xf5r\x15gs\xa33'z\xf2\xb6y\x87g/\xb4\x10\xc0t\x85!r\x1dTM3\x07\xfa\xb7:\xe5q\x9d\xc8\r\r4\xa2\xf9T\xe3\xb1|\xe7\xf4%\x8fw\xf0bo\xcb\xbc\xa4\xcco6c'\xff\xa0\xcf`J\x87\x07bs\xdb\x9f\x86E\x16\xf6\xc5-\xb3%\x94\x10\xfb\x08+u\xfc\\\xe0\xe0\xa7\x86\xd5\x03\xbd\xf8)?~;"
expect = b'179c6876e8e4a5b26c978df3f93124e874a01535'
sha.update(input)

print(sha.digest())


