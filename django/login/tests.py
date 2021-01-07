from django.contrib.auth.models import User
from django.test import TestCase


class TestRegistration(TestCase):
    USERNAME = "testuser"
    EMAIL = "johnsmith123@gmail.com"
    PASSWORD = "strongpw123"

    def test_registration_page_url(self):
        response = self.client.get("/account/register/")
        self.assertEqual(response.status_code, 200)
        self.assertTemplateUsed(response, template_name="django_registration/registration_form.html")

    def test_register_new_user(self):
        self.assertEqual(len(User.objects.all()), 0)

        response = self.client.post("/account/register/", data={
            'username': self.USERNAME,
            'email': self.EMAIL,
            'password1': self.PASSWORD,
            'password2': self.PASSWORD,
        })

        self.assertEqual(response.status_code, 302)
        self.assertEqual(response.url, '/account/register/complete/')
        self.assertEqual(len(User.objects.all()), 1)
        self.assertEqual(User.objects.first().username, self.USERNAME)
        self.assertEqual(User.objects.first().email, self.EMAIL)
        self.assertIsNotNone(User.objects.first().password)
