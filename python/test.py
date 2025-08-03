N = 7
full = (1 << (N + 1)) - 2;

text = "Hi"

ecc = 'H'
npol = 17


bits  = '010000000010010010000110100100001110110000010001111011000001000111101100'

exp = len(bits) // 8 + npol
polynomial = ''

for i in range(0, len(bits), 8) :
    exp -= 1
    dec = int( bits[i:i+8], 2)

    polynomial += str(dec) + 'x^' + str(exp) + ' + '

print(polynomial)
