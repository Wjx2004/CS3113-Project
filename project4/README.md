# Bamboo Trial — Project 4

Build and run from this folder with `make run`. Raylib must be installed.

## Controls

- Enter: start from the menu.
- A/D or left/right arrows: move.
- Space: jump.
- 1/2/3: switch directly to a level for testing.
- R: return to the menu from either ending screen, restoring three lives.
- Q: quit.

## Game

Level 1 is the bamboo entrance with mushroom patrols. Level 2 is the mountain path with gaps and goblins that follow the player within their own sections. Level 3 is the ancient temple path with more gaps and flying eyes. Moving to the end of each level starts the next one. Enemy contact or falling off a map costs one life; lives carry between levels. The game uses separate Menu, You Lose, and You Win scenes. The player has idle, run, jump, and fall sprite animations. The game plays a looping music track and three sounds for starting, jumping, and taking damage.

`CS3113` uses the course's Week 9 Entity, Map, Scene, and support classes with project changes. The course repository is [CS3113 Introduction to Game Programming](https://github.com/sebastianromerocruz/CS3113-Intro-To-Game-Programming); its MIT notice is preserved in `CS3113/COURSE_LICENSE.txt`. Art and audio are new assets for this project; their sources are listed in [`assets/SOURCES.md`](assets/SOURCES.md).

