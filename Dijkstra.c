// Dijkstra API implementation
// COMP2521 Assignment 2

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Dijkstra.h"
#include "Graph.h"
#include "PQ.h"

// Helper functions
static PredNode *new_pred_node(Vertex v);
static void free_pred_nodes(ShortestPaths sps, int index);

ShortestPaths dijkstra(Graph g, Vertex src) {
	ShortestPaths sps;
	sps.numNodes = GraphNumVertices(g);
	sps.src = src;

    // Create an array to store the shortest distance of each vertex 
    // from the source vertex
	sps.dist = malloc(sps.numNodes * sizeof(int));

    // Set all values in distance array to infinity, except the
    // source index
	for (int i = 0; i < sps.numNodes; i++) {
		if (i == src) {
			sps.dist[i] = 0;
		} else {
			sps.dist[i] = INFINITY;
		}
	}

    // Create an array of linked lists to store the predecessor of 
    // each vertex
	sps.pred = malloc(sps.numNodes * sizeof(PredNode));

    // Set all values in predecessor array to NULL
	for (int i = 0; i < sps.numNodes; i++) {
		sps.pred[i] = NULL;
	}

    // Create a priority queue to get the vertex with minimum distance 
    // from the source vertex
	PQ vSet = PQNew();
	PQInsert(vSet, src, 0);

	while (!PQIsEmpty(vSet)) {
        // Get the vertex with the minimum distance from the source vertex
		int v = PQDequeue(vSet);
        
		AdjList v_adj = GraphOutIncident(g, v);

		for (AdjList curr_v = v_adj; curr_v != NULL; curr_v = curr_v->next) {
            // Edge relaxation
			if (sps.dist[curr_v->v] > sps.dist[v] + curr_v->weight) {
                // Found a shorter path from the source to current vertex
				sps.dist[curr_v->v] = sps.dist[v] + curr_v->weight;
                // Check if array index is empty or not
				if (sps.pred[curr_v->v] == NULL) {
                    // Set the predecessor
					sps.pred[curr_v->v] = new_pred_node(v);
				} 
				else {
                    // Remove current linked list
					free_pred_nodes(sps, curr_v->v);
                    // Set the new predecessor
					sps.pred[curr_v->v] = new_pred_node(v);
				}
                // Insert the updated vertex into the priority queue with
                // its distance 
				PQInsert(vSet, curr_v->v, sps.dist[curr_v->v]);
			} else if (sps.dist[curr_v->v] == sps.dist[v] + curr_v->weight) {
                // Found another path from the source to current vertex with
                // the same shortest distance
				PredNode *curr = sps.pred[curr_v->v];

                // Append the predecessor to the end of the current vertex's
                // predecessor list
				while (curr->next != NULL) {
					curr = curr->next;
				}

				curr->next = new_pred_node(v);
				PQInsert(vSet, curr_v->v, sps.dist[curr_v->v]);
			}
		}
	}
	
	PQFree(vSet);
	return sps;
}

void showShortestPaths(ShortestPaths sps) {

}

void freeShortestPaths(ShortestPaths sps) {
    // Need to free the linked lists in the array
    // before freeing the whole array
	for (int i = 0; i < sps.numNodes; i++) {
		if (sps.pred[i] != NULL) free_pred_nodes(sps, i);
	}
	
	free(sps.pred);
	free(sps.dist);
}

			/////////////////////////////
			///// HELPER FUNCTIONS //////
			/////////////////////////////

// Create and initialize a new prednode
static PredNode *new_pred_node(Vertex v) {
	PredNode *new_pd = malloc(sizeof(*new_pd));
	new_pd->v = v;
	new_pd->next = NULL;

	return new_pd;
}

// Free the linked list of predecessors
static void free_pred_nodes(ShortestPaths sps, int index) {
	PredNode *curr = sps.pred[index];

	while (curr != NULL) {
		PredNode *temp = curr;
		curr = curr->next;
		free(temp);
	}
}

