![Logo](resources/textures/title_text.png)

***

Classic Games is a compilation of retro-based games, developed in C++ with the SFML library.

It is an open-source project I have started after reading the ["SFML Game Development" book](https://www.packtpub.com/product/sfml-game-development/9781849696845).\
On top of the [source code](https://github.com/SFML/SFML-Game-Development-Book) accompanying the book, additional features will be added to fit my needs.

**NOTE:** *The project is currently in an (very) early stage of development, so you may experience various bugs. I freshly dive into game development, 
so my main goal for now is to experiment and learn different kind of implementations. It helps me understand their advantages and drawbacks.*

## Download

You can download the prebuilt binaries for Windows and Linux (x64) [here](https://github.com/razorbeard/classic-games/releases).


## Building

### Windows (Visual Studio 2019)

You need at least `Python 3.6` to install the prerequisites for this project. Also, make sure Python is added to your PATH environment variable.

1. In the scripts folder, launch the script file `setup_project.bat` and follow the instructions.
2. Once done, launch the script file `generate_project.bat` to generate Visual Studio project files.
3. In the root directory, a solution file (.sln) should be created. Launch it to start building the application.

### Linux

1. Depending on your Linux distribution, install the correct packages for developing applications that use SFML :

``` 
sudo apt-get install libsfml-dev     (Debian, Ubuntu)
sudo dnf install SFML-devel          (Fedora, CentOS, Solus)
sudo pacman -Syu sfml                (Arch Linux)
```

2. Then, download the last release of Premake [here](https://github.com/premake/premake-core/releases/download/v5.0.0-alpha16/premake-5.0.0-alpha16-linux.tar.gz). 
Place the executable to the root folder, and generate the Makefile by typing the following command in the terminal : `./premake5 gmake2`

3. You can start building the application by typing the following command in the terminal : `make config=release`.\
Debug configuration is the default value, but it isn't relevant for Linux. So you must add the release config flag to the make tool.

These steps will be scripted in the near future, like Windows, to make it more simple for the user (and avoid premake5 to be commited).

## Credits

Resources used for making this compilation are provided by various artists :

<details>
  <summary>Music</summary>
  
  * [Cunning plan](https://opengameart.org/content/cunning-plan-short-seamless-loop) (menu_theme.ogg) by congusbongus.
  * [On The Offensive](https://opengameart.org/content/8-bit-theme-on-the-offensive) (arkanoid_theme.ogg) by Wolfgang.
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