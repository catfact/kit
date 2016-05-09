# python script to generate half-cosine interpolation segment
# values are 32-bit fract (1.31)

from sympy import *

f = open('half_cos_fr32_1024.inc', 'w')

n = 1024

x = symbols('x')
expr = (cos(x / (n-1) * pi + pi) + 1) / 2

def eval(i) :
    y = (expr.subs(x, i).evalf() * 0x7fffffff).round(0)
    s = '{}, '.format(hex( int( y ) ) )
    print(s)
    f.write(s + '\n')

for i in range(0, n) :
    eval(i)

# cheat with an extra entry instead of wrapping
eval(n-1)

f.close()
