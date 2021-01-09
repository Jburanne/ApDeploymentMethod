from django.urls import path
from . import  views

app_name = "users"
urlpatterns = [
    path('add/',views.add, name='add'),
    path('uploadCad/',views.uploadCad, name='uploadCad'),
    path('deploymentResult/', views.deploymentResult, name='deploymentResult'),
    path('myDeployment/', views.myDeployment, name='myDeployment')
]