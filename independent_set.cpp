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
#include <deque>

using namespace std;

#define WHITE 1
#define GREY 2
#define BLACK 3
#define INFTY 9999999


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

void writeToCout(set<int>bestVertices){
    for(auto v: bestVertices){
        cout << vertexCoordinates[v].x <<" "<< vertexCoordinates[v].y<<endl;
    }
}

bool isInsideTheBoard(int x,int y,int W,int H){
    return (x>=0 && x<W && y>=0 && y<H);
}

void resolveNewEdge(int x,int y,char symbol1,char symbol2, int vertexId){
    if(isInsideTheBoard(x,y,W,H)){
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

//--------------------------------------------------------------------------------------

int transform(int x){
    return x*2;
}

void addSign(Board **board,Board **_board,int x,int y,int neigh_x, int neigh_y, char sign,int _W, int _H){

    if( isInsideTheBoard(neigh_x,neigh_y,W,H) &&isInsideTheBoard(transform(neigh_x),transform(neigh_y),_W,_H)){
        int current = board[x][y].vertexId;
        int neigh = board[neigh_x][neigh_y].vertexId;
        if(vertices[current].find(neigh) != vertices[current].end()){ //is neighbour
            int tx = (int) (transform(neigh_x)+transform(x))/2;
            int ty = (int) (transform(neigh_y)+transform(y))/2;
            _board[tx][ty] = {sign, -2};
        }else{
            int tx = (int) (transform(neigh_x)+transform(x))/2;
            int ty = (int) (transform(neigh_y)+transform(y))/2;
            _board[tx][ty] = {' ',-2};

        }
    }
}


void showBoard(set<int>bestVertices){

    int _W = transform(W);
    int _H = transform(H);

    Board** _board  = new Board*[_W];
    for(int i = 0; i < _W; ++i)
        _board[i] = new Board[_H];

    for(int y=0;y<_H;y++) {
        for (int x = 0; x < _W; x++) {
            _board[x][y] = {' ',-2};
        }
    }

    for(int y=0;y<H;y++) {
        for (int x = 0; x < W; x++) {
            _board[transform(x)][transform(y)] = board[x][y];
        }
    }



    for(int y=0;y<H;y++) {
        for (int x = 0; x < W; x++) {

            addSign(board,_board,x,y,x-1,y,'-',_W,_H);
            addSign(board,_board,x,y,x+1,y,'-',_W,_H);
            addSign(board,_board,x,y,x,y-1,'|',_W,_H);
            addSign(board,_board,x,y,x,y+1,'|',_W,_H);

        }
    }

    cout<<endl;
    for(int y=0;y<_H;y++){
        for(int x=0;x<_W;x++){
            if(_board[x][y].vertexId==-1){
                printf("   ");
            }else if(_board[x][y].vertexId==-2){
                printf(" %c ", _board[x][y].symbol);
            }else{
//                if(bestVertices==NULL){
//                    printf("%2d ", _board[x][y].vertexId);
//                }else{
                if(bestVertices.find(_board[x][y].vertexId) != bestVertices.end()){
                    printf("\033[1;31m%2d \033[0m", _board[x][y].vertexId);
                }else{
                    printf("%2d ", _board[x][y].vertexId);
                }

            }
        }
        cout<<endl;
    }
    cout<<endl;
}



/////////////////////my independent set algorithm////////////////////////////////////////////////////////////////////////////////////////////////


void bfs(int start,vector<set<int>>&new_graph){

// bsf
    vector<int> distances;
    vector<int> colours;

    for(int i=0;i<NVERTICES;i++){
        colours.push_back(WHITE);
        distances.push_back(INFTY);
    }

    distances[start] =0;
    colours[start] = GREY;

    deque<int> FIFO;
    FIFO.push_back(start);
    while(!FIFO.empty()){
        int u = FIFO.front(); FIFO.pop_front();
        for(auto neigh : vertices[u]){
                if(colours[neigh] == WHITE){
                    colours[neigh] = GREY;
                    distances[neigh] = distances[u]+1;
                    FIFO.push_back(neigh);
                }

        }
        colours[u] =BLACK;
    }

    for(int i=0;i<NVERTICES;i++){
        if(i!=start && distances[i]<=L){
            new_graph[start].insert(i);
            new_graph[i].insert(start);
        }
    }
}

void removeVertex(vector<set<int>>& new_graph,int element,vector<bool>&available){
    if(not available[element]) cout<<"Removing already removed vertex"<<endl;

    for(int i=0;i<NVERTICES;i++){
        if(available[i] && (new_graph[i].find(element) != new_graph[i].end())){
            new_graph[i].erase(element);
        }
    }
    new_graph[element].clear();
    available[element] =false;

}


bool isGraphEmpty(vector<bool> available){
    for(int i=0;i<NVERTICES;i++){
        if(available[i]){
            return false;
        }
    }
    return true;
}

int getVertexWithDegreeZero(vector<set<int>> new_graph, vector<bool>available){
    for(int i=0;i<NVERTICES;i++){
        if(available[i] && new_graph[i].size()==1) {
            return i;
        }
    }
    return -1;
}


int getVertexWithDegreeOne(vector<set<int>> new_graph, vector<bool>available){
    for(int i=0;i<NVERTICES;i++){
        if(available[i] && new_graph[i].empty()) {
            return i;
        }
    }
    return -1;
}

int getVertexWithMinimumDegree(vector<set<int>> new_graph,vector<bool>available){
    int min=INFTY;
    int vertex =-56;
    for(int i=0;i<NVERTICES;i++){
        if(available[i] && not new_graph[i].empty()){
            if(new_graph[i].size() < min){
                min = new_graph[i].size();
                vertex = i;
            }
        }
    }
    return vertex;
}

void independent_set(vector<set<int>> &new_graph, vector<int> &iset,vector<bool> &available){

    if(isGraphEmpty(available)){
        cout<<"!"<<endl;
        return;
    }



    //handle zero degree vertices
    int vertex = getVertexWithDegreeZero(new_graph,available);
    if(vertex!=-1){
        iset.push_back(vertex);
        removeVertex(new_graph,vertex,available);

//        if(isGraphEmpty(available)){
//            cout<<"!"<<endl;
//            return;
//        }
        independent_set(new_graph,iset,available);
        return;
    }

    //handle one degree vertices
    vertex = getVertexWithDegreeOne(new_graph,available);
    if(vertex!=-1){
        set<int> neighbours(new_graph[vertex]);
        for(auto neigh : neighbours){
            removeVertex(new_graph,neigh,available);
        }
        removeVertex(new_graph,vertex,available);
        iset.push_back(vertex);


//        if(isGraphEmpty(available)){
//            cout<<"!"<<endl;
//            return;
//        }
        independent_set(new_graph,iset,available);
        return;
    }

    int vertexWithMinimumDegree = getVertexWithMinimumDegree(new_graph,available);

    //Assume vertexWithMinimumDegree is in the MIS –
    //remove vertexWithMinimumDegree and its neighbors, since the
    //neighbors can no longer be in the MIS.
    vector<set<int>>new_graph1 = verticesCopy(new_graph); vector<bool> available1(available);
    set<int> neighbours(new_graph[vertexWithMinimumDegree]);
    for(auto neigh : neighbours){
        removeVertex(new_graph1,neigh,available1);
    }
    removeVertex(new_graph1,vertexWithMinimumDegree,available1);

    vector<int> iset1(iset);
    iset1.push_back(vertexWithMinimumDegree);
    independent_set(new_graph1,iset1,available1);

    //Assume vertexWithMinimumDegree not in the MIS just remove vertexWithMinimumDegree
    vector<set<int>>new_graph2 = verticesCopy(new_graph); vector<bool> available2(available);
    removeVertex(new_graph2,vertexWithMinimumDegree,available2);
    vector<int> iset2(iset);
    independent_set(new_graph2,iset2,available2);


    if(iset1.size() > iset2.size()){
        new_graph = new_graph1;
        iset = iset1;
        available = available1;
    }else{
        new_graph = new_graph2;
        iset = iset2;
        available = available2;
    }


}


int main() {
    srand(time(0));

    readFromInput("/home/x/DEVELOPER1/WORK/CLionProjects/izolacja/input.txt");
//    readFromCin();
    makeEdgesFromBoard();
    vertices =edgesToVertices(edges);

    vector<set<int>> new_graph;
    for(int i=0;i<NVERTICES;i++){
        new_graph.push_back(set<int>());
    }



    for(int i=0;i<NVERTICES;i++){
        bfs(i,new_graph);
    }

    vector<bool> available;
    for(int i=0;i<NVERTICES;i++){
        available.push_back(true);
    }
    vector<int>candidates;
    independent_set(new_graph,candidates,available);




    set<int> _candidates(candidates.begin(), candidates.end());
    showBoard(_candidates);



//    writeToCout(_candidates);
//    writeToOutput("output.txt",bestVertices);
//    cout<<"";


    return 0;
}
