#Autonomous Rover Project
<<<<<<< HEAD

##The Prompt
This team project was done in fulfillment of the requirements for Loyola Marymount University's Electrical Engineering program. Joshua Arakaki, Phillip Thomas, and Kevin Refermat were contributors.

The task was to design and build an autonomous rover. The rover had to be able to make its own way to a series of barcodes located at random coordinates in the Electrical Engineering lab. It needed to scan the barcode at each pair of coordinates before making its way to the finish line. Upon completion, the rover had to perform a celebratory routine. At no point in the rover's journey was it allowed to touch anything (walls, lab stations, obstacles placed there by professors, etc.).

Apart from the above mentioned requirements, no further direction was provided.

![EE Lab](https://cloud.githubusercontent.com/assets/3867343/14502973/c4831cfa-0162-11e6-9d8d-908c2a0d0967.jpg)

##Architecture
There were several logical units of functionality that the rover needed to have in order to accomplish its goals. It needed to be able to:

1. locate itself (*positioning system*)
2. plot a route to its next target (*navigation system*)
3. physically go to a given target (*motor control system*)  
4. scan a barcode (*barcode reading system*)
5. detect and avoid obstacles (*obstacle avoidance system*)
6. perform a celebratory routine (*celebratory system*)

The functionality was broken down into 6 modules and the system was designed to keep all the modules as loosely coupled as possible.
=======
This team project was done in fulfillment of the requirements for Loyola Marymount University's Electrical Engineering program. Joshua Arakaki, Phillip Thomas, and Kevin Refermat were contributors.

The task was to design and build a rover capable of autonomously navigating to a series of randomly selected locations in the Electrical Engineering Lab while being able to avoid randomly placed obstacles.

![EE Lab](https://cloud.githubusercontent.com/assets/3867343/14502973/c4831cfa-0162-11e6-9d8d-908c2a0d0967.jpg)

Apart from the above mentioned requirements, no limitations were put on us. We were simply given the requirements and asked to design and build for them.

##Architecture
There were a several logical units of functionality that the rover had to do. It needed to:

1. locate itself (*positioning system*)
2. know how to get to its next target (*navigation system*)
3. physically go to targets (*motor control system*)  
4. scan a barcode at each target (*barcode reading system*)
5. detect and avoid obstacles (*obstacle avoidance system*)
6. perform a celebratory routine (*celebratory system*)

We broke the functionality into 6 modules and designed the system to keep all modules loosely coupled.
>>>>>>> 88177b273e59fd2ebed35e4447201196b48876e6

![System Modules](https://cloud.githubusercontent.com/assets/3867343/14503280/45352c5c-0164-11e6-9c5b-c0fd9ca44631.jpg)

###Positioning System
<<<<<<< HEAD
The **positioning system** was responsible for determining the rover's XY coordinates and its current bearing. It was arguably the most important module in the system and one of the most challenging to implement well.

####Design
It was designed much like the modern global position system using the principle of [*trilateration*](https://en.wikipedia.org/wiki/Trilateration) (this project's codebase incorrectly uses the term *triangulation* instead). Trilateration in a 2D plane is the process of determining the location of an object using the known distances from that object to three other objects with known positions.

It was known that a reasonably accurate and effective "indoor GPS" could be built if there was a reliable way to get the rover's distance to 3 known positions. GPS accomplishes this, more or less, by having GPS satellites broadcast their own positions in space as well as the current time. When the GPS device on the ground receives the transmissions, it compares the time that the satellites each sent the transmission with the time that the receiver received each of the transmissions. Given these time intervals, the coordinates of the transmitting satellites, and the fact that radio waves travel at the speed of light, the GPS receiver can calculate its distance from the known positions of each satellite. With this information, a trilateration agorithm can output the coordinates of the GPS device with reasonable accuracy.

Because radio waves travel so quickly and the microcontrollers on the rover and the beacons have a relatively slow clock speed, it would be too difficult to time the radio waves with enough precision to be useful. So, it was decided that sound waves would be used instead of radio waves because sound travels about a million times slower.

It is already common to use sound to calculate distances to things. To determine her/his distance from lightning, a person is advised to start counting seconds when the lightning is seen and then stop counting when it is heard. The distance in miles to the lightning is roughly the number of seconds counted divided by 5.

![Lightning](https://cloud.githubusercontent.com/assets/3867343/14502965/c46476c4-0162-11e6-9569-1cd441a1385b.jpg)

####Implementation

Each beacon was equipped with an RF receiver, an ultrasonic transmitter, a unique identifier, and a microcontroller to connect it all together. When a beacon received an RF transmission containing its unique identifier, it would transmit an ultrasonic pulse.
=======
The **positioning system** is responsible for determiningg the rover's XY coordinates and its current bearing. It is arguably the most important module in the system and one of the most challenging to implement well.

Taking our lead from our engineering forefathers, we decided to design our system using the main principle of GPS, [*trilateration*](https://en.wikipedia.org/wiki/Trilateration) (this project's code incorrectly uses the term *triangulation* instead). Trilateration in a 2D plane is the process of determining the location of an object using the known distances from that object to three other objects with known positions.

We knew we could build an accurate and effective "indoor GPS" if we could figure out how to get the rover's distance from 3 known positions. GPS accomplishes this by broadcasting its own position in space and the current time. When the GPS device on the ground receives the transmission it compares the time that the satellite sent the transmission with the time that the receiver received the transmission. Given this time interval and the fact that radio waves travel at the speed of light, the GPS receiver can calculate it's distance from the position included in the transmission. 

Because radio waves travel so quickly and the microcontrollers on the rover and beacons have a relatively slow clock speed, we knew that it would be too difficult to time the radio waves with enough precision. So, we decided to use sound waves instead. 

Sounds travels at ~340 m/s compared to the ~300,000,000 m/s that radio waves travel at. That's almost a million times slower. Most of us have used this exact principle. When we see lightning we start counting and when we hear it we stop counting. Divide the number of seconds by 5 and you have the approximate distance (in miles) to the lightning.

![Lightning](https://cloud.githubusercontent.com/assets/3867343/14502965/c46476c4-0162-11e6-9569-1cd441a1385b.jpg)

Each beacon has an RF receiver, an ultrasonic transmitter, a unique identifier, and a microcontroller to hold it all together. When a beacon receives an RF transmission containing its identifier, it transmits an ultrasonic pulse.
>>>>>>> 88177b273e59fd2ebed35e4447201196b48876e6

![Beacon](https://cloud.githubusercontent.com/assets/3867343/14502960/c4455848-0162-11e6-9ff8-19f9e461321c.jpg) 
![Transducer Array](https://cloud.githubusercontent.com/assets/3867343/14502970/c479ba20-0162-11e6-9004-376f309216d2.jpg)

<<<<<<< HEAD
On the rover's side of things, when the rover wanted to trilaterate its position, it would send an RF message out containing the identifier of the beacon that it wanted to measure its distance from and then it would start a timer. When the rover detected an ultrasonic pulse, the rover would stop the timer. The time on the timer was then multiplied by the speed of sound to get the distance.

![Positioning System](https://cloud.githubusercontent.com/assets/3867343/14511375/f073660c-018b-11e6-8bec-394e63d2b087.jpg)

Once the rover had the distance to three beacons, it calculated its position.

![Trilateration](https://cloud.githubusercontent.com/assets/3867343/14511389/064c05f6-018c-11e6-8960-989368850628.jpg)

####Results

The positioning system proved to be quite accurate with an average error of ~1 inch.


###Navigation System
The **navigation system** would take the rover's current position as input and would output turn by turn directions to the nearest target.

There were certain known obstacles in the rover's environment, namely walls and furniture. There could also be unknown obstacles, say a backpack on the floor. The navigation system organized the environment into a set of waypoints and obstacles. There was a numbered waypoint on the outside of each convex obstacle corner as seen below. This is an ASCII art manifestation of the rover's environmental model.

![ASCII Art Lab](https://cloud.githubusercontent.com/assets/3867343/14541382/9baba4d2-023e-11e6-9f25-971891f85107.png)

When a new obstacle was detected, the obstacle's position was calculated and added to the rover's model of the environment. 4 new waypoints were also added just outside the corners of the obstacle. The graph of the environment would then be updated, which involved reestablishing which waypoints were connected. Often times a discovered obstacle would block a previously existing connection and thus that connection disappears. The 4 new waypoints added around the obstacle, however, gave the rover a decent shot of being able to take approximately the same route.

Dijkstra's algorithm was employed to plot the shortest route to the next target. Had this path finding algorithm proved to be costly on our little microcontroller, we would have implemented the A* version but as Dijkstra's peer Knuth said, “Premature optimization is the root of all evil.”

###Motor Control System

The **motor control system** converts `rotate` or `moveForward` commands to digital signals that drive the motor drivers that drive the motors.


The challenging part of this system was to develop accurate feedback mechanisms so that the rover reliably rotated and moved forward the proper amount. Because the rover had two independent treads, the rover always rotated a bit differently depending on the slippage of each tread and the amount of power each tread was getting. Likewise, when moving the rover forward, depending on the battery's charge state, the rover's speed would vary. Thus simply timing the rover's forward motion was not a reliable way to travel a specific distance. Feedback mechanisms were needed.
=======
On the rover's side of things, when the rover wants to trilaterate its position, it sends an RF message out containing the identifier of the beacon that it wants to measure its distance and then it starts a timer. When the rover detects an ultrasonic pulse, the rover stops the timer. The time on the timer is then multiplied by the speed to get the distance.

![Positioning System](https://cloud.githubusercontent.com/assets/3867343/14511375/f073660c-018b-11e6-8bec-394e63d2b087.jpg)

Once the rover has the distance to three beacons, it calculates its position.

![Trilateration](https://cloud.githubusercontent.com/assets/3867343/14511389/064c05f6-018c-11e6-8960-989368850628.jpg)



###Navigation System
The **navigation system** takes the rover's current position as input and outputs turn by turn directions to the nearest target.

There are certain known obstacles in the rover's environment, namely walls and furniture. There could also be unknown obstacles, say a backpack on the floor. The navigation system organizes the environment into a set of waypoints. There is a numbered waypoint on the outside of each convex corner as seen below. This is an ASCII art print out to debug the rover's environment implementation.

![ASCII Art Lab](https://cloud.githubusercontent.com/assets/3867343/14541382/9baba4d2-023e-11e6-9f25-971891f85107.png)

When a new obstacle is detected, the obstacle's position is calculated and added to the rover's model of the environment. 4 new waypoints are also added just outside the corners of the obstacle. The graph of the environment is then updated. The rover reestablishes which waypoints should be connected. Often times a discovered obstacle would block a previously existing connection and thus that connection disappears. The 4 new waypoints added around the obstacle guarantee that the rover will still be able to go approximately the same way but be able to avoid the obstacle.

Dijkstra's algorithm was employed to plot the shortest route to the next target. Had this path finding algorithm proved to be expensive, we would have implemented the A* version as Dijkstra's peer Knuth said, “Premature optimization is the root of all evil.”

###Motor Control System

The **motor control system** converts commands to rotate so many degrees or move forward so many inches to signals that drive the motor drivers that drive the motors.


The challenging part of this system was to develop accurate feedback mechanisms so that the rover reliably rotates and moves forward the proper amount. Because the rover has two independent treads, the rover always rotates a bit differently depending on the slippage of each tread and the amount of power each tread is getting. Likewise, when moving the rover forward, depending on the battery's charge state, the rover's speed would vary. Thus simply timing the rover's forward motion was not a reliable way to travel a specific distance. Feedback mechanisms were needed.
>>>>>>> 88177b273e59fd2ebed35e4447201196b48876e6

![Motor Control System](https://cloud.githubusercontent.com/assets/3867343/14511374/f0731b98-018b-11e6-8bf5-d588a2b26feb.jpg)

####Forward Feedback

<<<<<<< HEAD
The rover's chassis we chose for our project came with quadrature encoders. These encoders allowed us to count the number of times the motor's shaft rotated. By experimentation, we were able to determine the conversion rate of shaft rotations to distance traveled, which proved to be a reliable mechanism for measuring forward movement accurately.

####Rotational Feedback

Rotational feedback was much trickier and at least as important as forward feedback. If the rover under or over rotated by several degrees and then traveled forward tens of feet, the error would be amplified. To mitigate this risk, a MEMS compass was used to get the rover's bearing. After attempting a rotation, the rover would check the compass to see if it had the expected bearing. If it did, it proceeded with the next command, if not, it would continue to rotate towards the desired bearing until it eventually got to within 1 degree. This ended up working exceptionally well as it would keep adjusting its bearing until it was just right.

###Obstacle Avoidance System

The **obstacle avoidance system** would constantly scan the area in front of the rover for potential obstacles.

![Obstacle Avoidance Scan](https://cloud.githubusercontent.com/assets/3867343/14543736/714fb870-024a-11e6-82ae-448021c6714a.gif)

An ultrasonic ranging unit mounted on a servo was used to accomplish this. The sensor would sweep in front of the rover and if it detected an obstacle within 2 feet, the rover would stop to avoid a collision. Then, the rover would turn its servo all the way to the left side and then slowly begin to sweep to the right until it found the left edge. Once it found the edge, it knew the obstacle's approximate location and it would add it to its map and updates all the waypoints' connections.

###Barcode Reading System

The faculty ruled that it was good enough to just get to the location of the barcodes without having to scan them so this system was never actually implemented.

###Celebratory Routine

The **celebratory routine** was predefined set of celebratory maneuvers the rover did in conjunction with the rover playing a prerecorded celebratory song.
=======
The rover chassis we chose for our project came with quadrature encoders. These encoders allowed us to count the number of times the motor's shaft rotated. By experimentation we were able to determine the conversion rate of shaft rotations to distance and this proved to be a reliable mechanism for measuring forward movement accurately.

####Rotational Feedback

Rotational feedback was much trickier and at least as important as forward feedback. If the rover under or over rotated by 30 degrees, the difference between midnight and one o'clock, and then traveled forward 20 feet, the error could get quite bad. To mitigate this risk, we used the MEMS compass to get feedback. After attempting a rotation, the rover would check the compass to see if it had the expected heading. If it did it proceeded with the next command, if not it would continue to rotate towards the desired heading until it eventually got to within 1 degree.

###Obstacle Avoidance System

The **obstacle avoidance system** constantly scans the area in front of the rover for potential obstacles.

![Obstacle Avoidance Scan](https://cloud.githubusercontent.com/assets/3867343/14543736/714fb870-024a-11e6-82ae-448021c6714a.gif)

We used and ultrasonic ranging unit mounted on a servo to accomplish. The sensor would sweep in front of the rover and if it detected an obstacle within 2 feet, the rover would stop to avoid a collision. Then, the rover turns its servo all the way to the left side and then slowly begins to sweep to the right. It is looking for the left edge. Once it finds the edge it knows the obstacle's approximate location and it adds it to its map and updates all the waypoints' connections.

###Celebratory Routine

The **celebratory routine** is predefined set of celebratory maneuvers the rover does in conjunction with the rover playing a prerecorded celebratory song.
>>>>>>> 88177b273e59fd2ebed35e4447201196b48876e6


##Photos

![](https://cloud.githubusercontent.com/assets/3867343/14502961/c457c5e6-0162-11e6-82b6-0ce205b13907.jpg)

All the bits and pieces: 5 beacons, 5 ultrasonic transducer arrays to be strategically placed upon the ceiling, and 1 rover.  

![](https://cloud.githubusercontent.com/assets/3867343/14502967/c46f2628-0162-11e6-99c2-f8c2aeedd190.jpg)

<<<<<<< HEAD
Our "loosely coupled" system. The original plan was to print up some circuit boards to clean everything up but this fell by the wayside as the deadline approached.
  
![](https://cloud.githubusercontent.com/assets/3867343/14502963/c461b06a-0162-11e6-8846-f5eb3b6a1561.jpg)

Pictured above, the rover is deep in thought about the upcoming demo day...


##Demo Day

Unfortunately, the author of this file cannot remember demo day in detail likely due to the high-stress moments immediately preceeding the demo. The author only remembers that the significant obstacle causing the stress was eventually cleared. From there on out the rover did its builders proud and hit most, if not all, of the given targets. The faculty were pleased with the project's success and the team received full marks.
=======
Our "Loosely coupled" system. Originally we wanted to be able print up some circuit boards to clean everything up but we fell victim to deadlines.  
  
![](https://cloud.githubusercontent.com/assets/3867343/14502963/c461b06a-0162-11e6-8846-f5eb3b6a1561.jpg)

Rover with the thousand yard stare after demo day...
>>>>>>> 88177b273e59fd2ebed35e4447201196b48876e6
