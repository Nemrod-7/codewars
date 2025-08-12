txt = b'abc'

def rotl(n, d): return ((n << d) | (n >> (32 - d))) & 0xffffffff

msg = bytearray(txt)
end = (448 - ((len(txt) * 8 + 8) - 64)) // 8 - 1

msg.append(0x80)
msg += bytes(end)
msg.append(len(txt) * 8)

w = [0] * 80

for i in range(16) :
    w[i] = msg[i * 4 + 0] << 24 | msg[i * 4 + 1] << 16 | msg[i * 4 + 2] <<  8 | msg[i * 4 + 3]

for i in range(16,80) :
    w[i] = rotl(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1)

hash = [0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0]

hash = ''.join([format(x,'x') for x in hash])

print(bytes(hash, 'utf-8'))
