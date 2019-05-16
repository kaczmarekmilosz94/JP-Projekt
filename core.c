#include <stdafx.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

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

typedef struct Planet {
	char name[10];
	position position;
	velocity velocity;
	double mass;
}planet;

static double G = 6.6726 * pow(10, -1);

static double dt = 0.01f;

const int map_size_x = 50;
const int map_size_y = 100;

char map[50][100];

double t = 0;

/*
void Verlet(double t, double dt, double r, double r2, double a)
{
if (t < 20)
{
double r3 = r2;

r2 = r;

r = 2 * r2 - r3 + a * dt * dt;


t = t + dt;

printf("t: %lf   r: %lf\n", t, r);

Verlet(t, dt, r, r2, a);
}
}
void Euler(double t, double dt, double r, double r2, double a, double v)
{
v = v + a * dt ;
r = r + v * dt;

t = t + dt;

if (t < 2000*dt)
{
r2 = r;

printf("t: %lf   v: %lf  r: %lf   E\n", t, v, r);

Euler(t, dt, r, r2, a, v);
}
else
{
printf("t: %lf   r: %lf  E\n", t, r);

Verlet(t, dt, r, r2, a);
}
}
*/

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

			if (distance <= 1)
			{
				system("pause");
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

void PrintPosition(planet planets[10], int size)
{
	system("@cls||clear");

	int i, j;

	printf("TIME: %0.2f \n", t);

	for (i = 0; i < size; i++)
	{
		printf("%s:    Mass: %0.1f    X: %f    Y: %f      Vx: %f    Vy: %f\n",
			planets[i].name, planets[i].mass, planets[i].position.x, planets[i].position.y, planets[i].velocity.x, planets[i].velocity.y);
	}

	printf("\n");



	for (i = 0; i < size; i++)
	{
		map[(int)planets[i].position.y][2 * (int)planets[i].position.x] = 'O';

		map[(int)planets[i].position.y + 1][(2 * (int)planets[i].position.x) + 2] = planets[i].name[0];
		map[(int)planets[i].position.y + 1][(2 * (int)planets[i].position.x) + 3] = planets[i].name[1];
		map[(int)planets[i].position.y + 1][(2 * (int)planets[i].position.x) + 4] = planets[i].name[2];
		map[(int)planets[i].position.y + 1][(2 * (int)planets[i].position.x) + 5] = planets[i].name[3];
	}

	for (i = 0; i < map_size_x; i++)
	{
		for (j = 0; j < map_size_y; j++)
		{
			printf("%c", map[i][j]);
		}
		printf("\n");
	}

	for (i = 0; i < size; i++)
	{
		map[(int)planets[i].position.y][2 * (int)planets[i].position.x] = ' ';

		map[(int)planets[i].position.y + 1][(2 * (int)planets[i].position.x) + 2] = ' ';
		map[(int)planets[i].position.y + 1][(2 * (int)planets[i].position.x) + 3] = ' ';
		map[(int)planets[i].position.y + 1][(2 * (int)planets[i].position.x) + 4] = ' ';
		map[(int)planets[i].position.y + 1][(2 * (int)planets[i].position.x) + 5] = ' ';
	}
}

planet* SetMovement(planet *planets, int size)
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

	PrintPosition(planets, size);

	t += dt;

	//Sleep(1);

	return planets;
}


void Controller(planet* planets, int size)
{
	while (!_kbhit())
	{
		planets = SetMovement(planets, size);
	}
}



int main()
{
	printf("Start\n");

	for (int i = 0; i < map_size_x; i++)
	{
		for (int j = 0; j < map_size_y; j++)
		{
			map[i][j] = ' ';
		}
	}

	planet planet1 = { "Sun ",{ 20,20 },{ 0,0 }, 50000 };
	planet planet2 = { "Earth",{ 20,12 },{ 50,0 }, 10 };
	planet planet3 = { "Venus",{ 18,18 },{ 0,0 }, 50000 };
	planet planet4 = { "Merc ",{ 10,6 },{ 30,0 }, 10 };

	int size = 2;


	planet planets[10] = { planet3, planet4 };

	Controller(planets, size);

	return 0;
}