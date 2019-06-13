# The Gravity Simulator


## Simulator of gravitational interaction.

The program allows you to create objects that can represent astronomical object and see how they interact with each other in 2D space.

### Programming language: C

Used libraries: 
 <stdio.h>
 <math.h>
 <stdlib.h>
 <string.h>
 <SDL.h>

Program needs confing file that consits:

### Simulation setting:

	* time step
	* amount of objects
	* duration
	* orbit draw on/off
	* array of objects

### Every object has to be described with given values:
	* name
	* position X
	* position Y
	* velocity X
	* velocity Y
	* mass
	* radius
	* color in rgba

When simulation is finished program returns result file.

Both config and example result file can be found in the repo.




Project status: finished
