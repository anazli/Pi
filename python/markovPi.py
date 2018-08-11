#!/usr/bin/env python
import numpy as np
from matplotlib import pyplot as plt
from sys import argv

script,points = argv

N = int(points)
count = 0
delta = 0.8
a = -delta 
b =  delta 

x = 0 
y = 0 

sx = []
sy = []

np.random.seed(4270)

for i in range(N):
    dx = a + (b - a) * np.random.rand()
    dy = a + (b - a) * np.random.rand()

    if abs(x + dx) < 1 and abs(y + dy) < 1:
        x = x + dx
        y = y + dy

    sx.append(x)
    sy.append(y)
    if np.sqrt(x**2 + y**2) <= 1:
        count += 1

pi = 4. * count/float(N)

print "%.5g" % pi 

plt.style.use('dark_background')
plt.figure()
plt.title(r'Markov chain $\pi$')

plt.scatter(sx,sy,1.5,color='blue')
circle1 = plt.Circle((0.,0.),1.,color='white',fill=False)

axis = plt.gca()
axis.set_xlim([-1.1,1.1])
axis.set_ylim([-1.1,1.1])
plt.xlabel('x coord')
plt.ylabel('y coord')

fig = plt.gcf()
fig.gca().add_artist(circle1)
plt.show()

    
        


