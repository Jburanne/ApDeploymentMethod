import pandas as pd
import dxfgrabber
import os
# import matplotlib.pyplot as plt
import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot as plt

# get lines information for whole area
def saveLines(filepath):
    #read cad files
    dxf = dxfgrabber.readfile(filepath)

    lines = []
    layerkeywordlist = ['窗','WALL','wall','WINDOWS','玻璃','垂直面','门扇','阴影']

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
                lines.append([x1, y1, x2, y2])
            if e.dxftype == 'LWPOLYLINE':
                p = e.points
                for i in range(len(p) - 1):
                    if p[i][0] < 150000:
                        continue
                    lines.append([round(p[i][0]), round(p[i][1]), round(p[i + 1][0]), round(p[i + 1][1])])
                if e.is_closed:
                    if p[0][1] < 0 or p[0][0] < 150000:
                        continue
                    lines.append([round(p[0][0]), round(p[0][1]), round(p[-1][0]), round(p[-1][1])])
            break

    nums = len(lines)
    print(nums)
    df = pd.DataFrame(lines)
    print(df)
    save_path = os.getcwd().replace("\\",'/')+"/data/linesdata.csv"
    df.to_csv(save_path, index=0, sep=',', header=None)
    print('success')

# get lines information for whole area with regard to different types
def saveLinesByTypes(filepath,minX,minY,maxX,maxY,wall_kw1,glass_kw1,wood_kw1,other_kw1):
    #read cad files
    dxf = dxfgrabber.readfile(filepath)

    lines = []
    wall_kw, glass_kw, wood_kw, other_kw = [],[],[],[]
    if wall_kw1[0] != '':
        wall_kw = wall_kw1
    if glass_kw1[0] != '':
        glass_kw = glass_kw1
    if wood_kw1[0] != '':
        wood_kw = wood_kw1
    if other_kw1[0] != '':
        other_kw = other_kw1
    border = [minY, maxY, minX, maxX]
    excludekeywordlist = []
    print(wall_kw)
    print(glass_kw)
    print(wood_kw)
    print(other_kw)
    print(border)
    # wall_kw = wall_kw
    # glass_kw = glass_kw
    # wood_kw = wood_kw
    # other_kw = other_kw
    # border = [minY, maxY, minX, maxX]
    # excludekeywordlist = []
    # instance01
    # wall_kw = ['WALL','wall']
    # glass_kw = ['窗','WINDOWS','玻璃']
    # wood_kw = ['门扇']
    # other_kw = ['阴影','垂直面']
    # border = [-5000, 60000, 150000, 250000]
    # excludekeywordlist = []
    #instance02
    # wall_kw = ['墙']
    # glass_kw = ['WINDOW']
    # wood_kw = []
    # other_kw = []
    # border = [10000, 48000, 10000, 80000]
    # excludekeywordlist = ['1-TIF-原墙体-LT']
    #instance03
    # wall_kw = ['原有墙体','划分新墙','幕墙']
    # glass_kw = ['A-PLAN-WIND-窗']
    # wood_kw = ['管井门','A-PLAN-DOOR-门',]
    # other_kw = ['基层']
    # border = [-1520000, -1440000, 960000, 1020000]
    # excludekeywordlist = []
    #instance04
    # wall_kw = ['0']
    # glass_kw = []
    # wood_kw = ['图层4']
    # other_kw = []
    # border = [-75000, -50000, -40000, 5000]
    # excludekeywordlist = []
    #instance05
    # wall_kw = ['WALL-装修面','墙']
    # glass_kw = ['窗']
    # wood_kw = ['DOOR']
    # other_kw = []
    # border = [-100000,-50000,-350000,-300000]
    # excludekeywordlist = []
    #instance06
    # wall_kw = ['WALL', '隔墙']
    # glass_kw = ['门窗']
    # wood_kw = []
    # other_kw = ['黄','COLUMN']
    # border = [125000,155000,1790000,1860000]
    # excludekeywordlist = []
    #instance08
    # wall_kw = ['墙面完成面', '新建墙体']
    # glass_kw = []
    # wood_kw = ['门']
    # other_kw = ['结构']
    # border = [21000,60000,12000,40000]
    # excludekeywordlist = []
    #instance09
    # wall_kw = ['0', 'WALL']
    # glass_kw = ['GLASS','WINDOW']
    # wood_kw = ['DOOR']
    # other_kw = []
    # border = [165000,195000,380000,430000]
    # excludekeywordlist = []
    #instance10
    # wall_kw = ['STAIR', 'WALL']
    # glass_kw = ['WINDOW']
    # wood_kw = ['Door']
    # other_kw = []
    # border = [0,25000,150000,250000]
    # excludekeywordlist = []
    #instance11
    # wall_kw = ['0', '墙体']
    # glass_kw = []
    # wood_kw = ['平面门']
    # other_kw = []
    # border = [57500,74000,-13000,19000]
    #instance07
    # wall_kw = ['WALL']
    # glass_kw = ['WINDOW','WINdows']
    # wood_kw = []
    # other_kw = []
    # border = [0,25000,300000,340000]
    # excludekeywordlist = []
    #instance12
    # wall_kw = ['水泥墙']
    # glass_kw = ['玻璃', 'WINdows']
    # wood_kw = ['门-灯具']
    # other_kw = []
    # border = [-730000,-700000,2250000,2350000]
    # excludekeywordlist = []
    #instance13
    # wall_kw = ['Wall']
    # glass_kw = ['Window', 'WINDOW']
    # wood_kw = ['Door','固定家具']
    # other_kw = []
    # border = [-650000,-560000,2100000,2200000]
    # excludekeywordlist = []
    #instance14
    # wall_kw = ['WALL']
    # glass_kw = ['WIN']
    # wood_kw = ['FURNI']
    # other_kw = ['PARTI']
    # border =  [-250000,-220000,-60000,-20000]
    # excludekeywordlist = ['P-FURNI']
    #instance15
    # wall_kw = ['0-JZ','4-JZ','9-JZ','ID-LOUT-1','ID-WALL-F']
    # glass_kw = []
    # wood_kw = []
    # other_kw = []
    # border = [-35000,-15000,220000,240000]
    # excludekeywordlist = []
    all_kw = wall_kw+glass_kw+wood_kw+other_kw
    #layerkeywordlist = ['窗','WALL','wall','WINDOWS','玻璃','垂直面','门扇','阴影']
    for e in dxf.entities:
        if e.layer in excludekeywordlist:
            continue
        for kw in all_kw:
            if kw not in e.layer:
                continue
            #linestype = -1
            #reduce_dist = 0
            #print(linestype)
            if kw in wall_kw:
                linestype = 0
            elif kw in glass_kw:
                linestype = 1
            elif kw in wood_kw:
                linestype = 2
            else:
                linestype = 3
            if e.dxftype == 'LINE':
                # if e.start[0] < 150000:
                #     continue
                x1 = round(e.start[0])
                y1 = round(e.start[1])
                x2 = round(e.end[0])
                y2 = round(e.end[1])
                if x1 < border[2] or x1 > border[3] or x2 < border[2] or x2 > border[3]:
                    continue
                if y1 < border[0] or y1 > border[1] or y2 < border[0] or y2 > border[1]:
                    continue
                lines.append([x1, y1, x2, y2, linestype])
            elif e.dxftype == 'LWPOLYLINE':
                p = e.points
                for i in range(len(p) - 1):
                    # if p[i][0] < 150000:
                    #     continue
                    x1 = round(p[i][0])
                    x2 = round(p[i + 1][0])
                    y1 = round(p[i][1])
                    y2 = round(p[i + 1][1])
                    if x1 < border[2] or x1 > border[3] or x2 < border[2] or x2 > border[3]:
                        continue
                    if y1 < border[0] or y1 > border[1] or y2 < border[0] or y2 > border[1]:
                        continue
                    # if e.layer == 'Wall-墙线' and y1 > -580000 and y2 > -580000 and x1 != x2 and y1 != y2:
                    #     # print(x1, y1, x2, y2)
                    #     continue
                    lines.append([x1, y1, x2, y2, linestype])
                # if e.is_closed:
                #     if p[0][1] < 0 or p[0][0] < 150000:
                #         continue
                #     lines.append([round(p[0][0]), round(p[0][1]), round(p[-1][0]), round(p[-1][1]), linestype])
            break

    nums = len(lines)
    print(nums)
    df = pd.DataFrame(lines)
    print(df)
    save_path = os.getcwd().replace("\\",'/')+"/data/linesDataByMaterials.csv"
    df.to_csv(save_path, index=0, sep=',', header=None)
    print('success')

# get lines information for test area
def saveLines1(filepath):
    dxf = dxfgrabber.readfile(filepath)

    lines = []
    for e in dxf.entities:
        if (
                '窗' in e.layer or 'WALL' in e.layer or 'wall' in e.layer or 'WINDOWS' in e.layer or '玻璃' in e.layer or '垂直面' in e.layer or '阴影' in e.layer or '门扇' in e.layer):
            if e.dxftype == 'LINE':
                if e.start[0] < 150000 or e.start[0] > 199000 or e.end[0] > 199000:
                    continue
                if e.start[1] < 500:
                    continue
                x1 = round(e.start[0])
                y1 = round(e.start[1])
                x2 = round(e.end[0])
                y2 = round(e.end[1])
                lines.append([x1, y1, x2, y2])
            if e.dxftype == 'LWPOLYLINE':
                p = e.points
                for i in range(len(p) - 1):
                    if p[i][0] < 150000 or p[i][0] > 199000 or p[i + 1][0] > 199000:
                        continue
                    if p[i][1] < 500:
                        continue
                    lines.append([round(p[i][0]), round(p[i][1]), round(p[i + 1][0]), round(p[i + 1][1])])
                if e.is_closed:
                    if p[0][1] < 0 or p[0][0] < 150000 or p[0][0] > 199000 or p[-1][0] > 199000:
                        continue
                    if p[0][1] < 500:
                        continue
                    lines.append([round(p[0][0]), round(p[0][1]), round(p[-1][0]), round(p[-1][1])])
    nums = len(lines)
    print(nums)
    df = pd.DataFrame(lines)
    print(df)
    save_path = os.getcwd().replace("\\", '/') + "/data/linesdata.csv"
    df.to_csv(save_path, index=0, sep=',', header=None)
    print('success')

def readLines():
    file_path = os.getcwd().replace("\\",'/')+"/data/mergedLines.csv"
    df = pd.read_csv(file_path, sep=' ', header=None, names=['x1', 'x2', 'y1', 'y2']);
    # print(df)
    df = df.drop_duplicates()
    # print(df)
    x = []
    y = []
    for index, row in df.iterrows():
        x.append([int(row['x1']), int(row['x2'])])
        y.append([int(row['y1']), int(row['y2'])])

    for i in range(len(x)):
        plt.plot(x[i], y[i], color='b', linewidth=0.6)

    # save img
    img_path = os.getcwd().replace("\\",'/')+"/media/img/test.png"
    plt.savefig(img_path)
    return img_path

def readScpRes(spread_dist, reduce_dist,cover_num):
    file_path = os.getcwd().replace("\\", '/') + "/data/mergedLines.csv"
    print(file_path)
    df = pd.read_csv(file_path, sep=' ', header=None, names=['x1', 'x2', 'y1', 'y2'])
    # print(df)
    df = df.drop_duplicates()
    # print(df)
    x = []
    y = []
    for index, row in df.iterrows():
        x.append([int(row['x1']), int(row['x2'])])
        y.append([int(row['y1']), int(row['y2'])])

    for i in range(len(x)):
        plt.plot(x[i], y[i], color='b', linewidth=0.6)
    file_path = os.getcwd().replace("\\", '/') + "/data/resPoints"
    print(file_path)
    cnames = {
        'aliceblue': '#F0F8FF',
        'antiquewhite': '#FAEBD7',
        'aqua': '#00FFFF',
        'aquamarine': '#7FFFD4',
        'azure': '#F0FFFF',
        'beige': '#F5F5DC',
        'bisque': '#FFE4C4',
        'black': '#000000',
        'blanchedalmond': '#FFEBCD',
        'blue': '#0000FF',
        'blueviolet': '#8A2BE2',
        'brown': '#A52A2A',
        'burlywood': '#DEB887',
        'cadetblue': '#5F9EA0',
        'chartreuse': '#7FFF00',
        'chocolate': '#D2691E',
        'coral': '#FF7F50',
        'cornflowerblue': '#6495ED',
        'cornsilk': '#FFF8DC',
        'crimson': '#DC143C',
        'cyan': '#00FFFF',
        'darkblue': '#00008B',
        'darkcyan': '#008B8B',
        'darkgoldenrod': '#B8860B',
        'darkgray': '#A9A9A9',
        'darkgreen': '#006400',
        'darkkhaki': '#BDB76B',
        'darkmagenta': '#8B008B',
        'darkolivegreen': '#556B2F',
        'darkorange': '#FF8C00',
        'darkorchid': '#9932CC',
        'darkred': '#8B0000',
        'darksalmon': '#E9967A',
        'darkseagreen': '#8FBC8F',
        'darkslateblue': '#483D8B',
        'darkslategray': '#2F4F4F',
        'darkturquoise': '#00CED1',
        'darkviolet': '#9400D3',
        'deeppink': '#FF1493',
        'deepskyblue': '#00BFFF',
        'dimgray': '#696969',
        'dodgerblue': '#1E90FF',
        'firebrick': '#B22222',
        'floralwhite': '#FFFAF0',
        'forestgreen': '#228B22',
        'fuchsia': '#FF00FF',
        'gainsboro': '#DCDCDC',
        'ghostwhite': '#F8F8FF',
        'gold': '#FFD700',
        'goldenrod': '#DAA520',
        'gray': '#808080',
        'green': '#008000',
        'greenyellow': '#ADFF2F',
        'honeydew': '#F0FFF0',
        'hotpink': '#FF69B4',
        'indianred': '#CD5C5C',
        'indigo': '#4B0082',
        'ivory': '#FFFFF0',
        'khaki': '#F0E68C',
        'lavender': '#E6E6FA',
        'lavenderblush': '#FFF0F5',
        'lawngreen': '#7CFC00',
        'lemonchiffon': '#FFFACD',
        'lightblue': '#ADD8E6',
        'lightcoral': '#F08080',
        'lightcyan': '#E0FFFF',
        'lightgoldenrodyellow': '#FAFAD2',
        'lightgreen': '#90EE90',
        'lightgray': '#D3D3D3',
        'lightpink': '#FFB6C1',
        'lightsalmon': '#FFA07A',
        'lightseagreen': '#20B2AA',
        'lightskyblue': '#87CEFA',
        'lightslategray': '#778899',
        'lightsteelblue': '#B0C4DE',
        'lightyellow': '#FFFFE0',
        'lime': '#00FF00',
        'limegreen': '#32CD32',
        'linen': '#FAF0E6',
        'magenta': '#FF00FF',
        'maroon': '#800000',
        'mediumaquamarine': '#66CDAA',
        'mediumblue': '#0000CD',
        'mediumorchid': '#BA55D3',
        'mediumpurple': '#9370DB',
        'mediumseagreen': '#3CB371',
        'mediumslateblue': '#7B68EE',
        'mediumspringgreen': '#00FA9A',
        'mediumturquoise': '#48D1CC',
        'mediumvioletred': '#C71585',
        'midnightblue': '#191970',
        'mintcream': '#F5FFFA',
        'mistyrose': '#FFE4E1',
        'moccasin': '#FFE4B5',
        'navajowhite': '#FFDEAD',
        'navy': '#000080',
        'oldlace': '#FDF5E6',
        'olive': '#808000',
        'olivedrab': '#6B8E23',
        'orange': '#FFA500',
        'orangered': '#FF4500',
        'orchid': '#DA70D6',
        'palegoldenrod': '#EEE8AA',
        'palegreen': '#98FB98',
        'paleturquoise': '#AFEEEE',
        'palevioletred': '#DB7093',
        'papayawhip': '#FFEFD5',
        'peachpuff': '#FFDAB9',
        'peru': '#CD853F',
        'pink': '#FFC0CB',
        'plum': '#DDA0DD',
        'powderblue': '#B0E0E6',
        'purple': '#800080',
        'red': '#FF0000',
        'rosybrown': '#BC8F8F',
        'royalblue': '#4169E1',
        'saddlebrown': '#8B4513',
        'salmon': '#FA8072',
        'sandybrown': '#FAA460',
        'seagreen': '#2E8B57',
        'seashell': '#FFF5EE',
        'sienna': '#A0522D',
        'silver': '#C0C0C0',
        'skyblue': '#87CEEB',
        'slateblue': '#6A5ACD',
        'slategray': '#708090',
        'snow': '#FFFAFA',
        'springgreen': '#00FF7F',
        'steelblue': '#4682B4',
        'tan': '#D2B48C',
        'teal': '#008080',
        'thistle': '#D8BFD8',
        'tomato': '#FF6347',
        'turquoise': '#40E0D0',
        'violet': '#EE82EE',
        'wheat': '#F5DEB3',
        'white': '#FFFFFF',
        'whitesmoke': '#F5F5F5',
        'yellow': '#FFFF00',
        'yellowgreen': '#9ACD32'
    }

    colorlist = list(cnames.values())
    with open(file_path, 'r') as f:
        lines = f.readlines()
        cover_rate = round(float(lines[0]), 2)
        flag = 0
        colorcnt = 0
        print(len(lines))
        cnt = 0
        for i in range(1,len(lines)):
            pos = lines[i].split(' ')
            if len(pos) == 1:
                flag = 1
                colorcnt += 1
                # if colorcnt == 2:
                # break
                continue
            if flag == 1:
                plt.scatter(int(pos[0]), int(pos[1]), color='r', s=6)
                cnt += 1
                flag = 0
            else:
                plt.scatter(int(pos[0]), int(pos[1]), color=colorlist[colorcnt], s=1)
    # plt.show()
    for i in range(len(x)):
        plt.plot(x[i], y[i], color='b', linewidth=0.6)
        # plt.scatter(x[i],y[i],color='b')
    print(cnt)
    # plt.xlim(184934, 246714)

    pic_name = str(round(spread_dist/1000)) + "m_"+ str(round(reduce_dist/1000)) + "m_cover"+str(cover_num) + "_red" + str(cnt)
    plt.title(pic_name)
    plt.axis('scaled')
    # save_path = os.getcwd().replace("\\", '/')+"/media/img/result/"+pic_name+".png"
    save_path = os.getcwd().replace("\\", '/') + "/media/temp_img/positionImg.png"
    plt.savefig(save_path)
    plt.close()
    return save_path,pic_name,cnt,cover_rate

def mergeLines(cpp_path):
    os.system(cpp_path)

def getInput(spread_dist, wall_reduce_dist,glass_reduce_dist, wood_reduce_dist, other_reduce_dist, cover_num, dist_thre):
    #to_exec = os.getcwd().replace("\\",'/')+"/cppFiles/processData/processData.exe 0 "+str(spread_dist)+" "\
              #+str(reduce_dist) + " " + str(cover_num) + " " + str(dist_thre)
    to_exec = os.getcwd().replace("\\", '/') + "/cppFiles/processData/processData.exe 0 " + str(spread_dist) + " " \
               + " " + str(cover_num) + " " + str(dist_thre) + " " + str(wall_reduce_dist) \
              + " " + str(glass_reduce_dist) + " " + str(wood_reduce_dist) + " " + str(other_reduce_dist)
    os.system(to_exec)

def runScp(time_limit):
    file_path = os.getcwd().replace("\\",'/')+'/data/newtestfile'
    code_path = os.getcwd().replace("\\",'/')+'/cppFiles/SCP1/pms.exe'
    to_exec = code_path + " " + file_path + " " + str(time_limit)
    os.system(to_exec)

def getResPoints(spread_dist, wall_reduce_dist,glass_reduce_dist, wood_reduce_dist, other_reduce_dist, cover_num, dist_thre):
    to_exec = os.getcwd().replace("\\", '/') + "/cppFiles/processData/processData.exe 1 "+ str(spread_dist) + " " \
               + " " + str(cover_num) + " " + str(dist_thre) + " " + str(wall_reduce_dist) \
              + " " + str(glass_reduce_dist) + " " + str(wood_reduce_dist) + " " + str(other_reduce_dist)
    os.system(to_exec)


def readTest(request):
    print(request.split(","))

