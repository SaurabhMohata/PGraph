#include<bits/stdc++.h> 
using namespace std; 

#define MAX_VERTICES 550
#define MAX_EDGES 75000
#define MAXWEIGHT 200 
  
int main() 
{ 
    set<pair<int, int>> container; 
    set<pair<int, int>>::iterator it; 
 
    srand(time(NULL)); 
  
    int NUM;    
    int NUMEDGE; 
  
    for (int i=1; i<=1; i++) 
    { 
        NUM = MAX_VERTICES; 
  
        NUMEDGE = MAX_EDGES; 
  
        while (NUMEDGE > NUM*(NUM-1)/2) 
            NUMEDGE = 1 + rand() % MAX_EDGES; 
  
        printf("%d %d\n", NUM, NUMEDGE); 
  
        for (int j=1; j<=NUMEDGE; j++) 
        { 
            int a = rand() % NUM; 
            int b = rand() % NUM; 
            pair<int, int> p = make_pair(a, b); 
            pair<int, int> reverse_p = make_pair(b, a); 
  
            while (container.find(p) != container.end() || 
                    container.find(reverse_p) != container.end()) 
            { 
                a = rand() % NUM; 
                b = rand() % NUM; 
                p = make_pair(a, b); 
                reverse_p = make_pair(b, a); 
            } 
            
            container.insert(p); 
        } 
  
        for (it=container.begin(); it!=container.end(); ++it) 
        { 
            int wt = 1 + rand() % MAXWEIGHT; 
            printf("%d %d %d\n", it->first, it->second, wt); 
        } 
  
        container.clear(); 
        printf("\n"); 
  
    } 
    
    cout<<0<<"\n";
    return(0); 
} 
