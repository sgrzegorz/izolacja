#include <iostream>
#include <list>
#include <iterator>
#include <fstream>
#include <sstream>
#include <set>
#include <tuple>



using namespace std;

//class Vertex {
//public:
//    int x;
//    int y;
//
//    Vertex(int x, int y) {
//        this->x = x;
//        this->y = y;
//    }
//
//};
//
//class Graph {
//public:
//    list<Vertex> vertices;
//
//};

//struct vertex {
//    int x;
//    int y;
//    set<vertex> neighbours;
//
//};
//
//struct edge{
//    vertex u;
//    vertex v;
//};

struct Board{
    char symbol;
    int vertexId;
};

Board ** board;
int W, H, L, K;
int NVERTICES = 0;

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
class Vertex {
public:
    Vertex(int x,int y){
        this->x =x;
        this->y =y;
    }
    Vertex(){
    }
    int x;
    int y;
};

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
                NVERTICES++;
            }else{
                board[x][y].vertexId=-1;
            }
            board[x][y].symbol=symbol;
        }
    }
}

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


set<int> *vertices;
Vertex *verticesXY;
set<Edge> edges;


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






int main() {
    readFromInput("input.txt");
    makeEdgesFromBoard();






    showBoard(true);

    showBoard(false);




    cout<<"h";




    return 0;
}
