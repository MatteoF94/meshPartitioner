#include <iostream>
#include <InputManager.h>
#include <CGAL/boost/graph/Dual.h>
#include <boost/graph/filtered_graph.hpp>
#include <stopwatch.h>
#include <GraphReducer.h>
#include <boost/graph/graph_utility.hpp>
#include <MeshReducer.h>
#include <GraphManipulator.h>

typedef CGAL::Dual<Mesh> Dual;
typedef boost::graph_traits<Dual>::edge_descriptor edge_descriptor;

template <typename G>
struct noborder{
    noborder() : g(NULL) {}

    explicit noborder(G& g) : g(&g) {}

    bool operator() (const edge_descriptor& e) const {
        return !is_border(e,*g);
    }

    G* g;
};

typedef boost::filtered_graph<Dual,noborder<Mesh>> FiniteDual;

void labeledGraphTest()
{
    GraphReducer lb;
    std::vector<unsigned int> miao = {0,1,2,1,2,0,0,1,0,0,1,2};
    std::vector<unsigned int> ola(12,0);
    Graph gg(12);
    boost::add_edge(0,5,gg);
    boost::add_edge(0,6,gg);
    boost::add_edge(0,7,gg);
    boost::add_edge(5,6,gg);
    boost::add_edge(5,4,gg);
    boost::add_edge(5,2,gg);
    boost::add_edge(6,2,gg);
    boost::add_edge(4,2,gg);
    boost::add_edge(4,3,gg);
    boost::add_edge(4,1,gg);
    boost::add_edge(2,3,gg);
    boost::add_edge(1,3,gg);
    boost::add_edge(8,9,gg);
    boost::add_edge(8,10,gg);
    boost::add_edge(8,11,gg);
    boost::add_edge(9,10,gg);
    boost::add_edge(9,11,gg);
    boost::add_edge(10,11,gg);
    Graph miaso = lb.reduceLabeledGraph(gg, miao, ola);
    for(auto elem : ola)
        std::cout << elem << std::endl;

    boost::print_graph(miaso);
}

void labeledMeshTest()
{
    InputManager inputManager;
    Mesh mesh;
    inputManager.readMeshFromOff("../data/flat.off", mesh);
    std::cout << mesh.num_faces() << std::endl;
    MeshReducer meshReducer;
    std::vector<unsigned int> miao = {0,1,2,1,2,0,0,1,0,0,0,1,2};
    std::vector<unsigned int> ola(13,0);
    Graph miaso = meshReducer.reduceLabeledMesh(mesh,miao,ola);
    //for(auto elem : ola)
      //  std::cout << elem << std::endl;

    //boost::print_graph(miaso);

    GraphManipulator manipulator;
    manipulator.mapElementsToReducedGraph(miaso,ola);
}

int main() {

    //labeledGraphTest();
    //labeledMeshTest();

    InputManager inputManager;
    Mesh mesh;
    Stopwatch stopwatch;
    stopwatch.start();
    inputManager.readMeshFromOff("../data/bunny.off", mesh);
    std::cout << stopwatch.stop() << std::endl;
    return 0;
    std::cout << mesh.num_faces() << std::endl;
    Dual dual(mesh);
    FiniteDual finiteDual(dual,noborder<Mesh>(mesh));

    CGAL::Surface_mesh<Point3> mesh1;
    stopwatch.start();
    Graph g(mesh.num_faces());
    CGAL::Face_around_face_iterator<Mesh> fafBegin,fafEnd;
    for(Mesh::face_index fi : mesh.faces())
    for (boost::tie(fafBegin,fafEnd)=CGAL::faces_around_face(mesh.halfedge(fi),mesh); fafBegin != fafEnd; ++fafBegin) {
        boost::add_edge(fi,*fafBegin,g);
    }
    std::cout << stopwatch.stop() << std::endl;

    return 0;
}