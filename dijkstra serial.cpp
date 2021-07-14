#include <stdio.h>
#include <omp.h>
#include<algorithm>
#define INFINITY 999999

int V,E;

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
	
	for(i = 0; i < V; i++)
	{
		if(vertices[i].visited == false && path_length[vertices[i].label] == min_path_length)
		{
			vertices[i].visited = true;
			return i;
		}
	}
}


void Dijkstra_Serial(Vertex *vertices, Edge *edges, int *weights, Vertex *root)
{	

	double serial_start, serial_end;
	int path_length[V]; 

	
	root->visited = true;
	path_length[root->label] = 0;
	
	for(int i = 0; i < V;i++)
	{

		if(vertices[i].label != root->label) 
		{
			path_length[vertices[i].label] = findEdgeWeight(*root, vertices[i], edges, weights);
		}
	}
	
	serial_start = omp_get_wtime();
	for(int j = 0; j < V; j++)
	{
		Vertex u;
		
		int h = minimimPathVertex(vertices, path_length);
		u = vertices[h];	

		for(int i = 0; i < V; i++)
		{
			if(vertices[i].visited == false)
			{	
				int c = findEdgeWeight( u, vertices[i], edges, weights);
				path_length[vertices[i].label] = std::min(path_length[vertices[i].label], path_length[u.label] + c);
			}
		}
	}
	serial_end = omp_get_wtime();
	printShortestPathLength(path_length);
	printf("\nRunning time: %lf ms\n", (serial_end - serial_start)*1000);
}

int main()
{
	freopen("input.txt", "r", stdin);
	scanf("%d",&V);
	scanf("%d",&E);
	Vertex vertices[V]; 
	Edge edges[E];
	int weights[E];
	
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

	Dijkstra_Serial(vertices, edges, weights, &root);

	return 0;
}
