import pandas as pd
import dxfgrabber
from matplotlib import pyplot as plt

dxf = dxfgrabber.readfile("instance15.dxf")
x = []
y = []
#layerkeywordlist = ['WALL','wall','WINDOW','0','2D']
#layerkeywordlist = ['墙','WINDOW']
#layerkeywordlist = ['管井门','A-PLAN-DOOR-门','A-PLAN-WIND-窗','原有墙体','划分新墙','基层','幕墙']
#border = [-320000, -295000, 195000, 240000]
#border = [-5000, 60000, 150000, 250000]
#border = [-1520000, -1440000, 960000, 1020000]
#instance01
# layerkeywordlist = ['WALL','wall','窗','WINDOWS','玻璃','门扇','阴影','垂直面']
# excludekeywordlist = []
# border = [-5000, 60000, 150000, 250000]
#instance04
# layerkeywordlist = ['0','图层4']
# excludekeywordlist = []
# border = [-75000,-50000,-40000,5000]
#instance05
# layerkeywordlist = ['DOOR','WALL-装修面','墙','窗']
# excludekeywordlist = []
# border = [-100000,-50000,-350000,-300000]
#instance06
# layerkeywordlist = ['WALL','COLUMN','黄','隔墙','门窗']
# excludekeywordlist = []
# border = [125000,155000,1790000,1860000]
#instance07
# layerkeywordlist = ['WALL','WINDOW','WINdows']
# excludekeywordlist = []
# border = [0,25000,300000,340000]
#instance08
# layerkeywordlist = ['墙面完成面','新建墙体','结构','门']
# excludekeywordlist = []
# border = [21000,60000,12000,40000]
#instance09
# layerkeywordlist = ['0','DOOR','GLASS','WALL','WINDOW']
# excludekeywordlist = []
# border = [165000,195000,380000,430000]
#instance10
# layerkeywordlist = ['Door','STAIR','WALL','WINDOW']
# excludekeywordlist = []
# border = [0,25000,150000,250000]
#instance11
# layerkeywordlist = ['0','墙体','平面门']
# excludekeywordlist = []
# border = [57500,74000,-13000,19000]
#instance12
# layerkeywordlist = ['水泥墙','玻璃','门-灯具']
# excludekeywordlist = []
# border = [-730000,-700000,2250000,2350000]
#instance13
# layerkeywordlist = ['Door','固定家具','WINDOW','Wall','Window']
# excludekeywordlist = []
# border = [-650000,-560000,2100000,2200000]
#instance14
# layerkeywordlist = ['WALL','WIN','FURNI','PARTI']
# excludekeywordlist = ['P-FURNI']
# border = [-250000,-220000,-60000,-20000]
#instance15
layerkeywordlist = ['0-JZ','4-JZ','9-JZ','ID-LOUT-1','ID-WALL-F']
excludekeywordlist = []
border = [-35000,-15000,220000,240000]
print(dxf)
for e in dxf.entities:
    if e.layer in excludekeywordlist:
        continue
    for kw in layerkeywordlist:
        if kw not in e.layer:
            continue
        if e.dxftype == 'LINE':
            x1 = round(e.start[0])
            y1 = round(e.start[1])
            x2 = round(e.end[0])
            y2 = round(e.end[1])
            if x1<border[2] or x1>border[3] or x2<border[2] or x2>border[3]:
                continue
            if y1<border[0] or y1>border[1] or y2<border[0] or y2>border[1]:
                continue
            x.append([x1, x2])
            y.append([y1, y2])
        if e.dxftype == 'LWPOLYLINE':
            p = e.points
            for i in range(len(p) - 1):
                x1 = round(p[i][0])
                x2 = round(p[i + 1][0])
                y1 = round(p[i][1])
                y2 = round(p[i + 1][1])
                if x1 < border[2] or x1 > border[3] or x2 < border[2] or x2 > border[3]:
                    continue
                if y1 < border[0] or y1 > border[1] or y2 < border[0] or y2 > border[1]:
                    continue
                # if e.layer == 'Wall-墙线' and y1 > -580000 and y2 > -580000 and x1 != x2 and y1 != y2:
                #     print(x1, y1, x2, y2)
                #     continue
                x.append([x1,x2])
                y.append([y1,y2])
            # if e.is_closed:
            #     x1 = round(p[0][0])
            #     x2 = round(p[-1][0])
            #     y1 = round(p[-1][0])
            #     y2 = round(p[-1][1])
            #     if x1 < border[2] or x1 > border[3] or x2 < border[2] or x2 > border[3]:
            #         continue
            #     if y1 < border[0] or y1 > border[1] or y2 < border[0] or y2 > border[1]:
            #         continue
            #     x.append([x1, x2])
            #     y.append([y1, y2])

for i in range(len(x)):
    plt.plot(x[i], y[i], color='b', linewidth=0.6)
plt.axis('scaled')
plt.savefig("test.png")
plt.show()
