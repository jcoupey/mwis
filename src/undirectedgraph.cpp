#include "undirectedgraph.h"

UndirectedGraph::UndirectedGraph(){}

unsigned UndirectedGraph::number_of_vertices() const{
  return _vertices.size();
};

unsigned UndirectedGraph::number_of_edges() const{
  return _edges.size();
};

void UndirectedGraph::add_vertex(unsigned id, unsigned weight){
  // Nothing done if some vertex already have this id
  _vertices.emplace(id, weight);
};

void UndirectedGraph::add_edge(unsigned first_vertex_id,
                               unsigned second_vertex_id){
  auto first_vertex_map = _vertices.find(first_vertex_id);
  auto second_vertex_map = _vertices.find(second_vertex_id);
  if((first_vertex_map != _vertices.end())
     and (second_vertex_map != _vertices.end())){
    // Only if given ids are valid vertices ids
    _edges.emplace(first_vertex_id, second_vertex_id);
    // Updating vertices degrees
    first_vertex_map->second._degree++;
    second_vertex_map->second._degree++;
  }
};

void UndirectedGraph::log() const{
  std::cout << "********** Graph log **********\n"
    << "* Vertices:\n";
  for(auto vertex = _vertices.begin(); vertex != _vertices.end(); vertex++){
    std::cout << "** Id: "
              << vertex->first
              << std::endl
              << "*** ";
    vertex->second.log();
    std::cout << std::endl;
  }
  std::cout << "* Edges:\n";
  for(auto edge = _edges.begin(); edge != _edges.end(); edge++){
    edge->log();
  }
  std::cout << "*******************************\n";
};
