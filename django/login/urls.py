from django.urls import include, path
from . import views

urlpatterns = [
    path('', include('django.contrib.auth.urls')),
    path('', include('django_registration.backends.one_step.urls')),
]
