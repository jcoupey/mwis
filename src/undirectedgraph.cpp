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
    // Removing all edges to neighbours of given vertex
    std::list<unsigned> neighbours = this->neighbours_for_vertex(id);
    for(auto vertex = neighbours.begin();
        vertex != neighbours.end();
        vertex++){
      Edge target_edge (id, *vertex);
      _edges.erase(target_edge);
      // Updating neighbour degree
      _vertices.find(*vertex)->second._degree--;
    }
    
    // Removing vertex
    _vertices.erase(target_vertex);
  }
};

bool UndirectedGraph::has_vertex(unsigned vertex_id) const{
  return _vertices.find(vertex_id) != _vertices.end();
};

std::list<unsigned> UndirectedGraph::neighbours_for_vertex(unsigned vertex_id) const{
  if(! this->has_vertex(vertex_id)){
    throw ArgsErrorException("Not a valid vertex id!");
  }
  std::list<unsigned> neighbours;
  for(auto vertex_pair = _vertices.cbegin();
      vertex_pair != _vertices.cend();
      vertex_pair++){
    if(this->has_edge(vertex_id, vertex_pair->first)){
      neighbours.push_back(vertex_pair->first);
    }
  }
  return neighbours;
};

unsigned UndirectedGraph::weight_of_set(const std::list<unsigned>& vertex_set) const{
  unsigned weight = 0;
  for(auto vertex = vertex_set.begin(); vertex != vertex_set.end(); vertex++){
    auto vertex_in_map = _vertices.find(*vertex);
    if(vertex_in_map != _vertices.end()){
      weight += vertex_in_map->second._weight;
    }
  }
  return weight;
};

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
  unsigned vertex_ids_size = vertex_ids.size();
  // Case of an empty path or a path with a single vertex
  if(vertex_ids_size < 2){
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

  // Filtering the case where the path contains a cycle by removing
  // duplicates ids
  std::list<unsigned> vertex_ids_copy (vertex_ids);
  vertex_ids_copy.sort();
  vertex_ids_copy.unique();
  if(vertex_ids_size != vertex_ids_copy.size()){
    return false;
  }
  
  return true;
};

std::list<unsigned> UndirectedGraph::mwis_for_path(const std::list<unsigned>& path) const{
  if(! this->has_path(path)){
    throw ArgsErrorException("Not a valid path!");
  }

  const unsigned path_size = path.size();
  auto path_iter = path.cbegin();

  unsigned* max_weight;
  max_weight = new unsigned [path_size];

  if(path_size >= 1){
    // Max weight of an independent set for a path of length 1 is the
    // weight of the first vertex
    max_weight[0] = _vertices.find(*path_iter)->second._weight;
    path_iter++;
  }

  if(path_size >= 2){
    // Max weight of an independent set for a path of length 2 is the
    // max weight of the first two vertices
    max_weight[1] = std::max(max_weight[0],
                             _vertices.find(*path_iter)->second._weight);
    path_iter++;
  }

  // Computing maximum weight of an independent set for first i
  // elements of path
  for(int i = 2; i < path_size; i++){
    // Max weight unchanged if element i is not in a mwis for
    // path[0..i]. Else its weight is added to the max weight for an
    // independent set for path[0..i-2].
    max_weight[i] = std::max(max_weight[i - 1],
                             max_weight[i - 2]
                             + _vertices.find(*path_iter)->second._weight);
    path_iter++;
  }

  // Building the independent set backwards from the max weight
  // evolutions
  std::list<unsigned> mwis;
  // Browsing path backward
  auto path_backward_iter = path.crbegin();
  int index = path_size - 1;
  while(index >= 1){
    if(max_weight[index] == max_weight[index - 1]){
      // std::cout << index << " = ;  " << *path_backward_iter << std::endl;
      index--;
      ++path_backward_iter;
    }
    else{
      // std::cout << index << " != ; " << *path_backward_iter << std::endl;
      mwis.push_front(*path_backward_iter);
      index -= 2;
      ++path_backward_iter;
      ++path_backward_iter;
    }
  }
  if(index == 0){
    mwis.push_front(*path_backward_iter);
  }

  delete[] max_weight;
  return mwis;
};

std::list<unsigned> UndirectedGraph::mwis_greedy_gwmin(){
  // Copy of current objet to further restore its state, should
  // definitely be improved
  UndirectedGraph g (*this);

  // Independent set to return
  std::list<unsigned> is;

  while(this->number_of_vertices() > 0){
    auto vertex_iter = _vertices.cbegin();

    // Greedy choice for the vertex to add to independent set: the
    // selecting-rule picks biggest gwmin_value(). If values are
    // equal, the vertex that has the lowest degree is chosen (thus
    // removing less neighbours)

    unsigned chosen_vertex_id = vertex_iter->first;
    double chosen_vertex_value = vertex_iter->second.gwmin_value();
    double chosen_vertex_degree = vertex_iter->second._degree;

    ++vertex_iter;
    for(; vertex_iter != _vertices.cend(); ++vertex_iter){
      double current_value = vertex_iter->second.gwmin_value();
      double current_degree = vertex_iter->second._degree;

      if((current_value > chosen_vertex_value)
         or (current_value == chosen_vertex_value
             and chosen_vertex_degree > current_degree)){
        // Better choice for greedy algorithm
        chosen_vertex_id = vertex_iter->first;
        chosen_vertex_value = current_value;
      }
    }

    // Adding chosen vertex to the independent set
    is.push_back(chosen_vertex_id);
    
    // Removing the vertex and its neighbours from the graph
    std::list<unsigned> neighbours
      = this->neighbours_for_vertex(chosen_vertex_id);
    this->remove_vertex(chosen_vertex_id);
    for(auto vertex = neighbours.begin();
        vertex != neighbours.end();
        vertex++){
      this->remove_vertex(*vertex);
    }
    // Uncomment to log state of the updated graph with removed vertex
    // and neighbours
    // this->log();
  }
  
  // Restore the state of the current graph
  *this = g;
  
  return is;
};

std::list<unsigned> UndirectedGraph::mwis_greedy_gwmax(){
  // Copy of current objet to further restore its state, should
  // definitely be improved
  UndirectedGraph g (*this);

  while(this->number_of_edges() > 0){
    auto vertex_iter = _vertices.cbegin();

    // Greedy choice for the vertex to remove: the selecting-rule
    // picks lowest gwmax_value(). If values are equal, the vertex
    // that has the biggest degree is chosen (we try to get rid of all
    // edges here)

    unsigned chosen_vertex_id = vertex_iter->first;
    double chosen_vertex_value = vertex_iter->second.gwmax_value();
    double chosen_vertex_degree = vertex_iter->second._degree;
    
    ++vertex_iter;
    for(; vertex_iter != _vertices.cend(); ++vertex_iter){
      double current_value = vertex_iter->second.gwmax_value();
      double current_degree = vertex_iter->second._degree;

      if((current_value < chosen_vertex_value)
         or (current_value == chosen_vertex_value
             and current_degree > chosen_vertex_degree)){
        // Better choice for greedy algorithm
        chosen_vertex_id = vertex_iter->first;
        chosen_vertex_value = current_value;
      }
    }

    // Removing the vertex (and associated edges) from the graph
    this->remove_vertex(chosen_vertex_id);

    // Uncomment to log state of the updated graph with removed vertex
    // and neighbours
    // this->log();
  }

  // Independent set to return is the remaining vertices
  std::list<unsigned> is;

  for(auto vertex = _vertices.begin(); vertex != _vertices.end(); vertex++){
    is.push_back(vertex->first);
  }

  // Restore the state of the current graph
  *this = g;
  
  return is;
};

void UndirectedGraph::log() const{
  std::cout << "****************** Graph log ******************\n"
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
  std::cout << std::endl;
};
