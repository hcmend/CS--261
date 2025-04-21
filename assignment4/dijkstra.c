/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name: Hailey Mendenhall
 * Email: mendenhh@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "pq.h"

#define DATA_FILE "airports.dat"
#define START_NODE 0

//this is the struct for one path step. includes dest node, node it came from, and path cost
struct PathStep{
	int node;
	int cost;
	int prev;
};

//fucntion to create pathstep 
struct PathStep* create_path_step (int node, int cost, int prev){ 
	struct PathStep* ps = malloc(sizeof(struct PathStep)); //allocates memory for it
	//assigns all info passed in to it
	ps->node = node;
	ps->cost = cost;
	ps->prev = prev;
	return ps;
}

int main(int argc, char const *argv[]) {
	/*
	 * Open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;
	FILE* file = fopen(DATA_FILE, "r");
	fscanf(file, " %d %d ", &n_nodes, &n_edges);
	
	//creates n by n cost matrix
	int** cost_matrix = malloc(n_nodes * sizeof(int*));
	for(int i = 0; i < n_nodes; i++){
		cost_matrix[i] = malloc(n_nodes * sizeof(int));
		for(int j = 0; j < n_nodes; j++){
			cost_matrix[i][j] = 0;	//initializes all to 0	
		}
	}
	//gets costs from file and updates all the one that have paths between them
	for(int i = 0; i < n_edges; i++){
		int u, v, cost;
		fscanf(file, "%d %d %d ", &u, &v, &cost);
		cost_matrix[u][v] = cost;		
	}	

	fclose(file);

	//creates array of pathsteps
	struct PathStep* paths[n_nodes];
	for(int i = 0; i < n_nodes; i++){
		paths[i] = create_path_step(i, INT_MAX, -1); //sets up all to be at i node, infinity cost cause simple path may not exist, and previous nodes to undefined
	}
	paths[START_NODE]->cost = 0; //cost to get to starting node is always zero

	struct pq* queue = pq_create(); //creates priority queue
	pq_insert(queue, paths[START_NODE], 0); //inserts starting node and puts its priority to zero

	while(!pq_isempty(queue)){ 
		//gets the starting node and its info
		struct PathStep* curr = pq_remove_first(queue); 
		int current_node = curr->node;
		int current_cost = curr->cost;

		for(int neighbor = 0; neighbor < n_nodes; neighbor++){ //goes through every neighbor 
			if(cost_matrix[current_node][neighbor] != 0){ //checks if that path's costs has been changed from 0 i.e. there was a direct path between the 2
				int new_cost = current_cost + cost_matrix[current_node][neighbor]; //new_cost = currnet + that cost between neighbor and current
				if (new_cost < paths[neighbor]->cost){ //if that new cost is a smaller path then what is current there
					paths[neighbor]->cost = new_cost; //set that cost to the neighbor's cost
					paths[neighbor]->prev = current_node; //set its prev to current node we came from
					pq_insert(queue, paths[neighbor], new_cost); //insert that neighbor and cost we found into priority queue
				}
			}
		}
	}
	//prints out the info we found
	for(int i = 0; i < n_nodes; i++){
		printf("Cost to node %d : %d -- Previous Node: %d\n", i, paths[i]->cost, paths[i]->prev);
	}
	for(int i = 0; i < n_nodes; i++){
		free(paths[i]);
	}
	for(int i = 0; i < n_nodes; i++){
		free(cost_matrix[i]);
	}
	free(cost_matrix);
	pq_free(queue);
	return 0;
}
