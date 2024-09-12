# Stage 1: Build the application
FROM ubuntu:20.04 AS build-stage

# Set timezone to Indian Standard Time
ENV TZ=Asia/Kolkata
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install build dependencies
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

# Clone and build Crow
RUN git clone https://github.com/Kiishor-1/Crow /tmp/crow && \
    cd /tmp/crow && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_CXX_STANDARD=17 .. && \
    make install

# Copy MongoDB C driver from local files instead of cloning
COPY ./docker/mongo-c-driver /tmp/mongo-c-driver

# Build and install MongoDB C driver
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

# Copy the application source code
COPY . /app

# Build the application
WORKDIR /app
RUN rm -rf build && \
    mkdir -p build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/local .. && \
    make -j4

# Stage 2: Create the final image
FROM ubuntu:20.04 AS runtime-stage

# Set timezone to Indian Standard Time
ENV TZ=Asia/Kolkata
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libboost-system-dev \
    libssl-dev \
    curl \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Copy the necessary shared libraries for MongoDB C++ driver
COPY --from=build-stage /usr/local/lib/libmongoc-1.0.so* /usr/local/lib/
COPY --from=build-stage /usr/local/lib/libbson-1.0.so* /usr/local/lib/
COPY --from=build-stage /usr/local/lib/libmongocxx.so* /usr/local/lib/
COPY --from=build-stage /usr/local/lib/libbsoncxx.so* /usr/local/lib/

# Set environment variables
ENV ENVIRONMENT=development \
    MONGO_URI=mongodb+srv://MyServer:B9OT0rVauDjczMTi@cluster0.qoifz6m.mongodb.net/wanderlust2?retryWrites=true&w=majority

# Copy only the necessary files from the build stage
COPY --from=build-stage /app/build/CrowServer /app/build/CrowServer
COPY --from=build-stage /app/build /app/build

# Set the working directory
WORKDIR /app

# Set the entry point for the container
ENTRYPOINT ["/app/build/CrowServer"]