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


struct Board {
    char symbol;
    int vertexId;
};

class VertexCoordinates {
public:
    VertexCoordinates(int x, int y) {
        this->x = x;
        this->y = y;
    }

    VertexCoordinates() {
    }

    int x;
    int y;
};

class Edge {
public:
    Edge(int x, int y) {
        this->x = min(x, y);
        this->y = max(x, y);
    }

    int x;
    int y;

    bool operator<(const Edge &rhs) const {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);

    }
};

Board **board;
int W, H, L, K;
int NVERTICES = 0;
vector<set<int>> vertices;
vector<VertexCoordinates> vertexCoordinates;
set<Edge> edges;


void readFromCin() {
    string boardName;
    string line;

    cin >> W >> H >> L >> K;
    cin >> boardName;

    board = new Board *[W];
    for (int i = 0; i < W; ++i)
        board[i] = new Board[H];
//    board  = new Board*[H];
//    for(int i = 0; i < H; ++i)
//        board[i] = new Board[W];

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            char symbol;
            cin >> symbol;
            if (symbol != '.') {

                board[x][y].vertexId = NVERTICES;
                vertexCoordinates.push_back(VertexCoordinates(x, y));

                NVERTICES++;
            } else {
                board[x][y].vertexId = -1;
            }
            board[x][y].symbol = symbol;
        }
    }
}

void readFromInput(string filename) {
    string boardName;
    ifstream file(filename);
    string line;

    file >> W >> H >> L >> K;
    file >> boardName;

    board = new Board *[W];
    for (int i = 0; i < W; ++i)
        board[i] = new Board[H];
//    board  = new Board*[H];
//    for(int i = 0; i < H; ++i)
//        board[i] = new Board[W];

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            char symbol;
            file >> symbol;
            if (symbol != '.') {

                board[x][y].vertexId = NVERTICES;
                vertexCoordinates.push_back(VertexCoordinates(x, y));

                NVERTICES++;
            } else {
                board[x][y].vertexId = -1;
            }
            board[x][y].symbol = symbol;
        }
    }
    file.close();
}

void writeToCout(set<int> bestVertices) {
    for (auto v: bestVertices) {
        cout << vertexCoordinates[v].x << " " << vertexCoordinates[v].y << endl;
    }
}

bool isInsideTheBoard(int x, int y, int W, int H) {
    return (x >= 0 && x < W && y >= 0 && y < H);
}

void resolveNewEdge(int x, int y, char symbol1, char symbol2, int vertexId) {
    if (isInsideTheBoard(x, y, W, H)) {
        Board other = board[x][y];
        if (other.symbol == symbol1 || other.symbol == symbol2) edges.insert(Edge(vertexId, other.vertexId));
    }
}

void makeEdgesFromBoard() {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            char symbol = board[x][y].symbol;
            if (symbol == '.') continue;
            int vertexId = board[x][y].vertexId;

            switch (symbol) {
                case '+':
                    resolveNewEdge(x, y - 1, '+', '|', vertexId);
                    resolveNewEdge(x, y + 1, '+', '|', vertexId);
                    resolveNewEdge(x - 1, y, '+', '-', vertexId);
                    resolveNewEdge(x + 1, y, '+', '-', vertexId);
                    break;
                case '-':
                    resolveNewEdge(x - 1, y, '+', '-', vertexId);
                    resolveNewEdge(x + 1, y, '+', '-', vertexId);
                    break;
                case '|':
                    resolveNewEdge(x, y - 1, '+', '|', vertexId);
                    resolveNewEdge(x, y + 1, '+', '|', vertexId);
                    break;
            }
        }
    }
}


vector<set<int>> edgesToVertices(set<Edge> edges) {
    vector<set<int>> vertices;

    for (int i = 0; i < NVERTICES; i++) {
        vertices.push_back(set<int>());
    }

    for (auto edge : edges) {
        vertices[edge.x].insert(edge.y);
        vertices[edge.y].insert(edge.x);
    }
    return vertices;
}

//////////////////////Utils////////////////////////////////////////////////////////////////////////////////////////////////


vector<set<int>> verticesCopy(vector<set<int>> vertices) {
    vector<set<int>> vertices1;
    for (int i = 0; i < NVERTICES; i++) {
        vertices1.push_back(set<int>(vertices[i]));
    }
    return vertices1;
}

void verticesDelete(vector<set<int>> vertices) { //TODO: check

    for (int i = 0; i < NVERTICES; i++) {
        vertices[i].clear();
    }
    vertices.clear();
    vector<set<int>>().swap(vertices);
}

void writeToOutput(string filename, set<int> bestVertices) {
    ofstream file(filename);
    for (auto v: bestVertices) {
        file << vertexCoordinates[v].x << " " << vertexCoordinates[v].y << endl;
    }
    file.close();
}

void showVertices(vector<set<int>> vertices) {
    for (int v = 0; v < NVERTICES; v++) {
        printf("\n%d ->", v);

        for (auto i : vertices[v]) {
            cout << i << " ";
        }

    }
    cout << endl;
}

void showBoard(bool flag) {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            if (flag) {
                printf(" %c ", board[x][y].symbol);
            } else {
                if (board[x][y].vertexId == -1) {
                    printf(" . ");
                } else {
                    printf("%2d ", board[x][y].vertexId);
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}

//--------------------------------------------------------------------------------------

int transform(int x) {
    return x * 2;
}

void addSign(Board **board, Board **_board, int x, int y, int neigh_x, int neigh_y, char sign, int _W, int _H) {

    if (isInsideTheBoard(neigh_x, neigh_y, W, H) && isInsideTheBoard(transform(neigh_x), transform(neigh_y), _W, _H)) {
        int current = board[x][y].vertexId;
        int neigh = board[neigh_x][neigh_y].vertexId;
        if (vertices[current].find(neigh) != vertices[current].end()) { //is neighbour
            int tx = (int) (transform(neigh_x) + transform(x)) / 2;
            int ty = (int) (transform(neigh_y) + transform(y)) / 2;
            _board[tx][ty] = {sign, -2};
        } else {
            int tx = (int) (transform(neigh_x) + transform(x)) / 2;
            int ty = (int) (transform(neigh_y) + transform(y)) / 2;
            _board[tx][ty] = {' ', -2};

        }
    }
}


void showBoard(set<int> bestVertices) {

    int _W = transform(W);
    int _H = transform(H);

    Board **_board = new Board *[_W];
    for (int i = 0; i < _W; ++i)
        _board[i] = new Board[_H];

    for (int y = 0; y < _H; y++) {
        for (int x = 0; x < _W; x++) {
            _board[x][y] = {' ', -2};
        }
    }

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            _board[transform(x)][transform(y)] = board[x][y];
        }
    }


    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {

            addSign(board, _board, x, y, x - 1, y, '-', _W, _H);
            addSign(board, _board, x, y, x + 1, y, '-', _W, _H);
            addSign(board, _board, x, y, x, y - 1, '|', _W, _H);
            addSign(board, _board, x, y, x, y + 1, '|', _W, _H);

        }
    }

    cout << endl;
    for (int y = 0; y < _H; y++) {
        for (int x = 0; x < _W; x++) {
            if (_board[x][y].vertexId == -1) {
                printf("   ");
            } else if (_board[x][y].vertexId == -2) {
                printf(" %c ", _board[x][y].symbol);
            } else {
//                if(bestVertices==NULL){
//                    printf("%2d ", _board[x][y].vertexId);
//                }else{
                if (bestVertices.find(_board[x][y].vertexId) != bestVertices.end()) {
                    printf("\033[1;31m%2d \033[0m", _board[x][y].vertexId);
                } else {
                    printf("%2d ", _board[x][y].vertexId);
                }

            }
        }
        cout << endl;
    }
    cout << endl;
}



/////////////////////bfs based algorithm////////////////////////////////////////////////////////////////////////////////////////////////



bool
noCandidatesInNeighbourhood(vector<set<int>> &vertices1, vector<bool> &available, set<int> &candidates, int i, int L) {
    if (L <= 0) return true;

    set<int> neighbours(vertices1[i]);

    for (auto neigh:neighbours) {
        if (candidates.find(neigh) != candidates.end()) {
            return false;
        }
        return noCandidatesInNeighbourhood(vertices1, available, candidates, neigh, L - 1);
    }
}


bool noAvailableVertices(vector<set<int>> vertices1, vector<bool> available) {
    //if no available return candidates
    bool noAvailableVertex = true;
    for (int i = 0; i < vertices1.size(); i++) {
        if (available[i]) noAvailableVertex = false;
    }
    return noAvailableVertex;
}


int lowestDegreeRoot(vector<bool> available, vector<set<int>> vertices) {
    vector<int> availableNeighbours;


    for (int i = 0; i < vertices.size(); i++) {
        int number = 0;
        for (auto neigh : vertices[i]) {
            if (available[neigh]) number++;
        }
        availableNeighbours.push_back(number);
    }

    //from vertices with lowest degree select random
    int min = INFTY;
    for (int i = 0; i < vertices.size(); i++) {
        if (available[i] && availableNeighbours[i] < min) {
            min = availableNeighbours[i];
        }
    }
    set<int> chosenOnes;
    for (int i = 0; i < vertices.size(); i++) {
        if (available[i] && availableNeighbours[i] == min) chosenOnes.insert(i);
    }

    return *min_element(chosenOnes.begin(), chosenOnes.end());
}

void bfs(int start, vector<int> &candidates, vector<bool> &available) {

// bsf
    vector<int> distances;
    vector<int> colours;

    for (int i = 0; i < vertices.size(); i++) {
        colours.push_back(WHITE);
        distances.push_back(INFTY);
    }

    distances[start] = 0;
    colours[start] = GREY;

    deque<int> FIFO;
    FIFO.push_back(start);
    while (!FIFO.empty()) {
        int u = FIFO.front();
        FIFO.pop_front();
        for (auto neigh : vertices[u]) {
            // if(available[neigh]){//w przeciwnym razie nie ma o czym gadac...
            if (colours[neigh] == WHITE) {
                colours[neigh] = GREY;
                distances[neigh] = distances[u] + 1;
                FIFO.push_back(neigh);
            }
            //   }
        }
        colours[u] = BLACK;
    }

    candidates.push_back(start);

    for (int i = 0; i < distances.size(); i++) {
        if (distances[i] <
            L + 1) { //be carful trzeba przeskalowac L tak zeby odleglosc jak sa obok siebie wynosila 1 a nie 0
            available[i] = false;
        }
    }

//    set<int> aureola;
//    for(int i=0;i<distances.size();i++){
//        if(available[i] &&distances[i]==L+1){ //be carful trzeba przeskalowac L
//            aureola.insert(i);
//        }
//    }

//    for(auto i : aureola){
    if (noAvailableVertices(vertices, available)) return;

    bfs(lowestDegreeRoot(available, vertices), candidates, available);
//    }
}

int randomSubtreeRoot(vector<bool> available) {
    vector<int> theChosenOnes;
    for (int i = 0; i < available.size(); i++) {
        if (available[i]) theChosenOnes.push_back(i);
    }
    int randomIndex = rand() % theChosenOnes.size();
    return theChosenOnes[randomIndex];
}

vector<int> bfs_full() {
    vector<bool> available;
    vector<int> candidates;
    for (int i = 0; i < vertices.size(); i++) {
        available.push_back(true);
    }


    bool done = false;
    while (!done) { //do bfs for all subgraphs
        done = true;
        for (int i = 0; i < vertices.size(); i++) {
            if (available[i]) {
                done = false;
            }
        }
        if (done) break;
        bfs(randomSubtreeRoot(available), candidates, available); //bfs for one subgraph
    }

    return candidates;
}

vector<int> BFSalgorihm() {

    vector<int> bestCandidates;
    int bestK = 0;


    while (bestK < K) {
        vector<int> candidates = bfs_full();
        if (candidates.size() > bestK) {
            bestK = candidates.size();
            bestCandidates = candidates;
        }
    }

    vector<int> safePlaces;//zeby nie robic urzednikom klopotow
    for (int i = 0; i < K; i++) {
        safePlaces.push_back(bestCandidates[i]);
    }
    return safePlaces;
}


int main() {
    srand(time(0));

//    readFromInput("/home/x/DEVELOPER1/WORK/CLionProjects/izolacja/input.txt");
    readFromCin();
    makeEdgesFromBoard();
    vertices =edgesToVertices(edges);


    vector<int> candidates = BFSalgorihm();

    set<int> _candidates(candidates.begin(), candidates.end());
//    showBoard(_candidates);



    writeToCout(_candidates);
//    writeToOutput("output.txt",bestVertices);


    return 0;
}
