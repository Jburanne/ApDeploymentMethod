from django import forms

# 表单类用以生成表单
class AddForm(forms.Form):
    filename = forms.CharField()
    headimg = forms.FileField()
    cover_num = forms.IntegerField()
    propagation_distance = forms.IntegerField()
    # reduction distance with regard to different materials
    wall_reduction_distance = forms.IntegerField()
    glass_reduction_distance = forms.IntegerField()
    wood_reduction_distance = forms.IntegerField()
    other_reduction_distance = forms.IntegerField()

    time_limit = forms.IntegerField()
    dist_thre = forms.IntegerField()
