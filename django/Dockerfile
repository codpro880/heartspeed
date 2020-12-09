# Set base image (host OS)
FROM python:3.8

# Setting this ensures python output is sent straight to terminal without being buffered first
ENV PYTHONUNBUFFERED=1

# Set the working director inside the container
WORKDIR /code

# Copy list of dependencies to working directory
COPY requirements.txt /code/

# Install dependencies
RUN pip install -r requirements.txt

# Copy the content of working directory to /code
COPY . /code/
