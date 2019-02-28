//
// Created by matteo on 14/02/19.
//

#ifndef MESHPARTITIONER_TYPES_H
#define MESHPARTITIONER_TYPES_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <boost/graph/adjacency_list.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point3;
typedef CGAL::Surface_mesh<Point3> Mesh;
typedef Mesh::face_index mFaceDsc;
typedef boost::property<boost::edge_weight_t,double> EdgeWeightProperty;
typedef boost::adjacency_list<boost::setS,boost::vecS,boost::undirectedS,boost::no_property,EdgeWeightProperty> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor gNodeDsc;

#endif //MESHPARTITIONER_TYPES_H
