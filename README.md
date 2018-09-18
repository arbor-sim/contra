# contra -- a lightweight transport library for conduit data

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
    - To build it set the CMake option "WITH_SHARED_MEMORY" to true.

    ```
    cmake -DWITH_SHARED_MEMORY=True
    ```      
  - ZeroMQ Transport
      - A TCP based transport relying on ZeroMQ for the Network Communication.
      - To build it set the CMake option "WITH_ZEROMQ" to true.

      ```
    cmake -DWITH_ZEROMQ=True
      ```

# How to install contra

## REQUIREMENTS
  - [Python](https://www.python.org/)
  - [CMake](https://cmake.org/) (v.3.6.0 or higher)

contra is installed with CMake.
We also recommend using Conan to simplify the installation process. If you decide to not use Conan please continue at "Installing contra without conan".

## Installing contra using Conan

Make sure that you are using a consistent python version for both Conan and Contra

Use pip to install pytest conan:
```
pip install -U pytest
pip install conan
```
Configure conan to use packages which are (stil) provided by the VR-Group:
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

If you do not use conan u will need the following prerequisites:
- [Cpplint](https://github.com/google/styleguide/tree/gh-pages/cpplint)
- [Catch2](https://github.com/catchorg/Catch2)
- [Conduit](https://github.com/LLNL/conduit)
- [Cppcheck](https://sourceforge.net/projects/cppcheck/)
- [Boost.Python](https://www.boost.org/users/download/)

Wehn building with Shared Memory Transport you will also need:
- [Boost.Interprocess](https://www.boost.org/users/download/)

Wehn building with ZeroMQ Transport you will also need:
- [cppzmq](https://github.com/zeromq/cppzmq)


## Using Contra

Contra is designed to use Relays as communication nodes.
In most cases you will have 2 relays Communicating which each other. For the ZeroMQ transport you can have multiple CLIENT relays though.
A relay uses a Transport layer defined at construction e.g:
```cpp
contra::Relay<contra::SharedMemoryTransport> relay;
```

#### The constructor arguments for the different layers with examples:
- File Transport:
    - Filenename (String) The names of the two relays communicating have to match!

```cpp
contra::Relay<contra::FileTransport> relay("testfile.contra");
```
- Shared Memory Transport:
    - Memory name(String) (Default = "contraShMemTransp")The names of the two relays communicating have to match!

```cpp
contra::Relay<contra::SharedMemoryTransport> relay;
```
- ZeroMQ Transport:
    - Type (SERVER/CLIENT)
    - TCP Address (String)
    - Optional flag to indicate if you want the relay to wait for a connection to be established before sending Data (boolean)

```cpp
contra::Relay<contra::ZMQTransport> relay(contra::Relay<contra::ZMQTransport>(contra::ZMQTransport::Type::SERVER, "tcp://*:5555");
```


#### Sample sending relay:
```cpp
#include <string>

#include "contra/boost-shmem/shared_memory_transport.hpp"
#include "contra/relay.hpp"

int main() {
  conduit::Node n;
  n["foo/bar"] = 3.14;
  contra::Relay<contra::SharedMemoryTransport> relay;
  for (int i = 0; i < 5; ++i) {
    relay.Send(n);
  }
  return EXIT_SUCCESS;
}
```
#### Sample recieving relay:
```cpp
#include <string>
#include <thread>
#include "contra/boost-shmem/shared_memory_transport.hpp"
#include "contra/relay.hpp"

int main() {
  contra::Relay<contra::SharedMemoryTransport> relay;
  std::vector<conduit::Node> data;
  do {
    data = relay.Receive();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  } while (data.size() == 0);

  std::cout << data[0]["foo/bar"].to_double();

  return EXIT_SUCCESS;
}
```
