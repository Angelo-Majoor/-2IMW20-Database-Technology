//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"
#include <list>
#include <map>

std::map<uint32_t , cardStat> est_result;

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
}

void SimpleEstimator::prepare() {

    // do your prep here

    //int edgesPerLabel [graph->getNoLabels()];
    //int inDegreePerLabel [graph->getNoLabels()];
    //int outDegreePerLabel [graph->getNoLabels()];

    std::list<uint32_t> uniqueNodesForLabel;

    for(int noLabels = 0; noLabels < graph->getNoLabels(); noLabels++) {
        est_result[noLabels] = cardStat {0, 0, 0};

        //edgesPerLabel[noLabels] = 0;
        //inDegreePerLabel[noLabels] = 0;
        //outDegreePerLabel[noLabels] = 0;

        for (int source = 0; source < graph->getNoVertices(); source++) {
            for (auto labelSource : graph->adj[source]) {

                bool found = (std::find(uniqueNodesForLabel.begin(), uniqueNodesForLabel.end(), labelSource.second) !=
                              uniqueNodesForLabel.end());
                if (labelSource.first == noLabels && !found) {
                    est_result[noLabels].noIn++;
                    //outDegreePerLabel[noLabels]++;
                    uniqueNodesForLabel.push_back(labelSource.second);
                }
            }
        }

        uniqueNodesForLabel.clear();

        for (int target = 0; target < graph->getNoVertices(); target++) {
            for (auto labelTarget : graph->reverse_adj[target]) {

                bool found = (std::find(uniqueNodesForLabel.begin(), uniqueNodesForLabel.end(), labelTarget.second) !=
                              uniqueNodesForLabel.end());
                if (labelTarget.first == noLabels && !found) {
                    est_result[noLabels].noOut++;
                    //inDegreePerLabel[noLabels]++;
                    uniqueNodesForLabel.push_back(labelTarget.second);
                }
            }
        }

        uniqueNodesForLabel.clear();

    }

    for (int source = 0; source < graph->getNoVertices(); source++) {
        for (auto labelSource : graph->adj[source]) {
            est_result[labelSource.first].noPaths++;
            //edgesPerLabel[labelSource.first]++;
        }
    }

    //std::cout << std::endl << "Number of edges: " << graph->getNoEdges();
    //std::cout << std::endl << "Edges per label: " << edgesPerLabel[0] << "; " << edgesPerLabel[1] << "; " << edgesPerLabel[2] << "; " << edgesPerLabel[3];
    //std::cout << std::endl << "Unique start (outgoing) nodes per label: " << outDegreePerLabel[0] << "; " << outDegreePerLabel[1] << "; " << outDegreePerLabel[2] << "; " << outDegreePerLabel[3] << "; ";
    //std::cout << std::endl << "Unique end (incoming) nodes per label: " << inDegreePerLabel[0] << "; " << inDegreePerLabel[1] << "; " << inDegreePerLabel[2] << "; " << inDegreePerLabel[3] << "; " << std::endl;
    //std::cout << std::endl << graph->getNoVertices();
    //est_result[0].print();
    //est_result[1].print();
    //est_result[2].print();
    //est_result[3].print();

}

cardStat SimpleEstimator::estimate(RPQTree *q) {

    // perform your estimation here

    // project out the label in the AST
    std::regex directLabel (R"((\d+)\+)");
    std::regex inverseLabel (R"((\d+)\-)");

    std::smatch matches;

    uint32_t label;
    bool inverse = false;

    if (q->isLeaf()) {
        if (std::regex_search(q->data, matches, directLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            inverse = false;
        } else if (std::regex_search(q->data, matches, inverseLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            inverse = true;
        } else {
            std::cerr << "Label parsing failed!" << std::endl;
            return {0, 0, 0};
        }
    }

    if (q->isConcat()) {
        // evaluate the children
        cardStat leftGraph;
        cardStat rightGraph;

        if (!inverse) {
            leftGraph = SimpleEstimator::estimate(q->left);
            rightGraph = SimpleEstimator::estimate(q->right);
        }
        else {
            leftGraph = SimpleEstimator::estimate(q->right);
            rightGraph = SimpleEstimator::estimate(q->left);
        }

        //float result = leftGraph.noPaths * (leftGraph.noIn * (float(rightGraph.noOut) / (float)(graph->getNoVertices())) * ((float)(rightGraph.noPaths) / (float)(rightGraph.noOut)));
        float result1 = (float)(leftGraph.noPaths * rightGraph.noPaths) / (float)(leftGraph.noOut);
        float result2 = (float)(leftGraph.noPaths * rightGraph.noPaths) / (float)(rightGraph.noOut);
        uint32_t final_result = std::min((int)(result1), (int)(result2));

        //std::cout << std::endl << "Result: " << final_result;
        //std::cout << std::endl << "Left graph: " << leftGraph.noOut << "; " << leftGraph.noPaths << "; " << leftGraph.noIn;
        //std::cout << std::endl << "Right graph: " << rightGraph.noOut << "; " << rightGraph.noPaths << "; " << rightGraph.noIn;

        return cardStat {std::min(leftGraph.noOut, rightGraph.noOut), final_result,
                         std::min(leftGraph.noIn, rightGraph.noIn)};
    }

    return est_result[label];
}