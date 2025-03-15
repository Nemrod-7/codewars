
# a = { 'op': 'imm', 'n': 5 } 
# b = { 'op': 'imm', 'n': 5 } 

t2 = {'op':'/',
      'a':{'op':'-',
            'a':{'op':'+',
                 'a':{'op':'*',
                      'a':{'op':'imm','n':6},
                      'b':{'op':'arg','n':0}},
                 'b':{'op':'*',
                      'a':{'op':'imm','n':5},
                      'b':{'op':'arg','n':1}}},
           'b':{'op':'*',
                'a':{'op':'imm','n':3},
                'b':{'op':'arg','n':2}}},

      'b':{'op':'imm','n':8}};

def to_postfix(ast) :
    s1,s2 = [],[]
    s1.append(ast)

    while s1 :
        node = s1.pop()
        s2.append(node)

        if node['op'] != 'imm' and node['op'] != 'arg' :
            if node['a'] : s1.append(node['a'])
            if node['b'] : s1.append(node['b'])

    return s2

ast = to_postfix(t2)

for node in ast :
    if node['op'] != 'imm' and node['op'] != 'arg' :
        print('[', node['op'],']')
    else :
        print('[',node['op'],']:', node['n'])
