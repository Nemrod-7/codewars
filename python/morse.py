import numpy as np
# from scipy.cluster.vq import whiten, kmeans
# import matplotlib.pyplot as plt

MORSE_CODE = { '.-': 'A', '-...': 'B', '-.-.': 'C', '-..': 'D', '.': 'E', '..-.': 'F', '--.': 'G', '....': 'H', '..': 'I', '.---': 'J', '-.-': 'K', '.-..': 'L', '--': 'M', '-.': 'N',
    '---': 'O', '.--.': 'P', '--.-': 'Q', '.-.': 'R', '...': 'S', '-': 'T', '..-': 'U', '...-': 'V', '.--': 'W', '-..-': 'X', '-.--': 'Y', '--..': 'Z', '-----': '0', '.----': '1',
    '..---': '2', '...--': '3', '....-': '4', '.....': '5', '-....': '6', '--...': '7', '---..': '8', '----.': '9', '.-.-.-': '.', '--..--': ',', '..--..': '?', '.----.': '`',
    '-.-.--': '!', '-..-.': '/', '-.--.': '(', '-.--.-': ')', '.-...': '&', '---...': ':', '-.-.-.': ';', '-...-': '=', '.-.-.': '+', '-....-': '-', '..--.-': '_', '.-..-.': '"',
    '...-..-': '$', '.--.-.': '@', '...-.-': 'End of work', '........': 'Error', '-.-.-': 'Starting Signal', '...-.': 'Understood', '...---...': 'SOS', '': ' ',
}

class k_means :
    def nearest_point (graph, p1) :
        nearest = []
        min = np.inf

        for p2 in graph :
            dist = np.hypot(p1[0] - p2[0], p1[1] - p2[1])

            if dist < min :
                min = dist
                nearest = p2

        return nearest

    def clust (graph, seeds, iter = 10) :
        npts = [0 for _ in range(len(seeds)) ]
        sumX = [0 for _ in range(len(seeds)) ]
        sumY = [0 for _ in range(len(seeds)) ]

        for _ in range(iter) :
            for p in graph :
                near = k_means.nearest_point(seeds, p)

                for i in range(len(seeds)) :
                    if near[0] == seeds[i][0] and near[1] == seeds[i][1] :
                        npts[i] += 1
                        sumX[i] += p[0]
                        sumY[i] += p[1]

            for i in range(len(seeds)) :
                # if npts[i] == 0 : continue
                seeds[i][0] = sumX[i] / npts[i]
                seeds[i][1] = sumY[i] / npts[i]

        # print(seeds)
        return seeds
        


def mk_seeds (grph, mode) :
    size = len(grph) / 7.0
    if not size : return []
    mid = round(grph[0][0] * 3 * size)
    V = [grph[0]]
    if mode > 2 : V.append(np.array([mid, 0.0]))
    V.append(grph[-1])
    return np.array(V)

def mkunit (token) :
    ones, zero = {}, {}

    for cell in token :
        x = len(cell)

        if cell[0] == '0' : zero[x] = zero[x] + 1 if x in zero else 1
        if cell[0] == '1' : ones[x] = ones[x] + 1 if x in ones else 1

    zero, ones = np.array([[x,zero[x]] for x in zero]).astype(float), np.array([[x,ones[x]] for x in ones]).astype(float)

    zero = sorted(zero,key = lambda x: x[0])
    ones = sorted(ones,key = lambda x: x[0])

    # print(zero,ones)
    # c0, c1 = kmeans(whiten(zero), 3)[0], kmeans(whiten(ones), 2)[0]
    # r0, r1 = np.sort(c0[:,0] * np.std(zero, 0)[0]), np.sort(c1[:,0] * np.std(ones, 0)[0])
    # c0, c1 = kmeans(zero, mk_seeds(zero, 3))[0], kmeans(ones, mk_seeds(ones, 2))[0]

    u0 = min(len(zero), 3)
    u1 = min(len(ones), 2)

    c0, c1 = k_means.clust(zero, mk_seeds(zero, u0)), k_means.clust(ones, mk_seeds(ones, u1))
    r0, r1 = np.sort(c0[:,0]), np.sort(c1[:,0])
    # print(c0 * np.std(zero, 0))
    # print(c1 * np.std(ones, 0))

    u1 = np.min([r0[0], r1[0]])
    u2 = np.mean([r0[1], r1[-1]])
    u3 = np.max([r0[-1], r1[-1]])

    # plt.scatter(zero[:, 0], zero[:, 1])
    # plt.scatter(ones[:, 0], ones[:, 1])
    # plt.scatter([u1,u2,u3], [1,1,1])
    # plt.show()

    print(u1, u2, u3)
    return [u1,u2,u3]

def unit (length, units) :
    if abs (length - units[0]) < abs (length - units[1]) : return 0
    if abs (length - units[2]) < abs (length - units[1]) : return 2
    return 1

def decodeBitsAdvanced (bin) :
    morse = ''
    token = bin.strip('0').replace("01", "0 1").replace("10", "1 0").split()

    match token :
        case [   ] : return morse
        case ['1'] : return '.'
        case ['0'] : return ''

    rate = mkunit(token)

    for sign in token :
        bit = sign[0]

        match unit(len(sign), rate) :
            case 0 : morse += '.' if bit == '1' else ''
            case 1 : morse += '-' if bit == '1' else ' '
            case 2 : morse += ''  if bit == '1' else '  '

    print(morse)
    return morse

def decodeMorse (morse) :
    if morse == '' : return ''
    code = morse.strip().split(' ')
    msg = ''

    for glyph in code :
        msg += MORSE_CODE[glyph] if glyph in MORSE_CODE else '_'

    # print(msg)
    # return ''.join( [' ' if sign == '' else MORSE_CODE[sign] for sign in code]).replace('  ',  ' ')
    return msg

class test :
    def assert_equals(actual, expect) :

        if actual != expect :
            print ('actual : ', actual)
            print ('expect : ', expect)

text = '[' + decodeMorse(decodeBitsAdvanced('101')) + ']'
print(text)

#
# msg = '0000000011011010011100000110000001111110100111110011111100000000000111011111111011111011111000000101100011111100000111110011101100000100000'
#
# msg = "00000000000111111100000011010001110111000000001110000000000000000001111111011111100001101111100000111100111100011111100000001011100000011111110010001111100110000011111100101111100000000000000111111100001111010110000011000111110010000011111110001111110011111110000010001111110001111111100000001111111101110000000000000010110000111111110111100000111110111110011111110000000011111001011011111000000000000111011111011111011111000000010001001111100000111110111111110000001110011111100011111010000001100001001000000000000000000111111110011111011111100000010001001000011111000000100000000101111101000000000000011111100000011110100001001100000000001110000000000000001101111101111000100000100001111111110000000001111110011111100011101100000111111000011011111000111111000000000000000001111110000100110000011111101111111011111111100000001111110001111100001000000000000000000000000000000000000000000000000000000000000" # "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG"
#
# # ones = [it for it in bin.split('0') if it != '']
# # zero = [it for it in bin.split('1') if it != '']
# def testAndPrint(got, expected):
#     if got != expected:
#         print("<pre style='display:inline'>Got '%s', expected '%s'</pre>" % (got, expected))
#
# testAndPrint(decodeMorse(decodeBitsAdvanced('0000000011011010011100000110000001111110100111110011111100000000000111011111111011111011111000000101100011111100000111110011101100000100000')), 'HEY JUDE')
#
# text = decodeMorse(decodeBitsAdvanced("00000000000111111100000011010001110111000000001110000000000000000001111111011111100001101111100000111100111100011111100000001011100000011111110010001111100110000011111100101111100000000000000111111100001111010110000011000111110010000011111110001111110011111110000010001111110001111111100000001111111101110000000000000010110000111111110111100000111110111110011111110000000011111001011011111000000000000111011111011111011111000000010001001111100000111110111111110000001110011111100011111010000001100001001000000000000000000111111110011111011111100000010001001000011111000000100000000101111101000000000000011111100000011110100001001100000000001110000000000000001101111101111000100000100001111111110000000001111110011111100011101100000111111000011011111000111111000000000000000001111110000100110000011111101111111011111111100000001111110001111100001000000000000000000000000000000000000000000000000000000000000")) # print(text)
