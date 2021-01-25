import json

from django.test import TestCase
from django.contrib.auth import get_user_model

from graphene_django.utils.testing import GraphQLTestCase


'''
Test cases for creating users in db through Django
'''
class UsersManagersTests(TestCase):

    def test_create_user(self):
        User = get_user_model()
        user = User.objects.create_user(email='normal@user.com', password='foo')

        self.assertEqual(user.email, 'normal@user.com')
        self.assertTrue(user.is_active)
        self.assertFalse(user.is_staff)
        self.assertFalse(user.is_superuser)
        # username is None for our custom user model
        self.assertIsNone(user.username)

        # Must specify email and password to create user
        with self.assertRaises(TypeError):
            User.objects.create_user()
        with self.assertRaises(TypeError):
            User.objects.create_user(email='')
        with self.assertRaises(ValueError):
            User.objects.create_user(email='', password="foo")

    def test_create_superuser(self):
        User = get_user_model()
        admin_user = User.objects.create_superuser('super@user.com', 'foo')
        self.assertEqual(admin_user.email, 'super@user.com')
        self.assertTrue(admin_user.is_active)
        self.assertTrue(admin_user.is_staff)
        self.assertTrue(admin_user.is_superuser)
        try:
            # username is None for the AbstractUser option
            # username does not exist for the AbstractBaseUser option
            self.assertIsNone(admin_user.username)
        except AttributeError:
            pass
        with self.assertRaises(ValueError):
            User.objects.create_superuser(
                email='super@user.com', password='foo', is_superuser=False)


'''
Test cases for creating users in db through GraphQL
'''
class TestMutations(GraphQLTestCase):

    VALID_REGISTRATION = '''
        mutation {
          register(
            email: "hufflepuffordie@aol.com",
            firstName: "Cedric",
            lastName:"Diggory",
            password1: "supersecretpassword",
            password2: "supersecretpassword",
          ) {
            success,
            errors,
            token,
            refreshToken
          }
        }
        '''

    MISMATCHED_PASSWORD_REGISTRATION = '''
        mutation {
          register(
            email: "hufflepuffordie@aol.com",
            firstName: "Cedric",
            lastName:"Diggory",
            password1: "mismatchedpassword",
            password2: "mismatchedpassword1",
          ) {
            success,
            errors,
            token,
            refreshToken
          }
        }
        '''

    def test_create_valid_user(self):
        response = self.query(self.VALID_REGISTRATION)
        content = json.loads(response.content)

        self.assertResponseNoErrors(response)

        User = get_user_model()
        user = User.objects.first()
        self.assertEqual(user.first_name, 'Cedric')
        # User has not verified his email address
        self.assertFalse(user.status.verified)

    def test_passwords_must_match(self):
        response = self.query(self.MISMATCHED_PASSWORD_REGISTRATION)
        content = json.loads(response.content)

        # Verify response shows error
        self.assertFalse(content['data']['register']['success'])
        self.assertEqual(content['data']['register']['errors']['password2'][0]['message'],
            'The two password fields didn’t match.')

        # Verify user was not created
        User = get_user_model()
        self.assertIsNone(User.objects.first())

    def test_user_login(self):
        self.query(self.VALID_REGISTRATION)

        response = self.query(
            '''
            mutation {
              tokenAuth(email: "hufflepuffordie@aol.com",
              password: "supersecretpassword") {
                success
                errors
                unarchiving
                token
                refreshToken
                unarchiving
                user {
                  id
                  email
                }
              }
            }
            ''')
        data = json.loads(response.content)['data']

        self.assertTrue(data['tokenAuth']['success'])
        self.assertIsNone(data['tokenAuth']['errors'])

