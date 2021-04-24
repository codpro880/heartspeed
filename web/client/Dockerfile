# Tell Docker to grab a copy of Node and specify its Linux distribution as Alpine
FROM node:alpine

USER root

# Install python/pip
ENV PYTHONUNBUFFERED=1
RUN apk add --update --no-cache python3 && ln -sf python3 /usr/bin/python
RUN python3 -m ensurepip
RUN pip3 install --no-cache --upgrade pip setuptools

# Install canvas/deps to build...
RUN apk add --no-cache \
        sudo \
        curl \
        build-base \
        g++ \
        libpng \
        libpng-dev \
        jpeg-dev \
        pango-dev \
        cairo-dev \
        giflib-dev \
        python \
        ;


# Set the working directory inside the container
WORKDIR /app

# Copy the rest of the app into container
COPY . /app

ARG API_URL

ENV REACT_APP_HOST_IP_ADDRESS $API_URL

RUN yarn

RUN yarn build
