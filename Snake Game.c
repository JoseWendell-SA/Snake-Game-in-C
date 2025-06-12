#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int SnakeInitialSize = 3;
const int sizeX = 30, sizeY = 20;

typedef struct Segment
{
	int x;
	int y;
	
	struct Segment* next;
	struct Segment* previous;
}Segment;

Segment FRUIT;

typedef struct
{
	Segment* head;
	Segment* tail;
	
	int dirX;
	int dirY;
	
	int size;
}Snake;

void AddSegment(Snake* snake)
{
	Segment* newSegment = malloc(sizeof(Segment));
	newSegment->next = NULL;
	newSegment->previous = snake->tail;
	
	if (snake->tail != NULL)
		snake->tail->next = newSegment;
	else
		snake->head = newSegment;
	
	snake->tail = newSegment;
	snake->size += 1;
}

void InitializeSnake(Snake* snake)
{
	int n;
	
	for (n = 0 ; n < SnakeInitialSize ; n++)
	{
		AddSegment(snake);
		snake->tail->x = 5-n;
		snake->tail->y = 4;
	}
}

Snake* CreateSnake()
{
	Snake* snake = malloc(sizeof(Snake));
	snake->head = NULL;
	snake->tail = NULL;
	snake->size = 0;
	snake->dirX = 1;
	snake->dirY = 0;
	
	InitializeSnake(snake);
	
	return snake;
}

void SpawnFruit(char map[sizeY][sizeX])
{
	FRUIT.x = rand() % sizeX;
	FRUIT.y = rand() % sizeY;
	
	while (map[FRUIT.y][FRUIT.x] != ' ')
	{
		FRUIT.x = rand() % sizeX;
		FRUIT.y = rand() % sizeY;
	}
	
	map[FRUIT.y][FRUIT.x] = '*';
}

void MoveSnake(Snake* snake, char input, char map[sizeY][sizeX])
{
	Segment* aux = snake->head->next;
	
	int auxX, auxY;
	int auxX2 = snake->head->x, auxY2 = snake->head->y;
	
	if (input > 'Z')
	{
		input -= 32;
	}
	
	switch(input)
	{
		case 'W':
			if (snake->dirY == 1)
				goto YouCantDoThis;
			else
			{
				snake->dirY = -1;
				snake->dirX = 0;
				snake->head->y -= 1;
			}
			break;
		case 'A':
			if (snake->dirX == 1)
				goto YouCantDoThis;
			else
			{
				snake->dirY = 0;
				snake->dirX = -1;
				snake->head->x -= 1;
			}
			break;
		case 'S':
			if (snake->dirY == -1)
				goto YouCantDoThis;
			else
			{
				snake->dirY = 1;
				snake->dirX = 0;
				snake->head->y += 1;
			}
			break;
		case 'D':
			if (snake->dirX == -1)
				goto YouCantDoThis;
			else
			{
				snake->dirY = 0;
				snake->dirX = 1;
				snake->head->x += 1;
			}
			break;
	}
	
	if (((snake->head->x < 0 || snake->head->x >= sizeX) || (snake->head->y < 0 || snake->head->y >= sizeY)) || map[snake->head->y][snake->head->x] == 'O')
	{
		printf("Voce perdeu!\n");
		return;
	}

	while (aux != NULL)
	{	
		map[aux->y][aux->x] = ' ';
		
		auxX = aux->x;
		auxY = aux->y;

		aux->x = auxX2;
		aux->y = auxY2;
		
		map[aux->y][aux->x] = 'O';
		aux = aux->next;
		
		auxX2 = auxX;
		auxY2 = auxY;
	}
	
	if (map[snake->head->y][snake->head->x] == '*')
	{
		AddSegment(snake);
		snake->tail->x = auxX;
		snake->tail->y = auxY;
		
		map[snake->tail->y][snake->tail->x] = 'O';
		
		SpawnFruit(map);
	}
	
	map[snake->head->y][snake->head->x] = 'O';
	
	return;
	
	YouCantDoThis:
	
	printf("Nao pode ir na direcao oposta a atual!\n");
}

void ShowMap(char map[sizeY][sizeX])
{
	int x, y;
	
	for (y = -1 ; y <= sizeY ; y++)
	{
		for (x = -1 ; x <= sizeX ; x++)
		{
			if ((x == -1 || x == sizeX) || (y == -1 || y == sizeY))
				printf("#");
			else
				printf("%c", map[y][x]);
		}
		
		printf("\n");
	}
}

void main()
{
	srand(time(NULL));
	
	Snake* snake = CreateSnake();
	char map[sizeY][sizeX];
	
	int n, m;
	
	for (n = 0 ; n < sizeY ; n++)
	{
		for (m = 0 ; m < sizeX ; m++)
		{
			map[n][m] = ' ';
		}
	}
	
	Segment* aux = snake->head;
	
	for (n = 0 ; n < snake->size ; n++)
	{
		map[aux->y][aux->x] = 'O';
		aux = aux->next;
	}
	
	SpawnFruit(map);
	
	char input, enterCollector;
	
	while (1)
	{
		ShowMap(map);
		
		printf("\n\n\n W \nASD\n\nEscolha o proximo movimento: ");
		do
		{
			scanf("%c", &input);
		}while (input == '\n');
		
		scanf("%c", &enterCollector);
		
		MoveSnake(snake, input, map);
	}
	
}
