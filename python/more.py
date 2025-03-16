
MORSE_CODE = { '.-': 'A', '-...': 'B', '-.-.': 'C', '-..': 'D', '.': 'E', '..-.': 'F', '--.': 'G', '....': 'H', '..': 'I', '.---': 'J', '-.-': 'K', '.-..': 'L', '--': 'M', '-.': 'N',
    '---': 'O', '.--.': 'P', '--.-': 'Q', '.-.': 'R', '...': 'S', '-': 'T', '..-': 'U', '...-': 'V', '.--': 'W', '-..-': 'X', '-.--': 'Y', '--..': 'Z', '-----': '0', '.----': '1',
    '..---': '2', '...--': '3', '....-': '4', '.....': '5', '-....': '6', '--...': '7', '---..': '8', '----.': '9', '.-.-.-': '.', '--..--': ',', '..--..': '?', '.----.': '`',
    '-.-.--': '!', '-..-.': '/', '-.--.': '(', '-.--.-': ')', '.-...': '&', '---...': ':', '-.-.-.': ';', '-...-': '=', '.-.-.': '+', '-....-': '-', '..--.-': '_', '.-..-.': '"',
    '...-..-': '$', '.--.-.': '@', '...-.-': 'End of work', '........': 'Error', '-.-.-': 'Starting Signal', '...-.': 'Understood', '...---...': 'SOS', '': ' ',
}

def decode_morse(source):
    code = source.strip().split(' ')
    return ''.join( [' ' if sign == '' else MORSE_CODE[sign] for sign in code]).replace('  ',  ' ')

class test :
    def assert_equals(actual, expect) :

        if actual != expect :
            print ('actual : ', actual)
            print ('expect : ', expect)

test.assert_equals(decode_morse('.... . -.--   .--- ..- -.. .'), 'HEY JUDE')

test.assert_equals(decode_morse('.-'), 'A')
test.assert_equals(decode_morse('--...'), '7')
test.assert_equals(decode_morse('...-..-'), '$')
test.assert_equals(decode_morse('.'), 'E')
test.assert_equals(decode_morse('..'), 'I')
test.assert_equals(decode_morse('. .'), 'EE')
test.assert_equals(decode_morse('.   .'), 'E E')
test.assert_equals(decode_morse('...-..- ...-..- ...-..-'), '$$$')
test.assert_equals(decode_morse('----- .---- ..--- ---.. ----.'), '01289')
test.assert_equals(decode_morse('.-... ---...   -..-. --...'), '&: /7')
test.assert_equals(decode_morse('...---...'), 'SOS')
test.assert_equals(decode_morse('... --- ...'), 'SOS')
test.assert_equals(decode_morse('...   ---   ...'), 'S O S')

test.assert_equals(decode_morse(' . '), 'E')
test.assert_equals(decode_morse('   .   . '), 'E E')

test.assert_equals(decode_morse('      ...---... -.-.--   - .... .   --.- ..- .. -.-. -.-   -... .-. --- .-- -.   ..-. --- -..-   .--- ..- -- .--. ...   --- ...- . .-.   - .... .   .-.. .- --.. -.--   -.. --- --. .-.-.-  '), 'SOS! THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.')

