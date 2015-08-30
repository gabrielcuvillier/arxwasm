            _______________________
           /\                      \
           \_|         Arx         |
             |      Libertatis     |
             |   __(PNaCl port)____|__
              \_/____________________/

This is an experimental port of Arx Libertatis project to Portable Native Client architecture and Pepper API (PNaCl / PPAPI), allowing to run the Arx Fatalis game in a native sandbox of Google Chrome browser.

## Contents

* [Project Status](#project-status)
* [How To Run](#how-to-run)
* [How To Build](#how-to-build)
* [Technical Details](#technical-details)
* [FAQ](#faq)
* [Credits](#credits)
* [Contact](#contact)

## Project Status

**Working** (...but not yet completely playable).

The game runs smoothly in Chrome browser, embedded as a PNaCl module in a HTML page. It can be loaded directly from the Web, by accessing an URL in the browser. There is no need to install a Chrome App, or use the Chrome Web Store.

Almost everything works flawlessly (game logic, menus, rendering, audio, save/load, etc..), with one notable exception in the input handling backend: the 'mouselook' functionality does not work correctly. This makes navigation in the game quite difficult. As a workaround, keyboard navigation may be used, but this is not practical for real gameplay.

Hence, this port should be considered as an almost-working experiment. See section 'Technical Details / Limitations' for more information on the input handling backend.

The port have been tested with latest version of Chrome for Windows and Linux (starting from version 41), both on x86 and x86_64 architectures. I did not tested with Chromebooks, or any ARM / MIPS based device. I'd be interested to know if this is working !

## How to run

### Prerequisite

First, you must install the latest stable version of Google Chrome Browser, which can be found here for your system (Windows, Linux, OS X):

http://www.google.com/chrome/

Note that for some unknown reason, the open-source browser "Chromium" provided by Debian/Ubuntu distributions is not compiled with Native Client support by default. For now, you have to use the browser provided by Google.

### Running the Game (from the Web!)

Next, the easiest way to see the game running in your browser is simply to go to the following URL using latest version Chrome: 

http://arxpnacl.cad-labs.fr

A static website is hosted here, with a main HTML page embedding the PNaCl module ready to run, and some Javascript code to provide browser-side access to game data.

#### Alternative methods

An alternative method to test this port is to run yourself a local HTTP server, serving the same sample website. 

To do this, download a preview release from github: _arx-pnacl-preview-NNN.zip_, uncompress it, and execute the _run.sh_ script. 

It will start a simple HTTP server, configured to give access to the website content (located in 'www-root' subfolder). In this case, the following URL have to be used:

http://localhost:5100

Finally, you may also build the Native Client module yourself, and host it in a similar way. The source code of the sample website is fully available in git repository. See section 'How To Build' for more information. 

### Access to game data

In all cases, you will need the game data for anything to work !

Game data can be hosted on a remote server and accessed through HTTP, or provided directly by the user from its own local filesystem. This latter option uses the HTML5 Filesystem feature of Chrome, also used to store configuration files and saves locally.

Obviously, I do *not* provide any game data, as the game is licensed under a commercial license. 

So you have to provide the game data yourself, using your own original copy of the game, and use browser-side functionality of the website to allow the Native Client module having access to it. 

Here is how to do:

* Get an original copy of the game.

If you don't already own an original copy of the game, it can be bought for a reasonable price from well known digital stores (GOG, Steam, etc.).
If you like immersive RPGs and have ~30 hours available, don't hesitate: it is an enjoyable game (and really flawed too... but this is another story).

Note there is a Demo version too. For the purpose of testing this port, it is a reasonable choice.

More infos here: http://wiki.arx-libertatis.org/Getting_the_game_data

* Extract game content

Once you have the game, you will need to extract its content: the __*.pak__ files. 

Depending on where you got the game (original CD, digital download, etc.), the method may be different.
More infos here: http://wiki.arx-libertatis.org/Installing_the_game_data_under_Linux

* Make the Native Client module access this content

There is two means to do this: either locally through HTML5 filesystem feature of Chrome, or remotely through HTTP.

If you are running the game from the web, using the URL provided at beginning of this section, you have to use the first option.

* 
    * Option 1: Using the HTML5 filesystem

The process is straightforward: use the "Select Files" button in the main web page, and select all the *.pak files. They will be copied to a HTML5 filesystem from which the Native Client module will be able to read. 

Note this option will really duplicate game data to somewhere in Chrome cache, as Native Client modules ran from the Web can't have direct access to local filesystem (note: the Chrome Filesystem feature is only available to Chrome Apps and Extensions).

Once all data is copied (be sure to wait for all selected files to complete), simply click on "Launch Arx" button... and here you go: Arx Fatalis is running in your browser!

* 
     * Option 2: Access through HTTP

This method require to to host the website yourself.

In this case, copy all the *.pak files under a new 'arx' subfolder of the 'www-root' folder.

Upon access to localhost in the browser, the game will be loaded directly.

### A note on initial loading time

The first time the program is loaded, it will take quite some time to complete (with some visible "hang" around 95%). This is because PNaCl modules are compiled to native architecture from LLVM bytecode, and Arx Libertatis is almost 10MB in size. This is quite a lot.

## How to Build

Building consist in cross-compilation of Arx Libertatis and its dependencies to the Native Client platform, by using the NaCl SDK (providing Toolchain and Pepper API), the Naclports project (providing Native Client ports of most of the required dependencies), and obviously this fork of the Arx Libertatis project.

As usual with cross-compilation, the most difficult part is to correctly install the prerequisites (SDK, dependencies, etc.). So, here we go:

### Software Prerequisites

#### 1) Operating System

I tested building with the following Linux distributions that are known to work: Ubuntu 12.04 LTS, Ubuntu 14.04 LTS, and Debian 8.

Note that Debian 7 is not working due to an outdated GNU libc (the NaCl SDK needs at least 2.15)

I did not tested building on Windows or Mac OS X. Feel free to share experience on this!

#### 2) Install the NaCl SDK

The NaCl SDK provide the core toolchains for NaCl and PNaCl (compiler, linker, standard header and libraries), as well as Pepper API headers and libraries.

Here are the basic steps to install the SDK. More detailed information may be found here: http://developer.chrome.com/native-client/sdk/download

* Check you have the following packages installed: python2.7, make, binutils, and libc6:i386
     
      The last one is required if your distribution is 64 bits. Note that Debian x86_64 needs some specific steps to install x86 packages. Check on the web for "debian multiarch" for more info on this.

* Download the NaCl SDK: http://developer.chrome.com/native-client/sdk/download
* Unzip nacl_sdk.zip 

    Important note: do NOT have non-ascii char in the SDK path. For unknown reason some SDK scripts fails otherwise.

* Run the following commands to install a version of the SDK:

         $ cd nacl_sdk
         $ ./nacl_sdk list
         $ ./nacl_sdk install pepper_44

    Of course, you may use any other stable version of the SDK (and not only 44), starting from pepper_39. The best is to use the latest one flagged as being "stable".

* Finally, setup the NACL_SDK_ROOT environment variable:

        $ export NACL_SDK_ROOT=<path_to_nacl_sdk_folder>/pepper_<version>

    This is very important, as the Naclport project rely on this. And I highly recommend you to add this line to your .bashrc, and update it when you use a new version of the SDK. This will prevent you to do this everytime you start a new shell!

#### 3) Install Naclport project

The Naclport project provide an easy access to Native Client ports of well-known libraries. 
As most of Arx Libertatis dependencies have already been ported to Native Client, they are available in this project out of the box: Zlib, Freetype, SDL2, Boost, OpenAL, and Regal (an OpenGL 1.x wrapper on top of OpenGL ES 2).

Note that they are not always upstream, but it is sufficient for this ArxLibertatis port.

Here are the basic steps to install Naclports. More detailed information may be found here:https://code.google.com/p/naclports/wiki/HowTo_Checkout .

* Check you have the following packages installed: python2.7, python-dev, make, sed, bash, curl, zip

* For some reasons, Naclport relies on "Depot Tools" to be installed. More detailed information may be found here: https://www.chromium.org/developers/how-tos/install-depot-tools .

    As a summary, run the following commands:

            $ git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
            $ export PATH=`pwd`/depot_tools:"$PATH"

* Now you can install Naclports, using the following commands:
    
            $ mkdir naclports
            $ cd naclports
            $ gclient config --name=src https://chromium.googlesource.com/external/naclports.git
            $ gclient sync
            $ git checkout -b pepper_44 origin/pepper_44
            $ gclient sync

    Of course, use whatever version of the SDK you really installed (which is not necessarilly 44).
    
### Building and Linking ArxLibertatis-pnacl

1) Building the project dependencies

Note:
Setting up GLM library dependency is not needed. For convenience, this dependency is stored directly as a git submodule of the ArxLibertatis-pnacl project. 

2) Building ArxLibertatis-pnacl

3) Testing the build

<... to complete ...>

## Technical Details

Even though most of dependencies were already ported thanks to the NACLPort project, porting of the game itself required a wide variety of changes. Most notably:

* Build system
    * Updates to the existing CMake build system, to support cross-compilation to NACL and PNACL toolchains using NACL SDK and Naclports.
* Game source code
    * Specific initialization for backends, and various updates for them to work correctly (for example, disabling usage of GLEW).
    * Filesystem backend changes to access game data from HTTP or HTML5
    * Fix several compilation issues, or other warnings
* Support files
    * Sample website and JS scripts to host the PNaCl module, and provide browser-side access to game data

### Implementation notes

<... to complete ...>

## FAQ

### What is Native Client ?

Native Client (NaCl) is a sandboxing technology for running compiled C and C++ code at native speed in a secure sandbox. Portable Native Client (PNaCl) allow the compiled code to be architecture independent.

For now, this technology is used almost exclusively by the Google Chrome browser to run native applications embedded in web pages. But its usage could go beyond the browser. 

For more information, see: https://developer.chrome.com/native-client

### Why this port ?

This port have been done mostly to learn Native Client stack (toolchain, API, runtime, etc.), and to evaluate if this technology is mature enough to support development of real moderately-sized C++ applications (and not only demonstration programs or benchmarks).

Arx Libertatis project have been chosen because it is an open source project that fit this size requirement, with added bonus to have most of its dependencies already ported to Native Client. 

Also, it is a game I enjoyed in the 2000's..

### 'Mouselook' is unusable! 

Yes, this is _the_ most important limitation of this port. See section 'Technical Details / Input Handling'

## Credits

Arkane Studio, for creating this enjoyable-yet-flawed game, and released the game code under an open source license.

Arx Libertatis team, for its impressive amount of work to enhance it.

The Native Client team, for creating this interesting technology.

## Contact
dev_at_gabrielcuvillier_dot_fr
