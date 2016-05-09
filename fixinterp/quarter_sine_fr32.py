# python script to generate 1/4-sine table
# values are 32-bit fract (1.31)

from sympy import *

f = open('quarter_sine_fr32_1024.inc', 'w')

n = 1024

x = symbols('x')
expr = sin(x / (n-1) * pi / 2)

def eval(i) :
    y = (expr.subs(x, i).evalf() * 0x7fffffff).round(0)
    s = '{}, '.format(hex( int( y ) ) )
    print(s)
    f.write(s + '\n')

for i in range(0, n) :
    eval(i)

f.close()
