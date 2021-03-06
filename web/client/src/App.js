import React from 'react';
import { BrowserRouter, Route, Switch } from 'react-router-dom';

import NavBar from './components/navbar';
import Login from './pages/Login';
import Registration from './pages/Registration';
import Welcome from './pages/Welcome';
import Rollout from './pages/Rollout';

import {
  LOGIN_PATH, REGISTER_PATH, ROLLOUT_TOOL_PATH, WELCOME_PATH,
} from './utils/constants';

export default function App() {
  return (
    <>
      <NavBar />
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
        <Switch>
          <Route path={ROLLOUT_TOOL_PATH}>
            <Rollout />
          </Route>
        </Switch>
      </BrowserRouter>
    </>
  );
}
