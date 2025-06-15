#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#define clear() clearW()
#else
#include "kbhitlinux.h"
#define clear() clearL()
#define kbhit() kbhitL()
#endif

const int SnakeInitialSize = 3;
const int maxSizeX = 50, maxSizeY = 50; // minimum is 10
int sizeX = 30, sizeY = 20; //Default map size config

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

void clearW()
{
	system("cls");
}

void clearL()
{
	system("clear");
}

Snake* CreateSnake()
{
	Snake* snake = malloc(sizeof(Snake));
	snake->head = NULL;
	snake->tail = NULL;
	snake->size = 0;
	snake->dirX = 1;
	snake->dirY = 0;
	
	return snake;
}

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
		snake->tail->x = sizeX/2-n;
		snake->tail->y = sizeY/2;
	}
}

void ResetSnake(Snake* snake)
{
	Segment* aux = snake->head;
	
	while (aux->next != NULL)
	{
		Segment* aux2 = aux;
		aux = aux->next;
		free(aux2);
	}
	
	free(aux);
	
	snake->head = NULL;
	snake->tail = NULL;
	snake->size = 0;
}

void ResetMap(Snake* snake, char map[maxSizeY][maxSizeX])
{
	int n, m;

	for (n = 0 ; n < sizeY ; n++)
	{
		for (m = 0 ; m < sizeX ; m++)
		{
			map[n][m] = ' ';
		}
	}
	
	if (snake->size > 0)
		ResetSnake(snake);
	InitializeSnake(snake);
	snake->dirX = 1;
	snake->dirY = 0;
	
	Segment* aux = snake->head;
	
	for (n = 0 ; n < snake->size ; n++)
	{
		map[aux->y][aux->x] = 'O';
		aux = aux->next;
	}
}

void SpawnFruit(char map[maxSizeY][maxSizeX])
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

int MoveSnake(Snake* snake, char map[maxSizeY][maxSizeX])
{
	Segment* aux = snake->head->next;
	
	int auxX, auxY;
	int auxX2 = snake->head->x, auxY2 = snake->head->y;
	
	snake->head->x += snake->dirX;
	snake->head->y += snake->dirY;
	
	if (((snake->head->x < 0 || snake->head->x >= sizeX) || (snake->head->y < 0 || snake->head->y >= sizeY)) || map[snake->head->y][snake->head->x] == 'O')
	{
		printf("\nVoce perdeu!\n");
		return 0;
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
		
		if (snake->size == sizeX*sizeY)
		{
			return 2;
		}
		
		SpawnFruit(map);
	}
	
	map[snake->head->y][snake->head->x] = 'O';
	
	return 1;
}

void ShowMap(char map[maxSizeY][maxSizeX])
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

void UpdateSnakeDirection(char input, Snake* snake)
{
    switch(input)
	{
		case 'w':
			if (snake->dirY == 0)
			{
				snake->dirY = -1;
				snake->dirX = 0;
			}
			break;
		case 'a':
			if (snake->dirX == 0)
			{
				snake->dirY = 0;
				snake->dirX = -1;
			}
			break;
		case 's':
			if (snake->dirY == 0)
			{
				snake->dirY = 1;
				snake->dirX = 0;
			}
			break;
		case 'd':
			if (snake->dirX == 0)
			{
				snake->dirY = 0;
				snake->dirX = 1;
			}
			break;
	}
}

void GetKeyDown(Snake* snake)
{
	char ch = kbhit();
    if (ch > 0)
    {
    	if (ch < 2)
    		ch = getch();
    	UpdateSnakeDirection(ch, snake);
    }
}

void main()
{
	srand(time(NULL));
	
	Snake* snake = CreateSnake();
	char map[maxSizeY][maxSizeX];
	
	int n, m;
	
	char Game = 1;
	char userWantsToPlay = 1;
	
	printf("Ola!\n");
	
	while (userWantsToPlay)
	{
		printf("Informe o tamanho do mapa que deseja:\nO tamanho maximo eh %dx%d e o minimo eh 10x10\nEscolha o tamanho x e y: ", maxSizeX, maxSizeY);
		scanf("%d %d", &sizeX, &sizeY);
		if (sizeX < 10) sizeX = 10;
		else if (sizeX > maxSizeX) sizeX = maxSizeX;
		if (sizeY < 10) sizeY = 10;
		else if (sizeY > maxSizeY) sizeY = maxSizeY;
		
		int timer = 5;
		printf("Muito bem!\nConfigurando mapa...");
		ResetMap(snake, map);
		SpawnFruit(map);
		sleep(1);
		printf(" Iniciando o jogo em:\n");
		while (timer > 0)
		{
			printf("%d ", timer);
			sleep(1);
			timer -= 1;
		}
		
		do
		{
			clear();
			ShowMap(map);
			usleep(150000);
			GetKeyDown(snake);
			Game = MoveSnake(snake, map);
		}while (Game == 1);
		
		if (Game == 2)
			printf("\n\nParabens! Voce venceu!\n\n");
		printf("\nPontuacao final: %d\n", snake->size - SnakeInitialSize);
	}
}
