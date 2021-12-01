![Logo](resources/textures/title_text.png)

***

Classic Games is a compilation of retro-based games, developed in C++ with the SFML library.

It is an open-source project I have started after reading the ["SFML Game Development" book](https://www.packtpub.com/product/sfml-game-development/9781849696845).\
On top of the [source code](https://github.com/SFML/SFML-Game-Development-Book) accompanying the book, additional features has been added to fit my needs.

## Download

You can download the prebuilt binaries for Windows and Linux (x64) [here](https://github.com/razorbeard/classic-games/releases).

**Note for Linux users** :
Make sure you have installed SFML librairies, or else you will be not able to launch the executable.


## Building

### Windows (Visual Studio 2019)

You need at least `Python 3.6` to install the prerequisites for this project. Also, make sure Python is added to your PATH environment variable.

1. In the scripts folder, launch the script file `setup_project.bat` and follow the instructions.
2. Once done, launch the script file `generate_project.bat` to generate Visual Studio project files.
3. In the root directory, a solution file (.sln) should be created. Launch it to start building the application.

### Linux

1. In the scripts folder, launch the script file `setup_project.sh` and follow the instructions.

2. Once done, launch the script file `generate_project.sh` to generate a Makefile for this project.

3. You can start building the application by typing the following command in the terminal : `make config=release`.\
Debug configuration is the default value, but it isn't relevant for Linux. So you must add the release config flag to the make tool.

## Credits

Resources used for making this compilation are provided by various artists :

<details>
  <summary>Music</summary>
  
  * [Cunning plan](https://opengameart.org/content/cunning-plan-short-seamless-loop) (menu_theme.ogg) by congusbongus.
  * [On The Offensive](https://opengameart.org/content/8-bit-theme-on-the-offensive) (arkanoid_theme.ogg) by Wolfgang.
  * [Stage 1](https://opengameart.org/content/4-chiptunes-adventure) (tetris_theme.ogg) from Chiptune Adventures, by Juhani Junkala.
</details>

<details>
  <summary>Sounds</summary>
  
  * [win_jingle.wav and intro_jingle.wav](https://opengameart.org/content/nes-shooter-music-5-tracks-3-jingles) from NES Shooter Music, by SketchyLogic.
  * [start_pressed.wav](https://opengameart.org/content/nes-8-bit-sound-effects) from NES 8-bit sound effects, by shiru8bit.
  * [All others sounds](https://opengameart.org/content/512-sound-effects-8-bit-style) from 512 Sound Effects (8-bit style), by Juhani Junkala.
</details>

<details>
  <summary>Fonts</summary>
  
  * [sansation.ttf](https://www.dafont.com/fr/sansation.font) by Bernd Montag.
  * [upheavtt.ttf](https://www.dafont.com/upheaval.font) by Brian Kent.
  * [8_bit_arcade_in.ttf](https://www.dafont.com/8-bit-arcade.font) by Damien Gosset.
</details>

Don't hesitate to check their work !

All the textures are created by me using Aseprite, you are free to use them if you want.


## Contributing

I highly appreciate suggestions to help me improve the code readability.