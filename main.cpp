#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include "argtable3.h"
#include "node.h"
#include "functions.h"
#include "heap.h"
#include <fstream>

#ifndef ARC_SIZE
#define ARC_SIZE 32 // Standardgröße des Bitsets, wenn ARC_SIZE nicht definiert ist
#endif

// "Real main function"
int mymain(int source, int target, std::string graphfile, std::string testfile, int mode){
    std::string coordfile = graphfile + ".xyz";
    std::string partitionfile = graphfile + ".part" + std::to_string(ARC_SIZE);
    std::string arcflagsfile = partitionfile + ".arcfl";

    // Setup needed arrays
    auto EdgeListNM = readGraphFile(graphfile);
    auto& EdgeList = std::get<0>(EdgeListNM);
    int& n = std::get<1>(EdgeListNM);
    int& m = std::get<2>(EdgeListNM);

    auto graph = createAdjArr(EdgeList, n, m);

    auto nodeArray = readCoordFile(coordfile);
    computeDistances(graph, nodeArray);
    initEdgeArcflags(graph, ARC_SIZE);
    readPartitionFile(partitionfile, nodeArray);

    // get bottom left to top right
    /*int bottomLeft = 0;
    int topRight = 0;
    for (int i = 0; i < nodeArray.size(); i++){
        if (nodeArray[i].getX() <= nodeArray[bottomLeft].getX() && nodeArray[i].getY() <= nodeArray[bottomLeft].getY()){
            bottomLeft = i;
        }
        if (nodeArray[i].getX() >= nodeArray[topRight].getX() && nodeArray[i].getY() >= nodeArray[topRight].getY()){
            topRight = i;
        }
    }
    source = bottomLeft + 1;
    target = topRight + 1;*/

    //Start preprocessing with ArcFlags and timing it
    auto preProcStart = std::chrono::high_resolution_clock::now();

    std::ifstream file(arcflagsfile);
    if (!file.good()){
        parallelComputeArcFlags(EdgeList, graph, nodeArray, n);
        saveArcFlags(graph, ARC_SIZE, partitionfile);
    } else {
        readArcFlags(graph, arcflagsfile);
    }

    auto preProcDuration = std::chrono::high_resolution_clock::now() - preProcStart;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(preProcDuration).count();
    std::cout << "Time taken by ArcFlags Preprocessing: " << microseconds <<  " microseconds.\n";


    if(testfile == ""){
        // Running and Timing Dijkstra on one instance using source and target
        allVisitedToFalse(nodeArray);
        std::string type;
        auto dijkstraStart = std::chrono::high_resolution_clock::now();

        std::pair<std::vector<double>, std::vector<Edge*>> result;
        switch (mode){
            case 1:
                result = Dijkstra(source, target, graph, nodeArray);
                type = "Dijkstra";
                break;
            case 2:
                result = ArcFlagsDijkstra(source, target, graph, nodeArray);
                type = "ArcFlagsDijkstra";
                break;
            case 3:
                result = AStarDijkstra(source, target, graph, nodeArray);
                type = "AStarDijkstra";
                break;
            case 4:
                result = AStarArcFlagsDijkstra(source, target, graph, nodeArray);
                type = "AStarArcFlagsDijkstra";
                break;
            default:
                std::cout << "No mode selected. Select 1 for Dijkstra, 2 for ArcFlagsDijkstra, 3 for AStarDijkstra, 4 for AStarArcFlagsDijkstra." << std::endl;
                break;
        }

        // End Timer
        auto dijkstraDuration = std::chrono::high_resolution_clock::now() - dijkstraStart;
        microseconds = std::chrono::duration_cast<std::chrono::microseconds>(dijkstraDuration).count();

        std::cout << "Distance: " << result.first[target - 1] << std::endl;
        std::cout << "Time taken by " + type + ": " << microseconds <<  " microseconds.\n";

    } else {
        // Testing Dijkstra on multiple instances
        auto testPairs = readTestFile(testfile);
        int result;
        std::vector<std::pair<int, long long>> searchSpaceAndTime(testPairs.size());
        std::string type;

        std::cout << "Testing..." << std::endl;
        int i = 0;

        for (auto& pair : testPairs){
            if (i % 100 == 0){
                std::cout << i << " done..." << std::endl;
            }
            allVisitedToFalse(nodeArray);
            auto dijkstraStart = std::chrono::high_resolution_clock::now();

            switch (mode){
            case 1:
                result = searchSpaceDijkstra(pair.first, pair.second, graph, nodeArray);
                type = "Dijkstra";
                break;
            case 2:
                result = searchSpaceArcFlagsDijkstra(pair.first, pair.second, graph, nodeArray);
                type = "ArcFlagsDijkstra";
                break;
            case 3:
                result = searchSpaceAStarDijkstra(pair.first, pair.second, graph, nodeArray);
                type = "AStarDijkstra";
                break;
            case 4:
                result = searchSpaceAStarArcFlagsDijkstra(pair.first, pair.second, graph, nodeArray);
                type = "AStarArcFlagsDijkstra";
                break;
            default:
                std::cout << "No mode selected. Select 1 for Dijkstra, 2 for ArcFlagsDijkstra, 3 for AStarDijkstra, 4 for AStarArcFlagsDijkstra." << std::endl;
                break;
            }

            auto dijkstraDuration = std::chrono::high_resolution_clock::now() - dijkstraStart;
            microseconds = std::chrono::duration_cast<std::chrono::microseconds>(dijkstraDuration).count();

            searchSpaceAndTime[i] = std::make_pair(result, microseconds);
            i++;
        }
        saveResults(searchSpaceAndTime, testfile, ARC_SIZE, type);
    }




    return 0;
}

// Argtable Functionality
int main(int argc, char **argv) {
    const char* progname = "./practical";
    struct arg_int *source = arg_int0("s", "source-node",NULL,          "define the source node");
    struct arg_int *target = arg_int0("t", "target-node",NULL,          "define the target node");
    struct arg_file *graphfile = arg_file0("g",NULL,"<input>",          "graph file source");
    struct arg_file *testfile = arg_file0("f",NULL,"<input>",      "file with multiple instances, use instead of -s and -t");
    struct arg_int *mode = arg_int0("m", "mode",NULL,          "select: Dijkstra (1), ArcFlagsDijkstra (2), AStarDijkstra (3), AStarArcFlagsDijkstra (4)");
    struct arg_lit *help  = arg_lit0(NULL,"help",             "print this help and exit");
    struct arg_end *end   = arg_end(20);
    void* argtable[] = {source,target,graphfile,testfile,mode,help,end};
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
    exitcode = mymain(source->ival[0], target->ival[0], graphfile->filename[0], testfile->filename[0], mode->ival[0]);



    exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));

    return exitcode;
}
