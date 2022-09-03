import sympy
from sympy import symbols, Matrix, MatrixSymbol

N = 1

r, g, b, a = symbols("r g b a")
x_vec = Matrix( [[r, g, b, a] for _ in range(N)] ).T
print(x_vec)

D = MatrixSymbol('D', 4, N)

# 4 x N
pixelDiff = x_vec - D
print(pixelDiff)

pixelDiff2 = pixelDiff.T * pixelDiff
print(pixelDiff2)

total = sum(sympy.sqrt(pixelDiff2[i]) for i in range(N))
#total = sympy.sqrt(pixelDiff2[0]) + sympy.sqrt(pixelDiff2[1])
#print(sympy.sqrt(pixelDiff2[0]))
#print(sympy.sqrt(pixelDiff2[1]))

#print(sympy.solve(sympy.diff(total, r), r))
print(sympy.diff(total, r))
#print(sympy.diff(total, g))
#print(sympy.diff(total, b))
#print(sympy.diff(total, a))
