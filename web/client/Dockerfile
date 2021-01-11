# Tell Docker to grab a copy of Node and specify its Linux distribution as Alpine
FROM node:alpine

USER root

# Set the working directory inside the container
WORKDIR /app

# Copy the rest of the app into container
COPY . /app

ARG API_URL

ENV REACT_APP_HOST_IP_ADDRESS $API_URL

RUN yarn

RUN yarn build
