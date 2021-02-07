import React, { useState } from 'react';
import PropTypes from 'prop-types';
import { gql, useMutation } from '@apollo/client';

import Avatar from '@material-ui/core/Avatar';
import Button from '@material-ui/core/Button';
import Snackbar from '@material-ui/core/Snackbar';
import CssBaseline from '@material-ui/core/CssBaseline';
import TextField from '@material-ui/core/TextField';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import Checkbox from '@material-ui/core/Checkbox';
import Link from '@material-ui/core/Link';
import Grid from '@material-ui/core/Grid';
import Box from '@material-ui/core/Box';
import LockOutlinedIcon from '@material-ui/icons/LockOutlined';
import Typography from '@material-ui/core/Typography';
import { makeStyles } from '@material-ui/core/styles';
import Container from '@material-ui/core/Container';
import Alert from '@material-ui/lab/Alert';

const useStyles = makeStyles((theme) => ({
  paper: {
    marginTop: theme.spacing(8),
    display: 'flex',
    flexDirection: 'column',
    alignItems: 'center',
  },
  avatar: {
    margin: theme.spacing(1),
    backgroundColor: theme.palette.secondary.main,
  },
  form: {
    width: '100%', // Fix IE 11 issue.
    marginTop: theme.spacing(1),
  },
  submit: {
    margin: theme.spacing(3, 0, 2),
  },
}));

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

export default function Login({ setToken }) {
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
          // This will only display first nonFieldError
          if (data.tokenAuth.errors?.nonFieldErrors) {
            setSnackBarMessage(data.tokenAuth.errors?.nonFieldErrors[0].message);
            setSnackBarOpen(true);
          }
        }
        setToken(data.tokenAuth.token);
        console.log(data);
      },
    }, {
      variables: {
        email: formState.email,
        password: formState.password,
      },
    },
  );

  if (loading) console.log('loading');
  if (error) console.log('error');

  return (
    <Container component="main" maxWidth="xs">
      <CssBaseline />
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
              <Link href="/register" variant="body2">
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

Login.propTypes = {
  setToken: PropTypes.func.isRequired,
};
