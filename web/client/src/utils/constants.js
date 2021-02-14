import { gql } from '@apollo/client';

export const AUTH_TOKEN = 'auth-token';
export const GQL_ENDPOINT = 'http://localhost:8000/graphql/';
export const LOGIN_PATH = '/login';
export const REGISTER_PATH = '/register';
export const WELCOME_PATH = '/welcome';

export const ME_QUERY = gql`
  query {
    me {
      email,
      verified
    }
  }`;

export const TOKEN_QUERY = gql`
  query {
    token
  }`;

export const LOGIN_MUTATION = gql`
  mutation login(
    $email: String!
    $password: String!
  ) {
    tokenAuth(email: $email,
      password: $password
    ) {
      success
      errors
      token
      refreshToken
      user {
        id
        email
      }
    }
  }
`;

export const REGISTER_MUTATION = gql`
  mutation register(
    $email: String!
    $firstName: String!
    $lastName: String!
    $password1: String!
    $password2: String!
  ) {
    register(
      email: $email
      firstName: $firstName
      lastName: $lastName
      password1: $password1
      password2: $password2
    ) {
        success,
        errors,
        token,
        refreshToken
    }
  }
`;
