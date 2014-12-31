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

void UndirectedGraph::remove_vertex(unsigned id){
  auto target_vertex = _vertices.find(id);
  if(target_vertex != _vertices.end()){
    // Removing vertex
    _vertices.erase(target_vertex);
    for(auto edge = _edges.begin(); edge != _edges.end(); edge++){
      // Finding edges that should be removed as well
      std::map<unsigned, Vertex>::iterator vertex_pair = _vertices.end();
      if(edge->_first_vertex_id == id){
        vertex_pair = _vertices.find(edge->_second_vertex_id);
      }
      if(edge->_second_vertex_id == id){
        vertex_pair = _vertices.find(edge->_first_vertex_id);
      }
      if(vertex_pair != _vertices.end()){
        // Current edge should be removed
        _edges.erase(edge);
        // Updating vertices degrees
        vertex_pair->second._degree--;
      }
    }
  }
};

bool UndirectedGraph::has_vertex(unsigned vertex_id) const{
  return _vertices.find(vertex_id) != _vertices.end();
}

void UndirectedGraph::add_edge(unsigned first_vertex_id,
                               unsigned second_vertex_id){
  if(first_vertex_id == second_vertex_id){
    return;
  }
  auto first_vertex_pair = _vertices.find(first_vertex_id);
  auto second_vertex_pair = _vertices.find(second_vertex_id);
  if((first_vertex_pair != _vertices.end())
     and (second_vertex_pair != _vertices.end())){
    // Only if given ids are valid vertices ids
    _edges.emplace(first_vertex_id, second_vertex_id);
    // Updating vertices degrees
    first_vertex_pair->second._degree++;
    second_vertex_pair->second._degree++;
  }
};

void UndirectedGraph::remove_edge(unsigned first_vertex_id,
                                  unsigned second_vertex_id){
  // Removing edge
  Edge target_edge (first_vertex_id, second_vertex_id);
  _edges.erase(target_edge);
  // Updating degrees
  auto first_vertex_pair = _vertices.find(first_vertex_id);
  auto second_vertex_pair = _vertices.find(second_vertex_id);
  first_vertex_pair->second._degree--;
  second_vertex_pair->second._degree--;
};

bool UndirectedGraph::has_edge(unsigned first_vertex_id,
                               unsigned second_vertex_id) const{
  Edge target_edge (first_vertex_id, second_vertex_id);
  auto target = _edges.find(target_edge);
  return target != _edges.end();
};

bool UndirectedGraph::has_path(const std::list<unsigned>& vertex_ids) const{
  // Case of an empty path or a path with a single vertex
  if(vertex_ids.size() < 2){
    return vertex_ids.empty()
      or this->has_vertex(vertex_ids.front());
  }
  // General case
  auto first_id_iter = vertex_ids.cbegin();
  auto second_id_iter = ++vertex_ids.cbegin();
  while(second_id_iter != vertex_ids.end()){
    if(! this->has_edge(*first_id_iter, *second_id_iter)){
      // No edge exists between current two vertices
      return false;
    }
    first_id_iter++;
    second_id_iter++;
  }
  return true;
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
  std::cout << std::endl << "*******************************\n";
};
