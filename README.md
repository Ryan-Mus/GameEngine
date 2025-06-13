## Controls for msPacman
DPAD for movement/menus
A to confirm

WASD for movement/menus
Enter for confirm

F1 to skip a level 
F2 to mute.

## Game Engine Fundamentals
This game engine is made for 2D games. 
### GameObjects
gameobjects can be linked with eachother with the parent-child relation.
Gameobjects have a worldspace and localspace.
### Components
gameobjects can have custom components.

List of provided components:
  - ButtonComponent
  - ButtonManagerComponent
  - FpsComponent
  - SpriteSheetAnimatorComponent
  - TextComponent
  - TextureComponent

### Service Locator
Game engine has service locator where currently can get the soundservice

### Commands
The engine can make use of commands to execute certain code. Is used for button presses and for input. 

### Input Manager
Keyboard and controller are supported to handle input. Execute custom commands depending on keystate of keyboard or controller. Input handled by xinput.

### Scene Manager
Make different scenes using the scene manager and easily switch scenes.

### Events
Subject and observer class can be used to make use of events.

### Time Singleton
Acces the deltatime anywhere by using the engine time singleton.

## Game Engine special Functionalities
### Sound Manager
The sound manager class can multithread any soundservice.
SDL sound service is provided by the engine.
### SpriteSheet Animator
Easily add animations to a gameobject with a texture. 
All animations should be on the same texture file.
### Button and ButtonManager
Easily make menus for keyboard and controller with the use of buttons and the buttons manager to navigate all the buttons>



# Use

Either download the latest release of this project and compile/run in visual studio or, since students need to have their work on github too, they can use this repository as a template (see the "Use this template" button at the top right corner). There is no point in forking this project.
