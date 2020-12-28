import pandas as pd
import dxfgrabber
import os
import matplotlib.pyplot as plt

dxf = dxfgrabber.readfile('../media/img/test_graph_1.dxf')

x = []
y = []
#layerkeywordlist = ['WALL','wall','垂直面','阴影']
#layerkeywordlist = ['窗','WINDOWS','玻璃','门扇']
layerkeywordlist = ['门扇']
for e in dxf.entities:
    for kw in layerkeywordlist:
        if kw not in e.layer:
            continue
        if e.dxftype == 'LINE':
            if e.start[0] < 150000:
                continue
            x1 = round(e.start[0])
            y1 = round(e.start[1])
            x2 = round(e.end[0])
            y2 = round(e.end[1])
            x.append([x1,x2])
            y.append([y1,y2])
        if e.dxftype == 'LWPOLYLINE':
            p = e.points
            for i in range(len(p) - 1):
                if p[i][0] < 150000:
                    continue
                x.append([round(p[i][0]),round(p[i + 1][0])])
                y.append([round(p[i][1]),round(p[i + 1][1])])
            if e.is_closed:
                if p[0][1] < 0 or p[0][0] < 150000:
                    continue
                x.append([round(p[i][0]), round(p[-1][0])])
                y.append([round(p[i][1]), round(p[-1][1])])
        break

for i in range(len(x)):
    plt.plot(x[i], y[i], color='b', linewidth=0.6)
plt.show()
