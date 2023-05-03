#include <iostream>
#include <array>
#include <utility>
#include <vector>
#include <ctime>
#include <chrono>
#include <fstream>
#include <sstream>
#include "argtable3.h"

const size_t NODE_COUNT = 15606;
const size_t EDGE_COUNT = 45878;

// print graph (AdjArr)
void printGraph (std::pair<std::array<int, NODE_COUNT+1>, std::array<int, EDGE_COUNT>> adjArr){
    std::cout << "Printing Graph: \n";

    // print V
    std::cout << "V: ";
    std::cout << adjArr.first[0];

    for (int i = 1; i<adjArr.first.size(); i++){
        std::cout << ", " << adjArr.first[i];
    }
    std::cout << std::endl;

    // print E
    std::cout << "E: ";
    std::cout << adjArr.second[0];

    for (int i = 1; i<adjArr.second.size(); i++){
        std::cout << ", " << adjArr.second[i];
    }
    std::cout << std::endl;
}

void printGraphShort (std::pair<std::array<int, NODE_COUNT+1>, std::array<int, EDGE_COUNT>> adjArr, int length){
    std::cout << "Printing Graph: \n";

    // print V
    std::cout << "V: ";
    std::cout << adjArr.first[0];

    for (int i = 1; i<length; i++){
        std::cout << ", " << adjArr.first[i];
    }

    std::cout << std::endl;

    // print E
    std::cout << "E: ";
    std::cout << adjArr.second[0];

    for (int i = 1; i<length; i++){
        std::cout << ", " << adjArr.second[i];
    }

    std::cout << std::endl;
}

// create a graph data structure (AdjArr)
std::pair<std::array<int, NODE_COUNT+1>, std::array<int, EDGE_COUNT>> adjacencyArray (const std::vector<std::pair<int, int>>& EdgeList) {
    std::array<int, NODE_COUNT+1> V;
    std::array<int, EDGE_COUNT> E;

    // init V to be only 0's
    for (int i = 0; i<NODE_COUNT+1; i++){
        V[i] = 0;
    }

    // count how many edges each node has
    for (const auto& edge: EdgeList){
        V[edge.first]++;
    }

    // prefix sums for V
    for (int i = 1; i<NODE_COUNT+1; i++){
        V[i]+=V[i-1];
    }

    // init E
    for (const auto& edge: EdgeList){
        E[--V[edge.first]] = edge.second;
    }

    return std::make_pair(V, E);
}

// read a Graph File
// TODO: change NODE_COUNT, EDGE_COUNT system
void readGraphFile (std::string filename){
    std::string line;
    std::ifstream MyReadFile;
    MyReadFile.open(filename);

    if (MyReadFile){

    } else {
        std::cout << "Couldn't read graph file "<< filename << std::endl;
        return;
    }

    // Read Number of Nodes (n) and Number of Edges (m)
    getline (MyReadFile, line);
    std::istringstream iss(line);
    int n, m;
    iss >> n;
    iss >> m;
    std::cout << n << " " << m << "\n";

    // Read edges
    std::vector<std::pair<int, int>> EdgeList;
    while (getline (MyReadFile, line)){
        std::istringstream iss(line);
        int u, v;
        iss >> u;
        std::cout << u << "\n";
        u--;
        do {
            iss >> v;
            v--;
            EdgeList.push_back(std::make_pair(u, v));
        } while (iss);
    }

    std::pair<std::array<int, NODE_COUNT+1>, std::array<int, EDGE_COUNT>> graph = adjacencyArray(EdgeList);
    //printGraphShort(graph, 10);
}

int mymain(int source, int target, std::string graphfile, std::string coordfile){
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    readGraphFile(graphfile);

    /*
    // Example Edgelist
    std::vector<std::pair<int, int>> EdgeList;
    EdgeList.push_back(std::make_pair(0, 1));
    EdgeList.push_back(std::make_pair(0, 2));
    EdgeList.push_back(std::make_pair(1, 3));
    EdgeList.push_back(std::make_pair(1, 4));
    EdgeList.push_back(std::make_pair(2, 4));
    EdgeList.push_back(std::make_pair(4, 1));
    EdgeList.push_back(std::make_pair(4, 0));
    EdgeList.push_back(std::make_pair(4, 2));

    std::pair<std::array<int, NODE_COUNT+1>, std::array<int, EDGE_COUNT>> graph = adjacencyArray(EdgeList);
    printGraph(graph);  */

    // End Timer
    auto duration = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    std::cout << "Time taken by algorithm: " << microseconds <<  " microseconds.\n";
    return 0;
}

int main(int argc, char **argv) {
    const char* progname = "practical";
    struct arg_int *source = arg_int0("s", "source-node",NULL,          "define the source node (default is 1)");
    struct arg_int *target = arg_int0("t", "target-node",NULL,          "define the target node (default is 2)");
    struct arg_file *graphfile = arg_file0("g",NULL,"<input>",          "graph file name");
    struct arg_file *coordfile = arg_file0("c",NULL,"<input>",          "coordinate file name");
    struct arg_lit *help  = arg_lit0(NULL,"help",             "print this help and exit");
    struct arg_end *end   = arg_end(20);
    void* argtable[] = {source,target,graphfile,coordfile,help,end};
    int nerrors;
    int exitcode=0;

    /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0){
        /* NULL entries were detected, some allocations must have failed */
        printf("%s: insufficient memory\n",progname);
        exitcode=1;
        goto exit;
    }

     /* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0){
        printf("Usage: %s", progname);
        arg_print_syntax(stdout,argtable,"\n");
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        exitcode=0;
        goto exit;
    }

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
        {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout,end,progname);
        printf("Try '%s --help' for more information.\n",progname);
        exitcode=1;
        goto exit;
        }

    /* special case: uname with no command line options induces brief help */
    if (argc==1){
        printf("Try '%s --help' for more information.\n",progname);
        exitcode=0;
        goto exit;
    }

    /* normal case: take the command line options at face value */
    exitcode = mymain(source->ival[0], target->ival[0], graphfile->filename[0], coordfile->filename[0]);

    exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));

    return exitcode;
}
