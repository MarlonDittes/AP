#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include "argtable3.h"
#include "node.h"
#include "functions.h"
#include "heap.h"

// "Real main function"
int mymain(int source, int target, std::string graphfile, std::string coordfile, std::string partitionfile, int partSize, std::string arcflagsfile, int mode){
    // Setup needed arrays
    auto EdgeListNM = readGraphFile(graphfile);
    auto& EdgeList = std::get<0>(EdgeListNM);
    int& n = std::get<1>(EdgeListNM);
    int& m = std::get<2>(EdgeListNM);

    auto graph = createAdjArr(EdgeList, n, m);

    auto nodeArray = readCoordFile(coordfile);
    computeDistances(EdgeList, nodeArray);
    initEdgeArcflags(EdgeList, partSize);
    readPartitionFile(partitionfile, nodeArray);

    //Start preprocessing with ArcFlags and timing it
    auto preProcStart = std::chrono::high_resolution_clock::now();

    if (arcflagsfile == ""){
        parallelComputeArcFlags(EdgeList, graph, nodeArray, n);
        saveArcFlags(EdgeList, partSize, partitionfile);
    } else {
        readArcFlags(EdgeList, arcflagsfile);
    }

    auto preProcDuration = std::chrono::high_resolution_clock::now() - preProcStart;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(preProcDuration).count();
    std::cout << "Time taken by ArcFlags Preprocessing: " << microseconds <<  " microseconds.\n";

    // Running and Timing Dijkstra
    allVisitedToFalse(nodeArray);
    auto dijkstraStart = std::chrono::high_resolution_clock::now();

    std::pair<std::vector<double>, std::vector<Edge*>> result;
    switch (mode){
        case 1:
            result = Dijkstra(source, target, graph, nodeArray);
            break;
        case 2:
            result = ArcFlagsDijkstra(source, target, graph, nodeArray);
            break;
        case 3:
            result = AStarDijkstra(source, target, graph, nodeArray);
            break;
        case 4:
            result = AStarArcFlagsDijkstra(source, target, graph, nodeArray);
            break;
        default:
            std::cout << "No mode selected. Select 1 for Dijkstra, 2 for ArcFlagsDijkstra, 3 for AStarDijkstra, 4 for AStarArcFlagsDijkstra." << std::endl;
            break;
    }

    // End Timer
    auto dijkstraDuration = std::chrono::high_resolution_clock::now() - dijkstraStart;
    microseconds = std::chrono::duration_cast<std::chrono::microseconds>(dijkstraDuration).count();
    std::cout << "Time taken by Dijkstra: " << microseconds <<  " microseconds.\n";

    return 0;
}

// Argtable Functionality
int main(int argc, char **argv) {
    const char* progname = "./practical";
    struct arg_int *source = arg_int0("s", "source-node",NULL,          "define the source node (default is 1)");
    struct arg_int *target = arg_int0("t", "target-node",NULL,          "define the target node (default is 2)");
    struct arg_file *graphfile = arg_file0("g",NULL,"<input>",          "graph file source");
    struct arg_file *coordfile = arg_file0("c",NULL,"<input>",          "coordinate file source");
    struct arg_file *partitionfile = arg_file0("p",NULL,"<input>",      "partition file source");
    struct arg_int *partSize = arg_int0("k", "k",NULL,          "size of the partititon");
    struct arg_file *arcflagsfile = arg_file0("a",NULL,"<input>",      "(optional) arcflags file source");
    struct arg_int *mode = arg_int0("m", "mode",NULL,          "select mode: Dijkstra (1), ArcFlagsDijkstra (2), AStarDijkstra (3), AStarArcFlagsDijkstra (4)");
    struct arg_lit *help  = arg_lit0(NULL,"help",             "print this help and exit");
    struct arg_end *end   = arg_end(20);
    void* argtable[] = {source,target,graphfile,coordfile,partitionfile,partSize,arcflagsfile,mode,help,end};
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

    // Falls ein ArcFlags File mit dazu gegeben wurde:
    if(arcflagsfile->count == 0){
        exitcode = mymain(source->ival[0], target->ival[0], graphfile->filename[0], coordfile->filename[0], partitionfile->filename[0], partSize->ival[0], "", mode->ival[0]);
    } else{
        /* normal case: take the command line options at face value */
        exitcode = mymain(source->ival[0], target->ival[0], graphfile->filename[0], coordfile->filename[0], partitionfile->filename[0], partSize->ival[0], arcflagsfile->filename[0], mode->ival[0]);
    }



    exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));

    return exitcode;
}
