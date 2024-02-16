#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 2147483648
#define DIM_LINE 1000000
#define DIM_LINE_2 20
char output[DIM_LINE];

// Heap node structure for priority queue
typedef struct {
    int name; // Name of the related vertex
    long int dist; // Current minimum distance from the source
} heapNode;

int heapSize=0;

unsigned int left(unsigned int i);
unsigned int right(unsigned int i);
unsigned int parent(unsigned int i);
void swap( heapNode* a,  heapNode* b,int* indexes,int pos1,int pos2);
void minHeapify(heapNode* hp, unsigned int i,int* indexes);
void heapDecreaseKey(heapNode *hp,unsigned int i, long int key, int* indexes);
void minHeapInsert(heapNode *hp, long int key,unsigned int dim_max,int *indexes);
heapNode heapExtractMin(heapNode* hp, int* indexes);

// Rank node structure for max heap
typedef struct{
    unsigned int index;
    long int result;
}rank_node;

int rank_size=0;

void swapRank(rank_node* a, rank_node* b);
void rankHeapify(rank_node rank[], unsigned int i);
void insertNode(rank_node rank[],unsigned int ind, long int res, unsigned int k);

// Static such that it does not change value from one call to another of the function, global such that it is easily accessible from main and functions
// It is used by the parser function to keep track of the position in the input string
static int cont=0; 

int my_atoi(const char* input);
int parser(const char* s, char delim);

int main(){

    unsigned int contGraph=0;

    // Get input "d" e "k"
    unsigned int d,k;
    char line[DIM_LINE];
    char line2[DIM_LINE_2];
    char* c; // Avoid warning for fgets
    c=fgets(line2,DIM_LINE_2,stdin);
    if(c){} // Avoid warning for fgets
    d=(unsigned int)parser(line2,' ');
    k=(unsigned int)parser(line2,'\n');
    cont=0;
    long int distances[d]; // Array to store the index of the nodes in the priority queue, e.g. in 0 there is the index of the relative node in the priority queue
    int indexes[d];

    rank_node rank[k];
    heapNode minHeap[d];
    unsigned int g[d][d]; // Graph represented as an adjacency matrix

    // Init rank
    for(unsigned int i=0;i<k;i++){
        rank[i].index=0;
        rank[i].result=0;
    }

    // Insertion of graphs from input
    unsigned int i,j,w;
    char command1[DIM_LINE_2]="TopK";
    char command2[DIM_LINE_2]="AggiungiGrafo";

    c=fgets(line,DIM_LINE,stdin);
    line[strlen(line)-1]='\0'; // Remove newline

    while(c!=NULL){ // As not as it is not EOF
        while(strcmp(line,command1) !=0){ // As long as the command is not "TopK"
            if(strcmp(line,command2)==0){ // If the command is "AggiungiGrafo" then insert the graph

                for(i=0;i<d;i++){  // Iteration over the rows
                    c=fgets(line,DIM_LINE,stdin); // Read the row
                    if(c){}

                    for(j=0;j<d;j++){  // Iteration over the columns, keeping row fixed

                        w=(unsigned int)parser(line,',');
                        g[i][j]=w;

                    }

                    cont=0;
                }
                // Init for Dijkstra
                for(i=0;i<d;i++){
                    minHeap[i].dist=0;
                    minHeap[i].name=-1;
                }

                // DIJKSTRA
                long int res=0;
                int bool=0;
                for(i=0;i<d;i++){
                    if(g[0][i]!=0)
                        bool=1;
                }
                if(bool!=0) { // If the first row of the matrix has all 0

                    heapSize=0;
                    indexes[0]=0;
                    minHeap[0].name=0;
                    minHeap[0].dist=0;
                    minHeapInsert(minHeap,minHeap[0].dist, d,indexes);

                    for(i=1;i<d;i++){
                        indexes[i]=(int)i;
                        minHeap[i].name=(int)i;
                        minHeap[i].dist=INF;
                        minHeapInsert(minHeap,minHeap[i].dist,d,indexes);
                    }
                    while(heapSize!=0){
                        heapNode u;
                        u=heapExtractMin(minHeap,indexes);
                        if(u.dist==INF)  // Otherwise there are cases where the node is not reachable and the algorithm gives incorrect values
                            distances[u.name]=0;
                        else
                            distances[u.name]=u.dist;
                        j=0;

                        while(j<d){ 
                            if(g[u.name][j]!=0 && j!=u.name){ // Otherwise it counts also the self loops, but these are removed from the queue
                                unsigned int v=j; // Destination vertex
                                unsigned int weight=g[u.name][j];
                                int z;
                                z=indexes[v];
                                if( minHeap[z].dist > u.dist + weight ){
                                    minHeap[z].dist = u.dist + weight;
                                    heapDecreaseKey(minHeap,z,minHeap[z].dist,indexes);
                                }
                            }
                            j++;
                        }
                    }

                    for(i=1;i<d;i++)
                        res+=distances[i];
                }

                // Insert the graph into the rank
                insertNode(rank,contGraph,res,k);

                contGraph++;
            }
            // Check next command
            c=fgets(line,DIM_LINE,stdin);
            line[strlen(line)-1]='\0';
            if(c){}
        }
        // Print the rank
        for(i=0;i<contGraph && i<k;i++){
            printf("%d",rank[i].index);
            if(i!=k-1 && i!=contGraph-1)
                printf(" ");
        }
        printf("\n");

        // Check next command
        c=fgets(line,DIM_LINE,stdin);
        line[strlen(line)-1]='\0';

    }
    return 0;
}
// END main

// HEAP FUNCTIONS

unsigned int left(unsigned int i){
    return 2*i+1;
}

unsigned int right(unsigned int i){
    return 2*i+2;
}

unsigned int parent(unsigned int i){
    return (int)((i-1)/2);
}

void swap( heapNode* a,  heapNode* b,int* indexes, int pos1,int pos2){
    heapNode temp = *a;
    *a=*b;
    *b=temp;
    int tempINT=indexes[pos1];
    indexes[pos1]=indexes[pos2];
    indexes[pos2]=tempINT;

}

void minHeapify(heapNode *hp, unsigned int i,int* indexes){ // To keep the heap as min-heap 
    unsigned int l=left(i);
    unsigned int r=right(i);
    unsigned int min;
    if (l< heapSize && hp[l].dist < hp[i].dist )
        min=l;
    else
        min=i;
    if(r<heapSize && hp[r].dist < hp[min].dist)
        min=r;
    if(min!=i){
        swap(&hp[i],&hp[min],indexes,hp[i].name,hp[min].name);
        minHeapify(hp,min,indexes);
    }
}

void heapDecreaseKey(heapNode *hp, unsigned int i, long int key, int* indexes){
    if (key > hp[i].dist) // Update only if the path is less than the saved one
        printf("error\n");
    else{
        hp[i].dist=key;
        while(i>0 && hp[parent(i)].dist > hp[i].dist) {
            swap(&hp[parent(i)],&hp[i],indexes,hp[parent(i)].name,hp[i].name); // Pass the names as input so that the indexes array is updated
            i=parent(i);
        }
    }
}


void minHeapInsert(heapNode *hp,long int key,unsigned int dim_max,int* indexes){
    heapSize++;
    if(heapSize<dim_max){ // Check max dimension
        hp[ heapSize ].dist=INF;
        heapDecreaseKey(hp,heapSize,key,indexes);
    }
}

heapNode heapExtractMin(heapNode* hp,int* indexes){
    if(heapSize<1){ // If there are no elements in the heap
        printf("error\n");
        exit(0);
    }
    heapNode min=hp[0]; // The minimum is the root
    swap(&hp[0],&hp[heapSize-1],indexes,hp[0].name,hp[heapSize-1].name);
    heapSize--;
    minHeapify(hp,0,indexes);
    return min;
}

/// RANK HEAP FUNCTIONS

void swapRank(rank_node* a, rank_node* b){
    rank_node temp=*a;
    *a=*b;
    *b=temp;
}

// To keep max-heap properties
void rankHeapify(rank_node rank[], unsigned int i){ 
    unsigned int l=left(i);
    unsigned int r=right(i);
    unsigned int max;
    if (l< rank_size && rank[l].result > rank[i].result )
        max=l;
    else
        max=i;
    if(r<rank_size && rank[r].result > rank[max].result)
        max=r;
    if(max!=i){
        swapRank(&rank[i],&rank[max]);
        rankHeapify(rank,max);
    }
}

// To insert nodes into the rank
void insertNode(rank_node rank[],unsigned int ind, long int res, unsigned int k){
    if(rank_size>=k){ // If the heap is full, insert node as root so that the node with largest path is removed
        if(rank[0].result>res){ // Remove only if node that is being inserted has a shorter path compared to the existing one; otherwise, no coherent ranking is obtained
            rank[0].result=res;
            rank[0].index=ind;
            rankHeapify(rank,0);
        }
    }
    else{ // If the heap is not full, insert node as a leaf and then heapify
        rank[rank_size].result=res;
        rank[rank_size].index=ind;
        rank_size++;
        if(rank_size>1) // If there is only one node, no need to heapify
            rankHeapify(rank,parent(rank_size-1));
    }
}

// PARSER FUNCTIONS

int my_atoi(const char* input){
    int i,out=0;
    for(i=0;input[i]!='\0';i++)
        out=out*10+input[i]-'0';
        // - '0' removes 48 from the ASCII code to convert the character into the number
        // out*10 is used when the number to convert is >9
    return out;
}

int parser(const char* s, char delim){
    if(s[cont]=='\0' || s[cont]=='\n')
        return -1;
    int i=0;
    int temp;
    while(s[cont]!=delim && s[cont]!='\0' && s[cont]!='\n'){
        output[i]=s[cont];
        i++;
        cont++;
    }
    output[i]='\0';
    cont++;
    temp= my_atoi(output); // Convert string to int
    return temp;
}