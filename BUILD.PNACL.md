## How to Build

Building consist in cross-compilation of Arx Libertatis and its dependencies to the Native Client platform, by using the NaCl SDK, the Naclports project, and this fork of the Arx Libertatis project.

As usual with cross-compilation, the most difficult part is to correctly install and  setup the build environment, and then build the project dependencies.

### Build Prerequisites

#### 1) Operating System

Building have been tested with the following Linux distributions: Ubuntu 12.04 LTS, Ubuntu 14.04 LTS, and Debian 8.

Note that Debian 7 is not working due to an outdated GNU libc (the NaCl SDK needs at least 2.15)

I did not test building on Windows and Mac OS X. Feel free to share experience on this.

#### 2) Installing the NaCl SDK

The NaCl SDK provide the core toolchains for NaCl and PNaCl (compiler, linker, standard header and libraries), and the Pepper API headers and libraries.

Here are the basic steps to install the SDK. More information may be found here: 

http://developer.chrome.com/native-client/sdk/download

* The following packages needs to be installed:

        $ sudo apt-get install python2.7 make binutils libc6:i386 libstdc++6:i386
     
  The last two packages are only required if your distribution is based on a 64-bit architecture. 
      
  Note that Debian 64-bit will require some additional steps to be able to install 32-bit packages. Detailed information here: https://wiki.debian.org/Multiarch/HOWTO

* Download the NaCl SDK:

        $ wget http://storage.googleapis.com/nativeclient-mirror/nacl/nacl_sdk/nacl_sdk.zip
        
  If the direct link doesn't work, the SDK may be found on this page: 
    
  http://developer.chrome.com/native-client/sdk/download

* Unzip the archive:

        $ unzip nacl_sdk.zip 

  __Important note__: do NOT have unicode characters in the SDK path. For unknown reasons some SDK scripts will fail otherwise.

* Run the following commands to install a version of the SDK:

        $ cd nacl_sdk
        $ ./naclsdk list
        $ ./naclsdk install pepper_<version>

  Replace "version" with the latest version flagged as being "stable" from the "list" command. For example, you may use version 42, which is a nice number.

* Finally, setup the NACL_SDK_ROOT environment vhariable:

        $ export NACL_SDK_ROOT=<path_to_nacl_sdk_folder>/pepper_<version>

  This is very important, as the rest of build instructions will rely on this environment variable. I highly recommend you to add this line to your .bashrc, and update it when you use a new version of the SDK. This will prevent you to do this everytime you start a new shell!

#### 3) Installing Naclports

The Naclports project provide an easy access to Native Client ports of well-known open-source applications and libraries. 
As most of Arx Libertatis dependencies have already been ported to Native Client, they are available out of the box in Naclports.

Here are the basic steps to install Naclports. More information may be found here:  

http://code.google.com/p/naclports/wiki/HowTo_Checkout 

* If you come from the previous chapter, be sure to move outside the nacl_sdk folder:
            
        $ cd .. 

* The following packages needs to be installed:

        $ sudo apt-get install git gcc python2.7 python-dev make sed bash curl zip

* For some reason, Naclport relies on "Depot Tools" to be installed. More information may be found here: 

  http://www.chromium.org/developers/how-tos/install-depot-tools 

  As a summary, run the following commands:
    
        $ git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
        $ export PATH=`pwd`/depot_tools:"$PATH"

* Now you can install Naclports, using the following commands:
    
        $ mkdir naclports
        $ cd naclports
        $ gclient config --name=src https://chromium.googlesource.com/external/naclports.git
        $ gclient sync
        $ cd src
        $ git checkout -b pepper_<version> origin/pepper_<version>

  Of course, replace "version" with whatever version of the SDK you installed in the previous chapter.
    
### Building the project dependencies

The library dependencies are of Arx Libertatis are: zlib, Freetype, SDL2, Boost, OpenAL, Regal (an OpenGL 1.x wrapper on top of OpenGL ES 2), and GLM.

Most of them are already ported to Native Client (with the exception of GLM), and part of the Naclports project. They are not always upstream, but it is sufficient for this Arx Libertatis port.

Here are the steps to build these libraries:

* Move into the naclports/src directory (if you come from the previous chapter, you should already be there):

        $ cd naclports/src

* The following packages needs to be installed:

        $ sudo apt-get install autoconf automake libtool texinfo gettext pkg-config xsltproc cmake
        
  These are build-time tools required by the various build systems of the projects.
    
* To build for __Portable Native Client__, run the following commands:

        $ export NACL_ARCH=pnacl
        $ make zlib freetype sdl2 openal-soft regal boost 

  This may be quite long to build, but it will eventually complete.
  Once a library is built, the NACL SDK is directly updated with the build result.

* [Optional] To build for __Native Client__ (that is, x86 or x86_64 architectures), use:

        $ export NACL_ARCH=x86_64
        $ make zlib freetype sdl2 openal-soft regal boost 
    
  Using a Native Client executable instead of Portable Native Client may be useful for debugging, and it allows faster compilation. However, be advised that such Native Client executables can't be run in Chrome without a special flag activated in the browser (this is because they are not architecture independant, hence not ready for the open web).
    
#### A note on GLM library

This library is not part of Naclports.
But as it is a header-only library, there is in fact nothing to be build externally.

For convenience, this library have been stored directly as a git submodule of the ArxLibertatis-pnacl project.

### Building ArxLibertatis-pnacl

Here is the final part, build ArxLibertatis Portable Native Client module:

* If you come from the previous chapter, be sure to move outside _naclports/src_ folder:
    
        $ cd ../..

* Fetch the source code of this fork of Arx Libertatis:

        $ git clone https://github.com/gabrielcuvillier/ArxLibertatis-pnacl.git
        $ cd ArxLibertatis-pnacl
        $ git submodule init
        $ git submodule update

  The last two lines are required to initialize the GLM library stored as submodule.
    
* Generate the makefiles using CMake, configured with the special toolchain file for PNaCl. Be sure to have build all the required dependencies, and setup the NACL_SDK_ROOT environment variable correctly.

        $ mkdir build_pnacl
        $ cd build_pnacl
        $ cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-pnacl.cmake ..
        
  Check twice the name of the toolchain file: it is _toolchain-pnacl.cmake_
        
* Finally, build the final executable using:

        $ make
        
Once the build is complete, there will be two files generated:

* _arx.pexe_    
        
  This is the final Portable Native Client module (.pexe suffix). 
  It have been stripped and 'finalized' (see SDK docs about this concept).
    
  Such portable executables are intended to be loaded and executed in Chrome, directly from the Web, without any special flags activated.
           
* _arx.bc_      
        
  This one have been not finalized. It may be used for debugging pnacl code directly.

As portable native client executable might be quite big for the Web, there is an additional step you may want to consider:
        
        $ make pnacl-compress
        
This will compress the final arx.pexe executable, reducing its size by roughly 30%.

#### [Optional] building for Native Client (x86 or x86_64)

Like the project dependencies, it is possible to build ArxLibertatis for Native Client directly. A special toolchain file is provided for the x86_64 architecture. 

* The makefile generation can be done with the following commands:
    
        $ mkdir build_x86_64-nacl
        $ cd build_x86_64-nacl
        $ cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-x86_64-nacl.cmake ..
        
  Check twice the name of the toolchain file, now it is _toolchain-x86_64-nacl.cmake_

* Then build the final executable:

        $ make
        
Once the build is complete, there will be two files generated:

* _arx_x86_64.nexe_    
        
  This is the final Native Client module (.nexe suffix), for x86_64 architecture.
  It have been stripped.
    
  **Important**: Contrary to Portable modules, these "unportable" ones can't be executed in the Browser by default. The special flag _#enable-nacl_ needs to be activated in Chrome to allow their execution.
           
* _arx_x86_64_unstripped.nexe_
        
  This one have been not stripped. It may be used for debugging native client code.
    

### Testing the build

Portable Native Client modules are executed in Chrome by embedding them in a HTML page delivered through HTTP.

For convenience, a static website that does exactly this have been created. It is located in the ___data/nacl___ subfolder of the project, and contains the following data:

* a _www-root_ subfolder, with the website content.

  This consists of the main HTML page embedding the Portable Native Client module (index.html), a native client manifest file (arx.nmf), as well as a support javascript file to provide browser-side access to game data (arx.js).
    
* a _run.sh_ script allowing to start a local HTTP server configured to give access to this www-root folder.

Note that this website is exactly the same as the one hosted at http://arxpnacl.cad-labs.fr

So, in order to test the build, just copy the generated Portable Native Client module (_arx.pexe_) in the _www-root_ folder, execute the _run.sh_ script from a terminal, and finally open Chrome to load the following URL:

http://localhost:5100

Once there, please refer to the [Access to game data](#access-to-game-data) section for more information on how to setup game data (either by using the HTML5 filesystem, or accessing through HTTP directly).

Here it is, your compiled Arx Liberatis PNaCl module is now running in the browser!

Note it is possible to debug it using GDB, but this is another topic. Please, see the NaCl SDK documentation about how to debug PNaCl modules.

#### [Optional] Test for Native Client (x86 or x86_64)

If you have build for Native Client on x86_64, the way to test the build is almost the same. The differences are:

* This time, it is the Native Client module (_arx_x86_64.nexe_) that must be copied in the _x86_64_ subfolder of the _www-root_ folder.

* The URL to use in Chrome is:
    
  http://localhost:5100/x86_64
    
* ... and don't forget to activate the flag _#enable-nacl_ in Chrome.

You will notice that initial loading time is a lot faster than with PNaCl modules. However, overall program performance is the same afterward. 