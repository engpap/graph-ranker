#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 2147483648
#define DIM_LINE 1000000
#define DIM_LINE_2 20
char output[DIM_LINE];



typedef struct{ //elem dell'heap
    int name; //nome del relativo vertice
    long int dist; //distanza min corrente dal sorgente
}heapNode;

int heapSize=0;


unsigned int left(unsigned int i);
unsigned int right(unsigned int i);
unsigned int parent(unsigned int i);
void swap( heapNode* a,  heapNode* b,int* indexes,int pos1,int pos2);
void minHeapify(heapNode* hp, unsigned int i,int* indexes);
void heapDecreaseKey(heapNode *hp,unsigned int i, long int key, int* indexes);
void minHeapInsert(heapNode *hp, long int key,unsigned int dim_max,int *indexes);
heapNode heapExtractMin(heapNode* hp, int* indexes);


//RANK tramite un max-heap
typedef struct{
    unsigned int index;
    long int result;
}rank_node;

int rank_size=0;

void swapRank(rank_node* a, rank_node* b){
    rank_node temp=*a;
    *a=*b;
    *b=temp;
}

void rankHeapify(rank_node rank[], unsigned int i){ //per mantenere le proprietà di max-heap
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


void insertNode(rank_node rank[],unsigned int ind, long int res, unsigned int k){ //per inserire i nodi in classifica
    if(rank_size>=k){ //se lo heap è pieno, metto il nodo in cima in modo da togliere quello con path più grande
        if(rank[0].result>res){ //lo tolgo solo se quello che inserisco ha path più corto rispetto a quello presente, altrimenti non ottengo una classifica coerente
            rank[0].result=res;
            rank[0].index=ind;
            rankHeapify(rank,0);
        }
    }
    else{ //se lo heap non è pieno
        rank[rank_size].result=res;
        rank[rank_size].index=ind;
        rank_size++;
        if(rank_size>1) //se è presente più di un elem, è necessario mantenere le prop di max heap
            rankHeapify(rank,parent(rank_size-1));
    }
}

static int cont=0; //static in modo che non cambi valore da una chiamata e  l'altra di funzione, globale in modo che sia facilmente accessibile da main e funzioni
//serve per la funzione parser, per scorrere nella stringa di input

int my_atoi(const char* input){
    int i,out=0;
    for(i=0;input[i]!='\0';i++)
        out=out*10+input[i]-'0';
        //-'0' toglie 48 alla codifica ASCII in modo da convertire il carattere nel numero
        //out*10 serve quando il numero da convertire è >9
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
    temp= my_atoi(output); //converto in int
    return temp;
}




int main(){

    unsigned int contGraph=0;

    //get input "d" e "k"
    unsigned int d,k;
    char line[DIM_LINE];
    char line2[DIM_LINE_2];
    char* c; //per evitare warning fgets
    c=fgets(line2,DIM_LINE_2,stdin);
    if(c){} //per evitare warning fgets
    d=(unsigned int)parser(line2,' ');
    k=(unsigned int)parser(line2,'\n');
    cont=0;
    long int distances[d]; //array dove memorizzo gli indici in cui sono presenti i nodi nella PQ:
                            // es. in 0 c'è l'indice del relativo nodo nella coda di priorità
    int indexes[d];

    rank_node rank[k];
    heapNode minHeap[d];
    unsigned int g[d][d]; //grafo rapp tramite matrice di adiacenza

    //init rank
    for(unsigned int i=0;i<k;i++){
        rank[i].index=0;
        rank[i].result=0;
    }

    //inserimento grafi da input
    unsigned int i,j,w;
    char command1[DIM_LINE_2]="TopK";
    char command2[DIM_LINE_2]="AggiungiGrafo";

    //PRENDO IL COMANDO
    c=fgets(line,DIM_LINE,stdin);
    line[strlen(line)-1]='\0'; //per togliere \n

    while(c!=NULL){ //fintantochè non è EOF
        while(strcmp(line,command1) !=0){ //fintatochè il comando non è Topk
            if(strcmp(line,command2)==0){ //se il comando è AggiungiGrafo --> aggiungo grafo


                for(i=0;i<d;i++){  //scorro riga per riga
                    c=fgets(line,DIM_LINE,stdin); //leggo la riga
                    if(c){}

                    for(j=0;j<d;j++){   //scorro colonna per collona, tenendo fissa la riga

                        w=(unsigned int)parser(line,',');
                        g[i][j]=w;

                    }

                    cont=0;
                }
                //init per dijkstra
                for(i=0;i<d;i++){
                    minHeap[i].dist=0;
                    minHeap[i].name=-1;
                }

                //DIJKSTRA
                long int res=0;
                int bool=0;
                for(i=0;i<d;i++){
                    if(g[0][i]!=0)
                        bool=1;
                }
                if(bool!=0) {//se la prima riga della matrice ha tutti 0

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
                        if(u.dist==INF)    //altrimenti ci sono casi in cui il nodo non è raggiungibile e l'algoritmo dà valori non corretti
                            distances[u.name]=0;
                        else
                            distances[u.name]=u.dist;
                        j=0;

                        while(j<d){
                            if(g[u.name][j]!=0 && j!=u.name){ //altrimenti conta anche gli autoanelli ma questi sono tolti dalla coda
                                unsigned int v=j; //dest
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
                    //--------------
                }

                //insertion in classifica
                insertNode(rank,contGraph,res,k);

                contGraph++;
            }
            //CHECK NEXT COMMAND
            c=fgets(line,DIM_LINE,stdin);
            line[strlen(line)-1]='\0';
            if(c){}
        }
        //stampo la classifica
        for(i=0;i<contGraph && i<k;i++){
            printf("%d",rank[i].index);
            if(i!=k-1 && i!=contGraph-1)
                printf(" ");
        }
        printf("\n");


        //CHECK NEXT COMMAND
        c=fgets(line,DIM_LINE,stdin);
        line[strlen(line)-1]='\0';

    }
    return 0;
}
//END main



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

void minHeapify(heapNode *hp, unsigned int i,int* indexes){ //per rendere lo heap sempre un min-heap
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
    if (key > hp[i].dist) //devo aggiornare solo se il path è minore di quello salvato
        printf("error\n");
    else{
        hp[i].dist=key;
        while(i>0 && hp[parent(i)].dist > hp[i].dist) {
            swap(&hp[parent(i)],&hp[i],indexes,hp[parent(i)].name,hp[i].name); //passo anche i nomi in modo da aggiornare l'array indexes
            i=parent(i);
        }
    }
}


void minHeapInsert(heapNode *hp,long int key,unsigned int dim_max,int* indexes){
    heapSize++;
    if(heapSize<dim_max){ //check dim max
        hp[ heapSize ].dist=INF;
        heapDecreaseKey(hp,heapSize,key,indexes);
    }
}


heapNode heapExtractMin(heapNode* hp,int* indexes){
    if(heapSize<1){ //se non ci sono elem nel heap
        printf("error\n");
        exit(0);
    }
    heapNode min=hp[0]; //il min è la radice
    swap(&hp[0],&hp[heapSize-1],indexes,hp[0].name,hp[heapSize-1].name);
    heapSize--;
    minHeapify(hp,0,indexes);
    return min;
}

