            _______________________
           /\                      \
           \_|         Arx         |
             |      Libertatis     |
             |   __(PNaCl port)____|__
              \_/____________________/

This is an experimental port of Arx Libertatis to Portable Native Client architecture (PNaCl) and Pepper API (PPAPI), allowing to run the Arx Fatalis game in a native sandbox of Google Chrome browser.

## Contents

* [Port Status](#port-status)
* [How To Run](#how-to-run)
* [How To Build](#how-to-build)
* [Technical Details](#technical-details)
* [FAQ](#faq)
    * [What is Native Client?](#what-is-native-client?)
    * [Why this port?](#why-this-port?)
    * [What's next?](#what-s-next?)
* [Credits](#credits)
* [Contact](#contact)

## Port Status

**Working !** 

... although not yet fully playable due to one remaining technical limitation.

### What is working

The game runs smoothly in Chrome browser, embedded as a PNaCl module in a HTML page. It can be loaded directly from the Web, simply by loading an URL in the browser. There is no need to install a Chrome App, or use the Chrome Web Store. 

Almost all of the game subsystems are working flawlessly (rendering, audio, game logic, menus, save/load, etc.), and it is definitively possible to play through the game. See section [How To Run](#how-to-run) if you want to give it a try.

Please, note that even though Arx Libertatis source code and binary are freely usable, the original game data is required to play. As this data is licensed under a commercial license, you will have to provide it using your own original copy of the game, or get it from the available game demonstration. 

See section [Access to game data](#access-to-game-data) for more information on this topic.

The port have been tested with latest version of Chrome for Windows and Linux (starting from version 41), both on x86 and x86_64 architectures. I did not test with Chromebooks, or other ARM / MIPS based device. I'd be interested to know if this is working !

### What is NOT (yet) working

There is still one technical limitation that prevent 'real' gameplay for now: the _mouselook_ functionality does not work correctly, making navigation in the game 3D world quite difficult.

Keyboard navigation may still be used as a workaround, even though this is not practical for this game. Note that in-game menu/inventory mode works as expected, as well as the specific mouse-based magic gameplay.

This limitation is due to a missing feature in the Native Client backend implementation of the SDL2 library: the mouse-locking mechanism provided by Pepper API is not handled by SDL. So, with a few more work it would be possible to overcome this issue. If someone is interested to take a look at this, I can provide additional information.

There is other minor issues, most of them are documented in the [Implementation notes](#implementation-notes) section.

## How to run

### Prerequisites

The latest stable version of Google Chrome Browser is required, which can be found here:

http://www.google.com/chrome/

Chrome is supported on Windows, Linux, and Mac OS X, on Intel x86 32-bit and 64-bit architectures. 

A few remarks:

- Although Chrome is supported on Android and iOS (both on ARM architecture), Native Client is not supported on these mobile operating systems for now.

- For an unknown reason, the open-source browser "Chromium" provided by Debian/Ubuntu distributions does not come with Native Client support by default. For now, you have to use Google Chrome instead (or build Chromium from sources yourself).

### Running the Game in the browser

The easiest way to see the game running in your browser is simply to go to the following URL using latest version Chrome: 

http://arxpnacl.cad-labs.fr

A static website is hosted here, with a main HTML page embedding the PNaCl module ready to run, and some Javascript code to provide browser-side access to game data.

#### Alternative methods

An alternative method to test this port is to run yourself a local HTTP server, delivering the same sample website. 

To do this, download a preview release from github: _arx-pnacl-preview-NNN.zip_, uncompress it, and execute the _run.sh_ script. 

It will start a simple HTTP server, configured to give access to the website content (located in '_www-root_' subfolder). In this case, the following URL have to be used:

http://localhost:5100

Finally, you may also build the Native Client module yourself, and host it in a similar way. The source code of the sample website is fully available in git repository. See section 'How To Build' for more information. 

### Access to game data

In all cases, you will need the game data for anything to work !

This game data can be hosted on a local or remote server and accessed through HTTP, or provided directly by the user from its own local filesystem. This latter option uses the HTML5 Filesystem feature of Chrome, which is also used to store configuration files and saves locally.

Obviously, I do *not* provide any game data, as the game is licensed under a commercial license. 

So you have to provide the game data yourself, and use browser-side functionality of the website to allow the Native Client module having access to it. 

Here is how to do:

* Get your original copy of the game, or get the game demo.

  If you don't already own an original copy of the game, it can be bought for a reasonable price from well known digital stores (GOG, Steam, etc.).
  If you like immersive RPGs and have ~30 hours available, don't hesitate: it is an enjoyable game (and really flawed too... but this is another story).

  You may also use the game demo available from the publisher. For the purpose of testing this port, it is a reasonable choice. 

  More information can be found on the following page (including where to find the game demo):

  http://wiki.arx-libertatis.org/Getting_the_game_data

* Extract game content

  Once you have the game, you will need to extract its content: the _*.pak_ files. 

  If you are on Windows, the easiest way to get this data is to install the game locally, and locate the .pak files in the installation folder. But for other systems such as Linux, the method might be different. More information may be found here: 

  http://wiki.arx-libertatis.org/Installing_the_game_data_under_Linux

* Make the Native Client module access to the game data

  There is two ways to do this: either locally through HTML5 filesystem feature of Chrome, or remotely through HTTP.

  If you are running the game from the web, using the URL provided at beginning of this section, you have to use the first option.

  * Option 1: Using the HTML5 filesystem

  The process is straightforward: use the "Select Files" button in the main web page, and select all the _*.pak_ files. They will be copied to a HTML5 filesystem from which the Native Client module will be able to read. 

  Note this option will really duplicate game data to somewhere in Chrome cache, as Native Client modules ran from the Web can't have direct access to local filesystem.

  Once all data is copied (be sure to wait for all selected files to complete), simply click on "Launch Arx" button... and here you go: Arx Fatalis is running in your browser!

  * Option 2: Access through HTTP

  This method require to host the website yourself. See section [Alternative methods](#alternative-methods).

  In this case, copy all the _*.pak_ files under a new '_arx_' subfolder of the '_www-root_' folder.

  Upon access to localhost in the browser, the game will be loaded directly.

### A note on initial loading time

The first time the program is loaded, it will take quite some time to complete (with some visible "hang" around 95%). This is because PNaCl modules are compiled to native architecture from LLVM bytecode, and Arx Libertatis is almost 10MB in size. This is quite a lot.

## How to Build

Building consists in cross-compilation of Arx Libertatis and its dependencies to the Native Client / Pepper API platform, by using the _NaCl SDK_, the _Naclports_ project, and this fork of Arx Libertatis.

As usual with cross-compilation, the most difficult part is to correctly install and  setup the build environment, and then build the project dependencies.

If you feel brave enough to build the project yourself, detailed build instructions are located in the file [BUILD.PNACL.md](http://github.com/gabrielcuvillier/ArxLibertatis-pnacl/blob/master/BUILD.PNACL.md).

## Technical Details

Even though most of dependencies were already ported thanks to the Naclports project, porting of the game itself required a wide variety of changes. Most notably:

* Build system
    * Updates to the existing CMake build system, to support cross-compilation to NACL and PNACL toolchains using NACL SDK and Naclports.
* Game source code
    * Specific initialization for backends, and various updates for them to work correctly (for example, disabling usage of GLEW).
    * Filesystem backend changes to access game data from HTTP or HTML5
    * Fix several compilation issues, or other warnings
* Support files
    * Sample website and JS scripts to host the PNaCl module, and provide browser-side access to game data

### Implementation notes

If you are interested to know more about the implementation details of this port (including some minor limitations), please see the file [IMPLEMENTATION_NOTES.PNACL.md](http://github.com/gabrielcuvillier/ArxLibertatis-pnacl/blob/master/IMPLEMENTATION_NOTES.PNACL.md)

## FAQ

### What is Native Client ?

Native Client (NaCl) is a sandboxing technology for running compiled C and C++ code at native speed in a secure sandbox. Portable Native Client (PNaCl) allow the compiled code to be architecture independent.

For now, this technology is used almost exclusively by the Google Chrome browser to run native applications embedded in web pages. But its usage could go beyond the browser.

For more information, see: https://developer.chrome.com/native-client

### Why this port ?

This port have been done mostly to learn the Native Client stack (toolchain, API, runtime, etc.), and evaluate if this technology is mature enough to support development of real moderately-sized C++ applications (and not only demonstration programs or benchmarks).

Arx Libertatis have been chosen because it is an open source project that fit the C++ project size requirement, with added bonus to have most of its dependencies already ported to Native Client. Also, it is a game I enjoyed in the past.

This port also have been an opportunity to think about how Native Client could be used beyond the browser. A couple of other projects already have found interesting ways to use it, for example the "Unvanquished" project (as a secure sandbox for native game plugins) or "ZeroVM" (for server-side single process virtualization).

For my part, I do think that Native Client could be used as a standard target platform for many desktop applications, and in particular CAD softwares. But this is another story that have yet to be written...

### What's next ?

I consider having achieved my goals with this Native Client port: being able to compile/link using the NaCl SDK, play the game in Chrome without major crashes, and publish needed material so that anybody is able to do the same (source code, detailed information and build instructions in Readme files, a sample hosting website along with the precompiled game module).

There are still some work to do for a more enjoyable experience. I'd be glad to give necessary knowledge to someone willing to continue work on this port. 

The two most important things remaining to do are:

- Input: implement relative mouse mode in SDL using Pepper API mouse-lock interface, and use it in game world navigation.

- Data:  it would be really nice to be able to just drop the GOG executable or game demo directly in Chrome, and the program extract all the _*.pak_ files automatically.

OR

- Contact the game publisher and ask them to make the game demo data directly available on their website . It could then be fetched automatically by the module without user intervention. That would be awesome !


### 'Mouselook' is unusable!

Yes, this is _the_ most important limitation of this port. See section [Port Status](#port-status) and [Technical Details](#technical-details) for more information on this limitation, and how it could be solved.

## Credits

The Native Client team, for creating this interesting sandboxing technology.

Arkane Studios, for creating the Arx Fatalis game and released its source code under an open source license.

The Arx Libertatis team, for its impressive amount of work to enhance it.

## Contact
dev_at_gabrielcuvillier_dot_fr
