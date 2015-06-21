            _______________________
           /\                      \
           \_|         Arx         |
             |      Libertatis     |
             |   __(PNaCl port)____|__
              \_/____________________/

This is an experimental port of Arx Libertatis project to Portable Native Client (PNaCl) and Pepper API, allowing to run the Arx Fatalis game in a native sandbox of Google Chrome browser.

## Contents

* [Project Status](#Project Status)
* [How To Run](#How To Run)
* [How To Build](#How To Build)
* [Technical Details](#Technical Details)
* [FAQ](#FAQ)
* [Credits](#Credits)
* [Contact](#Contact)

## Project Status

**Working** ...but not yet completely playable.

The game runs smoothly in Chrome browser, embedded as a PNaCl module in a HTML page. It can be loaded directly from the Web, simply by accessing an URL in Chrome. There is no need to install a Chrome App, or use the Chrome Web Store.

Most of the game subsystems are working: main game, rendering, audio, menus, filesystem access, save/load, etc.

*However*, there is still one major limitation in the input handling backend: the 'mouselook' functionality does not work correctly. This makes navigation in a level quite difficult. You may still use the keyboard to look and move around, but this is really not practical. 

Hence, this port is not yet ready for real gameplay, and should be considered as an experiment. See section 'Technical Details / Limitations' for more information on the input handling backend.

The port have been tested with latest version of Chrome for Windows and Linux (starting from version 41), both on x86 and x86_64 architectures. I did not tested with Chromebooks, or any ARM / MIPS based device. I'd be interested to know if this is working !

## How to run

The easiest way to see the game running in your browser is to go to the following URL using latest version Chrome: 

http://arxpnacl.cad-labs.fr

A static website is hosted here, with a main HTML page embedding the PNaCl module ready to run, and some Javascript code to implement browser-side access to game data.

#### Alternative method

An alternative way to test this port is to run yourself a local HTTP server, serving the same website. 

To do this, you have to download a preview release from github: _arx-pnacl-preview-NNN.zip_, uncompress it, and execute the _run.sh_ script. 

It will start a simple HTTP server, configured to give access to the website contents (located in 'www-root' subfolder). In this case, you have to go to the following URL with Chrome:

http://localhost:5100

Finally, you may also build the Native Client module yourself, and host it in a similar way. The source code of the hosted website is fully available in git repository. See section 'How To Build' for more information on this last method. 

#### Access to game data

In all cases, you will need the game data for anything to work !

Game data can be hosted on a remote server and accessed through HTTP, or provided directly by the user from its own local filesystem. This latter option uses the HTML5 Filesystem feature of Chrome, also used to store configuration files and saves locally.

Obviously, I do *not* provide any game data, as it is licensed under a commercial license. 

So you have to provide the game data  yourself, using your own original copy of the game, and use custom functionality of the hosted website to allow the Native Client module having access to it. 

Here is how to do:

* Get an original copy of the game.

If you don't already own an original copy of the game, it can be bought for a reasonable price from well known digital stores (GOG, Steam, etc.).
If you like imersive RPGs and have ~30 hours available, don't hesitate: it is an enjoyable game (and really flawed too... but this is another story).

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

The process is straightforward: use the "Select Files" button in the main web page, and select all the *.pak files. They will be copied to a local HTML5 filesystem from which the Native Client module will be able to read. 

Note this option will really duplicate game data to somewhere in Chrome cache, as when Native Client modules are ran from the Web, direct access to local filesystem is not possible.

Once all data is copied (be sure to wait for all selected files to complete), simply click on "Launch Arx" button... and here you go: Arx Fatalis is running in your browser!

* 
     * Option 2: Access through HTTP

This method require to to host the website yourself.

In this case, copy all the *.pak files under a new 'arx' subfolder of the 'www-root' folder.

Upon access to localhost in the browser, the game will be loaded directly.

#### A note on initial loading time

Caution: it will take quite some time to load the first time, because PNaCl module is almost 10MB size. This is quite a lot for Portable Native Client.


## How to Build

Building consist in cross-compilation Arx Libertatis and its dependencies to Native Client (either NaCl or PNaCl), using this fork of the project, the NACL SDK and Naclport.

<...to complete...>

Important note: I only tested building on Linux (Ubuntu 12.04 LTS), and not on Windows.

### Prerequisites

The following prerequisites needs to be installed: the NACL SDK, and Naclport.

Note: CMake is required too, as it used as build system by Arx Libertatis and one of its dependencies.

1) NACL SDK

The NACL SDK provide the core NACL and PNACL toolchains (compiler, linker, standard header and libraries), as well as Pepper Plugin API headers and libraries.

<...to complete...>

2) Naclport project

This project provide a convenient access to Native Client ports of well-known libraries, by capturing specific NaCl build process and patches required to compile these projects in one place. It turned out to be really handy!

As most of Arx Libertatis dependencies have already been ported to Native Client, they are available in Naclports: Zlib, Freetype, SDL2, Boost, and Regal (classic OpenGL emulation layer on top of OpenGL ES 2).

They are not always upstream, but it is sufficient for this ArxLibertatis port.

<...to complete...>

3) -- GLM --

Setting up GLM is no more needed. This library dependency is stored directly as a git submodule of the ArxLibertatis-pnacl project. 

I found this to be convenient, as it is a header-only library (there is nothing separate to compile from the main program), and it is the only dependency not handled by NACL SDK or Naclport project.

### Building ArxLibertatis-pnacl


<...to complete...>

## Technical Details

Even though a lot of dependencies were already ported, porting of the game itself required many changes. Most notably:

* Build system
    * Changes to the existing CMake build system, to support cross-compilation to NACL and PNACL toolchains using NACL SDK and Naclports.
* Game source code
    * Specific initialization for backends, and misc updates for them to work correctly (for example, disabling usage of GLEW).
    * Filesystem backend changes to access game data from HTTP or HTML5
    * Fix several compilation issues, or other warnings
* Support files
    * Sample website and JS scripts to host the game, and provide access to game data

## FAQ

### What is Native Client ?

Native Client (NaCl) is a sandboxing technology for running compiled C and C++ code at near-native speed in a secure sandbox. Portable Native Client (PNaCl) brings architecture independance to the compiled code.

For now, this technology is used almost exclusively by the Google Chrome browser to run native applications embedded in web pages. But its usage could go beyond the browser. 

For more information, see: https://developer.chrome.com/native-client

### Why this port ?

This port have been done mostly to learn Native Client stack (toolchain, API, runtime, etc.), and to evaluate if this technology is mature enough to support development of real moderately-sized C++ applications (and not only demonstration programs or benchmarks).

Arx Libertatis project have been chosen because it is an open source project that fit this size requirement, with added bonus to have most of its dependencies already ported to Native Client. 

Also, it is a game I enjoyed a while back..

### Mouse look is unusable! 

Yes, this is a known limitation. See section 'Limitations / Input Handling'

## Credits

Arkane Studio, for creating this enjoyable-yet-flawed game, and released the game code under an open source license.

Arx Libertatis team, for its impressive amount of work to enhance it.

The Native Client team, for creating this nice technology.

## Contact
dev at gabrielcuvillier dot fr
