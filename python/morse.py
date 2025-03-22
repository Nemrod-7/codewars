
import numpy as np
from scipy.cluster.vq import whiten, kmeans
import matplotlib.pyplot as plt


MORSE_CODE = { '.-': 'A', '-...': 'B', '-.-.': 'C', '-..': 'D', '.': 'E', '..-.': 'F', '--.': 'G', '....': 'H', '..': 'I', '.---': 'J', '-.-': 'K', '.-..': 'L', '--': 'M', '-.': 'N',
    '---': 'O', '.--.': 'P', '--.-': 'Q', '.-.': 'R', '...': 'S', '-': 'T', '..-': 'U', '...-': 'V', '.--': 'W', '-..-': 'X', '-.--': 'Y', '--..': 'Z', '-----': '0', '.----': '1',
    '..---': '2', '...--': '3', '....-': '4', '.....': '5', '-....': '6', '--...': '7', '---..': '8', '----.': '9', '.-.-.-': '.', '--..--': ',', '..--..': '?', '.----.': '`',
    '-.-.--': '!', '-..-.': '/', '-.--.': '(', '-.--.-': ')', '.-...': '&', '---...': ':', '-.-.-.': ';', '-...-': '=', '.-.-.': '+', '-....-': '-', '..--.-': '_', '.-..-.': '"',
    '...-..-': '$', '.--.-.': '@', '...-.-': 'End of work', '........': 'Error', '-.-.-': 'Starting Signal', '...-.': 'Understood', '...---...': 'SOS', '': ' ',
}

def decode_bits2 (bin) :
    morse = ''
    token = bin.strip('0').replace("01", "0 1").replace("10", "1 0").split()
    rate = len(min(token))

    for sign in token :
        if sign == '1' * rate * 1 : morse += '.'
        if sign == '1' * rate * 3 : morse += '-'
        if sign == '0' * rate * 3 : morse += ' '
        if sign == '0' * rate * 7 : morse += '  '

    return morse

def mkunit2 (token) :
    ones, zero = {}, {}

    for cell in token :
        x = len(cell)
        if cell[0] == '0' : 
            zero[x] = zero[x] + 1 if x in zero else 1
        if cell[0] == '1' : 
            ones[x] = ones[x] + 1 if x in ones else 1

    zero, ones = np.array([[x,zero[x]] for x in zero]), np.array([[x,ones[x]] for x in ones])

    c0, c1 = kmeans(whiten(zero), 3)[0], kmeans(whiten(ones), 2)[0]
    r0, r1 = np.sort(c0[:,0] * np.std(zero)), np.sort(c1[:,0] * np.std(zero))

    u1 = np.mean([r0[0], r1[0]])
    u2 = np.mean([r0[1], r1[1]])
    u3 = np.mean([r0[2], r1[1]])

    # u1 = min([r0[0], r1[0]])
    # u2 = np.mean([r0[1], r1[0]])
    # u3 = max([r0[2], r1[1]])

    # plt.scatter(zero[:, 0], zero[:, 1])
    # plt.scatter(ones[:, 0], ones[:, 1])
    # plt.scatter([u1,u2,u3], [1,1,1])
    # plt.show()

    # print(u1, u2, u3)
    # return [round(u1), round(u2), round(u3)]
    return [u1,u2,u3]

def type (length, units) :
    if abs (length - units[0]) < abs (length - units[1]) : return 0
    if abs (length - units[2]) < abs (length - units[1]) : return 2
    return 1

def decode_bits (bin) :
    morse = ''
    rate = mkunit2(token)

    print(rate)

    for sign in token :
        bit = sign[0]

        match type(len(sign), rate) :
            case 0 : morse += '.' if bit == '1' else ''
            case 1 : morse += '-' if bit == '1' else ' '
            case 2 : morse += ''  if bit == '1' else '  '

    print(morse)
    return morse

def decode_morse (morse):
    code = morse.strip().split(' ')
    return ''.join( [' ' if sign == '' else MORSE_CODE[sign] for sign in code]).replace('  ',  ' ')

class test :
    def assert_equals(actual, expect) :

        if actual != expect :
            print ('actual : ', actual)
            print ('expect : ', expect)

msg = '0000000011011010011100000110000001111110100111110011111100000000000111011111111011111011111000000101100011111100000111110011101100000100000'
token = msg.strip('0').replace("01", "0 1").replace("10", "1 0").split()

# ones = [it for it in bin.split('0') if it != '']
# zero = [it for it in bin.split('1') if it != '']

morse = decode_bits(msg)
text = decode_morse(morse)
print(text)


