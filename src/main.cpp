#include <iostream>
#include <set>
#include <list>
#include <chrono>
#include <random>
#include <unistd.h>
#include "undirectedgraph.h"
#include "exceptions.h"

void display_usage()
{
  std::cout << "Usage : mwis INPUT [-h] [-p] [-g]"
            << std::endl;
  exit(1);
}

// Helper function
void path_mwis(const std::list<unsigned>& path, UndirectedGraph& g){
  std::cout << "* Path: ";
  for(auto step = path.begin(); step != path.end(); step++){
    std::cout << *step << " ; ";
  }
  std::cout << std::endl;

  std::list<unsigned> path_mwis = g.mwis_for_path(path);

  std::cout << "** Maximum weight independent set: ";
  for(auto step = path_mwis.begin(); step != path_mwis.end(); step++){
    std::cout << *step << " ; ";
  }
  std::cout << std::endl;
  std::cout << "** Weight of mwis: "
            << g.weight_of_set(path_mwis)
            << std::endl;
};

int main(int argc, char **argv){

  // Parsing command-line arguments
  struct globalArgs_t {
    // -p option
    bool path_example;
    // -g option
    unsigned graph_example_size;
  } globalArgs;

  globalArgs.path_example = false;
  globalArgs.graph_example_size = 0;
  
  const char* optString = "pg:h?";
  
  int opt = getopt(argc, argv, optString);

  while(opt != -1) {
    switch(opt) {
    case 'p':
      globalArgs.path_example = true;
      break;
    case 'g':
      globalArgs.graph_example_size = atoi(optarg);
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

  if(globalArgs.path_example){
    // Examples of a maximum weight independant set for a path
    UndirectedGraph g;

    g.add_vertex(1, 11);
    g.add_vertex(2, 30);
    g.add_vertex(3, 20);
    g.add_vertex(4, 1);
    g.add_vertex(5, 17);
    g.add_vertex(6, 20);
  
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    g.add_edge(3, 4);
    g.add_edge(4, 5);
    g.add_edge(5, 6);
    g.add_edge(2, 5);
    g.add_edge(5, 4);
    g.add_edge(4, 1);
    g.add_edge(1, 3);
    g.add_edge(3, 6);

    g.log();

    std::cout << "**************** MWIS for path ****************\n";

    // Example 1
    std::list<unsigned> path_12;
    path_12.push_back(1);
    path_12.push_back(2);

    path_mwis(path_12, g);
  
    // Example 2
    std::list<unsigned> path_23;
    path_23.push_back(2);
    path_23.push_back(3);

    path_mwis(path_23, g);

    // Example 3
    std::list<unsigned> path_123;
    path_123.push_back(1);
    path_123.push_back(2);
    path_123.push_back(3);

    path_mwis(path_123, g);
  
    // Example 4
    std::list<unsigned> path_1234;
    path_1234.push_back(1);
    path_1234.push_back(2);
    path_1234.push_back(3);
    path_1234.push_back(4);

    path_mwis(path_1234, g);
  
    // Example 5
    std::list<unsigned> path_12345;
    path_12345.push_back(1);
    path_12345.push_back(2);
    path_12345.push_back(3);
    path_12345.push_back(4);
    path_12345.push_back(5);

    path_mwis(path_12345, g);
  
    // Example 6
    std::list<unsigned> path_123456;
    path_123456.push_back(1);
    path_123456.push_back(2);
    path_123456.push_back(3);
    path_123456.push_back(4);
    path_123456.push_back(5);
    path_123456.push_back(6);

    path_mwis(path_123456, g);

    // Example 7
    std::list<unsigned> path_254136;
    path_254136.push_back(2);
    path_254136.push_back(5);
    path_254136.push_back(4);
    path_254136.push_back(1);
    path_254136.push_back(3);
    path_254136.push_back(6);

    path_mwis(path_254136, g);
  }

  if(globalArgs.graph_example_size > 0){
    // Examples of a maximum weight independant set approximation for
    // a graph

    unsigned size = globalArgs.graph_example_size;

    unsigned max_weight = 50;
    double edges_rate = 0.4;

    // Random generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    // Random distribution for vertices weight
    std::uniform_int_distribution<int> weight_distribution(1, max_weight);

    // Random distribution for choosing whether or not to add an edge
    // between two vertices
    std::uniform_real_distribution<double> uniform_distribution(0.0, 1.0);
    
    // Building a graph with "size" vertices
    UndirectedGraph g2;

    for(int i = 1; i <= size; i++){
      g2.add_vertex(i, weight_distribution(generator));
      for(int j = 1; j < i; j++){
        if(uniform_distribution(generator) < edges_rate){
          // Add some edges randomly
          g2.add_edge(i, j);
        }
      }
    }

    g2.log();

    // Minorant for weight of a mwis
    std::cout << "**************\n"
              << "* Minorant for alpha\n"
              << "** a mwis weigths at least: "
              << g2.alpha_minorant()
              << std::endl;

    // First approximation algorithm
    std::cout << "**************" << std::endl;
    
    std::list<unsigned> gwmin_is = g2.mwis_greedy_gwmin();

    std::cout << "* GWMIN greedy algorithm"
              << std::endl
              << "** Independent set: ";

    for(auto n = gwmin_is.begin(); n != gwmin_is.end(); n++){
      std::cout << *n << " ; ";
    }
    std::cout << std::endl;

    std::cout << "** Weight: " << g2.weight_of_set(gwmin_is) << std::endl;

    // Second approximation algorithm
    std::cout << "**************" << std::endl;
    std::list<unsigned> gwmax_is = g2.mwis_greedy_gwmax();

    std::cout << "* GWMAX greedy algorithm"
              << std::endl
              << "** Independent set: ";

    for(auto n = gwmax_is.begin(); n != gwmax_is.end(); n++){
      std::cout << *n << " ; ";
    }
    std::cout << std::endl;

    std::cout << "** Weight: " << g2.weight_of_set(gwmax_is) << std::endl;
  }
}
