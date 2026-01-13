import re

name = '3,7-dicyclobutyloctane'

name = name.replace('cyclo', ' cyclo ')
name = re.findall(r"\S+", name)

print(name)
