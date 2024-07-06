# Server-Cpp Project

## Introduction

This project is a C++ server application using the Crow framework for building RESTful APIs. It integrates with MongoDB using the C and C++ drivers (mongo-c-driver and mongo-cxx-driver) for database operations.

## Building MongoDB C Driver

### Compile MongoDB C Driver

Use the package manager [pip](https://pip.pypa.io/en/stable/) to install foobar.

```bash
cmake -G "MinGW Makefiles" -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_INSTALL_PREFIX=path/to/install/dir ..

```

### Build MongoDB C Driver

```bash
cmake --build . --target install
```

## Building MongoDB C++ Driver

### Compile MongoDB C++ Driver

```bash
cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=C:/mongo-cxx-driver -DLIBBSON_DIR=C:/mongo-c-driver/cmake-build/src/libbson -DLIBMONGOC_DIR=C:/mongo-c-driver/cmake-build/src/libmongoc ..
```
or
```bash
cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=C:/mongo-cxx-driver -DLIBBSON_DIR=C:/mongo-c-driver/cmake-build/src/libbson -DLIBMONGOC_DIR=C:/mongo-c-driver/cmake-build/src/libmongoc -DCMAKE_MAKE_PROGRAM=C:/msys64/mingw64/bin/mingw32-make ..
```
or
```bash
cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=C:/mongo-cxx-driver -DLIBBSON_DIR=C:/mongo-c-driver/cmake-build/src/libbson -DLIBMONGOC_DIR=C:/mongo-c-driver/cmake-build/src/libmongoc -DCMAKE_MAKE_PROGRAM=C:/msys64/mingw64/bin/mingw32-make -DCMAKE_PREFIX_PATH=C:/mongo-c-driver -DBUILD_VERSION=1.25.0 ..
```

### Build MongoDB C++ Driver

```bash
cmake --build . --target install
```
or
```bash
cmake --build . --config Release
# Manually set "MinGW Makefiles" if nmake is executed on your OS
```
