from django.shortcuts import render
from .models import User, Cad, Result, DepParams, Deployment
from .forms import AddForm, LoginForm
from . import readCad
import os
from django.shortcuts import HttpResponse, render, redirect
import pymysql
import json


def connectToMySQL():
    conn = pymysql.connect(host='127.0.0.1', port=3306, user='root', passwd='hwy960608',
                           db='apdeployment', charset='utf8')
    cursor = conn.cursor(cursor=pymysql.cursors.DictCursor)
    return conn, cursor


def closeSQLConnection(cursor, conn):
    cursor.close()
    conn.close()


# Create your views here.
def uploadCad(request):
    # 判断是否为post方法提交
    if not request.session.has_key('is_login'):
        return render(request, 'users/login.html')
    elif request.method == "POST":
        print(request.POST)
        print(request.FILES)
        af = AddForm(request.POST, request.FILES)
        # 判断表单值是否合法
        if af.is_valid():
            filename = af.cleaned_data['filename']
            headimg = af.cleaned_data['headimg']
            cover_num = af.cleaned_data['cover_num']
            spread_dist = af.cleaned_data['propagation_distance'] * 1000
            reduce_dist = af.cleaned_data['wall_reduction_distance'] * 1000
            wall_reduce_dist = af.cleaned_data['wall_reduction_distance'] * 1000
            glass_reduce_dist = af.cleaned_data['glass_reduction_distance'] * 1000
            wood_reduce_dist = af.cleaned_data['wood_reduction_distance'] * 1000
            other_reduce_dist = af.cleaned_data['other_reduction_distance'] * 1000

            time_limit = af.cleaned_data['time_limit']
            dist_thre = af.cleaned_data['dist_thre'] * 1000

            # print(filename, cover_num, spread_dist, glass_reduce_dist)
            # cad文件路径
            cad_file_path = os.getcwd().replace("\\", "/") + "/media/img/cad/" + str(headimg)
            if os.path.exists(cad_file_path)==False:
                # 写入数据表Test中
                user = Cad(cad_description=filename, cad_path=headimg)
                user.save()
            # 读取cad文件
            readCad.saveLinesByTypes(cad_file_path)
            # c++处理线条&获取算法的输入文件
            readCad.getInput(spread_dist, wall_reduce_dist, glass_reduce_dist, wood_reduce_dist, other_reduce_dist,
                             cover_num, dist_thre)
            # 运行scp代码，输出解
            readCad.runScp(time_limit)
            # 读取解
            readCad.getResPoints(spread_dist, wall_reduce_dist, glass_reduce_dist, wood_reduce_dist, other_reduce_dist,
                                 cover_num, dist_thre)
            # 解的可视化
            img_path, pic_name, ap_num, cover_rate = readCad.readScpRes(spread_dist, reduce_dist, cover_num)
            # img_path = readCad.readLines()
            # 展示图片
            # img_data = open(img_path,'rb').read()
            # return HttpResponse(img_data, content_type="image/png")
            img_relative_path = '/media/img/result/' + pic_name + '.png'
            cad_relative_path = 'img/cad/'+str(headimg)
            dc = {"resimgpath": img_relative_path, "cover_num": cover_num,
                                   "spread_dist": spread_dist / 1000, "time_limit": time_limit,
                                   "dist_thre": dist_thre / 1000, "ap_num": ap_num,"cover_rate":cover_rate,
                                   "filename": filename,"wall_reduce_dist":wall_reduce_dist / 1000,
                  "glass_reduce_dist":glass_reduce_dist / 1000,"wood_reduce_dist":wood_reduce_dist / 1000,
                  "other_reduce_dist":other_reduce_dist / 1000,"cadpath":cad_relative_path,"pospath":""}
            return render(request, 'users/deploymentResult.html',
                          context={"dc":dc})
            # print(os.getcwd())
            # return render(request, 'users/index.html', context={"user": user})
            # return HttpResponse("上传成功")
            # return HttpResponse(cover_num)
    else:
        print(os.getcwd())
        af = AddForm()
        return render(request, 'users/uploadCad.html', context={'af': af})


def deploymentResult(request):
    contextDict={"resimgpath": "testpath", "cover_num": 3,
                                   "spread_dist": 30, "time_limit": 30,
                                   "dist_thre": 5, "ap_num": 39,
                                   "filename": "testgraph",
                 "wall_reduce_dist":10,"glass_reduce_dist":8,"wood_reduce_dist":5,
                 "other_reduce_dist":10,"cadpath":"cadpath","pospath":""}
    return render(request, 'users/deploymentResult.html',context={'dc':contextDict})

def saveResult(request):
    #conn, cursor = connectToMySQL()
    '''
    insert_sql = "insert into users(username,password) values('210111','hhhhhh')"
    cursor.execute(insert_sql)
    conn.commit()
    '''
    print(request.POST)
    cover_num = request.POST.get("dc[cover_num]")
    spread_dist = request.POST.get("dc[spread_dist]")
    time_limit = request.POST.get("dc[time_limit]")
    dist_thre = request.POST.get("dc[dist_thre]")
    resImgPath = request.POST.get("dc[resimgpath]")
    cad_description = request.POST.get("dc[filename]")
    wall_reduce_dist = request.POST.get("dc[wall_reduce_dist]")
    glass_reduce_dist = request.POST.get("dc[glass_reduce_dist]")
    wood_reduce_dist = request.POST.get("dc[wood_reduce_dist]")
    other_reduce_dist = request.POST.get("dc[other_reduce_dist]")
    ap_num = request.POST.get("dc[ap_num]")
    cadpath = request.POST.get("dc[cadpath]")
    pospath = request.POST.get("dc[pospath]")
    print(cover_num,spread_dist,time_limit,dist_thre,resImgPath,cad_description,wall_reduce_dist,glass_reduce_dist,
          wood_reduce_dist,other_reduce_dist,ap_num,cadpath,pospath)
    # 获得cad id
    # cad_select_sql = "select id from cad where filepath=%s"
    # cursor.execute(cad_select_sql,[cadpath,])
    # res = cursor.fetchall()
    # if(len(res) == 0):
    #     cad_insert_sql = "insert into cad(filename,filepath) values(%s,%s)"
    #     cursor.execute(cad_insert_sql,[filename,cadpath,])
    #     conn.commit()
    #     cursor.execute(cad_select_sql,[cadpath,])
    #     res = cursor.fetchall()
    # cad_id = res[0]['id']
    # print(cad_id)
    res = Cad.objects.filter(cad_path=cadpath)
    print(len(res))
    print(res)
    if len(res) == 0:
        Cad.objects.create(cad_description=cad_description,cad_path=cadpath)
        res = Cad.objects.filter(cad_path=cadpath)
    print("cad id:",res[0].id)
    cad_i = res[0]
    # 获得参数id
    # param_select_sql = "select id from params where cover_num=%s and dist_thre=%s and spread_dist=%s and " \
    #                    "wall_reduce_dist=%s and glass_reduce_dist=%s and wood_reduce_dist=%s and other_reduce_dist=%s and " \
    #                    "time_limit=%s"
    # cursor.execute(param_select_sql,[cover_num,dist_thre,spread_dist,wall_reduce_dist,glass_reduce_dist,
    #                                  wood_reduce_dist,other_reduce_dist,time_limit,])
    # res = cursor.fetchall()
    # if(len(res) == 0):
    #     param_insert_sql = "insert into params(cover_num,dist_thre,spread_dist, wall_reduce_dist, glass_reduce_dist," \
    #                        "wood_reduce_dist, other_reduce_dist, time_limit) values(%s,%s,%s,%s,%s,%s,%s,%s)"
    #     cursor.execute(param_insert_sql, [cover_num, dist_thre, spread_dist, wall_reduce_dist, glass_reduce_dist,
    #                                       wood_reduce_dist, other_reduce_dist, time_limit,])
    #     conn.commit()
    #     cursor.execute(param_select_sql, [cover_num, dist_thre, spread_dist, wall_reduce_dist, glass_reduce_dist,
    #                                       wood_reduce_dist, other_reduce_dist, time_limit,])
    #     res = cursor.fetchall()
    # param_id = res[0]['id']
    # print(param_id)
    res = DepParams.objects.filter(cover_num=cover_num,dist_thre=dist_thre,spread_dist=spread_dist,
                                   wall_reduce_dist=wall_reduce_dist,glass_reduce_dist=glass_reduce_dist,
                                   wood_reduce_dist=wood_reduce_dist,other_reduce_dist=other_reduce_dist,
                                   time_limit=time_limit)
    if len(res) == 0:
        DepParams.objects.create(cover_num=cover_num,dist_thre=dist_thre,spread_dist=spread_dist,
                                 wall_reduce_dist=wall_reduce_dist,glass_reduce_dist=glass_reduce_dist,
                                 wood_reduce_dist=wood_reduce_dist,other_reduce_dist=other_reduce_dist,
                                 time_limit=time_limit)
        res = DepParams.objects.filter(cover_num=cover_num, dist_thre=dist_thre, spread_dist=spread_dist,
                                       wall_reduce_dist=wall_reduce_dist, glass_reduce_dist=glass_reduce_dist,
                                       wood_reduce_dist=wood_reduce_dist, other_reduce_dist=other_reduce_dist,
                                       time_limit=time_limit)
    print("param id:",res[0].id)
    param_i = res[0]
    # # 获得结果id
    # res_insert_sql = "insert into result(filepath,ap_num,pospath) values(%s,%s,%s)"
    # cursor.execute(res_insert_sql,[resImgPath,ap_num,pospath,])
    # conn.commit()
    # res_select_sql = "select max(id) as id from result"
    # cursor.execute(res_select_sql)
    # res = cursor.fetchall()
    # res_id = res[0]['id']
    # print(res_id)
    # # user id 暂时固定为1
    userid = request.session['userid']
    user_i = User.objects.filter(id=userid)[0]

    # 获取结果id
    maxresid = Result.objects.values('id').order_by('-id').first()
    Result.objects.create(res_path=resImgPath,pos_path=pospath,ap_num=ap_num)
    res = Result.objects.filter(res_path=resImgPath,pos_path=pospath,ap_num=ap_num)
    print("max res id:",maxresid, "res id:",res[len(res)-1].id)
    # print(maxresid['id'])
    res_i = res[len(res)-1]
    # # 将结果插入表中
    # insert_sql = "insert into res2cad(resid,cadid,userid,paramid) values(%s,%s,%s,%s)"
    # cursor.execute(insert_sql,[res_id,cad_id,user_id,param_id,])
    # conn.commit()
    # conn.close()

    Deployment.objects.create(user=user_i,res=res_i,cad=cad_i,dep_param=param_i)
    return HttpResponse(json.dumps({'status': 'success'}))

def deleteRecord(request):
    print(request.POST)
    dep_id = request.POST.get("dc")
    print(dep_id)
    Deployment.objects.filter(id=dep_id).delete()
    return HttpResponse(json.dumps({'status': 'success'}))

def add(request):
    af = AddForm()
    return render(request, 'users/add.html', context={'af': af})


def myDeployment(request):
    #conn, cursor = connectToMySQL()
    if request.method == "GET":
        if request.session.has_key('is_login'):
            userid = request.session['userid']
            res = Deployment.objects.select_related().filter(user=userid)
            return render(request, 'users/myDeployment.html', context={"res": res})
        else:
            # show_content = "<li><a href=users/login>还没登录，前去登录</a></li>"
            return render(request,'users/login.html')
    else:
        return redirect('users/myDeployment.html/')

def login(request):
    return render(request,'users/login.html')

def login_ajax(request):
    print("login ajax test")
    print(request.POST)
    return HttpResponse(json.dumps({'status': 'success'}))

def checkLogin(request):
    print(request.POST)
    uname = request.POST.get("username")
    pwd = request.POST.get("password")
    res = User.objects.filter(username=uname, password=pwd)
    if len(res) == 0:
        return HttpResponse(json.dumps({'status': 'fail'}))
    else:
        request.session['is_login'] = True
        request.session['userid'] = res[0].id
        request.session['username'] = res[0].username
        request.session.set_expiry(3600)
        return HttpResponse(json.dumps({'status': 'success'}))

def register(request):
    return render(request, 'users/register.html')

def checkRegister(request):
    print(request.POST)
    uname = request.POST.get("username")
    pwd = request.POST.get("password")
    confirm_pwd = request.POST.get("confirm_password")
    print(uname,pwd,confirm_pwd)
    res = User.objects.filter(username=uname)
    if len(res) > 0:
        return HttpResponse(json.dumps({'status': 'fail'}))
    else:
        User.objects.create(username=uname, password=pwd)
        return HttpResponse(json.dumps({'status': 'success'}))

def logout(request):
    request.session.flush()
    return render(request, 'users/login.html')
