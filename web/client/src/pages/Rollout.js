import React from 'react';
import { useQuery } from '@apollo/client';

import Typography from '@material-ui/core/Typography';

import { ME_QUERY } from '../utils/constants';
import useStyles from '../utils/styles';

export default function Rollout() {
  const classes = useStyles();
  const { loading, error, data } = useQuery(ME_QUERY);

  if (loading) return 'Loading...';
  if (error) return 'Error';
  if (data.me == null) return 'data was null';

  return (
    <div className={classes.paper}>
      <Typography variant="h2" gutterBottom>
        Hello, welcome to rollout
        {' '}
        {data.me.email}
      </Typography>
    </div>
  );
}
