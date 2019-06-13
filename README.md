# The Gravity Simulator

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)

## General info

### Simulator of gravitational interaction.

#### The program allows you to create objects that can represent astronomical object and see how they interact with each other in 2D space.


## Technologies

### Programming language: C

Used libraries: 
 <stdio.h>
 <math.h>
 <stdlib.h>
 <string.h>
 <SDL.h>

## Setup

#### Program needs confing file that consits:

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

```
{
  step: 0.1
  size: 4
  duration: 100
  trace: 1
  
  object: {,Sun,400,400,0,0,300000,90,255,255,0,255,}
  object: {,Earth,100,600,5,20,100,5,50,50,255,255,}
  object: {,Venus,700,150,20,5,400,6,50,255,50,255,} 
  object: {,Mars,200,700,15,0,500,5,255,50,50,255,} 
	
  
}
```

When simulation is finished program returns result file.

```
    
DURATION: 100s

No.||   Name   || Position || Velocity || Mass  || Collisions

1. || Sun      ||  20,20   ||   1,0    || 50000 || No
2. || Earth    ||  20,6    ||   50,0   ||  10   || Mars(in 51s)
3. || Jupiter  ||  26,30   ||   9,12   ||  16   || No
4. || Mars     ||  12,12   ||   25,78  ||  7    || Earth(in 51s)
```

Both config and example result file can be found in the repo.

![simulation_ex](https://github.com/kaczmarekmilosz94/JP-Projekt/blob/master/img.png)


## Project status: finished
