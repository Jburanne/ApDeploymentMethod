from django.urls import path
from . import  views

app_name = "users"
urlpatterns = [
    path('add/',views.add, name='add'),
    path('uploadCad/',views.add, name='uploadCad'),
    path('deploymentResult/', views.add, name='deploymentResult')
]