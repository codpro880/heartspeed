from django.http import HttpRequest, HttpResponse
from django.shortcuts import render


def home(request: HttpRequest) -> HttpResponse:
    if request.user.is_authenticated:
        return render(request, 'home.html', {'user': request.user})
    else:
        return HttpResponse("Login to see this page")
