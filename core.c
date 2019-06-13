#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <SDL.h>

#pragma warning(disable:4996)

#undef main

typedef struct Position {
	double x;
	double y;

}position;

typedef struct Velocity {
	double x;
	double y;

}velocity;

typedef struct Force {
	double x;
	double y;

}force;

typedef struct Color {
	int r;
	int g;
	int b;
	int a;

}color;

typedef struct Collision {
	char *name;
	double time;

}collision;

typedef struct Planet {
	char name[15];
	position position;
	velocity velocity;
	double mass;
	int radius;
	color color;
	collision collisions[10];
	int collisions_count;
}planet;


planet main_planets[10];

static double G = 0.6726;

static double dt = 0.1f;

const int map_size_x = 50;
const int map_size_y = 100;

char map[50][100];

double t = 0;
double duration = 10;

int PollEventsForQuit() {
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT: return 1;
		case SDL_KEYDOWN: if (e.key.keysym.sym == SDLK_ESCAPE) 	return 1;
			break;
		}
	}

	return 0;
}

void DrawCircle(SDL_Renderer * renderer, int a, int b, double r, color color)
{
	for (int i = -r; i < r; i += r / 5)
	{
		SDL_Rect srcrect;

		int c1 = (int)sqrt(r * r - pow(i, 2));

		int pointSize = 3;

		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

		srcrect.x = i + a;
		srcrect.y = c1 + b;
		srcrect.w = pointSize;
		srcrect.h = pointSize;

		SDL_RenderFillRect(renderer, &srcrect);

		srcrect.x = i + a;
		srcrect.y = -c1 + b;
		srcrect.w = pointSize;
		srcrect.h = pointSize;

		SDL_RenderFillRect(renderer, &srcrect);

		srcrect.y = i + b;
		srcrect.x = c1 + a;
		srcrect.w = pointSize;
		srcrect.h = pointSize;

		SDL_RenderFillRect(renderer, &srcrect);

		srcrect.y = i + b;
		srcrect.x = -c1 + a;
		srcrect.w = pointSize;
		srcrect.h = pointSize;

		SDL_RenderFillRect(renderer, &srcrect);

		SDL_RenderPresent(renderer);
	}
}


planet Euler(planet planet, force force)
{
	double ax = force.x / planet.mass;
	double ay = force.y / planet.mass;

	planet.velocity.x = planet.velocity.x + ax * dt;
	planet.velocity.y = planet.velocity.y + ay * dt;

	planet.position.x = planet.position.x + planet.velocity.x * dt;
	planet.position.y = planet.position.y + planet.velocity.y * dt;

	return planet;
}

force Gravity(planet planet1, planet planets[10], int size)
{
	force final_force = { 0,0 };

	for (int i = 0; i < size; i++)
	{
		if (strcmp(planet1.name, planets[i].name) != 0)
		{
			double x = planets[i].position.x - planet1.position.x;
			double y = planets[i].position.y - planet1.position.y;

			double distance = sqrt((
				pow(x, 2) +
				pow(y, 2)));

			if (distance <= (planet1.radius + planets[i].radius)) 
			{
				int j;

				for (j = 0; j < size; j++) 
				{
					if ((strcmp(planet1.name, main_planets[j].name) == 0)) 
					{
						if (main_planets[j].collisions_count == 0) 
						{
							collision col = { planets[i].name, t };
							main_planets[j].collisions[0] = col;
							main_planets[j].collisions_count++;
						}
						else
						{
							if (t - main_planets[j].collisions[main_planets[j].collisions_count - 1].time > 5) 
							{
								collision col = { planets[i].name, t };
								main_planets[j].collisions[main_planets[j].collisions_count] = col;
								main_planets[j].collisions_count++;
							}
						}						
						break;
					}
				}
			}


			double F = (G * planet1.mass * planets[i].mass) / pow(distance, 2);


			force force;

			force.x = (F * x) / distance;
			force.y = (F * y) / distance;

			final_force.x = final_force.x + force.x;
			final_force.y = final_force.y + force.y;
		}
	}

	return final_force;
}

void PrintPosition(planet planets[10], int size, SDL_Renderer * renderer, int tracer)
{
	system("@cls||clear");

	int i,j;

	printf("TIME: %0.2f \n", t);

	




	for (i = 0; i < size; i++)
	{
		printf("%s:    Mass: %0.1f    X: %f    Y: %f      Vx: %f    Vy: %f\n",
			planets[i].name, planets[i].mass, planets[i].position.x, planets[i].position.y, planets[i].velocity.x, planets[i].velocity.y);
	}

	printf("\n");


	color black = { 0,0,0,255 };

	if (tracer == 0) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
	}


	for (i = 0; i < size; i++)
	{
		DrawCircle(renderer, (int)planets[i].position.x, (int)planets[i].position.y, (int)planets[i].radius, planets[i].color);
	}

	if (t >= duration) 
	{
		FILE *fp; 

		if ((fp = fopen("result.txt", "w")) == NULL) 
		{			
			exit(1);
		}
		fprintf(fp, "DURATION: %f\n\n", duration); 
		fprintf(fp, "     Name     ||   Position   ||   Velocity   ||    Mass    ||    Collisions\n"); 

		for (i = 0; i < size; i++)
		{
			fprintf(fp, "   %s           %0.1f, %0.1f     %0.1f, %0.1f        %0.1f     %d",
				planets[i].name,
				planets[i].position.x, planets[i].position.y,
				planets[i].velocity.x, planets[i].velocity.y,
				planets[i].mass,
				planets[i].collisions_count);

			if (planets[i].collisions_count > 0)
			{
				int k;

				for (k = 0; k < planets[i].collisions_count; k++) 
				{
					fprintf(fp, " %s(in %0.2f) | ", planets[i].collisions[k].name, planets[i].collisions[k].time);
				}
				fprintf(fp, "\n");
			}
			else 
			{
				fprintf(fp, "\n");
			}
		}



		fclose(fp); 

		exit(0);
	}
}

planet* SetMovement(planet * planets, int size, SDL_Renderer * renderer, int tracer)
{
	planet current_planets[10];

	int i;

	for (i = 0; i < size; i++)
	{
		current_planets[i] = planets[i];
	}

	for (i = 0; i < size; i++)
	{
		planets[i] = Euler(planets[i], Gravity(planets[i], current_planets, size));
	}

	PrintPosition(planets, size, renderer, tracer);

	t += dt;


	return planets;
}


void Controller(planet * planets, int size, SDL_Renderer * renderer, int tracer)
{
	while (!_kbhit())
	{
		if (PollEventsForQuit()) break;

		planets = SetMovement(planets, size, renderer, tracer);
	}
}


planet GetPlanet(char setts[1024]) 
{
	char name[15];
	double x;
	double y;
	double vx;
	double vy;
	double mass;
	double radius;
	int r;
	int g;
	int b;
	int a;
	
	int i = 0;

	int init_size = strlen(setts);
	char delim[] = ",";

	char *ptr = strtok(setts, delim);

	while (ptr != NULL)
	{
		printf("'%s'\n", ptr);
		ptr = strtok(NULL, delim);

		if (i == 0)  strcpy(name, ptr);
		if (i == 1)  x = atof(ptr);
		if (i == 2)  y = atof(ptr);
		if (i == 3)  vx = atof(ptr);
		if (i == 4)  vy = atof(ptr);
		if (i == 5)  mass = atof(ptr);
		if (i == 6)  radius = atof(ptr);
		if (i == 7)  r = atoi(ptr);
		if (i == 8)  g = atoi(ptr);
		if (i == 9)  b = atoi(ptr);
		if (i == 10)  a = atoi(ptr);

		i++;
	}

	 
	position pos = { x,y };
	velocity vel = { vx,vy };
	color col = { r,g,b,a };

	planet _planet;
	strcpy(_planet.name, name);
	_planet.position = pos;
	_planet.velocity = vel;
	_planet.color = col;
	_planet.mass = mass;
	_planet.radius = radius;
	_planet.collisions_count = 0;

	return _planet;
}


int main()
{
	printf("Start\n");

	char ch, file_name[25];
	FILE *fp;

	int tracer = 1;
	int size = 2;

	int id = 0;


	printf("Enter name of a file you wish to see\n");
	gets(file_name);

	fp = fopen(file_name, "r"); // read mode

	if (fp == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}


	char x[1024];
	int key_id=0;
	/* assumes no word exceeds length of 1023 */
	while (fscanf(fp, " %1023s", x) == 1) 
	{
		switch (key_id)
		{
		case 1:
			printf("size: %s\n", x); 
			size = atoi(x);
			key_id = 0;
			break;
		case 2: 
			printf("duration: %s\n", x);
			duration = atof(x);
			key_id = 0;
			break;
		case 3: 
			printf("trace: %s\n", x);
			tracer = atoi(x);
			key_id = 0;
			break;
		case 4:
			printf("object: %s\n", x); 
			main_planets[id] = GetPlanet(x);
			id++;
			key_id = 0; 
			break;
		case 5:
			printf("step: %s\n", x);
			dt = atof(x);
			key_id = 0;
			break;
		default:
			break;
		}

		if (strcmp(x, "size:") == 0) key_id = 1;
		if (strcmp(x, "duration:") == 0) key_id = 2;
		if (strcmp(x, "trace:") == 0) key_id = 3;
		if (strcmp(x, "object:") == 0) key_id = 4;
		if (strcmp(x, "step:") == 0) key_id = 5;
	}
		

	fclose(fp);
	
	while (1 == 0) {}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
	return -1;
	}

	SDL_Window* win = SDL_CreateWindow("Space", 760, 340, 1000, 1000, SDL_WINDOW_SHOWN);

	if (win == NULL) {
	return -1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL) {
	SDL_DestroyWindow(win);
	SDL_Quit();

	return -1;
	}

	for (int i = 0; i < map_size_x; i++)
	{
	for (int j = 0; j < map_size_y; j++)
	{
	map[i][j] = ' ';
	}
	}



	while (1==1) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);


	Controller(main_planets, size, renderer, tracer, duration);

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
