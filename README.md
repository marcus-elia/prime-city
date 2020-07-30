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
greater than 99, the leading digit disappears).