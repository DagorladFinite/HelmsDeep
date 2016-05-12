/* ========================================================================
   File: DynamicGridUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include <vector>
#include "GridUtils.h"
#include "GraphUtils.h"

#define CLUSTER_SIZE  10

struct GridCluster;

struct PlusNode : public Node
{
	int graphNodeIndex = -1;
};

PlusNode PlusNode_Create(int x, int y, float weight);
void PlusNode_Reset(PlusNode* node);

struct Entrance
{
	int index; 
	int fromGraphIndex = -1;
	int toGraphIndex = -1;
	int connectionIndex = -1;
	Vector2D fromPosition;
	GridCluster* fromCluster;
	Vector2D toPosition;
	GridCluster* toCluster;
};

struct GridCluster
{
	int index;
	int width = 0;
	int height = 0;
	Vector2D origin = { 0.0f, 0.0f };
};

struct DynamicGrid
{
	int width = 0;
	int height = 0;
	int cellSize = 1;
	PlusNode* array;
};

void DynamicGrid_Init(DynamicGrid* grid, int width, int height);
void DynamicGrid_FillFromMap(DynamicGrid* grid, char* mapBuffer);
void DynamicGrid_Free(DynamicGrid* grid);
void DynamicGrid_Render(DynamicGrid* grid, SDL_Renderer* rendeder, Vector2D origin, Uint32 gridColor);
PlusNode* DynamicGrid_GetNode(DynamicGrid* grid, int x, int y);
void DynamicGrid_GridReset(DynamicGrid* grid);
float DynamicGrid_GetCost(DynamicGrid* grid, Node* to);
bool DynamicGrid_IsWall(DynamicGrid* grid, int x, int y);
std::vector<Node*> DynamicGrid_GetNeighbors(DynamicGrid* grid, Node* node, bool allowDiagonals = false);
std::vector<Node*> DynamicGrid_GetClusterNeighbors(DynamicGrid* grid, GridCluster* cluster, Node* node, bool allowDiagonals = false);

// Jump Point Search methods
std::vector<Node*> DynamicGrid_GetJPSSuccessors(DynamicGrid* grid, Node* currentNode, Node* startNode, Node* endNode);
Node* DynamicGrid_Jump(DynamicGrid* grid, int currentX, int currentY, int deltaX, int deltaY, Node* startNode, Node* endNode);

struct AbstractGraph
{
	DynamicGrid grid;
	Graph graph;
	int clustersWidth = 0;
	int clustersHeight = 0;
	GridCluster* clusters;
	std::vector<Entrance> entrances;
	std::vector<std::vector<int>> entranceLUT; // Lookup table for entrances indexed by cluster

	void CreateClusters();
	void CreateEntrances();
	void CreateClusterConnections();
	void CreateNodesAndConnectionForEntrance(Entrance* entrance);
	void RenderClusters(SDL_Renderer* rendeder, Vector2D origin, Uint32 gridColor);
	void RenderEntrances(SDL_Renderer* renderer, Vector2D origin, Uint32 entranceColor);
	GridCluster* GetClusterForPosition(int x, int y);
};

