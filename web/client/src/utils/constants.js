import { gql } from '@apollo/client';

const GQL_ENDPOINT = 'http://localhost:8000/graphql/';
const LOGIN_PATH = '/login';
const REGISTER_PATH = '/register';
const WELCOME_PATH = '/welcome';

const ME_QUERY = gql`
  query {
    me {
      email,
      verified
    }
  }`;

const LOGIN_MUTATION = gql`
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

const REGISTER_MUTATION = gql`
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

export {
  GQL_ENDPOINT,
  LOGIN_PATH,
  REGISTER_PATH,
  WELCOME_PATH,
  ME_QUERY,
  LOGIN_MUTATION,
  REGISTER_MUTATION,
};
