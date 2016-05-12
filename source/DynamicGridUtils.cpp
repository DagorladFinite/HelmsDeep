/* ========================================================================
   File: DynamicGridUtils.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "DynamicGridUtils.h"
#include "PathfindingUtils.h"
#include "DynamicPathfindingUtils.h"

#include <string.h>
#include <ctype.h>
#include <vector>

#define DOUBLE_ENTRANCE_THRESHOLD 6

PlusNode PlusNode_Create(int x, int y, float weight)
{
	PlusNode node = {};
	node.position = { (float)x, (float)y };
	node.weight = weight;
	return node;
}

void PlusNode_Reset(PlusNode* node)
{
	node->costSoFar = -1.0f;
	node->priority = 0.0f;
	node->heuristic = 0.0f;
	node->cameFrom = nullptr;
	node->alreadyUsed = false;
}

void AbstractGraph::RenderClusters(SDL_Renderer* renderer, Vector2D origin, Uint32 clusterColor)
{
	for (int i = 0; i <= clustersWidth; ++i)
	{
		lineColor(renderer, origin.x + i * grid.cellSize * CLUSTER_SIZE, 
								origin.y,
									origin.x + i * grid.cellSize * CLUSTER_SIZE, 
										origin.y + grid.height * grid.cellSize, clusterColor);
	}

	for (int j = 0; j <= clustersHeight; ++j)
	{
		lineColor(renderer, origin.x, 
								origin.y + j * grid.cellSize * CLUSTER_SIZE,
									origin.x + grid.width * grid.cellSize, 
										origin.y + j * grid.cellSize * CLUSTER_SIZE, clusterColor);
	}
}

void AbstractGraph::RenderEntrances(SDL_Renderer* renderer, Vector2D origin, Uint32 entranceColor)
{
	for (int i = 0; i < (int)entrances.size(); ++i)
	{
		boxColor(renderer, origin.x + entrances[i].fromPosition.x * grid.cellSize,
			origin.y + entrances[i].fromPosition.y * grid.cellSize,
			origin.x + entrances[i].fromPosition.x * grid.cellSize + grid.cellSize,
			origin.y + entrances[i].fromPosition.y * grid.cellSize + grid.cellSize,
			entranceColor);
	}
}

void AbstractGraph::CreateEntrances()
{
	// Check in four directions of each cluster
	Vector2D directions[4] = { { 1.0f, 0.0f },{ 0.0f, 1.0f },
								{ -1.0f, 0.0f },{ 0.0f, -1.0f } };
	//SDL_Log("Creating Entrances");
	for (int i = 0; i < clustersWidth; ++i)
	{
		for (int j = 0; j < clustersHeight; ++j)
		{
			int currentClusterX = (int)clusters[i + j * clustersWidth].origin.x;
			int currentClusterY = (int)clusters[i + j * clustersWidth].origin.y;

			int currentX = currentClusterX * CLUSTER_SIZE;
			int currentY = currentClusterY * CLUSTER_SIZE;

			//SDL_Log("Checking Cluster (%d, %d) (%d, %d)", i, j, currentClusterX, currentClusterY);
			Vector2D currentDirection;
			for (int k = 0; k < 4; ++k)
			{
				//SDL_Log("> Checking Direction %d", k);
				currentDirection = directions[k];

				int neighborClusterX = currentClusterX + currentDirection.x;
				int neighborClusterY = currentClusterY + currentDirection.y;

				GridCluster* neighborCluster = &clusters[neighborClusterX + neighborClusterY * clustersWidth];

				int neighborX = neighborClusterX * CLUSTER_SIZE;
				int neighborY = neighborClusterY * CLUSTER_SIZE;

				//SDL_Log("> > Neighbor Cluster (%d, %d)", neighborClusterX, neighborClusterY);

				if (0 <= neighborX && neighborX < grid.width
					&& 0 <= neighborY && neighborY < grid.height)
				{
					int currentEntranceStart = 0;
					int currentEntranceEnd = 0;
					std::vector<int> entranceStartVector;
					std::vector<int> entranceEndVector;

					switch (k)
					{
					case 0:
					{
						// Check right
						//SDL_Log("> > > > RIGHT Checking");
						bool entranceDetected = false;
						int positionX = currentX + CLUSTER_SIZE - 1;

						for (int m = 0; m < CLUSTER_SIZE; ++m)
						{
							int positionY = currentY + m;

							PlusNode* currentNode = DynamicGrid_GetNode(&grid, positionX, positionY);
							PlusNode* symmetricNode = DynamicGrid_GetNode(&grid, positionX + 1.0f, positionY);

							if (currentNode != nullptr && currentNode->isWall == false
								&& symmetricNode != nullptr && symmetricNode->isWall == false)
							{
								if (entranceDetected == false)
								{
									entranceDetected = true;
									currentEntranceStart = currentEntranceEnd;
								}
								++currentEntranceEnd;
							}
							else
							{
								if (entranceDetected == true)
								{
									entranceStartVector.push_back(currentEntranceStart);
									entranceEndVector.push_back(currentEntranceEnd);
									entranceDetected = false;
									currentEntranceStart = currentEntranceEnd + 1;
									currentEntranceEnd = currentEntranceStart;
								}
								else
								{
									++currentEntranceStart;
									currentEntranceEnd = currentEntranceStart;
								}
							}

						}

						// Last entrace pending from loop exit
						if (entranceDetected == true)
						{
							entranceStartVector.push_back(currentEntranceStart);
							entranceEndVector.push_back(currentEntranceEnd);
							entranceDetected = false;
						}

						for (int m = 0; m < (int)entranceStartVector.size(); ++m)
						{
							//SDL_Log("Entrances detected (%d, %d)", entranceStartVector[m], entranceEndVector[m]);

							// Entrance detected, create it 
							if (entranceEndVector[m] - entranceStartVector[m] >= DOUBLE_ENTRANCE_THRESHOLD)
							{
								// Double entrance
								float entranceY = currentY + entranceStartVector[m];
								if (entranceY < grid.height)
								{
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(positionX, entranceY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(positionX + 1.0f, entranceY);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}
								entranceY = currentY + entranceEndVector[m] - 1;
								if (entranceY < grid.height)
								{
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(positionX, entranceY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(positionX + 1.0f, entranceY);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}
							}
							else
							{
								float entranceY = currentY + entranceStartVector[m] + floor((entranceEndVector[m] - entranceStartVector[m]) / 2.0f);
								if (entranceY < grid.height)
								{
									// Single Node
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(positionX, entranceY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(positionX + 1.0f, entranceY);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}
							}
						}
					}
					break;
					case 1:
					{
						// Check down
						//SDL_Log("> > > > DOWN Checking");
						bool entranceDetected = false;
						int positionY = currentY + CLUSTER_SIZE - 1;

						for (int m = 0; m < CLUSTER_SIZE; ++m)
						{
							int positionX = currentX + m;

							PlusNode* currentNode = DynamicGrid_GetNode(&grid, positionX, positionY);
							PlusNode* symmetricNode = DynamicGrid_GetNode(&grid, positionX, positionY + 1.0f);

							if (currentNode != nullptr && currentNode->isWall == false
								&& symmetricNode != nullptr && symmetricNode->isWall == false)
							{
								if (entranceDetected == false)
								{
									entranceDetected = true;
									currentEntranceStart = currentEntranceEnd;
								}
								++currentEntranceEnd;
							}
							else
							{
								if (entranceDetected == true)
								{
									entranceStartVector.push_back(currentEntranceStart);
									entranceEndVector.push_back(currentEntranceEnd);
									entranceDetected = false;
									currentEntranceStart = currentEntranceEnd + 1;
									currentEntranceEnd = currentEntranceStart;
								}
								else
								{
									++currentEntranceStart;
									currentEntranceEnd = currentEntranceStart;
								}
							}

						}

						// Last entrace pending from loop exit
						if (entranceDetected == true)
						{
							entranceStartVector.push_back(currentEntranceStart);
							entranceEndVector.push_back(currentEntranceEnd);
							entranceDetected = false;
						}

						for (int m = 0; m < (int)entranceStartVector.size(); ++m)
						{
							//SDL_Log("Entrances detected (%d, %d)", entranceStartVector[m], entranceEndVector[m]);

							// Entrance detected, create it 
							if (entranceEndVector[m] - entranceStartVector[m] >= DOUBLE_ENTRANCE_THRESHOLD)
							{
								// Double entrance
								float entranceX = currentX + entranceStartVector[m];
								if (entranceX < grid.width)
								{
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(entranceX, positionY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(entranceX, positionY + 1.0f);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}
								entranceX = currentX + entranceEndVector[m] - 1;
								if (entranceX < grid.width)
								{
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(entranceX, positionY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(entranceX, positionY + 1.0f);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}

							}
							else
							{
								float entranceX = currentX + entranceStartVector[m] + floor((entranceEndVector[m] - entranceStartVector[m]) / 2.0f);
								if (entranceX < grid.width)
								{
									// Single Node
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(entranceX, positionY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(entranceX, positionY + 1.0f);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}
							}
						}
					}
					break;
					case 2:
					{
						// Check left
						//SDL_Log("> > > > LEFT Checking");
						bool entranceDetected = false;
						int positionX = currentX;

						for (int m = 0; m < CLUSTER_SIZE; ++m)
						{
							int positionY = currentY + m;

							PlusNode* currentNode = DynamicGrid_GetNode(&grid, positionX, positionY);
							PlusNode* symmetricNode = DynamicGrid_GetNode(&grid, positionX - 1.0f, positionY);

							if (currentNode != nullptr && currentNode->isWall == false
								&& symmetricNode != nullptr && symmetricNode->isWall == false)
							{
								if (entranceDetected == false)
								{
									entranceDetected = true;
									currentEntranceStart = currentEntranceEnd;
								}
								++currentEntranceEnd;
							}
							else
							{
								if (entranceDetected == true)
								{
									entranceStartVector.push_back(currentEntranceStart);
									entranceEndVector.push_back(currentEntranceEnd);
									entranceDetected = false;
									currentEntranceStart = currentEntranceEnd + 1;
									currentEntranceEnd = currentEntranceStart;
								}
								else
								{
									++currentEntranceStart;
									currentEntranceEnd = currentEntranceStart;
								}
							}

						}

						// Last entrace pending from loop exit
						if (entranceDetected == true)
						{
							entranceStartVector.push_back(currentEntranceStart);
							entranceEndVector.push_back(currentEntranceEnd);
							entranceDetected = false;
						}

						for (int m = 0; m < (int)entranceStartVector.size(); ++m)
						{
							//SDL_Log("Entrances detected (%d, %d)", entranceStartVector[m], entranceEndVector[m]);

							// Entrance detected, create it 
							if (entranceEndVector[m] - entranceStartVector[m] >= DOUBLE_ENTRANCE_THRESHOLD)
							{
								// Double entrance
								float entranceY = currentY + entranceStartVector[m];
								if (entranceY < grid.height)
								{
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(positionX, entranceY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(positionX - 1.0f, entranceY);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}
								entranceY = currentY + entranceEndVector[m] - 1;
								if (entranceY < grid.height)
								{
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(positionX, entranceY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(positionX - 1.0f, entranceY);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}

							}
							else
							{
								float entranceY = currentY + entranceStartVector[m] + floor((entranceEndVector[m] - entranceStartVector[m]) / 2.0f);
								if (entranceY < grid.height)
								{
									// Single Node
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(positionX, entranceY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(positionX - 1.0f, entranceY);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}
							}
						}
					}
					break;
					case 3:
					{
						// Check up
						//SDL_Log("> > > > UP Checking");
						bool entranceDetected = false;
						int positionY = currentY;

						for (int m = 0; m < CLUSTER_SIZE; ++m)
						{
							int positionX = currentX + m;

							PlusNode* currentNode = DynamicGrid_GetNode(&grid, positionX, positionY);
							PlusNode* symmetricNode = DynamicGrid_GetNode(&grid, positionX, positionY - 1.0f);

							if (currentNode != nullptr && currentNode->isWall == false
								&& symmetricNode != nullptr && symmetricNode->isWall == false)
							{
								if (entranceDetected == false)
								{
									entranceDetected = true;
									currentEntranceStart = currentEntranceEnd;
								}
								++currentEntranceEnd;
							}
							else
							{
								if (entranceDetected == true)
								{
									entranceStartVector.push_back(currentEntranceStart);
									entranceEndVector.push_back(currentEntranceEnd);
									entranceDetected = false;
									currentEntranceStart = currentEntranceEnd + 1;
									currentEntranceEnd = currentEntranceStart;
								}
								else
								{
									++currentEntranceStart;
									currentEntranceEnd = currentEntranceStart;
								}
							}

						}

						// Last entrace pending from loop exit
						if (entranceDetected == true)
						{
							entranceStartVector.push_back(currentEntranceStart);
							entranceEndVector.push_back(currentEntranceEnd);
							entranceDetected = false;
						}

						for (int m = 0; m < (int)entranceStartVector.size(); ++m)
						{
							//SDL_Log("Entrances detected (%d, %d)", entranceStartVector[m], entranceEndVector[m]);

							// Entrance detected, create it 
							if (entranceEndVector[m] - entranceStartVector[m] >= DOUBLE_ENTRANCE_THRESHOLD)
							{
								// Double entrance
								float entranceX = currentX + entranceStartVector[m];
								if (entranceX < grid.width)
								{
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(entranceX, positionY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(entranceX, positionY - 1.0f);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}
								entranceX = currentX + entranceEndVector[m] - 1;
								if (entranceX < grid.width)
								{
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(entranceX, positionY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(entranceX, positionY - 1.0f);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}

							}
							else
							{
								float entranceX = currentX + entranceStartVector[m] + floor((entranceEndVector[m] - entranceStartVector[m]) / 2.0f);
								if (entranceX < grid.width)
								{
									// Single Node
									Entrance currentEntrance;
									currentEntrance.fromPosition = Vector2D(entranceX, positionY);
									currentEntrance.fromCluster = &clusters[i + j * clustersWidth];
									currentEntrance.toPosition = Vector2D(entranceX, positionY - 1.0f);
									currentEntrance.toCluster = neighborCluster;
									currentEntrance.index = (int)entrances.size();
									entrances.push_back(currentEntrance);
									entranceLUT[clusters[i + j * clustersWidth].index].push_back(currentEntrance.index);
									CreateNodesAndConnectionForEntrance(&entrances[currentEntrance.index]);
								}
							}
						}
					}
					break;
					}
				}
			}
		}
	}
}

void AbstractGraph::CreateClusters()
{
	clustersWidth = (int)ceil(grid.width / CLUSTER_SIZE) + 1;
	clustersHeight = (int)ceil(grid.height / CLUSTER_SIZE) + 1;

	clusters = (GridCluster*)SDL_malloc(sizeof(GridCluster) * clustersWidth * clustersHeight);

	int clusterIndex = 0;

	for (int i = 0; i < clustersWidth; ++i)
	{
		for (int j = 0; j < clustersHeight; ++j)
		{
			GridCluster* currentCluster = &clusters[i + j * clustersWidth];
			currentCluster->index = clusterIndex;
			currentCluster->width = CLUSTER_SIZE;
			currentCluster->height = CLUSTER_SIZE;
			currentCluster->origin = {(float)i, (float)j };
			std::vector<int> entranceVector;
			entranceLUT.push_back(entranceVector);
			++clusterIndex;
		}
	}

	CreateEntrances();
	CreateClusterConnections();
}


void AbstractGraph::CreateClusterConnections()
{
	for (int i = 0; i < (int)entranceLUT.size(); ++i)
	{
		//SDL_Log("Number of entrances for cluster %d are %d", i, (int)entranceLUT[i].size());

		for (int j = 0; j < (int)entranceLUT[i].size(); ++j)
		{
			for (int k = 0; k < (int)entranceLUT[i].size(); ++k)
			{
				if (j != k)
				{
					Entrance* fromEntrance = &entrances[entranceLUT[i][j]];
					Entrance* toEntrance = &entrances[entranceLUT[i][k]];
					PlusNode* currentFromPosition =
						DynamicGrid_GetNode(&grid, fromEntrance->fromPosition.x, fromEntrance->fromPosition.y);
					PlusNode* currentToPosition =
						DynamicGrid_GetNode(&grid, toEntrance->fromPosition.x, toEntrance->fromPosition.y);
					//SDL_Log("Checking %d cluster, entrances %d and %d", i, j, k);
					//SDL_Log("Entrance %d Position %f, %f", j, fromEntrance->fromPosition.x, fromEntrance->fromPosition.y);
					//SDL_Log("Entrance %d Position %f, %f", k, toEntrance->fromPosition.x, toEntrance->fromPosition.y);

					if (currentFromPosition->graphNodeIndex == -1 || currentToPosition->graphNodeIndex == -1)
					{
						SDL_Log("ERROR: Nodes not created!");
						continue;
					}

					GridCluster* currentCluster = GetClusterForPosition(currentFromPosition->position.x,
						currentFromPosition->position.y);
					GridCluster* endCluster = GetClusterForPosition(currentToPosition->position.x,
						currentToPosition->position.y);

					if (currentCluster != endCluster)
					{
						SDL_Log("ERROR: Not in same cluster error!");
					}
					std::vector<Node*> possiblePath = DynamicPathfindingUtils::PathfindAStarCluster(&grid, currentCluster, currentFromPosition, currentToPosition, HeuristicUtils::ManhattanDistance, true);
					
					if ((int)possiblePath.size() > 0)
					{
						//graph.AddConnection(currentFromPosition->graphNodeIndex, currentToPosition->graphNodeIndex,
						//	HeuristicUtils::ManhattanDistance(currentFromPosition->position, currentToPosition->position));
						
						// Use path length as Heuristic instead of Manhattan
						graph.AddConnection(currentFromPosition->graphNodeIndex, currentToPosition->graphNodeIndex,
							possiblePath.size() * D_PARAM);
						//SDL_Log("Added connection from %d to %d",
						//	currentFromPosition->graphNodeIndex, currentToPosition->graphNodeIndex);
					}
					else
					{
						//SDL_Log("ERROR: No connection added because no path found");
					}
					DynamicGrid_GridReset(&grid);
				}
			}
		}
	}
}

void AbstractGraph::CreateNodesAndConnectionForEntrance(Entrance* entrance)
{
	PlusNode* currentFromPosition = DynamicGrid_GetNode(&grid, entrance->fromPosition.x, entrance->fromPosition.y);
	if (currentFromPosition->graphNodeIndex == -1)
	{
		currentFromPosition->graphNodeIndex = graph.AddNode(entrance->fromPosition);
		entrance->fromGraphIndex = currentFromPosition->graphNodeIndex;
		//SDL_Log("Added FROM node %d for position %f, %f",
		//	currentFromPosition->graphNodeIndex, entrance->fromPosition.x, entrance->fromPosition.y);
	}

	PlusNode* currentToPosition = DynamicGrid_GetNode(&grid, entrance->toPosition.x, entrance->toPosition.y);
	if (currentToPosition->graphNodeIndex == -1)
	{
		currentToPosition->graphNodeIndex = graph.AddNode(entrance->toPosition);
		entrance->toGraphIndex = currentToPosition->graphNodeIndex;
		//SDL_Log("Added TO node %d for position %f, %f",
		//	currentToPosition->graphNodeIndex, entrance->toPosition.x, entrance->toPosition.y);
	}

	entrance->connectionIndex =
		graph.AddConnection(currentFromPosition->graphNodeIndex, currentToPosition->graphNodeIndex, 1.0f);
	//SDL_Log("Added connection from %d to %d",
	//	currentFromPosition->graphNodeIndex, currentToPosition->graphNodeIndex);
}

void DynamicGrid_Init(DynamicGrid* grid, int width, int height)
{
	grid->width = width;
	grid->height = height;
	grid->array = (PlusNode*)SDL_malloc(sizeof(PlusNode) * width * height);

	for (int i = 0; i < grid->width; ++i)
	{
		for (int j = 0; j < grid->height; ++j)
		{
			grid->array[i + j * grid->width] = PlusNode_Create(i, j, 1.0f);
		}
	}
}

PlusNode* DynamicGrid_GetNode(DynamicGrid* grid, int x, int y)
{
	if (x >= 0 && x < grid->width && y >= 0 && y < grid->height)
	{
		return &grid->array[x + y * grid->width];
	}
	else
	{
		//SDL_Log("Cell out of range");
		return nullptr;
	}
}

void DynamicGrid_GridReset(DynamicGrid* grid)
{
	for (int i = 0; i < grid->width * grid->height; ++i)
	{
		PlusNode_Reset(&grid->array[i]);
	}
}

void DynamicGrid_FillFromMap(DynamicGrid* grid, char* mapBuffer)
{
	int charNumber = 0;
	char tempBuffer[5];
	int currentIndex = 0;

	char* dataLocation = strstr(mapBuffer, "width") + 6 * sizeof(char);
	while (isdigit(dataLocation[currentIndex]))
	{
		tempBuffer[currentIndex] = dataLocation[currentIndex];
		++currentIndex;
	}
	tempBuffer[currentIndex] = '\0';
	int width = atoi(tempBuffer);

	SDL_Log("Map Width = %d", width);

	currentIndex = 0;
	dataLocation = strstr(mapBuffer, "height") + 7 * sizeof(char);
	while (isdigit(dataLocation[currentIndex]))
	{
		tempBuffer[currentIndex] = dataLocation[currentIndex];
		++currentIndex;
	}
	tempBuffer[currentIndex] = '\0';
	int height = atoi(tempBuffer);

	SDL_Log("Map Height = %d", height);

	DynamicGrid_Init(grid, width, height);

	currentIndex = 0;
	int gridIndex = 0;
	dataLocation = strstr(mapBuffer, "map") + 4 * sizeof(char);
	while (dataLocation[currentIndex] != '\0')
	{
		if (dataLocation[currentIndex] == 'T' || dataLocation[currentIndex] == '@')
		{
			grid->array[gridIndex].isWall = true;
			//SDL_Log("Wall Added = %d", gridIndex);
		}

		if (dataLocation[currentIndex] != '\n')
		{
			++gridIndex;
		}
		++currentIndex;
	}
}

void DynamicGrid_Free(DynamicGrid* grid)
{
	SDL_free(grid->array);
}

void DynamicGrid_Render(DynamicGrid* grid, SDL_Renderer* renderer, Vector2D origin, Uint32 gridColor)
{
	for (int i = 0; i <= grid->width; ++i)
	{
		lineColor(renderer, origin.x + i * grid->cellSize, origin.y,
			origin.x + i * grid->cellSize, origin.y + grid->height * grid->cellSize, gridColor);
	}

	for (int j = 0; j <= grid->height; ++j)
	{
		lineColor(renderer, origin.x, origin.y + j * grid->cellSize,
			origin.x + grid->width * grid->cellSize, origin.y + j * grid->cellSize, gridColor);
	}

	for (int i = 0; i < grid->width * grid->height; ++i)
	{
		if (grid->array[i].isWall == true)
		{
			int currentHeight = (int)floor(i / grid->width);
			int currentWidth = (int)(i % grid->width);
			boxColor(renderer, origin.x + currentWidth * grid->cellSize,
				origin.y + currentHeight * grid->cellSize,
				origin.x + currentWidth * grid->cellSize + grid->cellSize,
				origin.y + currentHeight * grid->cellSize + grid->cellSize,
				Colors::SUNFLOWER);
		}
	}
}

float DynamicGrid_GetCost(DynamicGrid* grid, Node* to)
{
	return to->weight;
}

std::vector<Node*> DynamicGrid_GetNeighbors(DynamicGrid* grid, Node* node, bool allowDiagonals)
{
	int numberOfDirections = 4;

	Vector2D directions[8] = { { 1.0f, 0.0f },{ 0.0f, 1.0f },
	{ -1.0f, 0.0f },{ 0.0f, -1.0f },
	{ -1.0f, -1.0f },{ 1.0f, -1.0f },
	{ -1.0f, 1.0f },{ 1.0f, 1.0f } };
	if (allowDiagonals == true)
	{
		numberOfDirections = 8;
	}

	std::vector<Node*> result;
	Vector2D currentDirection;
	for (int i = 0; i < numberOfDirections; ++i)
	{
		currentDirection = directions[i];

		int neighborX = node->position.x + currentDirection.x;
		int neighborY = node->position.y + currentDirection.y;

		if (0 <= neighborX && neighborX < grid->width
			&& 0 <= neighborY && neighborY < grid->height)
		{
			if (grid->array[neighborX + neighborY * grid->width].isWall == false)
			{
				result.push_back(&grid->array[neighborX + neighborY * grid->width]);
			}
		}
	}
	return result;
}

bool DynamicGrid_IsWall(DynamicGrid* grid, int x, int y)
{
	if (x < 0 || x >= grid->width || y < 0 || y >= grid->height)
	{
		return true;
	}
	else
	{
		return grid->array[x + y * grid->width].isWall;
	}
}

Node* DynamicGrid_Jump(DynamicGrid* grid, int currentX, int currentY, int deltaX, int deltaY, Node* startNode, Node* endNode)
{
	// currentX, currentY - Current Node Position 
	// deltaX, deltaY - Direction

	// Position of new node we are going to consider:
	int nextX = currentX + deltaX;
	int nextY = currentY + deltaY;

	// If it's blocked we can't jump here
	if (DynamicGrid_IsWall(grid, nextX, nextY) == true)
	{
		return nullptr;
	}

	// If the node is the goal return it
	if (nextX == (int)endNode->position.x
		&& nextY == (int)endNode->position.y)
	{
		return endNode;
	}

	int offsetX = nextX;
	int offsetY = nextY;

	// Diagonal Case   
	if (deltaX != 0 && deltaY != 0)
	{
		while (true)
		{
			// Check if Diagonal Forced Neighbor exists, return it if so
			if (DynamicGrid_IsWall(grid, offsetX - deltaX, offsetY + deltaY) == false
				&& DynamicGrid_IsWall(grid, offsetX - deltaY, offsetY) == true
				|| DynamicGrid_IsWall(grid, offsetX + deltaX, offsetY - deltaY) == false
				&& DynamicGrid_IsWall(grid, offsetX, offsetY - deltaY) == true)
			{
				return &grid->array[offsetX + offsetY * grid->width];
			}

			// Check in horizontal and vertical directions for forced neighbors
			// This is a special case for diagonal direction
			if (DynamicGrid_Jump(grid, offsetX, offsetY, deltaX, 0, startNode, endNode) != nullptr ||
				DynamicGrid_Jump(grid, offsetX, offsetY, 0, deltaY, startNode, endNode) != nullptr)
			{
				return &grid->array[offsetX + offsetY * grid->width];
			}

			offsetX += deltaX;
			offsetY += deltaY;

			// If it's blocked we can't jump here
			if (DynamicGrid_IsWall(grid, offsetX, offsetY) == true)
			{
				return nullptr;
			}

			// If the node is the goal return it
			if (offsetX == endNode->position.x
				&& offsetY == endNode->position.y)
			{
				return endNode;
			}
		}
	}
	else
	{
		// Horizontal case
		if (deltaX != 0)
		{
			while (true)
			{
				// Check Horizontal Forced Neighbor exists, return it if so
				if (DynamicGrid_IsWall(grid, offsetX + deltaX, nextY + 1) == false
					&& DynamicGrid_IsWall(grid, offsetX, nextY + 1) == true
					|| DynamicGrid_IsWall(grid, offsetX + deltaX, nextY - 1) == false
					&& DynamicGrid_IsWall(grid, offsetX, nextY - 1) == true)
				{
					return &grid->array[offsetX + nextY * grid->width];
				}

				offsetX += deltaX;

				// If it's blocked we can't jump here
				if (DynamicGrid_IsWall(grid, offsetX, nextY) == true)
				{
					return nullptr;
				}

				// If the node is the goal return it
				if (offsetX == endNode->position.x
					&& nextY == endNode->position.y)
				{
					return endNode;
				}
			}
		}
		else
		{
			while (true)
			{
				// Check if Vertical Forced Neighbor exists, return it if so
				if (DynamicGrid_IsWall(grid, nextX + 1, offsetY + deltaY) == false
					&& DynamicGrid_IsWall(grid, nextX + 1, offsetY) == true
					|| DynamicGrid_IsWall(grid, nextX - 1, offsetY + deltaY) == false
					&& DynamicGrid_IsWall(grid, nextX - 1, offsetY) == true)
				{
					return &grid->array[nextX + offsetY * grid->width];
				}

				offsetY += deltaY;

				// If it's blocked we can't jump here
				if (DynamicGrid_IsWall(grid, nextX, offsetY) == true)
				{
					return nullptr;
				}

				// If the node is the goal return it
				if (nextX == endNode->position.x
					&& offsetY == endNode->position.y)
				{
					return endNode;
				}
			}
		}
	}

	SDL_Log("What!");

	// Will never get here
	return nullptr;
}

std::vector<Node*> DynamicGrid_GetJPSSuccessors(DynamicGrid* grid, Node* currentNode, Node* startNode, Node* endNode)
{
	std::vector<Node*> successors;
	std::vector<Node*> neighbors = DynamicGrid_GetNeighbors(grid, currentNode, true);

	for (int i = 0; i < (int)neighbors.size(); ++i)
	{
		// Direction from current node to neighbor
		int dX = IntegerUtils::Clamp((int)(neighbors[i]->position.x - currentNode->position.x), -1, 1);
		int dY = IntegerUtils::Clamp((int)(neighbors[i]->position.y - currentNode->position.y), -1, 1);

		// Try to find a node to jump to:
		Node* jumpPoint = DynamicGrid_Jump(grid, currentNode->position.x, currentNode->position.y, dX, dY, startNode, endNode);

		// If found add it to the list:
		if (jumpPoint != nullptr)
		{
			successors.push_back(jumpPoint);
		}
	}

	return successors;
}

std::vector<Node*> DynamicGrid_GetClusterNeighbors(DynamicGrid* grid, GridCluster* cluster, Node* node, bool allowDiagonals)
{
	int numberOfDirections = 4;

	Vector2D directions[8] = { { 1.0f, 0.0f },{ 0.0f, 1.0f },
	{ -1.0f, 0.0f },{ 0.0f, -1.0f },
	{ -1.0f, -1.0f },{ 1.0f, -1.0f },
	{ -1.0f, 1.0f },{ 1.0f, 1.0f } };
	if (allowDiagonals == true)
	{
		numberOfDirections = 8;
	}

	std::vector<Node*> result;
	Vector2D currentDirection;
	for (int i = 0; i < numberOfDirections; ++i)
	{
		currentDirection = directions[i];

		int neighborX = node->position.x + currentDirection.x;
		int neighborY = node->position.y + currentDirection.y;

		if (cluster->origin.x * CLUSTER_SIZE <= neighborX && neighborX < cluster->origin.x * CLUSTER_SIZE + CLUSTER_SIZE
			&& cluster->origin.y * CLUSTER_SIZE <= neighborY && neighborY < cluster->origin.y * CLUSTER_SIZE + CLUSTER_SIZE)
		{
			if (grid->array[neighborX + neighborY * grid->width].isWall == false)
			{
				result.push_back(&grid->array[neighborX + neighborY * grid->width]);
			}
		}
	}
	return result;
}

GridCluster* AbstractGraph::GetClusterForPosition(int x, int y)
{
	int clustersX = (int)floor(x / CLUSTER_SIZE);
	int clustersY = (int)floor(y / CLUSTER_SIZE);
	GridCluster* currentCluster = &clusters[clustersX + clustersY * clustersWidth];
	return currentCluster;
}
