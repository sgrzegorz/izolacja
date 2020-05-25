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

int main() {

//    ifstream infile("board");

    int W, H, L, K;
    string boardName;
    ifstream file("input.txt");
    string line;

    getline(file,line);
    istringstream iss(line);
    if (!(iss >> W >> H >> L >> K)) {
        cout<<"Incorrect board parameters!";
    }
    cout<<W<<endl<<H<<endl<<L<<endl<<K;

    getline(file,line);
    istringstream iss1(line);
    if (!(iss1 >> boardName)) {
        cout<<"Incorrect board name!";
    }

    board  = new char*[H];
    for(int i = 0; i < H; ++i)
        board[i] = new char[W];

    while (getline(file, line)) {
        istringstream iss(line);
        for(int i=0;i<W;i++){
            if (!(iss >> boardName)) {
                cout<<"Incorrect park map!";
            }
        }
    }




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
