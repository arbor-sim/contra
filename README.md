# contra -- a lightweight CONduit - TRAnsport library.

### License:
### Latest Version: https://devhub.vr.rwth-aachen.de/VR-Group/contra


# Introduction

Contra is designed to transport conduit data between processes.
For doing so it offers different kinds of transport layers which can be used.

Contra has been cross-developed for both Windows and Unix systems.

## Supported compilers:
  - msvc (tested with msvc++14.1) 
  - gcc (tested with gcc 7.3.1)
  - clang (tested with clang 9.1)
  
Other compilers versions might work as well. 

## The different transport layer libraries:
  - File Transport
     - This is the basic transport method which comes with contra mainly for testing purposes. 
     
  - Shared Memory Transport
    - This library uses Boost.Interproces to enable IPC via shared memory.
    - To build it add the option "WITH_SHARED_MEMORY" to true.
    ```
        cmake -DWITH_SHARED_MEMORY=True
    ```      
  - ZeroMQ Transport
      - A TCP based transport relying on ZeroMQ for the Network Communication
      - To build it add the option "WITH_ZEROMQ" to true.
      ```
        cmake -DWITH_ZEROMQ=True
      ```
# How to install contra

## REQUIREMENTS
  - Python
  - CMake (v.3.6.0 or higher)

contra is installed with CMake (at least version 3.6.0)
We also recommend using Conan to simplify the installation process. If you decide to not use Conan please continue at "Installing contra without conan".

## Installing contra using Conan

Make sure that you are using a consistent python version for both Conan and Contra

Use pip to install conan:
```
  pip install conan
```
Configure conan to use packages which are (stil) provided by the VR-Group use:
```
  conan remote add rwth-vr--bintray https://api.bintray.com/conan/rwth-vr/conan
  conan remote add bincrafters_public https://api.bintray.com/conan/bincrafters/public-conan
```  
Once Conan is ready you can use CMake to configure and build contra:
```  
  cmake
  make
```  
Afterwards change to your build directory and execute
```
  ctest
```
to make sure contra has been properly installed.  

## Installing contra without Conan


## Using Contra
