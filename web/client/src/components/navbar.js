import React from 'react';

import {
  AppBar, Button, Toolbar, Typography,
} from '@material-ui/core';
import AddCircleOutlineIcon from '@material-ui/icons/AddCircleOutline';
import CodeIcon from '@material-ui/icons/Code';
import ExitToAppIcon from '@material-ui/icons/ExitToApp';
import FavoriteBorderIcon from '@material-ui/icons/FavoriteBorder';
import VpnKeyIcon from '@material-ui/icons/VpnKey';

import {
  AUTH_TOKEN, LOGIN_PATH, REGISTER_PATH,
} from '../utils/constants';
import useStyles from '../utils/styles';

export default function NavBar() {
  const classes = useStyles();
  const loggedOutButtons = (
    <>
      <Button variant="contained" href={LOGIN_PATH}>
        Login
        {' '}
        <VpnKeyIcon />
      </Button>
      &nbsp;
      &nbsp;
      &nbsp;
      <Button variant="contained" href={REGISTER_PATH}>
        Create Account
        {' '}
        <AddCircleOutlineIcon />
      </Button>
    </>
  );
  const loggedInButtons = (
    <Button
      variant="contained"
      href={LOGIN_PATH}
      onClick={() => localStorage.removeItem(AUTH_TOKEN)}
    >
      Logout
      {' '}
      <ExitToAppIcon />
    </Button>
  );
  const myButtons = localStorage.getItem(AUTH_TOKEN) ? loggedInButtons : loggedOutButtons;
  return (
    <div className={classes.root}>
      <AppBar position="static">
        <Toolbar>
          <FavoriteBorderIcon color="inherit" />
          <CodeIcon />
          <Typography variant="h6" className={classes.title}>
            Heartspeed
          </Typography>
          {myButtons}
        </Toolbar>
      </AppBar>
    </div>
  );
}
