import re

expression = "-13.52 - 85 * -5 - (5 * 6)"

code  = re.findall(r"[-+]?[0-9]+\.?[0-9]*|[-+/*()]", expression)

print()
print()
print()
print(code)

var: code
