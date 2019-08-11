
# Building on Ubuntu 18.04

#### 1) Install Emscripten
a) Get the Emscripten SDK and install Emscripten "upstream" variant:
```
git clone https://github.com/emscripten-core/emsdk.git
./emsdk/emsdk install latest-upstream
./emsdk/emsdk activate latest-upstream
 ``` 
b) Enable Emscripten tools from the command line
```
source ./emsdk/emsdk_env.sh
```

#### 2) Build arxwasm
a) Get arxwasm source code 
```
git clone https://github.com/gabrielcuvillier/arxwasm.git
cd arxwasm
git submodule init
git submodule update
```
b) Build the project
```
mkdir build-wasm
cd build-wasm
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release
emmake make
```
Normaly, this should have generated *arxwasm.html*, *arxwasm.js*, and *arxwasm.wasm* files.

#### 4) Package the game demo data
TBD - See data/emscripten/package_demo_data.sh

#### 5) Deploy locally
a) Run a local web server from the build directory. The simplest is to use python SimpleHTTPServer:
```
python -m SimpleHTTPServer 8080
```
or (python 3)
```
python -m http.server 8080
```
b) open your favorite Browser to http://localhost:8080/arxwasm.html

#### 6) Enjoy!

**NB**: it is possible to do a native build also, to compare the performance for example. 
To do so:
```
  mkdir build-native
  cd build-native
  cmake .. -DCMAKE_BUILD_TYPE=Release
  make
```
Packaging the data works differently for native builds. Have a look at https://github.com/dhewm/dhewm3/wiki/FAQ

# Building on Windows 10

While it is possible to build using the Windows version of Emscripten toolchain, it is a bit more tedious to do (require to manually install Windows version of Cmake, GNU Make or Ninja, and setup environment paths).

A simpler way is to use **WSL** (*Windows Subsystem for Linux*) with Ubuntu 18.04 installed: the instructions are then exactly the same as the Ubuntu build. Nice!
