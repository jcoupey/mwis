#include <iostream>
#include <set>
#include <list>
#include <chrono>
#include <unistd.h>
#include "undirectedgraph.h"

void display_usage()
{
  std::cout << "Usage : mwis INPUT [-h] [-v]"
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
  } globalArgs;

  globalArgs.output_file = "";
  globalArgs.verbose = false;
  
  const char* optString = "vh?";
  
  int opt = getopt(argc, argv, optString);

  while(opt != -1) {
    switch(opt) {
    // case 'o':
    //   globalArgs.output_file = optarg;
    //   break;
    case 'v':
      globalArgs.verbose = true;
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

  UndirectedGraph g;

  g.add_vertex(1, 11);
  g.add_vertex(2, 30);
  g.add_vertex(3, 1);
  g.add_vertex(4, 1);
  g.add_vertex(5, 1);
  
  g.add_edge(1, 2);
  g.add_edge(2, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 5);

  if(globalArgs.verbose){
    g.log();
  }

  std::list<unsigned> path;
  path.push_back(1);
  path.push_back(2);
  path.push_back(3);
  path.push_back(4);
  path.push_back(5);

  std::cout << g.has_path(path) << std::endl;
}
