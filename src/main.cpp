#include <iostream>
#include <set>
#include <chrono>
#include <unistd.h>
#include "undirectedgraph.h"

void display_usage()
{
  std::cout << "Usage : biosolver INPUT [-h] [-v] [-g]"
            << std::endl;//[-o OUTPUT]" );
  exit(1);
}

int main(int argc, char **argv){

  // Parsing command-line arguments
  struct globalArgs_t {
    // -o option
    std::string output_file;
    // -v option
    bool verbose;
    // -g option
    bool gpx;
  } globalArgs;

  globalArgs.output_file = "";
  globalArgs.verbose = false;
  globalArgs.gpx = false;
  
  const char* optString = "gvh?";
  
  int opt = getopt(argc, argv, optString);

  while(opt != -1) {
    switch(opt) {
    // case 'o':
    //   globalArgs.output_file = optarg;
    //   break;
    case 'v':
      globalArgs.verbose = true;
      break;
    case 'g':
      globalArgs.gpx = true;
      break;
    case 'h': 
    case '?':
      display_usage();
      break;
        
    default:
      /* You won't actually get here. */
      break;
    }
    
    opt = getopt(argc, argv, optString);
  }

  UndirectedGraph g;

  g.add_vertex(1, 11);
  g.add_vertex(2, 30);
  g.add_vertex(3, 1);
  g.add_vertex(4, 1);
  
  g.add_edge(1, 2);
  g.add_edge(2, 3);
  g.add_edge(2, 4);
  g.add_edge(4, 2);

  if(globalArgs.verbose){
    g.log();
  }
  
}
