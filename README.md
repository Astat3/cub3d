# cub3D

This project is inspired by the world-famous Wolfenstein 3D game, which was the first FPS ever. It will enable you to explore ray-casting. Our goal will be to make a dynamic view inside a maze, in which you'll have to find your way.

## Authors

- agallot
- roazouan

## Build

To compile the project, run:
```sh
make
```

## Usage

```sh
./cub3D [path_to_map.cub]
```

## Project Requirements
* You must use `miniLibX`. Either the version that is available on the operating system, or from its sources. If you choose to work with the sources, you will need to apply the same rules for your `libft` as those written above in Common Instructions part.
* The management of your window must remain smooth: changing to another window, minimizing, etc.
* Display different wall textures (the choice is yours) that vary depending on which side the wall is facing (North, South, East, West).
* Your program must be able to set the floor and ceiling colors to two different ones.
* The program displays the image in a window and respects the following rules:
  * The left and right arrow keys of the keyboard must allow you to look left and right in the maze.
  * The W, A, S, and D keys must allow you to move the point of view through the maze.
  * Pressing ESC must close the window and quit the program cleanly.
  * Clicking on the red cross on the window's frame must close the window and quit the program cleanly.

## AI Use

We used Gemini to get some docs about the parsing and the raycast, and writing the readme.
