#include <iostream>
#include <fstream>
#include <InputManager.h>
#include <CGAL/boost/graph/Dual.h>
#include <boost/graph/filtered_graph.hpp>
#include <stopwatch.h>
#include <GraphReducer.h>
#include <boost/graph/graph_utility.hpp>
#include <MeshReducer.h>
#include <GraphManipulator.h>
#include <SimMatrixHandler.h>
#include <SpectralClustering.h>
#include <GraphCoarsener.h>
#include <boost/graph/connected_components.hpp>
#include <CreatorChecker.h>
#include <TreeMetricsEvals.h>
#include "directedPartitioner/include/DirectedPartitioner.h"
#include "ADtreePartitioner/include/ADtypes.h"
#include "ADtreePartitioner/include/CreationManager.h"

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
    inputManager.readMeshFromOff("/home/matteo/Desktop/filtered/filtered/south.off", mesh);
    MeshReducer meshReducer;
    std::cout << "num faces " << mesh.num_faces() << std::endl;
    std::vector<unsigned int> miao(mesh.num_faces(),0);
    std::vector<unsigned int> ola(mesh.num_faces(),0);
    Stopwatch stoppy;
    stoppy.start();
    Graph miaso = meshReducer.reduceLabeledMesh(mesh,miao,ola);
    std::cout << stoppy.stop() << std::endl;
    //for(auto elem : ola)
      //  std::cout << elem << std::endl;

    boost::print_graph(miaso);

    /*GraphManipulator manipulator;
    std::unordered_map<gNodeDsc,std::deque<unsigned int>> map = manipulator.mapElementsToReducedGraph(miaso,ola);
    std::vector<unsigned int> reducedNodesLabels = manipulator.mapReducedGraphToLabels(map,miao);
    SimMatrixHandler simMatrixHandler;
    std::string matFilename("../data/simMatrixTest.txt");
    Eigen::MatrixXd simMat = simMatrixHandler.readMatrix(matFilename);
    Eigen::MatrixXd simGraph = simMatrixHandler.buildGraphSimMatrix(miaso,reducedNodesLabels,simMat);
    std::cout << simGraph << std::endl;*/
}

int main()
{
    Stopwatch STOPPY;
    ADnode* root = new ADnode(Mesh::face_index(0));
    CreationManager creationManager;
    creationManager.setSTCreatorVariant(SpanningTreeVariant::LtR);
    creationManager.setATCreatorVariant(AriadneTreeVariant::LtR);
    InputManager inputManagermesh;
    std::string inputm("/home/matteo/CLionProjects/manifold_splitting/data/Watermarking/bunny/bunny.off");
    std::string graphin("../data/medGraph.txt");
    Mesh mmm;
    Graph tinyGraph;
    inputManagermesh.readGraphFromEdgeList(graphin,tinyGraph);
    inputManagermesh.readMeshFromOff(inputm,mmm);
    STOPPY.start();
    creationManager.createFromGraph(tinyGraph,root);
    //creationManager.createFromMesh(mmm,root);
    std::cout << "AD-tree created in " << STOPPY.stop() << " seconds" << std::endl;

    CreatorChecker checker;
    if(checker.checkTreeIntegrity(root,boost::num_vertices(tinyGraph)))
        std::cout << "AD-tree built correctly" << std::endl;
    else
        std::cout << "AD-tree built incorrectly" << std::endl;

    ADnode *cursor(root);
    while(cursor)
    {
        std::cout << cursor->id_ << " -> ";
        cursor = cursor->next_;
    }
    std::cout << std::endl;

    TreeMetricsEvals treeMetricsEvals;
    treeMetricsEvals.evaluateTree(root,"");

    return 0;

    /*std::vector<std::string> olaa = {"_0.off","_1.off","_2.off","_3.off","_4.off","_5.off","_6.off","_7.off"};
    double value = 0;
    for(auto el : olaa) {
        InputManager im;
        std::string uffa = "/home/matteo/meshes/venus/MTP/venus" + el;
        //std::string uffa = "../data/partitions/venus/venus" + el;
        std::string in(uffa);
        Mesh min;
        im.readMeshFromOff(in, min);

        double counter = 0;
        for (auto fd : min.faces()) {
            CGAL::Face_around_face_iterator<Mesh> fafib, fafie;

            int op = 0;
            for (boost::tie(fafib, fafie) = CGAL::faces_around_face(min.halfedge(fd), min); fafib != fafie; ++fafib) {


                if (*fafib == Mesh::null_face()) {
                    op++;
                }
            }
            if(op > 0 && op != 3)
                counter++;
        }

        value = value + counter/min.num_faces();
        std::cout << counter << std::endl;
        std::cout << min.num_faces() << std::endl;

    }
    std::cout << value/8 << std::endl;
    return 0;*/

    InputManager inputManager1;
    //std::string input("/home/matteo/CLionProjects/manifold_splitting/data/Castle/castle.off");
    //std::string input("/home/matteo/Desktop/meshes/Castle.off");
    std::string input("../data/partitions/castle/castle2.off");
    Mesh mesho;
    inputManager1.readMeshFromOff(input,mesho);

    DirectedPartitioner directPartitioner;
    //directPartitioner.partitionMesh(mesho);
    std::vector<bool> isIn(mesho.num_faces(),true);
    Stopwatch stoppy;
    stoppy.start();
    std::vector<bool> op = directPartitioner.bisectMesh(mesho,Mesh::face_index(5000),isIn);
    std::cout << "Bisection in " << stoppy.stop() << std::endl;
    //return 0;

    Mesh m1 = mesho;
    Mesh m2 = mesho;

    int count1 = 0;
    int count2 = 0;
    for(int i = 0; i < op.size(); i++)
        if(op[i]) {
            m1.remove_face(Mesh::face_index(i));
            count1++;
        }
        else {
            m2.remove_face(Mesh::face_index(i));
            count2++;
        }

    std::cout << count1 << std::endl;
    std::cout << count2 << std::endl;
    std::stringstream sss;
    sss << "../data/partitions/castle/castle_3.off";
    std::ofstream outfiles(sss.str());
    outfiles << m1;
    std::stringstream sss2;
    sss2 << "../data/partitions/castle/castle_4.off";
    std::ofstream outfiles2(sss2.str());
    outfiles2 << m2;
    return 0;

    //for(int i = 0; i < op.size(); i++) op[i] = !op[i];
    std::ofstream outfile("../data/partitions.txt");
    int count = 0;
    for(auto lo : op)
        if(lo) {
            count++;
            outfile << 1 << std::endl;
        }
        else
            outfile << 0 << std::endl;
    std::cout << count << std::endl;
    return 0;

    //labeledGraphTest();
    //labeledMeshTest();
    Graph go(5);
    boost::add_edge(0,1,1,go);
    boost::add_edge(1,2,0.8,go);
    boost::add_edge(1,3,0.9,go);
    boost::add_edge(1,4,0.6,go);
    boost::add_edge(2,4,1,go);
    boost::add_edge(3,4,0.5,go);
    /*boost::property_map<Graph,boost::edge_weight_t>::type EdgeWeightMap = boost::get(boost::edge_weight_t(),go);
    std::pair<boost::graph_traits<Graph>::edge_iterator,boost::graph_traits<Graph>::edge_iterator> edgePair;
    for(edgePair = boost::edges(go);edgePair.first != edgePair.second; ++edgePair.first)
        std::cout << *edgePair.first << " " << EdgeWeightMap[*edgePair.first] << std::endl;

    boost::graph_traits<Graph>::edge_descriptor e1,e2;
    bool found1, found2;
    boost::tie(e1,found1) = boost::edge(1,2,go);
    boost::tie(e2,found2) = boost::edge(2,1,go);
    std::cout << (e1 == e2) << found1 << found2 << std::endl;

    boost::property_map<Graph,boost::edge_weight_t>::type weight = boost::get(boost::edge_weight,go);
    std::cout << boost::get(weight,e1) << std::endl;
    std::cout << boost::get(weight,e2) << std::endl;

    exit(0);*/
    /*GraphCoarsener graphCoarsener;
    Graph asd;
    graphCoarsener.coarsen(go,1,asd);
    return 0;*/

    Eigen::MatrixXd mami(5,5);
    /*mami << 0,1,0,0,0,
            1,0,1,1,1,
            0,1,0,1,0,
            0,1,1,0,1,
            0,1,0,1,0;*/
    mami << 0,1,0,0,0,
            1,0,0,0,0,
            0,0,0,0,0,
            0,0,0,0,0,
            0,0,0,0,0;
    Eigen::MatrixXd olo = Eigen::MatrixXd::Identity(1000,1000);
    Graph gasd(1000);
    Stopwatch ss;
    boost::add_edge(0,1,gasd);
    ss.start();
    std::vector<int> component(boost::num_vertices(gasd));
    size_t num_components = boost::connected_components(gasd,&component[0]);
    std::vector<int> miso(num_components);
    for(int i = 0; i < boost::num_vertices(gasd); ++i) {
        miso[component[i]]++;
    }
    std::cout << ss.stop() << std::endl;
    ss.start();
    std::cout << (olo.array() == 0).rowwise().all().count() << std::endl;
    std::cout << ss.stop() << std::endl;
    return 0;
    SpectralClustering normSpectralClustering;
    normSpectralClustering.spectralClustering(mami, 2);
    return 0;

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