//
// Created by matteo on 26/02/19.
//

#include <SimMatrixHandler.h>
#include <fstream>

#define MAXBUFSIZE ((int) 1e6)

Eigen::MatrixXd SimMatrixHandler::readMatrix(std::string &filename)
{
    unsigned int cols = 0, rows = 0;
    double buff[MAXBUFSIZE];

    std::ifstream infile;
    infile.open(filename);
    while(!infile.eof())
    {
        std::string line;
        std::getline(infile,line);

        unsigned int tmpCols = 0;
        std::stringstream stream(line);
        while(!stream.eof())
        {
            stream >> buff[cols*rows+tmpCols++];
        }

        if(tmpCols == 0) continue;

        if(cols == 0) cols = tmpCols;

        rows++;
    }

    infile.close();

    // Populate matrix with numbers
    Eigen::MatrixXd result(rows,cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result(i,j) = buff[cols*i+j];
        }
    }

    return result;
}

Eigen::MatrixXd
SimMatrixHandler::buildGraphSimMatrix(const Graph &g, const std::vector<unsigned int> &reducedNodeLabels,
                                      const Eigen::MatrixXd &labelsSimMatrix)
{
    unsigned int numNodes = boost::num_vertices(g);
    Eigen::MatrixXd graphSimMatrix(numNodes, numNodes);
    graphSimMatrix = Eigen::MatrixXd::Identity(numNodes,numNodes);

    for(auto nodeDsc : g.vertex_set())
    {
        boost::graph_traits<Graph>::adjacency_iterator aib,aie;

        for(boost::tie(aib,aie) = boost::adjacent_vertices(nodeDsc,g); aib != aie; ++aib)
        {
            double currSimValue = labelsSimMatrix(reducedNodeLabels[nodeDsc],reducedNodeLabels[*aib]);

            // Connected nodes with no similarity are given a small value to distinguish them to not connected nodes
            if(currSimValue == 0)
            {
                currSimValue = 0.01;
            }
            graphSimMatrix(nodeDsc,*aib) = currSimValue;
        }
    }

    return graphSimMatrix;
}

