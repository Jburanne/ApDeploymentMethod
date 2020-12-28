from django.shortcuts import render
from .models import User
from .forms import AddForm
from . import readCad
import os
from django.shortcuts import HttpResponse,render,redirect


# Create your views here.
def add(request):
    # 判断是否为post方法提交
    if request.method == "POST":
        af = AddForm(request.POST, request.FILES)
        # 判断表单值是否合法
        if af.is_valid():
            filename = af.cleaned_data['filename']
            headimg = af.cleaned_data['headimg']
            cover_num = af.cleaned_data['cover_num']
            spread_dist = af.cleaned_data['propagation_distance']
            reduce_dist = af.cleaned_data['wall_reduction_distance']
            wall_reduce_dist = af.cleaned_data['wall_reduction_distance']
            glass_reduce_dist = af.cleaned_data['glass_reduction_distance']
            wood_reduce_dist = af.cleaned_data['wood_reduction_distance']
            other_reduce_dist = af.cleaned_data['other_reduction_distance']

            time_limit = af.cleaned_data['time_limit']
            dist_thre = af.cleaned_data['dist_thre']

            #写入数据表User中
            user = User(name=filename,headimg=headimg)
            user.save()
            # 读取cad文件
            cad_file_path = os.getcwd().replace("\\","/")+"/media/img/"+str(headimg)
            readCad.saveLines(cad_file_path)
            #readCad.saveLinesByTypes(cad_file_path)
            # c++处理线条&获取算法的输入文件
            readCad.getInput(spread_dist, wall_reduce_dist,glass_reduce_dist, wood_reduce_dist, other_reduce_dist, cover_num, dist_thre)
            # 运行scp代码，输出解
            readCad.runScp(time_limit)
            # 读取解
            readCad.getResPoints(spread_dist, wall_reduce_dist,glass_reduce_dist, wood_reduce_dist, other_reduce_dist, cover_num, dist_thre)
            # 解的可视化
            img_path = readCad.readScpRes(spread_dist, reduce_dist, cover_num)
            #img_path = readCad.readLines()
            #展示图片
            img_data = open(img_path,'rb').read()
            return HttpResponse(img_data, content_type="image/png")
            #print(os.getcwd())
            #return render(request, 'users/index.html', context={"user": user})
            #return HttpResponse("上传成功")
            #return HttpResponse(cover_num)
    else:
        #readCad.testF()
        print(os.getcwd())
        af = AddForm()
        return render(request, 'users/add.html', context={'af':af})


