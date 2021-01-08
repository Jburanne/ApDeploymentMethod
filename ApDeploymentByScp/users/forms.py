from django import forms

# 表单类用以生成表单
class AddForm(forms.Form):
    filename = forms.CharField(label='文件说明',widget=forms.TextInput(attrs={'class':'form-control'}))
    headimg = forms.FileField()
    cover_num = forms.IntegerField(label='覆盖次数约束',widget=forms.NumberInput(attrs={'class':'form-control'}))
    propagation_distance = forms.IntegerField(label='传播距离',widget=forms.NumberInput(attrs={'class':'form-control'}))
    # reduction distance with regard to different materials
    wall_reduction_distance = forms.IntegerField(label='遇墙衰减距离',widget=forms.TextInput(attrs={'class':'form-control'}))
    glass_reduction_distance = forms.IntegerField(label='遇玻璃衰减距离',widget=forms.NumberInput(attrs={'class':'form-control'}))
    wood_reduction_distance = forms.IntegerField(label='遇木材衰减距离',widget=forms.NumberInput(attrs={'class':'form-control'}))
    other_reduction_distance = forms.IntegerField(label='遇其他衰减距离',widget=forms.NumberInput(attrs={'class':'form-control'}))

    time_limit = forms.IntegerField(label='运行时间',widget=forms.NumberInput(attrs={'class':'form-control'}))
    dist_thre = forms.IntegerField(label='设备间距',widget=forms.NumberInput(attrs={'class':'form-control'}))

#login测试
class LoginForm(forms.Form):
    username = forms.CharField(label="用户名",max_length=128,widget=forms.TextInput(attrs={'class':'form-control form-control-lg','placeholder':'用户名'}))
    password = forms.CharField(label='密码',max_length=256, widget=forms.PasswordInput(attrs={'class':'form-control form-control-lg','placeholder':'密码'}))