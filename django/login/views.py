from django.contrib.auth.hashers import make_password
from django.contrib.auth.models import User
from django.http import HttpRequest, HttpResponse
from django.shortcuts import render

from .forms import RegisterForm

# Create your views here.
def register(request: HttpRequest) -> HttpResponse:
    if request.method == 'POST':
        form = RegisterForm(request.POST)
        if form.is_valid():
            new_user = form.save(commit=False)
            new_user.username = new_user.email
            new_user.password = make_password(new_user.password)
            new_user.save()
            return render(request, 'register_confirm.html', {'user': new_user})
    else:
        form = RegisterForm()
    return render(request, 'register.html', {'form': form})


def home(request: HttpRequest) -> HttpResponse:
    if request.user.is_authenticated:
        return render(request, 'home.html', {'user': request.user})
    else:
        return HttpResponse("Login to see this page")
