# Use the official Ubuntu 20.04 image as a base
FROM ubuntu:20.04

# Set timezone to Indian Standard Time
ENV TZ=Asia/Kolkata
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    git \
    libboost-all-dev \
    libssl-dev \
    wget \
    curl \
    libasio-dev \
    python3 \
    python3-pip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Link Python to Python3
RUN ln -s /usr/bin/python3 /usr/bin/python

# Clone and build Crow
RUN git clone https://github.com/Kiishor-1/Crow /tmp/crow && \
    cd /tmp/crow && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_CXX_STANDARD=17 .. && \
    make install


# Copy MongoDB C driver from local files instead of cloning
COPY ./docker/mongo-c-driver /tmp/mongo-c-driver

# Remove existing cmake-build folder if it exists, then build and install MongoDB C driver
RUN cd /tmp/mongo-c-driver && \
    rm -rf cmake-build && \
    mkdir cmake-build && \
    cd cmake-build && \
    cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
    make -j8 && make install && \
    rm -rf /tmp/mongo-c-driver

# Copy and build MongoDB C++ driver from local repository
COPY ./docker/mongo-cxx-driver /tmp/mongo-cxx-driver
RUN cd /tmp/mongo-cxx-driver && \
    rm -rf build && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_CXX_STANDARD=17 -DBSONCXX_POLY_USE_BOOST=1 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DBUILD_VERSION=1.0.0 .. && \
    make -j8 && make install && \
    rm -rf /tmp/mongo-cxx-driver

# Copy the application source code to the container
COPY . /app

# Set environment variables using ENV
ENV ENVIRONMENT=development \
    MONGO_URI=mongodb+srv://MyServer:B9OT0rVauDjczMTi@cluster0.qoifz6m.mongodb.net/wanderlust2?retryWrites=true&w=majority

# Copy the .env file to the container's parent directory
# COPY .env /app/.env

# Build the application
WORKDIR /app

# Clean up any existing build folder to avoid CMake cache issues
RUN rm -rf build

RUN mkdir -p build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/local .. && \
    make -j4 

# Set the entry point for the container
ENTRYPOINT ["/app/build/CrowServer"]
