import React from 'react';
import { useQuery } from 'react-apollo';
import { gql } from 'apollo-boost';


const QUERY_USERS = gql`
query {
  users {
    edges {
      node {
        pk,
        firstName,
        lastName
      }
    }
  }
}
`;


export function UserInfo() {

  const { loading, error, data } = useQuery(QUERY_USERS);
  if (loading) return <p>Loading...</p>;
  if (error) return `Error! ${error.message}`;

  let userArray = [];
  for (let i = 0; i < data.users.edges.length; i++) {
    userArray.push(data.users.edges[i].node)
  }

  return userArray.map(({ pk, firstName, lastName }) => (
    <div key={pk}>
      <p>
        User - {pk}: {firstName} {lastName}
      </p>
    </div>
  ));
}
