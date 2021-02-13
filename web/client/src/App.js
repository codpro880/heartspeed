import React from 'react';
import { BrowserRouter, Route, Switch } from 'react-router-dom';
import { ApolloProvider } from '@apollo/client';
import { apolloClient } from './utils/auth';

import { LOGIN_PATH, REGISTER_PATH, WELCOME_PATH } from './utils/constants';
import Login from './pages/Login';
import Registration from './pages/Registration';
import Welcome from './pages/Welcome';

export default function App() {
  return (
    <ApolloProvider client={apolloClient}>
      <BrowserRouter>
        <Switch>
          <Route path={LOGIN_PATH}>
            <Login />
          </Route>
        </Switch>
      </BrowserRouter>
      <BrowserRouter>
        <Switch>
          <Route path={REGISTER_PATH}>
            <Registration />
          </Route>
        </Switch>
        <Switch>
          <Route path={WELCOME_PATH}>
            <Welcome />
          </Route>
        </Switch>
      </BrowserRouter>
    </ApolloProvider>
  );
}
