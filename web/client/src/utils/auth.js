import {
  ApolloClient, HttpLink, InMemoryCache,
} from '@apollo/client';
import { setContext } from '@apollo/client/link/context';
import { AUTH_TOKEN, TOKEN_QUERY, GQL_ENDPOINT } from './constants';

let currentApolloClient = null;

const updateAuthHeaders = (apolloClient) => {
  const httpLink = new HttpLink({ uri: GQL_ENDPOINT });
  const authLink = setContext((_, { headers }) => {
    const token = localStorage.getItem(AUTH_TOKEN);
    if (token == null) return null;
    return {
      headers: {
        ...headers,
        Authorization: token ? `JWT ${token}` : '',
      },
    };
  });
  apolloClient.setLink(authLink.concat(httpLink));
};

const genApolloClient = () => {
  const apolloClient = new ApolloClient({
    cache: new InMemoryCache(),
  });
  updateAuthHeaders(apolloClient);
  return apolloClient;
};

const getApolloClient = () => {
  if (currentApolloClient == null) {
    currentApolloClient = genApolloClient();
  }
  return currentApolloClient;
};

const setToken = (token) => {
  const apolloClient = getApolloClient();
  apolloClient.writeQuery(
    { query: TOKEN_QUERY, data: { token } },
  );
  updateAuthHeaders(apolloClient);
};

export { getApolloClient, setToken, updateAuthHeaders };
