/* ========================================================================
   File: GridUtils.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "GridUtils.h"
using namespace std;

Node Node_Create(int x, int y, float weight)
{
	Node node = {};
	node.position = {(float)x, (float)y};
	node.weight = weight;
	return node;
}

void Node_Reset(Node* node)
{
	node->costSoFar = -1.0f;
	node->priority = 0.0f;
	node->heuristic = 0.0f;
	node->cameFrom = nullptr;
	node->alreadyUsed = false;
}

int Node_GetDirection(Node* from, Node* to)
{
	if (to->position.x < from->position.x && to->position.y == from->position.y)
	{
		return 2;
	}
	else if (to->position.x > from->position.x && to->position.y == from->position.y)
	{
		return 0;
	}
	else if (to->position.x == from->position.x && to->position.y < from->position.y)
	{
		return 3;
	}
	else if (to->position.x == from->position.x && to->position.y > from->position.y)
	{
		return 1;
	}
	else if (to->position.x > from->position.x && to->position.y > from->position.y)
	{
		return 4;
	}
	else if (to->position.x < from->position.x && to->position.y > from->position.y)
	{
		return 5;
	}
	else if (to->position.x < from->position.x && to->position.y < from->position.y)
	{
		return 6;
	}

	return 7;
}

void Grid_Init(Grid* grid)
{
	for (int i = 0; i < grid->width; ++i)
	{
		for (int j = 0; j < grid->height; ++j)
		{
			grid->array[i][j] = Node_Create(i, j, 1.0f);
		}
	}
}

vector<Node*> Grid_GetNeighbors(Grid* grid, Node* node, bool allowDiagonal)
{
	int numberOfDirections = 4;

	Vector2D directions[8] = { { 1.0f, 0.0f },{ 0.0f, 1.0f },
								{ -1.0f, 0.0f },{ 0.0f, -1.0f },
								{ -1.0f, -1.0f },{ 1.0f, -1.0f },
								{ -1.0f, 1.0f },{ 1.0f, 1.0f } };
	if (allowDiagonal == true)
	{
		numberOfDirections = 8;
	}
	
	vector<Node*> result;
	Vector2D currentDirection;
	for (int i = 0; i < numberOfDirections; ++i)
	{
		currentDirection = directions[i];

		int neighborX = node->position.x + currentDirection.x;
		int neighborY = node->position.y + currentDirection.y;

		if (0 <= neighborX && neighborX < grid->width
			&& 0 <= neighborY && neighborY < grid->height)
		{
			if (grid->array[neighborX][neighborY].isWall == false)
			{
				result.push_back(&grid->array[neighborX][neighborY]);
			}
		}
	}
	return result;
}

void Grid_InsertWall(Grid* grid, int x, int y)
{
	grid->array[x][y].isWall = true;
}

void Grid_DeleteWall(Grid* grid, int x, int y)
{
	grid->array[x][y].isWall = false;
}


// Functions for Simple Dijkstra
float Grid_GetCost(Grid* grid, Node* to)
{
	return to->weight;
}

void Grid_ChangeWeight(Grid* grid, int x, int y)
{
	grid->array[x][y].weight += 1;
	if (grid->array[x][y].weight > MAX_WEIGHT)
	{
		grid->array[x][y].weight = 1.0f;
	}
}

void Grid_Render(Grid* grid, SDL_Renderer* renderer, Vector2D gridOrigin, Uint32 gridColor)
{
	for (int i = 0; i <= grid->width; ++i)
	{
		lineColor(renderer, gridOrigin.x + i * grid->cellSize, gridOrigin.y, 
			gridOrigin.x + i * grid->cellSize, gridOrigin.y + grid->height * grid->cellSize, gridColor);
	}

	for (int j = 0; j <= grid->height; ++j)
	{
		lineColor(renderer, gridOrigin.x, gridOrigin.y + j * grid->cellSize, 
			gridOrigin.x + grid->width * grid->cellSize, gridOrigin.y + j * grid->cellSize, gridColor);
	}
}