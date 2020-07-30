# prime-city
## Introduction
This is a first-person shooter made in C++ using OpenGL/Glut. It 
is based on a [2D Game](https://www.uvm.edu/~mselia/primewar.html) 
I made for my academic website. It takes place in a randomly-
generated city. The object is to earn points by shooting prime
numbers. 

## Sample Screenshots
![](/screenshots/buildings.png)
![](/screenshots/numbers.png)
![](/screenshots/AI.png)

## How To Play
Move around with the w,a,s,d keys, and press the spacebar to jump.
Press p to pause. Move the mouse to look around, and click with 
either button to shoot a missile. If you hit a prime number, you 
will gain that many points. If you hit a composite number, it will
explode and add itself to any nearby numbers (but if the result is
greater than 99, the leading digit disappears). The Computer is also
moving around and shooting at the numbers. The first to 1000 points
win. You and the Computer can also cause each other to lose 1 point
shooting their opponent.

## Build Instructions
If you are on windows, you need to download freeglut, make the
directory C:/Program Files/Common Files/freeglut/, copy the 
include/ and lib/ folders from freeglut into there, and copy
freeglut.dll from freeglut/bin/ into the cmake-build-debug/ folder.

If you are not on windows, maybe it will just work.