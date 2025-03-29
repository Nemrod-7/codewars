
hist = [('J', 6),('C',3), ('Mg',1),('Br', 1),('H',7)]

hist.sort(key = lambda x: (len(x[0]), x[0]))

print(hist)
