            _______________________
           /\                      \
           \_|      Arxwasm        |
             |                     |
             |  An Arx Libertatis  |
             |   WebAssembly port  |_
              \_/____________________/


__Arxwasm__ is an experimental port of the [Arx Libertatis](http://arx-libertatis.org/) project to [WebAssembly](https://emscripten.org/) 
using [Emscripten](https://emscripten.org/), allowing to run the [Arx Fatalis](https://en.wikipedia.org/wiki/Arx_Fatalis) 
video game (2002) in modern Web Browsers.

## Contents

* [Port Status](#port-status)
* [Online Demonstration](#online-demonstration)
* [Legal](#legal)
* [History and Future Directions](#history-and-future-directions)
* [Source Code](#source-code)
* [Author and Contact](#author-and-contact)
* [Disclaimer](#disclaimer)

## Port Status

The port is functional and working on all major Desktop Browsers: Firefox, Chrome, Safari, and Edge. So it is really 
possible to play the game. Mobile Browsers are working too, but the game is actually not playable due to lack of 
touch-based controls.

Performance is good, with around 40/60 FPS on a modern desktop system. Savegames are supported, and stored locally in 
Browser storage.

There is however a few minor visual glitches: some visual effects may appear to be missing compared to the original game. 
Nothing very dramatic though, this does not affect gameplay.

## Online Demonstration

The easiest way to see the game running in your browser is simply to go to the following URL:

__http://wasm.continuation-labs.com/arxdemo__

This is an online demonstration of the project using the __Arx Fatalis Demo__, that you can run directly in your 
browser and enjoy playing! It consists of a main HTML file embedding the Wasm module, and some Javascript code to 
automatically fetch the Arx Fatalis Demo data.

### Prerequisites

- Any modern browser, on any platform, providing it supports WebAssembly, WebGL, and browser storage is enabled (IndexedDB).

- A good CPU, and 350MB RAM.

- 150MB of game data will be downloaded, and cached to the IndexedDB browser storage. This is a one time download 
requirement only.

### Tidbits

- The “Esc” key so commonly used in old PC games have been replaced by the "Home" key, as the web is a very different 
world. This key is used to go to the main game menu, or to skip cinematics.
	
- Fullscreen mode is available using the "Fullscreen" button in the HTML page. Note that this will only rescale the 
canvas to match your fullscreen resolution, and not actually resize the canvas. You may change the video resolution 
(ie. the canvas size) directly from the game video settings. 

- It is possible to load/save games. Savegames are stored locally in Browser storage.

### Common issues

- IndexedDB storage will not work if you are in « Private Browsing ». You may disable private browsing safely, 
as I do not track anything from you. 

- Also, 150MB is somewhat huge for an IndexedDB store. To mitigate this, cached data is chunked into small parts of 
~50MB in the data store, but this is very empirical. It might fails on some browser configurations.

- As already mentioned, there is some missing visual effects (mostly on the magic side) and occasional visual glitches 
(random fog issues, and weird looking savegames screenshots). This is due to limitations of the underlying GL emulation 
layer used.

## Legal

The port itself is based on the GPL source code release of Arx Fatalis, and the online demonstration is redistributing 
the game data coming from the __Arx Fatalis Demo__ available for download freely on the Web ([Nordic Games](https://ds.thqnordic.com/arxfatalis/), 
[Fileplanet](https://www.fileplanet.com/archive/p-3356/Arx-Fatalis-Demo-English-Updated)). However the game Demo does 
not come with a specific license file like it is usually done.

So, to do my best to comply with legal requirements:

- The source code of the port is available to the public, as required by the GPL.
- I have contacted Zenimax Media (owners of Arkane Studios now) to ask for an explicit authorization of redistribution 
of the Demo. However, I never got any replies from them, which is unfortunate. At least, I tried to be fair and to get 
in touch with them. 

As it is just a Demo of a 20-years old video game, I think there is no harm by redistributing the data that way, 
in an "instant-runnable" form instead of the usual "download-install-run" form. However, as Zenimax is the IP 
holder of the Arx Fatalis franchise, they have all the rights reserved. They can ask me to remove access to the online 
demonstration for whatever reason. In such event, I'll simply remove the access. The port source code would remain 
public however, being covered by the GPL.

Anyway, if you enjoyed the Demo, you can buy the full commercial version of the game on various distribution platforms.
Don't hesitate if you enjoy old-school dungeon crawling PC games, it is a cool one (and somewhat buggy too, but that's 
another story). 

## History and Future Directions

The port is the continuation of an earlier experiment to port the game to [Portable Native Client](https://developer.chrome.com/native-client)
back in 2015, the [ArxLibertatis-pnacl](https://github.com/gabrielcuvillier/ArxLibertatis-pnacl) project. 
At that time, I wanted to know if it would be possible to run non-trivial C++ programs inside Web Browsers. After many 
hours of work on hobby time, I finally got the Native Client port to work, but it was limited to Chrome browser only 
and had some technical limitations. 

Eventually, Native Client have been deprecated by Google in favor of WebAssembly, and early 2018, that new technology 
have been standardized across browsers. So, by the end of 2018, I took opportunity to move this project to WebAssembly 
to continue my investigations on the topic of running native programs inside Web Browsers. The result of this effort is 
this __Arxwasm__ project.

There would be still several things to do on this port (fix the visual glitches, re-enable usage of VBOs, 
rework of the backend renderer to use WebGL), but instead I decided to move on to something much more challenging: 
to port the __Doom 3__ engine to WebAssembly and WebGL, in order to finally demonstrate that it is now possible to run 
some __very__ demanding C++ program inside Web Browsers.

This is the [D3wasm](http://continuation-labs.com/projects/d3wasm) project. Don't hesitate to check it out, as results 
are probably better than you think :) 

## Source Code

The source code is available on Github:

https://github.com/gabrielcuvillier/arxwasm

It is a fork of the [ArxLibertatis-pnacl](https://github.com/gabrielcuvillier/ArxLibertatis-pnacl) port, as explained in 
the [previous](#history-and-future-directions) section, which is itself a fork of [Arx Libertatis](http://arx-libertatis.org) source 
port maintained by the Arx Libertatis Team. In turn, Arx Libertatis is a fork of the Arx Fatalis GPL source code released 
by Arkane Studios a few years ago.

The project is using [Emscripten](https://emscripten.org). It allows to compile LLVM bytecode to WebAssembly, and 
provide all what is necessary to have a C/C++ runtime environment on the Web. 

The port is based on a personnal fork of the [Regal GL library](https://github.com/gabrielcuvillier/regal-emscripten-fixes), which 
have been updated to support Emscripten and WebAssembly, and released as an [Emscripten port](https://github.com/emscripten-ports/regal) 
for the occasion. This library is a GL 1.x emulation layer on top of OpenGL ES 2.0.

### How to build

Building consists in cross-compilation of Arxwasm and its dependencies to WebAssembly using Emscripten.

As usual with cross-compilation, the most difficult part is to correctly install and setup the build environment.

If you feel brave enough to build the port yourself, detailed build instructions are located in the file [BUILD.WASM.md] (__WIP__)

### Technical Details

There used to be some implementation notes in the former port to Portable Native Client, but they are deprecated now. 

If you are interested in the technical details involved in porting a game to WebAssembly, you should definitively 
take a look at the [D3wasm](http://continuation-labs.com/projects/d3wasm) project where I detailed the porting process. 
It is a similar process than this project, albeit much more difficult.

## Author and Contact

The author of this project is me, [Gabriel Cuvillier](http://gabrielcuvillier.pro), on behalf of my R&D Lab [Continuation Labs](http://www.continuation-labs.com).

This is an experimental research project, for the purpose of learning WebAssembly, demonstrating what can be done with 
that technology, and making the Web move forward.

You can reach me at _gabriel.cuvillier@continuation-labs.com_.

## Disclaimer

_Arxwasm, the provided online demonstration, me, and Continuation-Labs are in no way associated with or supported by 
[Arkane Studios](https://www.arkane-studios.com) or [ZeniMax Media Inc](https://www.zenimax.com)._

_Arx Fatalis, Arkane and related logos are registered trademarks or trademarks of ZeniMax Media Inc. in the U.S. and/or 
other countries. All Rights Reserved._

_The Arxwasm source code is a derivative work of the Arx Libertatis project, itself a derivative of the General Public 
License source code release of Arx Fatalis._

_WebAssembly is a standard being developed by the W3C._