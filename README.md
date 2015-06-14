            _______________________
           /\                      \
           \_|         Arx         |
             |      Libertatis     |
             |   __(PNaCl port)____|__
              \_/____________________/

This is an experimental port of Arx Libertatis project to Portable Native Client architecture (PNaCl), allowing to run the Arx Fatalis game in a native sandbox of Google Chrome browser.

## Status

**Working** (... but not yet completely playable*)

The game runs smoothly in Chrome browser, embedded as a PNaCl module in a HTML page. It can be loaded directly from the Web, just by opening an URL in Chrome. There is no need to install a Chrome App, or use the Chrome Web Store. 

Once the PNaCl module is loaded (this might be long the first time), the game start: you can see the introduction cinematic, navigate in menus, start a new game, play the game, save/load, etc.

* However, as of current project status there is still an important limitation to gameplay: the 'mouselook' functionality do not work correctly, making navigation in a level quite difficult. 

Hence, this port is not yet ready for real gameplay, and should be considered as an experiment. See section 'Limitations' for more information on this.

Game data can be accessed either through HTTP and hosted on a server, or provided directly by the user from its local filesystem. This latter option use the HTML5 filesystem feature of Chrome, which is also used to store configuration files and saves locally.

Obviously, I do *not* provide any game data, as it is licensed under a commercial license. So you have to provide the game data yourself, using your own original copy of the game. See section 'How to run' for more information.

The port have been tested with latest version of Chrome for Windows and Linux (starting from version 41), both on x86 and x86_64.
I did not tested with Chromebooks, or any ARM / MIPS based device. I'd be interested to know if this is working !

## How to run

The easiest way to see the game running in your browser is to go to the following URL using latest version Chrome: 

http://arxpnacl.cad-labs.fr

A static website is hosted here, with a main HTML page embedding the PNaCl module ready to run, as well as some Javascript utilities providing several methods to access game data.

An alternative way to test this port is to run yourself a local http server, serving this same website. 

To do this, you have to download the preview release from github ('arx-pnacl-preview-NNN.zip'), uncompress it, and execute the 'run.sh' script. It will start a simple http server, serving the same content as the hosted one (the main HTML page, PNaCl module, and JS scripts). In this case, you have to go to the following URL:

http://localhost:5100

Note: source code for this static website and its utilities are available in git repository too. See section 'How To Build' for more information.

#### Access to game data

In all cases, you will need the game data for anything to work !

Technically, it could be provided directly by the hosted server and accessed through HTTP. However, as the game data is licensed under a commercial license, it is not conceivable for me to do so.

So, you have to provide the game data yourself, and use custom functionality of the static website to allow the Native Client module having access to it. 

Here is how to do:

* Get an original copy of the game.

If you don't already own an original copy of the game, it can be bought for a reasonable price from well known digital stores (GOG, Steam, etc.).
If you like immersive RPGs and have ~30 hours available, don't hesitate: it is definitively an enjoyable game (and really flawed too... but this is another story).

Note there is a Demo version too. For the purpose of testing this port, it is a reasonable choice.

More infos here: http://wiki.arx-libertatis.org/Getting_the_game_data

* Extract game content

Once you have the game, you will need to extract its content: the __*.pak__ files. 

Depending on where you got the game (original CD, digital download, etc.), the method may be different.
More infos here: http://wiki.arx-libertatis.org/Installing_the_game_data_under_Linux

* Make the Native Client module access this content

There is two means to do this: either locally through HTML5 filesystem feature of Chrome, or remotely through HTTP.

If you are running the game from the web, using the URL provided at begining of this section, you have to use the first option.

* 
    * Option 1: Using the HTML5 filesystemProject is compiling and linking either using the PNACL toolchain (clang with newlib), the x86/newlib toolchain (gcc with newlib), and the x86/glibc toolchain (allowing usage of dynamic libraries).

Toolchain files are provided for each platform.

The process is straightforward: use the "Select Files" button in the main web page, and select all the *.pak files. They will be copied to a local HTML5 filesystem from which the PNaCl module will be able to read. 

Note this option will really duplicate game data to somewhere in Chrome cache, as there can't be direct access to local filesystem.

So, once all data is copied (be sure to wait for all selected files to complete), simply click on "Launch Arx" button... and here you go: Arx Fatalis is running in your browser!

Caution: it will take quite some time to load the first time, because PNaCl module is almost 10MB size. This is quite a lot for Portable Native Client.

* 
     * Option 2: Access through HTTP

This method require to run the game from a local server, mostly using the preview release package.

In this case, copy all the *.pak files under a new 'arx' subfolder of the 'www-root' folder.

Upon access to localhost in the Browser, the game will be loaded directly.

## How to Build

Project is compiling and linking either using the PNACL toolchain (clang with newlib), the x86/newlib toolchain (gcc with newlib), and the x86/glibc toolchain (allowing usage of dynamic libraries).

Toolchain files are provided for each platform.

## Technical Details

This port uses Chrome Pepper API as backends (windowing, rendering, input, audio, filesystem, etc), mostly through existing ports of the project dependencies to Native Client, and other compatibility/emulation libraries.

Most of project dependencies are coming directly from the Naclports project (http://code.google.com/p/naclports/wiki/PortList), which turned out to be really handy:

* Zlib
* Freetype
* Boost
* SDL2 (windowing and input backend)
* OpenAL (sound backend)

OpenGL rendering backend is done using the Regal library - also in Naclports project - allowing to emulate 'classic' OpenGL on top of OpenGL ES 2. However, note that GLEW is disabled in this port, so no OpenGL extensions are used at all. What you are seeing on screen is really legacy V1.x OpenGL ...

POSIX filesystem interface on top of HTML5 or HTTP is done using the nacl_io library, provided by NACL SDK directly.

Finally, the GLM header-only library is stored directly as a git submodule. This is convenient as it allow the build system to require only NACL SDK and Naclports to be installed.

Even though a lot of dependencies were already ported, porting of the game itself required a wide variety of changes either in build system, source code, and support files. Most notably:

* Major updates to the CMake build system, to support cross-compilation to NACL and PNACL toolchains (llvm compiler, newlib) using NACL SDK and Naclports. See 'How To Build' for more information on this.
* Specific initialization for backends, and misc updates for them to work correctly (for example, disabling usage of GLEW).
* Filesystem backend changes to access game data from HTTP or HTML5
* Fix several compilation error, or warning issues
* Sample website and JS scripts to host the game, and provide access to game data

Finally, I had a very time consuming bug-hunting session to figure out why things were randomly crashing NACL with newlib or PNACL newlib but not NACL using with glibc. 
Quite honestly, I almost given up on this problem at several times (and by the way, PNaCl debugging is somewhat a pain to do). Eventually, I finally found the cause of the problem, and fixed it. But, given what I had to fix, there can be no other explanation than a compiler bug... 


### Limitations

Here are the most visible limitations:

* Input handling

  There is an underlaying problem with mouse input handling, that prevent real gameplay. In short, mouselook do not work.
  This is due to some understandable limitation of Pepper API regarding mouse handling: it is not possible to enforce its position from the application code, and Arx expect it to be able to do so. 

  Note this limitation is something common on other platforms. Simply, Arx Libertatis do not implement this mode of operation, and implementing it require support from input backend (SDL2) which itself is missing a couple of things on NACL.

  As a workaround, it is possible to use keyboard to navigate in the world, but this is definitively not the way this game is inteded to be played :)

* Graphics

  No Fullscreen support.

  OpenGL support is far from perfect, with occasional performance drops and visual glitches. 
  The Regal compatibility library used as backend (to emulate 'classic' OpenGL on top of OpenGL ES 2) is still in development, and for some reason, I had to disable GLEW support (and so most OpenGL extensions).
  
With some additional configuration, it is possible to make Regal library provide a GLEW emulation library too. However I had problem with it, and I decided to disable this support.

* Game Data

  Only content from .pak files is used by the game and not 'patch' folders. 
  This is mainly because only files can be selected in the browser file selection dialog, and not folders. I did not took time to implement some workaround.

  Also, there is a similar symptom when data is accessed over HTTP, but here with a different cause: the nacl_io library used to emulate filesystem access on top of HTTP can't reliably detect if an URL should be considered as a folder or not. As a result, only .pak files are used here too.
  
* Binary size & initial loading time
  
  Binary size is big (almost 10MB), making the first loading time quite long. 

  Everything have been compiled with optimization flags set for Size, and pnacl-compress command have been used. So it is not possible to do anything about that. 

  An area of improvement will be when PNaCl will support 'zero-cost' exceptions handling. Actually, exception handling increase binary size by roughly 15%. This C++ feature seems to be required by Arx Libertatis, or one of its dependencies.

## FAQ

### What is Native Client ?

Native Client (NaCl) is a sandboxing technology for running compiled C and C++ code at near-native speed in a secure sandbox. Portable Native Client (PNaCl) brings architecture independance to the compiled code.

For now, this technology is used almost exclusively by the Google Chrome browser to run native applications embedded in web pages. But its usage could go beyond the browser. 

For more information, see: https://developer.chrome.com/native-client

### Why this port ?

This port have been done mostly to learn Native Client stack (toolchain, API, runtime, etc.), and to evaluate if this technology is mature enough to support development of real moderately-sized C++ applications (and not only demonstration programs or benchmarks).

Arx Libertatis project have been choosen because it is an open source project that fit this latter requirement, with added bonus to have most of its dependencies already ported to Native Client. 

Also, it is a game I enjoyed a while back..

### Mouse look is unusable! 

Yes, this is a known limitation. See section 'Limitations / Input Handling'

## Credits

Arkane Studio, for creating this enjoyable-yet-flawed game, and released the game code under an open source license.

Arx Libertatis team, for its impressive amount of work to enhance it.

The Native Client team, for creating this nice technology.

## Contact
dev at gabrielcuvillier dot fr
