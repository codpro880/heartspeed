import React, { useState }from 'react';
import { BrowserRouter, Route, Switch } from 'react-router-dom';
import { ApolloClient, ApolloProvider, InMemoryCache } from '@apollo/client';

import Container from '@material-ui/core/Container';
import Box from '@material-ui/core/Box';

import ProTip from './components/ProTip';
import Login from './components/Login';
import Registration from './components/Registration';

const client = new ApolloClient({
  uri: 'http://localhost:8000/graphql/',
  cache: new InMemoryCache(),
});

export default function App() {
  const [token, setToken] = useState();
  return (
    <ApolloProvider client={client}>
    <BrowserRouter>
      <Switch>
        <Route path="/login">
          <Login setToken={setToken} />
        </Route>
      </Switch>
    </BrowserRouter>
    <BrowserRouter>
      <Switch>
        <Route path="/register">
          <Registration />
        </Route>
      </Switch>
    </BrowserRouter>
      <Container maxWidth="sm">
        <Box my={4}>
          <ProTip />
        </Box>
      </Container>
    </ApolloProvider>
  );
}
