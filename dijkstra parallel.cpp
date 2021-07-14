#include <stdio.h>
#include <omp.h>
#include<algorithm>
#define INFINITY 999999

int V,E;

// typedef keyword is used to assign a new name to any existing data-type but it can
// also be used to assign a new name to structure which is a user-defined datatype.
// Syntax: typedef current_name new_name;

typedef struct 
{
	int label;
	bool visited;	

} Vertex;

typedef struct
{
	int u;
	int v;

} Edge;

void printShortestPathLength(int *path_length)
{
	printf("\nVERTEX \tSHORTEST PATH LENGTH \n");
	int i;
	for(i = 0; i < V; i++)
	{
		printf("%d \t",i);
		if (path_length[i]<INFINITY)
			printf("%d\n",path_length[i]);
		else
			printf("Infinity\n");
	}
}


//Weight of the edge that connects Vertex u with Vertex v (direct edge)
int findEdgeWeight(Vertex u, Vertex v, Edge *edges, int *weights)
{

	int i;
	for(i = 0; i < E; i++)
	{

		if(edges[i].u == u.label && edges[i].v == v.label)
		{
			return weights[i];
		}
	}

	return INFINITY;
}

//Gets the minimum path length among all the paths (of unvisited nodes)
int minimimPathLength(int *path_length, Vertex *vertices)
{
	int i;
	int min_path_length = INFINITY;
	for(i = 0; i < V; i++)
	{
		if(vertices[i].visited == true)
		{
			continue;
		}
		
		else if(path_length[i] < min_path_length)
		{
			min_path_length = path_length[i];
			
		}
		
	}
	return min_path_length;
}


int minimimPathVertex(Vertex *vertices, int *path_length)
{
	int i;
	int min_path_length = minimimPathLength(path_length, vertices);
	
	// Get the unvisited vertex with the minimum path length
	//Mark it as visited
	for(i = 0; i < V; i++)
	{
		if(vertices[i].visited == false && path_length[vertices[i].label] == min_path_length)
		{
			vertices[i].visited = true;
			return i;
		}
	}
}

// Dijkstra Algorithm
void Dijkstra_Parallel(Vertex *vertices, Edge *edges, int *weights, Vertex *root)
{	

	double parallel_start, parallel_end;
	int path_length[V]; // we are creating a vector to store path-length to all the vertices from root node

	
	root->visited = true; // root node visited
	path_length[root->label] = 0; // path_length to self
	
	
	// Compute direct edge weight to all vertices
	for(int i = 0; i < V;i++)
	{

		if(vertices[i].label != root->label) 
		{
			path_length[vertices[i].label] = findEdgeWeight(*root, vertices[i], edges, weights);
		}
	}
	
	parallel_start = omp_get_wtime();
	for(int j = 0; j < V; j++)
	{
		Vertex u;
		
		// Obtain the vertex which has shortest distance from root node
		int h = minimimPathVertex(vertices, path_length);
		u = vertices[h];
		
		// There is something called dynamic teams that could still pick smaller
		// number of threads if the run-time system deems it more appropriate. 
		// You can disable dynamic teams by calling omp_set_dynamic(0) or 
		// by setting the environment variable	
		
		// https://stackoverflow.com/questions/11095309/openmp-set-num-threads-is-not-working

		//Update shortest path wrt new source 
		
		omp_set_dynamic(0); // to make sure desired num of threads are alloted
		omp_set_num_threads(4); // required number of threads
		#pragma omp parallel for schedule(static) private(i)
		for(int i = 0; i < V; i++)
		{
			if(vertices[i].visited == false)
			{	
				int c = findEdgeWeight( u, vertices[i], edges, weights);
				path_length[vertices[i].label] = std::min(path_length[vertices[i].label], path_length[u.label] + c);
			}
			
		}
	}
	//	printf("\nthreads = %d\n", omp_get_num_threads());
	parallel_end = omp_get_wtime();
	printShortestPathLength(path_length);
	printf("\nRunning time: %lf ms\n", (parallel_end - parallel_start)*1000);
}

int main()
{
	freopen("input.txt", "r", stdin);
	scanf("%d",&V);
	scanf("%d",&E);
	Vertex vertices[V]; // Array of struct vertex named vertices[]
	Edge edges[E]; // Array of struct edge named edges[]
	int weights[E]; // contains weights of edge 0,1,2,3.....
	
	// Vertex struct contains label and visit status
	// Edge struct contains starting and ending vertex
	// labeling starts from zero.

	int i;
	for(i = 0; i < V; i++)
	{
		Vertex a = { .label =i , .visited=false};
		vertices[i] = a;
	}


	int from,to,weight;
	for(i = 0; i < E; i++)
	{
		scanf("%d %d %d",&from,&to,&weight);
		Edge e = {.u = from , . v = to};
		edges[i] = e;
		weights[i] = weight;
	}

	int source;

	scanf("%d",&source);
	Vertex root = {source, false};

	Dijkstra_Parallel(vertices, edges, weights, &root);

	return 0;
}
