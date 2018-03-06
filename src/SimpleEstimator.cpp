//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"
#include <list>
#include <map>
#include <cmath>

std::map<uint32_t , cardStat> est_result;
int n = 0;
int noItems = 1000;

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
}

void SimpleEstimator::prepare() {

    // do your prep here

    int edgesPerLabel [graph->getNoLabels()];
    //int inDegreePerLabel [graph->getNoLabels()];
    //int outDegreePerLabel [graph->getNoLabels()];
    //std::list<uint32_t > uniqueLabels;

    int randomIndices [graph->getNoVertices() - noItems];

    for(int noLabels = 0; noLabels < graph->getNoLabels(); noLabels++) {
        est_result[noLabels] = cardStat {0, 0, 0};
        edgesPerLabel[noLabels] = 0;
    }

    std::cout << std::endl << graph->adj.size() << std::endl;
    std::cout << std::endl << graph->getNoVertices() << "; " << graph->getNoEdges() << "; " << graph->getNoLabels() << "; " << graph->getNoDistinctEdges() << std::endl;

    for (int i = 0; i < graph->getNoVertices() - noItems; i++) {
        randomIndices[i] = rand() % graph->getNoVertices();
        std::cout << randomIndices[i] << "; ";

        graph->adj[randomIndices[i]].clear();
        graph->reverse_adj[randomIndices[i]].clear();

        for (auto labelSource : graph->adj[i]) {
            edgesPerLabel[labelSource.first]++;
        }
    }

    std::cout << std::endl << graph->adj.size() << std::endl;
    std::cout << std::endl << graph->getNoVertices() << "; " << graph->getNoEdges() << "; " << graph->getNoLabels() << "; " << graph->getNoDistinctEdges() << std::endl;

    /*for (int source = 0; source < graph->getNoVertices(); source++) {
        for (auto labelSource : graph->adj[source]) {
            edgesPerLabel[labelSource.first]++;
        }

        for (auto labelTarget : graph->reverse_adj[source]) {
        }
    }*/

    //std::cout << std::endl << edgesPerLabel[0] << "; " << edgesPerLabel[1] << "; " << edgesPerLabel[2] << "; " << edgesPerLabel[3] << std::endl;

}

cardStat SimpleEstimator::estimate(RPQTree *q) {

    // perform your estimation here

    return cardStat {0, 0, 0};
}