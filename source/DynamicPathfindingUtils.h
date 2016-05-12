/* ========================================================================
   File: DynamicPathfindingUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "PathfindingUtils.h"
#include "DynamicGridUtils.h"

namespace DynamicPathfindingUtils
{
	static std::vector<Node*> PathfindAStarCluster(DynamicGrid* grid, GridCluster* cluster, Node* start, Node* end, HeuristicFunction heuristicFunction = &HeuristicUtils::ManhattanDistance, bool useBreakTie = false, bool allowDiagonals = false, float gFactor = 1.0f, float hFactor = 1.0f)
	{
		// Declare result path
		std::vector<Node*> path;

		CoolPriorityQueue frontier;
		// Initialize Start node cost
		start->costSoFar = 0.0f;
		start->priority = 0.0f;
		frontier.put(start, start->priority);

		Node* current = nullptr;

		while (frontier.empty() == false)
		{
			// Get current node to process
			// Node is taken out of queue automatically
			current = frontier.get();

			// Early exit if we found goal
			if (current == end)
			{
				break;
			}

			// Get node neighbors
			std::vector<Node*> neighbors = DynamicGrid_GetClusterNeighbors(grid, cluster, current, allowDiagonals);

			// For each neighbor
			for (int i = 0; i < (int)neighbors.size(); ++i)
			{
				// Calculate new cost to get to this neighbor
				float newCost = current->costSoFar + DynamicGrid_GetCost(grid, neighbors[i]);
				// Get current stored cost
				float currentCostSoFar = neighbors[i]->costSoFar;
				// If no stored cost or newCost is less than current
				if (currentCostSoFar == -1.0f || newCost < currentCostSoFar)
				{
					// Update Cost because we found a better path
					neighbors[i]->costSoFar = newCost;
					float heuristicValue = heuristicFunction(end->position, neighbors[i]->position);
					if (useBreakTie == true)
					{
						//heuristicValue *= (1.0f + P_PARAM);
						heuristicValue += PathfindingUtils::StraightLineTieBreak(neighbors[i]->position,
							start->position,
							end->position);
					}
					float priority = (gFactor * newCost) + (hFactor * heuristicValue);

					neighbors[i]->heuristic = heuristicValue;
					neighbors[i]->priority = priority;
					neighbors[i]->cameFrom = current;
					//Add neighbor to queue
					frontier.put(neighbors[i], priority);
				}
			}
		}


		if (current != end)
		{
			SDL_Log("No path found!\n");
			return path;
		}

		path.push_back(current);

		while (current != start)
		{
			current = current->cameFrom;
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());

		return path;
	}

	static std::vector<Node*> PathfindAStar(DynamicGrid* grid, Node* start, Node* end, HeuristicFunction heuristicFunction = &HeuristicUtils::ManhattanDistance, bool useBreakTie = false, bool allowDiagonals = false, float gFactor = 1.0f, float hFactor = 1.0f, bool useJPS = false)
	{
		if (useJPS == true)
		{
			allowDiagonals = true;
		}

		// Declare result path
		std::vector<Node*> path;

		CoolPriorityQueue frontier;
		// Initialize Start node cost
		start->costSoFar = 0.0f;
		start->priority = 0.0f;
		frontier.put(start, start->priority);

		Node* current = nullptr;

		while (frontier.empty() == false)
		{
			// Get current node to process
			// Node is taken out of queue automatically
			current = frontier.get();

			// Early exit if we found goal
			if (current == end)
			{
				break;
			}

			// Get node neighbors
			std::vector<Node*> neighbors;
			// Get node neighbors
			if (useJPS == false)
			{
				neighbors = DynamicGrid_GetNeighbors(grid, current, allowDiagonals);
			}
			else
			{
				neighbors = DynamicGrid_GetJPSSuccessors(grid, current, start, end);
			}
			// For each neighbor
			for (int i = 0; i < (int)neighbors.size(); ++i)
			{
				// Calculate new cost to get to this neighbor
				float newCost = current->costSoFar + DynamicGrid_GetCost(grid, neighbors[i]);
				// Get current stored cost
				float currentCostSoFar = neighbors[i]->costSoFar;
				// If no stored cost or newCost is less than current
				if (currentCostSoFar == -1.0f || newCost < currentCostSoFar)
				{
					// Update Cost because we found a better path
					neighbors[i]->costSoFar = newCost;
					float heuristicValue = heuristicFunction(end->position, neighbors[i]->position);
					if (useBreakTie == true)
					{
						//heuristicValue *= (1.0f + P_PARAM);
						heuristicValue += PathfindingUtils::StraightLineTieBreak(neighbors[i]->position, 
																start->position, 
																end->position);
					}
					float priority = (gFactor * newCost) + (hFactor * heuristicValue);

					neighbors[i]->heuristic = heuristicValue;
					neighbors[i]->priority = priority;
					neighbors[i]->cameFrom = current;
					//Add neighbor to queue
					frontier.put(neighbors[i], priority);
				}
			}
		}


		if (current != end)
		{
			SDL_Log("No path found!\n");
			return path;
		}

		path.push_back(current);

		while (current != start)
		{
			current = current->cameFrom;
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());

		return path;
	}

	static void RenderPathfindingPath(DynamicGrid* grid, SDL_Renderer* renderer, std::vector<Node*>& path, Uint32 pathColor,
		Vector2D gridOrigin)
	{
		for (int i = 0; i < (int)path.size() - 1; ++i)
		{
			thickLineColor(renderer, gridOrigin.x + path[i]->position.x * grid->cellSize + grid->cellSize / 2,
				gridOrigin.y + path[i]->position.y * grid->cellSize + grid->cellSize / 2,
				gridOrigin.x + path[i + 1]->position.x * grid->cellSize + grid->cellSize / 2,
				gridOrigin.y + path[i + 1]->position.y * grid->cellSize + grid->cellSize / 2, 4, pathColor);
		}
	}
}
