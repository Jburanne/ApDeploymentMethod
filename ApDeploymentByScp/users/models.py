from django.db import models

# Create your models here.
# class Test(models.Model): #相当于定义了一个User数据库表，继承自models.Model
#     name = models.CharField(max_length=50) #定义数据表中的字段名及类型
#     # upload_to指定上传文件的位置
#     # 这里指定存放在'/img'目录下
#     headimg = models.FileField(upload_to='img/')
#
#     #返回名称
#     def __str__(self):
#         return self.name

# user表
class User(models.Model):
    username = models.CharField(max_length=64,default="")
    password = models.CharField(max_length=64,default="")

# 部署结果表
class Result(models.Model):
    res_path = models.ImageField(upload_to='img/result/',default="")
    pos_path = models.FileField(upload_to='csv/position_result/',default="")
    ap_num = models.IntegerField(default=0)

# cad表
class Cad(models.Model):
    cad_description = models.CharField(max_length=200,default="")
    cad_path = models.ImageField(upload_to='img/cad/',default="")

# 参数表
class DepParams(models.Model):
    cover_num = models.IntegerField(default=0) #每个位置接收的信号数量约束
    dist_thre = models.IntegerField(default=-1) #设备间隔约束（默认为-1）
    spread_dist = models.IntegerField(default=0) #信号无障碍情况下的传播距离，默认值0
    wall_reduce_dist = models.IntegerField(default=0) #信号遇墙壁衰减距离，默认值0
    glass_reduce_dist = models.IntegerField(default=0) #信号遇玻璃衰减距离，默认值0
    wood_reduce_dist = models.IntegerField(default=0) #信号遇木材衰减距离，默认值0
    other_reduce_dist = models.IntegerField(default=0) #信号遇其他衰减距离，默认值0
    time_limit = models.IntegerField(default=0) #算法运行时间，默认值0

# 部署表
class Deployment(models.Model):
    user = models.ForeignKey('User',null=True, on_delete=models.DO_NOTHING)
    cad = models.ForeignKey('Cad',null=True, on_delete=models.DO_NOTHING)
    res = models.ForeignKey('Result',null=True, on_delete=models.DO_NOTHING)
    dep_param = models.ForeignKey('DepParams',null=True, on_delete=models.DO_NOTHING)




