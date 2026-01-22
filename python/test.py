
benzene = [ [0,'C',[]],
[1,'C',[2,6,6]], [2,'C',[1,3,3]], [3,'C',[2,2,4]],
[4,'C',[3,5,5]], [5,'C',[4,4,6]], [6,'C',[5,1,1]] ]

phenyl = [ [0,'C',[]],
[1,'C',[2,6,6]], [2,'C',[1,3,3]], [3,'C',[2,2,4]],
[4,'C',[3,5,5]], [5,'C',[4,4,6]], [6,'C',[5,1,1]] ]

'1,2 di [ 1 ethyl 3 [2 methyl ] propyl ] heptyl cyclobutane'

    #        CH3        CH3
    #        |          |
    #        CH2    CH2-CH-CH3
    #        |      |
    # CH2-CH-CH-CH2-CH-CH2-CH2-CH2-CH3
    # |   |                                  => 1,2-di[1-ethyl-3-[2-methyl]propyl]heptylcyclobutane
    # CH2-CH-CH-CH2-CH-CH2-CH2-CH2-CH3
    #        |      |
    #        CH2    CH2-CH-CH3
    #        |          |
    #        CH3        CH3


# 1) cyclobutane
# CH2-CH2
# |   |
# CH2-CH2
#
# 2) heptyl
# CH3-CH2-CH2-CH2-CH2-CH2-CH3
#
# 3) propyl [subroutine]
# CH3-CH2-CH3
#
# 4) methyl [subroutine]
# CH4
#
# 5) position-2
# => CH3-CH-CH3
#        |
#       CH3
