# Flappy-Bird

A light version of the Flappy Bird game, 2D game implemented in OpenGL.

* The background of the game consists in: the ground (a brown screen sized 
rectangle) and clouds (6 blue clouds made from 6 circles each).
 
* The bird is created from 16 points (8 triangles) designed with different colours.
It only translates on OY axis. The bird flies by rotating and translating itself
gradually on pressing the SPACE key, otherwise it rotates down (by the gravity)
also gradually, not instantly.

* The obstacles are rectangles with fixed width (100), which translate themselves
on OX axis endlessly from right to left. When the OX position of one rectangle
arrives at the beginning of the screen (OX = 0), it translates itself backto the
end of the OX window resolution. Each height of the obstacles is randomly chosen at
the initialization between a specified range, so that the bird can fit between the
Up and Down rectangles.

* The collision with the rectangles ("CheckCollision" method) checks the closure of an
obstacle on OX and OY with the bird, taking into account the bird and the obstacle
position on both axis -> the bird falls.

The score is displayed on the terminal until the collision = GAME OVER.

