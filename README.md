            _______________________
           /\                      \
           \_|         Arx         |
             |      Libertatis     |
             |   __(PNACL_port)____|__
              \_/____________________/

This is an experimental port of Arx Libertatis to Portable Native Client architecture (PNaCl), running on Google Chrome browser with Pepper API.

## Status

**Working!**    

... but with a few limitations, some of which preventing enjoyable gameplay (see section 'Limitations').

The game runs smoothly in Chrome browser, embedded as a PNaCl module in a HTML page. 

It can be loaded directly from the 'Open Web', simply by opening an URL in the browser. In other words, there is no need to install a Chrome App, or use the Chrome Web Store.

The game data is accessed either through HTTP, or a local HTML5 filesystem (which is also used for config files and saves).
Obviously, I do not provide any game data, as it is licensed under a commercial license.

The port have been tested with latest version of Chrome for Windows and Linux, both on x86 and x86_64 architectures.
I did not tested with Chromebooks, or any ARM / MIPS based device. I'd be interested to know if this is working !

## What is PNaCl?

Portable Native Client is a sandboxing technology for running compiled C and C++ code at near-native speed in a secure sandbox, while remaining architecture independant.

For now, this technology is used almost exclusively by the Google Chrome browser to run native applications embedded in web pages. But its usage could go beyond the browser. 

## How to run

The easier way to see the game running in your browser, is to go to the following URL using latest version Chrome: 

http://arxpnacl.cad-labs.fr

A basic website is hosted here, with the embedded PNaCl module ready to run.

**But**... Obviously you will need the game data for anything to work !
Technically, it could be provided directly by the server and accessed through HTTP (and as you may notice, the website is trying to do so). However, as the game data is licensed under a  commercial license, it is not conceivable to do so.

So, you have to provide the game data yourself, by uploading it to a local temporary HTML5 filesystem.

Here is how to do:

- Get an original copy of the game.

If you don't already own an original copy of the game, it can be bought for a cheap price from well known digital stores (GOG, Steam, ...).
If you like immersive RPGs, and have ~30 hours available, don't hesitate: despite its many flaws, it is definitively an enjoyable game.

Note there is a Demo version too. For the purpose of testing this port, it is a reasonable choice.

More infos here: http://wiki.arx-libertatis.org/Getting_the_game_data

- Extract game content

Once you have the game, you will need to extract its content: the *.pak files. 

Depending on where you got the game (original CD, digital download, etc.), the method is different.
More infos here: http://wiki.arx-libertatis.org/Installing_the_game_data_under_Linux

- Copy game content to HTML5 filesystem

Once you have the .pak files, simply copy them to the local HTML5 filesystem using the "Select Files" button.
Once all data is uploaded (be sure to wait for all selected files to complete), simply click on "Launch Arx" button...

And here you go... Arx Fatalis is running in your browser!

Note it will take quite some time to load the first time, because PNACL module is almost 10MB size. This is a lot of intermediate LLVM bytecode to compile to native code.

End note: the hosted website is fully available in the source distribution. See section 'How to build' for more informations.

## Limitations

* Input handling

  Even if things seems to work nicely, there is in fact an underlaying problem with mouse input handling, that prevent any real serious gameplay :(
  When in "immersive/FPS" mode, the mouse is not captured correctly. As so, it is quite difficult to move in the world (unless using the keyboard to look around). Note there is no problem when in "menu mode" (menus, inventory, magic). 

  This behavior is due to some understandable limitation of Pepper API regarding mouse capture: it is not possible to enforce the mouse position to 0,0. It is a common situation on a few platforms, but this is neither handled by Arx Libertatis nor by implementation of SDL2 on NACL. 

  Volunteers welcome on this topic. 

* Graphics

  No Fullscreen.
  
  Limited OpenGL support (only V1.5)

* Game Data
<... section to complete ...>

## How to Build

Caution: unless you are already comfortable with the PNaCl platform and its toolchain, this is still not a straightforward process.

<... section to complete ...>

## Port implementation details
See notes in PNACL_NOTES

## Credits

Arkane Studio, for creating this enjoyable-yet-flawed game, and released the game code under an open source license.

Arx Libertatis team, for its impressive amount of work to enhance it.

The Native Client team, for creating such a nice piece of technology.

## Contact
dev at gabrielcuvillier dot fr
