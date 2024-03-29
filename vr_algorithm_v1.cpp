#include <iostream>
#include <list>
#include <iterator>
#include <fstream>
#include <sstream>
#include <set>
#include <tuple>
#include <algorithm>
#include <vector>
#include <random>

using namespace std;

struct Board{
    char symbol;
    int vertexId;
};

class VertexCoordinates {
public:
    VertexCoordinates(int x,int y){
        this->x =x;
        this->y =y;
    }
    VertexCoordinates(){
    }
    int x;
    int y;
};

class Edge{
public:
    Edge(int x,int y){
        this->x =min(x,y);
        this->y = max(x,y);
    }
    int x;
    int y;
    bool operator<(const Edge& rhs) const
    {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);

    }
};

Board ** board;
int W, H, L, K;
int NVERTICES = 0;
vector<set<int>>vertices;
vector<VertexCoordinates> vertexCoordinates;
set<Edge> edges;


void readFromCin(){
    string boardName;
    string line;

    cin >> W >> H >>L>>K;
    cin >>boardName;

    board  = new Board*[W];
    for(int i = 0; i < W; ++i)
        board[i] = new Board[H];
//    board  = new Board*[H];
//    for(int i = 0; i < H; ++i)
//        board[i] = new Board[W];

    for(int y=0;y<H;y++){
        for(int x=0;x<W;x++){
            char symbol;
            cin >> symbol;
            if(symbol!='.'){

                board[x][y].vertexId=NVERTICES;
                vertexCoordinates.push_back(VertexCoordinates(x,y));

                NVERTICES++;
            }else{
                board[x][y].vertexId=-1;
            }
            board[x][y].symbol=symbol;
        }
    }
}

void readFromInput(string filename){
    string boardName;
    ifstream file(filename);
    string line;

    file >> W >> H >>L>>K;
    file >>boardName;

    board  = new Board*[W];
    for(int i = 0; i < W; ++i)
        board[i] = new Board[H];
//    board  = new Board*[H];
//    for(int i = 0; i < H; ++i)
//        board[i] = new Board[W];

    for(int y=0;y<H;y++){
        for(int x=0;x<W;x++){
            char symbol;
            file >> symbol;
            if(symbol!='.'){

                board[x][y].vertexId=NVERTICES;
                vertexCoordinates.push_back(VertexCoordinates(x,y));

                NVERTICES++;
            }else{
                board[x][y].vertexId=-1;
            }
            board[x][y].symbol=symbol;
        }
    }
    file.close();
}



bool isInsideTheBoard(int x,int y){
    return (x>=0 && x<W && y>=0 && y<H);
}

void resolveNewEdge(int x,int y,char symbol1,char symbol2, int vertexId){
    if(isInsideTheBoard(x,y)){
        Board other = board[x][y];
        if(other.symbol==symbol1 || other.symbol==symbol2) edges.insert(Edge(vertexId,other.vertexId));
    }
}

void makeEdgesFromBoard(){
    for(int y=0;y<H;y++){
        for(int x=0;x<W;x++){
            char symbol = board[x][y].symbol;
            if(symbol=='.') continue;
            int vertexId = board[x][y].vertexId;

            switch (symbol) {
                case '+':
                    resolveNewEdge(x,y-1,'+','|',vertexId);
                    resolveNewEdge(x,y+1,'+','|',vertexId);
                    resolveNewEdge(x-1,y,'+','-',vertexId);
                    resolveNewEdge(x+1,y,'+','-',vertexId);
                    break;
                case '-':
                    resolveNewEdge(x-1,y,'+','-',vertexId);
                    resolveNewEdge(x+1,y,'+','-',vertexId);
                    break;
                case '|':
                    resolveNewEdge(x,y-1,'+','|',vertexId);
                    resolveNewEdge(x,y+1,'+','|',vertexId);
                    break;
            }
        }
    }
}


vector<set<int>> edgesToVertices(set<Edge> edges){
    vector<set<int>> vertices;

    for(int i=0;i<NVERTICES;i++){
        vertices.push_back(set<int>());
    }

    for (auto edge : edges){
        vertices[edge.x].insert(edge.y);
        vertices[edge.y].insert(edge.x);
    }
    return vertices;
}

//////////////////////Utils////////////////////////////////////////////////////////////////////////////////////////////////


vector<set<int>> verticesCopy(vector<set<int>> vertices){
    vector<set<int>> vertices1;
    for(int i=0;i<NVERTICES;i++){
        vertices1.push_back(set<int>(vertices[i]));
    }
    return vertices1;
}

void verticesDelete(vector<set<int>> vertices){ //TODO: check

    for(int i=0;i<NVERTICES;i++){
        vertices[i].clear();
    }
    vertices.clear();
    vector<set<int>>().swap(vertices);
}

void writeToOutput(string filename,set<int> bestVertices){
    ofstream file(filename);
    for(auto v: bestVertices){
        file << vertexCoordinates[v].x <<" "<< vertexCoordinates[v].y<<endl;
    }
    file.close();
}

void showVertices(vector<set<int>> vertices){
    for(int v=0;v<NVERTICES;v++){
        printf("\n%d ->",v);

        for(auto i : vertices[v]){
            cout<<i<<" ";
        }

    }
    cout<<endl;
}

void showBoard(bool flag){
    for(int y=0;y<H;y++){
        for(int x=0;x<W;x++){
            if(flag){
                printf(" %c ",board[x][y].symbol);
            } else{
                if(board[x][y].vertexId==-1){
                    printf(" . ");
                }else{
                    printf("%2d ", board[x][y].vertexId);
                }
            }
        }
        cout<<endl;
    }
    cout<<endl;
}


/////////////////////VRalgorithm////////////////////////////////////////////////////////////////////////////////////////////////

void handleVertexSelection( vector<set<int>>&vertices1, vector<bool> &available,int i, int L){ //TODO check
    if(L<=0) return;

    set<int> neighbours(vertices1[i]);

    for(auto neigh: neighbours){
        vertices1[i].erase(neigh);
        vertices1[neigh].erase(i);
    }

    for(auto neigh:neighbours){
        available[neigh]= false;
        if(L>0) handleVertexSelection(vertices1,available,neigh,L-1);
    }
}


int getRandomVertex(vector<bool> available){
    int maximum_number = available.size()-1;
    int minimum_number =0;
    const int randomVertex = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;

    if(available[randomVertex]) return randomVertex;
    int i = randomVertex+1;
    while(i!=randomVertex){
        i = i%available.size();
        if(available[i]) {
            return i;
        }
        i++;
    }
    cout<<"Shouldnt ever print this statement, no available vertex found";
    return -1;

}

set<int> VRalgorihmCandidates(){
    int L1=L;
    vector<set<int>> vertices1 =verticesCopy(vertices);
    vector<bool> available;
    for(int i=0;i<vertices1.size();i++){
        available.push_back(true);
    }
    set<int> candidates;

    while(true) {

        bool noAvailableVertex = true;
        for (int i = 0; i < vertices1.size(); i++) {
            if (available[i]) noAvailableVertex = false;
        }
        if (noAvailableVertex) return candidates;

        //selectAlones
        for (int i = 0; i < vertices1.size(); i++) {
            if (vertices1[i].empty() && available[i]) {
                available[i] = false;
                candidates.insert(i);
            }
        }

        //add random vertex to candidates
        int i = getRandomVertex(available);

        available[i] = false;
        candidates.insert(i);

        handleVertexSelection(vertices1, available, i, L1);
    }
}

set<int>  VRalgorihm(){

    set<int> bestVertices;
    int bestK=0;

    while(bestK<K){
        set<int> candidates = VRalgorihmCandidates();
        if(candidates.size() > bestK){
            bestK = candidates.size();
            bestVertices = candidates;
//            cout<<bestK<<endl;
        }

    }
    return bestVertices;
}




int main() {
    srand(time(0));

    readFromInput("input.txt");
//    readFromCin();
    makeEdgesFromBoard();
    vertices =edgesToVertices(edges);

    showBoard(true);

    showBoard(false);

    set<int>bestVertices = VRalgorihm();


//    for(auto v: bestVertices){
//        cout << vertexCoordinates[v].x <<" "<< vertexCoordinates[v].y<<endl;
//    }




    writeToOutput("output.txt",bestVertices);
//    cout<<"";


    return 0;
}
