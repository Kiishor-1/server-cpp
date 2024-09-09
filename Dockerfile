# Stage 1: Pull the mongo-cxx-driver image and extract the necessary files
FROM mongodb/mongo-cxx-driver:latest as mongo_cxx_driver

# Stage 2: Build environment for the Crow app
FROM ubuntu:20.04

# Set environment variables to avoid user interaction during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install basic dependencies and asio
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    git \
    libboost-all-dev \
    libssl-dev \
    pkg-config \
    wget \
    curl \
    python3 \
    libasio-dev \
    libsasl2-dev \
    libmongoc-1.0-0 \
    libbson-1.0-0 \
    && rm -rf /var/lib/apt/lists/*

# Create a symbolic link for python
RUN ln -s /usr/bin/python3 /usr/bin/python

# Copy mongo-cxx-driver files from the mongo_cxx_driver image
COPY --from=mongo_cxx_driver /usr/local /usr/local

# Ensure symbolic links for libssl and libcrypto are correctly set
RUN ln -s /usr/lib/x86_64-linux-gnu/libssl.so /usr/lib/libssl.so && \
    ln -s /usr/lib/x86_64-linux-gnu/libcrypto.so /usr/lib/libcrypto.so

# Install Crow
RUN git clone https://github.com/CrowCpp/Crow.git && \
    cd Crow && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install && \
    cd ../.. && rm -rf Crow

# Set the working directory
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . .

# Set the OpenSSL root directory
ENV OPENSSL_ROOT_DIR=/usr/lib/x86_64-linux-gnu

# Copy the .env file
COPY .env .

# Set environment variables from the .env file
ENV $(cat .env | xargs)

# Build the Crow app
RUN mkdir build && cd build && \
    cmake -DOPENSSL_ROOT_DIR=$OPENSSL_ROOT_DIR .. && \
    make

# Make port 8080 available to the world outside this container
EXPOSE 8080

# Run the Crow app
CMD ["./build/CrowServer"]
