#!/usr/bin/env python
from sys import argv
import numpy as np
from matplotlib import pyplot as plt

script, points = argv
N = int(points)
count = 0
a = -1.
b =  1.

np.random.seed(4270)
x = a + (b - a) * np.random.rand(N)
y = a + (b - a) * np.random.rand(N)

r = np.sqrt(x**2 + y**2)

for vector in r:
    if vector <= 1.:
        count += 1

pi = 4. * count/float(N)

print ("%.5g") % pi 

plt.style.use('dark_background')
plt.figure()
plt.title(r'Direct $\pi$')

plt.scatter(x,y,1.5,color = 'blue')
circle1 = plt.Circle((0,0),1.0,color='white',fill=False)

axis = plt.gca()
axis.set_xlim([-1.1,1.1])
axis.set_ylim([-1.1,1.1])
plt.xlabel('X Coord')
plt.ylabel('Y Coord')

fig = plt.gcf()
fig.gca().add_artist(circle1)
fig.savefig('direct_pi.jpg')
plt.show()
