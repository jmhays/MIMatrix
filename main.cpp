#include <iostream>
#include <getopt.h>
#include "MIUtils.h"
#include "MI.h"
#include <omp.h>

struct globalArgs_t {
    const char *inFileName;     /* -f option */
    const char *outFileName;    /* -o option */
    int numThreads;              /* -n option */
    const char *delimiter;      /* -d option */
    bool metric;                /* -m option*/
} globalArgs;

static const char *optString = "f:o:n:d:mh?";

void display_usage(void) {
    puts("\n========MI MATRIX CALCULATOR========\n");
    puts("Information - calculates MI matrix from input file");
    puts("-f\t\tFull path to input file.");
    puts("-d\t\tDelimiter of csv file");
    puts("-m\t\tMetric option");
    puts("-o\t\tOutput filename for MI matrix.");
    puts("-n\t\tInteger number of threads for parallel calculation");
    exit( EXIT_FAILURE );
}

using namespace std;

int main(int argc, char *argv[]) {
    cout << "\t====================\n"
            "\tMI MATRIX CALCULATOR\n"
            "\t====================" << endl;
    /*
     * OPTION PARSING
     */

    int opt = 0;

    // Initialize global arguments
    globalArgs.inFileName = NULL;
    globalArgs.outFileName = NULL;
    globalArgs.numThreads = 1;  // default is to run sequentially
    globalArgs.delimiter = ",";
    globalArgs.metric = 0;

    opt = getopt(argc, argv, optString);
    while (opt != -1) {
        switch (opt) {
            case 'f':
                globalArgs.inFileName = optarg;
                break;
            case 'o':
                globalArgs.outFileName = optarg;
                break;
            case 'n':
                globalArgs.numThreads = atoi(optarg);
                break;
            case 'd':
                globalArgs.delimiter = optarg;
                break;
            case 'm':
                globalArgs.metric = 1;
                break;
            case 'h':
            case '?':
                display_usage();
                break;
            default:
                break;
        }
        opt = getopt(argc, argv, optString);
    }

    /*
     * READ INPUT FILE
     */
    auto data = readCSV(globalArgs.inFileName,
                                       globalArgs.delimiter);

    auto numFeatures = data.size();

    /*
     * BEGIN MI CALCULATIONS
     */
    vector<vector<float>> miMatrix(numFeatures, vector<float>(numFeatures, 0));
    auto shardedIndices = shardIndices(numFeatures, globalArgs.numThreads);
    printf("Main sees %i available threads\n", omp_get_max_threads());
    printf("Starting a pool of %i threads\n", globalArgs.numThreads);
    clock_t t;
    t = clock();
    
#pragma omp parallel for
    for(int i = 0; i < globalArgs.numThreads; i++) {
        miMatrixBlock(data, miMatrix, shardedIndices[i], globalArgs.metric);
    }
    t = (clock() - t);
    printf("Time to complete all MI calculations: %f sec\n", (float)t/CLOCKS_PER_SEC);

    dumpCSV(globalArgs.outFileName, globalArgs.delimiter, miMatrix);

}
