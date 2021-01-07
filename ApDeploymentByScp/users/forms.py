from django import forms

# 表单类用以生成表单
class AddForm(forms.Form):
    filename = forms.CharField()
    headimg = forms.FileField()
    cover_num = forms.IntegerField(label='覆盖次数约束',widget=forms.NumberInput(attrs={'class':'form-control','style':'height:30px'}))
    propagation_distance = forms.IntegerField(label='传播距离',widget=forms.NumberInput(attrs={'class':'form-control','style':'height:30px'}))
    # reduction distance with regard to different materials
    wall_reduction_distance = forms.IntegerField(label='遇墙衰减距离',widget=forms.NumberInput(attrs={'class':'form-control','style':'height:30px'}))
    glass_reduction_distance = forms.IntegerField(label='遇玻璃衰减距离',widget=forms.NumberInput(attrs={'class':'form-control','style':'height:30px'}))
    wood_reduction_distance = forms.IntegerField(label='遇木材衰减距离',widget=forms.NumberInput(attrs={'class':'form-control','style':'height:30px'}))
    other_reduction_distance = forms.IntegerField(label='遇其他衰减距离',widget=forms.NumberInput(attrs={'class':'form-control','style':'height:30px'}))

    time_limit = forms.IntegerField(label='运行时间',widget=forms.NumberInput(attrs={'class':'form-control','style':'height:30px'}))
    dist_thre = forms.IntegerField(label='设备间距',widget=forms.NumberInput(attrs={'class':'form-control','style':'height:30px'}))

#login测试
class LoginForm(forms.Form):
    username = forms.CharField(label="用户名",max_length=128,widget=forms.TextInput(attrs={'class':'form-control form-control-lg','placeholder':'用户名'}))
    password = forms.CharField(label='密码',max_length=256, widget=forms.PasswordInput(attrs={'class':'form-control form-control-lg','placeholder':'密码'}))