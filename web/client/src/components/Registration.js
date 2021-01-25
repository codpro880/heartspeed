import React, { useState } from 'react';
import { gql, useMutation } from '@apollo/client';

import Avatar from '@material-ui/core/Avatar';
import Button from '@material-ui/core/Button';
import CssBaseline from '@material-ui/core/CssBaseline';
import TextField from '@material-ui/core/TextField';
import Link from '@material-ui/core/Link';
import Grid from '@material-ui/core/Grid';
import LockOutlinedIcon from '@material-ui/icons/LockOutlined';
import Typography from '@material-ui/core/Typography';
import { makeStyles } from '@material-ui/core/styles';
import Container from '@material-ui/core/Container';

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
    marginTop: theme.spacing(3),
  },
  submit: {
    margin: theme.spacing(3, 0, 2),
  },
}));

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

export default function Registration() {
  const classes = useStyles();

  const [errors, setErrors] = useState(null);
  const [formState, setFormState] = useState({
    firstName: '',
    lastName: '',
    email: '',
    password1: '',
    password2: '',
  });

  const [signup, { loading, error } ] = useMutation(
    REGISTER_MUTATION, {
            onCompleted(data) {
        if (data.register.errors) {
          setErrors(data.register.errors);
        } else {
          setErrors(null);
        }
      }
    }, {
      variables: {
        firstName: formState.firstName,
        lastName: formState.lastName,
        email: formState.email,
        password1: formState.password1,
        password2: formState.password2,
      }
    }
  );

  // if (loading) alert('loading');
  // if (error) alert ('error');

  return (
    <Container component="main" maxWidth="xs">
      <CssBaseline />
      <div className={classes.paper}>
        <Avatar className={classes.avatar}>
          <LockOutlinedIcon />
        </Avatar>
        <Typography component="h1" variant="h5">
          Sign up
        </Typography>
        <form className={classes.form} noValidate>
          <Grid container spacing={2}>
            <Grid item xs={12} sm={6}>
              <TextField
                autoComplete="fname"
                name="firstName"
                variant="outlined"
                fullWidth
                id="firstName"
                label="First Name"
                autoFocus
                error={!!errors?.firstName?.length}
                helperText={errors?.firstName?.[0].message}
                onBlur={(e) => setFormState({
                    ...formState,
                    firstName: e.target.value
                  })
                }
              />
            </Grid>
            <Grid item xs={12} sm={6}>
              <TextField
                variant="outlined"
                fullWidth
                id="lastName"
                label="Last Name"
                name="lastName"
                autoComplete="lname"
                error={!!errors?.lastName?.length}
                helperText={errors?.lastName?.[0].message}
                onBlur={(e) => setFormState({
                    ...formState,
                    lastName: e.target.value
                  })
                }
              />
            </Grid>
            <Grid item xs={12}>
              <TextField
                variant="outlined"
                required
                fullWidth
                id="email"
                label="Email Address"
                name="email"
                autoComplete="email"
                error={!!errors?.email?.length}
                helperText={errors?.email?.[0].message}
                onBlur={(e) => setFormState({
                    ...formState,
                    email: e.target.value
                  })
                }
              />
            </Grid>
            <Grid item xs={12}>
              <TextField
                variant="outlined"
                required
                fullWidth
                name="password1"
                label="Password"
                type="password"
                id="password1"
                autoComplete="current-password"
                error={!!errors?.password1?.length}
                helperText={errors?.password1?.[0].message}
                onBlur={(e) => setFormState({
                    ...formState,
                    password1: e.target.value
                  })
                }
              />
            </Grid>
            <Grid item xs={12}>
              <TextField
                variant="outlined"
                required
                fullWidth
                name="password2"
                label="Confirm Password"
                type="password"
                id="password2"
                autoComplete="current-password"
                error={!!errors?.password2?.length}
                helperText={errors?.password2?.[0].message}
                onBlur={(e) => setFormState({
                    ...formState,
                    password2: e.target.value
                  })
                }
              />
            </Grid>
          </Grid>
          <Button
            fullWidth
            variant="contained"
            color="primary"
            className={classes.submit}
            onClick={() => {
              signup({ variables: {
                firstName: formState.firstName,
                lastName: formState.lastName,
                email: formState.email,
                password1: formState.password1,
                password2: formState.password2
                }})
            }}
          >
            Sign Up
          </Button>
          <Grid container justify="flex-end">
            <Grid item>
              <Link href="#" variant="body2">
                Already have an account? Sign in
              </Link>
            </Grid>
          </Grid>
        </form>
      </div>
    </Container>
  );
}
