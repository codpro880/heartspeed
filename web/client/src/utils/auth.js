import {
  ApolloClient, ApolloLink, HttpLink, InMemoryCache, // gql, useQuery,
} from '@apollo/client';
import { GQL_ENDPOINT } from './constants';

const token = localStorage.getItem('token');
const httpLink = new HttpLink({ uri: GQL_ENDPOINT });
const authMiddleware = new ApolloLink((operation, forward) => {
  if (token) {
    operation.setContext({
      headers: {
        Authorization: `JWT ${token}`,
      },
    });
  }
  return forward(operation);
});

const apolloClient = new ApolloClient({
  link: ApolloLink.concat(authMiddleware, httpLink),
  cache: new InMemoryCache(),
});

// TODO: make resuable hook here?
// eslint-disable-next-line no-unused-vars
// const [queryUser, { loading, error }] = useQuery(gql`
//     query {
//       me {
//         email,
//         verified
//       }
//     }`, {
//   onCompleted(data) {
//     console.log(data);
//   },
// });
// eslint-disable-next-line import/prefer-default-export
export { apolloClient };
