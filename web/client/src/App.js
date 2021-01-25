import React from 'react';
import { ApolloClient, ApolloProvider, InMemoryCache } from '@apollo/client';

import Container from '@material-ui/core/Container';
import Box from '@material-ui/core/Box';

import ProTip from './components/ProTip';
import Registration from './components/Registration';

const client = new ApolloClient({
  uri: 'http://localhost:8000/graphql/',
  cache: new InMemoryCache(),
});

export default function App() {
  return (
    <ApolloProvider client={client}>
      <Registration/>
      <Container maxWidth="sm">
        <Box my={4}>
          <ProTip />
        </Box>
      </Container>
    </ApolloProvider>
  );
}
