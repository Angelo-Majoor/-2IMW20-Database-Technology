//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"
#include <list>
#include <map>
#include <cmath>

std::map<uint32_t , cardStat> first;

int factorial(int n)
{
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
}

void SimpleEstimator::prepare() {

    // do your prep here

    for(int noLabels = 0; noLabels < graph -> getNoLabels(); noLabels++) {
        cardStat test {0,0,0};
        first[noLabels] = test;
    }

    int n = graph->getNoVertices();
    int outDegrees [n];
    int inDegrees [n];

    for (int j = 0; j < n; j++) {
        outDegrees[j] = 0;
        inDegrees[j] = 0;
    }

    int maxDegree = 0;
    int vertexWithMaxDegree = -1;

    int sources = 0;
    int targets = 0;
    int m = graph->getNoEdges();
    int z = 0;
    int z2 = 0;
    int l = 5;

    for(int source = 0; source < graph->getNoVertices(); source++) {
        for (auto labelSource : graph->reverse_adj[source]) {
            outDegrees[labelSource.second - 1]++;
            sources++;
        }

        for (auto labelTarget : graph->adj[source]) {
            inDegrees[labelTarget.second - 1]++;
            targets++;
        }
    }

    for (int k = 0; k < n; k++) {
        if (outDegrees[k] > maxDegree) {
            maxDegree = outDegrees[k];
            vertexWithMaxDegree = k + 1;
        }
        if (outDegrees[k] == 0) {
            z++;
        }
        if (inDegrees[k] == 0) {
            z2++;
        }
    }

    int x = maxDegree;

    std::cout << "Vertex " << vertexWithMaxDegree << " has the highest outdegree: " << maxDegree << std::endl;


    float pZero = 0;
    for (int j = 1; j<l+1;j++){
        pZero += ((factorial(l)/(factorial(j)*factorial(l-j))) * pow(z/n,j) * pow((n-z)/n,l-j));
    }

    float pDupl = 0;
    for (int k = 2; k<l+1;k++){
        pDupl += ((factorial(l)/(factorial(k)*factorial(l-k))) * pow(x/n,k) * pow((n-x)/n,l-k));
    }

    float result = 0;

    for (int i = 1; i < l+1; i++){
        result += ((l+1) * pow((m/n + (x - m/n)),i) * (1-pZero) * (1-pDupl));
    }

    int cardinalityEstimate = (int)((sources - z) * (targets - z2) * result);

    std::cout << "Result: " << result << std::endl;
    std::cout << "Cardinality estimate: " << cardinalityEstimate << std::endl;

}

cardStat SimpleEstimator::estimate(RPQTree *q) {

    // perform your estimation here

    return cardStat {0, 0, 0};
}