from dxfwrite import DXFEngine as dxf
drawing = dxf.drawing('rectangle.dxf')
drawing.add_layer('WALL')
drawing.add(dxf.line(start=(184934,-2814,0), end=(184934,54266,0), layer='WALL'))
drawing.add(dxf.line(start=(246714,-2814,0), end=(246714,54266,0), layer='WALL'))
drawing.add(dxf.line(start=(184934,-2814,0), end=(246714,-2814,0), layer='WALL'))
drawing.add(dxf.line(start=(184934,54266,0), end=(246714,54266,0), layer='WALL'))
drawing.save()