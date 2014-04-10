void main( void )
{
   // get target coordinates from user

   // initialize

   for ( ; ; )
   {
      // DetermineLocation
      // DetermineClosestTarget
      // PursueTarget
      // ScanBarcode
      
      // InterruptDriven
      // AddObstacle 
   }
} 

/*

Save the stack right before entering the infinite loop so that an interrupt can throw
the IP right back to the top of the loop

*/


/*   

MoveToTarget()

INPUT: TARGET
OUTPUT: MOVE ROVER TO TARGET

1. fetch turn by turn direction

2. enable obstacle detection periodic interrupts

3. execute turn by turn direction with motor stabilization or compass

4. disable obstacle detection periodic interrupts

5. if more turn by turn directions exist go back to 1

If obstacle is detected exit function and set flag to DETECT_OBSTACLE

*/

/*

DetectObstacle()

INPUT: 
OUTPUT: SIZE THE OBSTACLE AND ADD IT TO THE DATABASE

*/



/*

UpdateTheMap()

INPUT:
OUTPUT: ADD OBSTACLES, ADD NODES AROUND OBSTACLES , AND UPDATE CONNECTIONS IN MAP
