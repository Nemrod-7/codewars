class show :
    def atom(atom) :
        print(atom)
    def molecule(molecule) :
        for chain in molecule :
            for i in range(1, len(chain)) :
                show.atom(chain[i])
            print()
    def stack(stack) :
        for molecule in stack :
            show.molecule(molecule)
            print('----------------------------')
    def represent(atom) :
        table = { 'C':[4, 12.0], 'H':[1, 1.0 ], 'O':[2, 16.0], 'B':[3, 10.8], 'Br':[1, 80.0], 'Cl':[1, 35.5], 'F':[1, 19.0], 'Mg':[2, 24.3], 'N':[3, 14.0], 'P':[3, 31.0], 'S':[2, 32.1], 'I':[1, 0.0], 'As':[3, 0.0] }
        id, elt, edge = atom
        cell = elt
        rest = table[elt][0] - len(edge)

        match rest :
           case 0 : pass
           case 1 : cell += 'H'
           case _ : cell += 'H' + str(rest)
        return cell
    def smile(molecule) :
        formula, links = '', ''

        for chain in molecule :
            for i in range(1, len(chain)) :
                id, element, edge = chain[i]
                nb = [0] if i == 0 else [ 1 for j in range(len(edge)) if edge[j] == chain[i-1][0] ].count(1)

                link = ''
                match nb :
                    case 0 : link += ' '
                    case 1 : link += '-'
                    case 2 : link += '='
                    case 3 : link += '{=}'

                if edge != [] and edge[-1] != chain[i-1][0] and i < len(chain) - 1 and edge[-1] != chain[i+1][0] :
                    hist = { edge.count(x):x for x in set(edge) }
                    nn = '||' if 2 in hist else '|'
                    links +=  len(link) * ' ' + nn + (len(show.represent(chain[i])) - len(nn)) * ' '
                else :
                    links += len(link + show.represent(chain[i])) * ' '
                formula += link + show.represent(chain[i])
            formula += ' '
        return formula + '\n' + links
    def formula(molecule) :
        formula = ''
        hist = {}

        for chain in molecule :
            for i in range(1, len(chain)) :
                elt = chain[i][1]
                hist[elt] = 1 if not elt in hist else hist[elt] + 1
                hist['H'] = 1 if not 'H' in hist else hist['H'] + table[elt][0] - len(chain[i][2])

        arr = [(key,hist[key]) for key in hist]
        arr.sort(key = lambda x: order.index(x[0]))
        formula = ''.join([ it[0] + (str(it[1]) if it[1] > 1 else '') for it in arr])
        return formula
