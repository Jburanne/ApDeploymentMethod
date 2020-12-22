from django.db import models

# Create your models here.
class User(models.Model): #相当于定义了一个User数据库表，继承自models.Model
    name = models.CharField(max_length=50) #定义数据表中的字段名及类型
    # upload_to指定上传文件的位置
    # 这里指定存放在'/img'目录下
    headimg = models.FileField(upload_to='img/')

    #返回名称
    def __str__(self):
        return self.name
