from matplotlib import pyplot as plt
from matplotlib import style
import numpy as np

import sys
filename=str(sys.argv[1])
print filename

style.use('ggplot')

y=np.loadtxt(filename,unpack=True,delimiter=',')
x=range(1,len(y)+1)
#print x ,y
#print len(x)
#print len(y)

plt.plot(x,y)

plt.title(filename)
plt.ylabel('error')
plt.xlabel('batch')

plt.show()
