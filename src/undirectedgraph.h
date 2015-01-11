#ifndef UNDIRECTEDGRAPH_H
#define UNDIRECTEDGRAPH_H

#include <iostream>
#include <algorithm>
#include <set>
#include <list>
#include <map>
#include <array>
#include "exceptions.h"

class UndirectedGraph{
private:
  // Vertex description
  class Vertex{
  public:
    const unsigned _weight;          // vertex weight
    unsigned _degree;                // vertex degree in the graph

    Vertex(const unsigned weight):
      _weight(weight),
      _degree(0){}

    double gwmin_value() const{
      return (double) _weight / (_degree + 1);
    }

    double gwmax_value() const{
      // Getting inf when _degree is zero is fine with further
      // comparisons
      return (double) _weight /(_degree * (_degree + 1));
    }
    
    // Print vertex information
    void log() const{
      std::cout << "Weight: " << _weight
                << " ; degree: " << _degree
                << " ; selecting value for GWMIN "
                << this->gwmin_value()
                << " ; selecting value for GWMAX "
                << this->gwmax_value()
        ;
    };
  };
  
  // Edge description
  class Edge{
  public:
    unsigned _first_vertex_id;
    unsigned _second_vertex_id;
    
    Edge(unsigned first_vertex_id,
         unsigned second_vertex_id):
      _first_vertex_id(std::min(first_vertex_id, second_vertex_id)),
      _second_vertex_id(std::max(first_vertex_id, second_vertex_id)){}
    
    // Operator used in a set<Edge>
    bool operator<(const Edge& rhs) const{
      return (this->_first_vertex_id < rhs._first_vertex_id)
        or ((this->_first_vertex_id == rhs._first_vertex_id)
            and (this->_second_vertex_id < rhs._second_vertex_id));
    };

    // Print edge information
    void log() const{
      std::cout << _first_vertex_id << "<->"
                << _second_vertex_id
                << " ; ";
    };
  };

  std::map<unsigned, Vertex> _vertices; // graph vertices mapped with their ids
  std::set<Edge> _edges;                // set of graph edges

public:

  UndirectedGraph();

  unsigned number_of_vertices() const;
  
  unsigned number_of_edges() const;

  void add_vertex(unsigned id, unsigned weight);

  void remove_vertex(unsigned id);

  bool has_vertex(unsigned vertex_id) const;

  std::list<unsigned> neighbours_for_vertex(unsigned vertex_id) const;

  unsigned weight_of_set(const std::list<unsigned>& vertex_set) const;

  void add_edge(unsigned first_vertex_id, unsigned second_vertex_id);

  void remove_edge(unsigned first_vertex_id, unsigned second_vertex_id);

  bool has_edge(unsigned first_vertex_id, unsigned second_vertex_id) const;
  
  bool has_path(const std::list<unsigned>& edge_ids) const;

  std::list<unsigned> mwis_for_path(const std::list<unsigned>& path) const;

  double alpha_minorant() const;
  
  std::list<unsigned> mwis_greedy_gwmin();

  std::list<unsigned> mwis_greedy_gwmax();
  
  void log() const;
};

#endif
