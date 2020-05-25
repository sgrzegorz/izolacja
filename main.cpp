#include <iostream>
#include <list>
#include <iterator>
#include <fstream>
#include <sstream>


using namespace std;

class Vertex {
public:
    int x;
    int y;

    Vertex(int x, int y) {
        this->x = x;
        this->y = y;
    }

};

class Graph {
public:
    list<Vertex> vertices;

};

struct vertex {
    int x;
    int y;
};


char** board;
int W, H, L, K;
void showBoard(){
    for(int i=0;i<H;i++){
        for(int j=0;j<W;j++){
            cout<<board[i][j]<<" ";
        }
        cout<<endl;
    }
}


int main() {

//    ifstream infile("board");

    string boardName;
    ifstream file("input.txt");
    string line;

    file >> W >> H >>L>>K;
    file >>boardName;

    board  = new char*[W];
    for(int i = 0; i < W; ++i)
        board[i] = new char[H];

    for(int i=0;i<H;i++){
        for(int j=0;j<W;j++){
            file >> board[i][j];
        }
    }

    showBoard();
//    cout<<"h";



//
//    string line;
//
//
//    getline(infile, line);
//
//    cout<<"lineis"<<line;
//

//    istringstream iss(line);
//    if (!(iss >> W >> H >> L >> K)) {
//
//        cout<<"Error";
////        throw "Incorrect board parameters!";
//    } // error
//    cout << W;


//
//
//    while (std::getline(infile, line))
//    {
//        istringstream iss(line);
//        int a, b;
//        if (!(iss >> a >> b)) { break; } // error
//        cout<<a,b;
//
//        // process pair (a,b)
//    }

//    Graph g = Graph();
//    for(int i =0;i<10;i++){
//        g.vertices.push_back(Vertex(1,2));
//    }
//
//
//    for(it = g.begin(); it != g.end(); ++it)
//        cout << '\t' << *it;
//    cout << '\n';
//    Vertex v = Vertex(1,2);
//
//    cout<<v.x;
//    cout<<v.y;
//
//    cout<<"faifaf";
//    std::cout << "Hello, World!" << std::endl;

    return 0;
}
