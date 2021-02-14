import React, { useState } from 'react';
import { useMutation } from '@apollo/client';

import Avatar from '@material-ui/core/Avatar';
import Button from '@material-ui/core/Button';
import Snackbar from '@material-ui/core/Snackbar';
import TextField from '@material-ui/core/TextField';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import Checkbox from '@material-ui/core/Checkbox';
import Link from '@material-ui/core/Link';
import Grid from '@material-ui/core/Grid';
import Box from '@material-ui/core/Box';
import LockOutlinedIcon from '@material-ui/icons/LockOutlined';
import Typography from '@material-ui/core/Typography';
import Container from '@material-ui/core/Container';
import Alert from '@material-ui/lab/Alert';

import {
  AUTH_TOKEN, REGISTER_PATH, WELCOME_PATH, LOGIN_MUTATION,
} from '../utils/constants';
import useStyles from '../utils/styles';

export default function Login() {
  const classes = useStyles();

  const [formState, setFormState] = useState({
    email: '',
    password: '',
  });
  const [openSnackBar, setSnackBarOpen] = useState(false);
  const [snackBarMessage, setSnackBarMessage] = useState('');

  const handleCloseSnackBar = () => {
    setSnackBarOpen(false);
    setSnackBarMessage('');
  };

  const [login, { loading, error }] = useMutation(
    LOGIN_MUTATION, {
      onCompleted(data) {
        if (!data.tokenAuth.success) {
          if (data.tokenAuth.errors?.nonFieldErrors) {
            // This will only display first nonFieldError
            setSnackBarMessage(data.tokenAuth.errors?.nonFieldErrors[0].message);
            setSnackBarOpen(true);
          }
        } else {
          localStorage.setItem(AUTH_TOKEN, data.tokenAuth.token);
          window.location.href = WELCOME_PATH;
        }
      },
    }, {
      variables: {
        email: formState.email,
        password: formState.password,
      },
    },
  );

  if (loading) return 'loading...';
  if (error) return 'error';

  return (
    <Container component="main" maxWidth="xs">
      <div className={classes.paper}>
        <Avatar className={classes.avatar}>
          <LockOutlinedIcon />
        </Avatar>
        <Typography component="h1" variant="h5">
          Sign in
        </Typography>
        <Snackbar
          anchorOrigin={{
            vertical: 'top',
            horizontal: 'center',
          }}
          open={openSnackBar}
          autoHideDuration={6000}
          onClose={handleCloseSnackBar}
        >
          <Alert severity="error">
            {snackBarMessage}
          </Alert>
        </Snackbar>
        <form className={classes.form} noValidate>
          <TextField
            variant="outlined"
            margin="normal"
            required
            fullWidth
            id="email"
            label="Email Address"
            name="email"
            autoComplete="email"
            autoFocus
            onBlur={(e) => setFormState({
              ...formState,
              email: e.target.value,
            })}
          />
          <TextField
            variant="outlined"
            margin="normal"
            required
            fullWidth
            name="password"
            label="Password"
            type="password"
            id="password"
            autoComplete="current-password"
            onBlur={(e) => setFormState({
              ...formState,
              password: e.target.value,
            })}
          />
          <FormControlLabel
            control={<Checkbox value="remember" color="primary" />}
            label="Remember me"
          />
          <Button
            type="submit"
            fullWidth
            variant="contained"
            color="primary"
            className={classes.submit}
            onClick={(e) => {
              e.preventDefault();
              login({
                variables: {
                  email: formState.email,
                  password: formState.password,
                },
              });
            }}
          >
            Sign In
          </Button>
          <Grid container>
            <Grid item xs>
              {/* eslint-disable-next-line jsx-a11y/anchor-is-valid */}
              <Link href="#" variant="body2">
                Forgot password?
              </Link>
            </Grid>
            <Grid item>
              <Link href={REGISTER_PATH} variant="body2">
                Don&#39;t have an account? Sign Up
              </Link>
            </Grid>
          </Grid>
        </form>
      </div>
      <Box mt={8} />
    </Container>
  );
}
