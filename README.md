# Screensaver
The application displays the front view of 3-D screensaver in which various number of balls are bouncing on the screen following the laws of physics.Remember it will look like a 2-D display however it is indeed 3-D more specifically a front view of 3-D display this argument is shown by fact that they don't collide each time with each other and seems to pass by becauses at that time they are at some distance from each other and their depth are different.

## General Features
- The position, speed and colour of each ball is initially random. 
- balls do not overlap intially. 
- The ball-ball collision and ball-wall collision is treated elastic.

##Features that  user can access using keys
-speed of the ball can be increased.
-screensaver can be paused anytime.
-screensaver can be resumed anytime.
-size of the ball can be increaed and decreased.



## Usage Instuctions

To run the main program with deafult number of balls i.e 15, run:
make

``
To run the main program with N number of balls, run:
make execute NUM_THREADS = N
``
where N is the number of balls.




Inside the GUI window, N number of bouncing balls will be seen on the screen. 4 buttons are included for various uses.

-	 KEY 'P': Pressing this key once will pause the movement of all the balls. Pressing it again will resume their movement.

-	KEY 'Q': This button is used to increase the speed of all balls. 

-	KEY 'I': This button increases the radius of each ball.   

-	KEY 'J': This button decreases the radius of each ball. 

-	
