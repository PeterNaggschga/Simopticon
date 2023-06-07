#include <iostream>
#include "evaluation/Pipeline.h"
#include "evaluation/constant_headway/ConstantHeadway.h"
#include "utils/CommandLine.h"
#include "optimizer/direct/hyrect/HyRect.h"
#include "optimizer/direct/GrahamScan.h"
#include "parameters/ParameterDefinition.h"
#include "parameters/Parameter.h"
#include "parameters/ContinuousParameter.h"
#include "controller/ValueMap.h"
#include "optimizer/direct/hyrect/BaseRect.h"
#include "controller/Controller.h"
#include "controller/StubController.h"
#include "runner/plexe/ConfigEditor.h"
#include "runner/plexe/PlexeSimulationRunner.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-result"

using namespace std;

void pipelineTest() {
    Pipeline *pipe = new ConstantHeadway();
    set<runId> experimentIds;
    string path = "/home/petern/src/plexe/examples/platooning/results";
    pipe->processOutput(experimentIds, path, 0);
    cout << to_string(pipe->getValue()) << endl;
}

template<class T>
string vecToString(vector<T> vector) {
    string buf = "(" + to_string(vector[0]);
    for (int i = 1; i < vector.size(); ++i) {
        buf += ", " + to_string(vector[i]);
    }
    buf += ")";
    return buf;
}

void hyRectStructureTest() {
    shared_ptr<HyRect> base(new BaseRect(2));
    auto children = base->divide(base);
    cout << base.use_count() << endl;
    cout << "Base: pos=" << (int) base->getPos() << ", t=" << base->getDepth() << ", vertice1="
         << vecToString(base->getSamplingVertices().front()) << ", vertice2=" << vecToString(
            base->getSamplingVertices().back())
         << endl;
    cout << "Child 1: pos=" << (int) children[0]->getPos() << ", t=" << children[0]->getDepth() << ", vertice1="
         << vecToString(children[0]->getSamplingVertices().front()) << ", vertice2="
         << vecToString(children[0]->getSamplingVertices().back()) << endl;
    cout << "Child 2: pos=" << (int) children[1]->getPos() << ", t=" << children[1]->getDepth() << ", vertice1="
         << vecToString(children[1]->getSamplingVertices().front()) << ", vertice2="
         << vecToString(children[1]->getSamplingVertices().back()) << endl;
    cout << "Child 3: pos=" << (int) children[2]->getPos() << ", t=" << children[2]->getDepth() << ", vertice1="
         << vecToString(children[2]->getSamplingVertices().front()) << ", vertice2="
         << vecToString(children[2]->getSamplingVertices().back()) << endl;
}

void grahamScanTest() {
    shared_ptr<HyRect> base(new BaseRect(2));
    auto childs1 = base->divide(base);
    childs1[2]->setAvgValue(75);
    auto childs2 = childs1[0]->divide(childs1[0]);
    childs2[2]->setAvgValue(25);
    auto childs3 = childs2[0]->divide(childs2[0]);
    childs3[2]->setAvgValue(22);
    auto childs4 = childs3[0]->divide(childs3[0]);
    childs4[2]->setAvgValue(15);
    auto childs5 = childs4[0]->divide(childs4[0]);
    childs5[2]->setAvgValue(15);
    //auto test = childs5[2].getDiagonalLength();
    list<shared_ptr<HyRect>> rects({childs5[2], childs4[2], childs3[2], childs2[2], childs1[2]});
    list<pair<shared_ptr<HyRect>, double>> optimal = GrahamScan::scan(rects);
    for (const auto &entry: optimal) {
        cout << entry.first->getDepth() << ", " << entry.first->getAvgValue() << ", " << entry.second << endl;
    }
}

void valueMapTest() {
    shared_ptr<ParameterDefinition> def(new ParameterDefinition(0, 1, ""));
    shared_ptr<Parameter> par1(new ContinuousParameter(def, 0.1));
    shared_ptr<Parameter> par2(new ContinuousParameter(def, 0.5));
    shared_ptr<Parameter> par3(new ContinuousParameter(def, 0.7));
    ValueMap map = ValueMap(2);
    cout << map.getSize() << ", " << map.getMedian() << ", " << map.isKnown(vector<shared_ptr<Parameter>>({par2}))
         << endl;
    map.insert(vector<shared_ptr<Parameter>>({par1}), 1);
    map.insert(vector<shared_ptr<Parameter>>({par3}), 3);
    cout << map.getSize() << ", " << map.getMedian() << ", " << map.isKnown(vector<shared_ptr<Parameter>>({par2}))
         << endl;
    map.insert(vector<shared_ptr<Parameter>>({par2}), 2);
    cout << map.getSize() << ", " << map.getMedian() << ", " << map.isKnown(vector<shared_ptr<Parameter>>({par2}))
         << endl;
    cout << map.getTopVals().begin()->second << endl;
    shared_ptr<Parameter> par4(new ContinuousParameter(def, 0.6));
    map.insert(vector<shared_ptr<Parameter>>({par4}), 3);
    map.getMedian();
}

void directTest() {
    shared_ptr<ParameterDefinition> def(new ParameterDefinition(-1, 1, ""));
    function<functionValue(vector<shared_ptr<Parameter>>)> f = [](const vector<shared_ptr<Parameter>> &v) {
        functionValue val = 0;
        for (const auto &par: v) {
            val += par->getVal() * par->getVal();
        }
        return val;
    };
    function<functionValue(vector<shared_ptr<Parameter>>)> branin = [](const vector<shared_ptr<Parameter>> &v) {
        functionValue res = pow(
                v[1]->getVal() - (5.1 / (4 * pow(M_PI, 2))) * pow(v[0]->getVal(), 2) + (5 / M_PI) * v[0]->getVal() - 6,
                2);
        return res + 10 * (1 - pow(8 * M_PI, -1)) * cos(v[0]->getVal()) + 10;
    };
    unique_ptr<Controller> ctr(new StubController({def, def}, f));
    ctr->run();
}

void configEditorTest() {
    ConfigEditor editor = ConfigEditor("/home/petern/src/plexe/examples/platooning");
    shared_ptr<ParameterDefinition> c1(new ParameterDefinition(0, 1, "*.node[*].scenario.caccC1"));
    shared_ptr<ParameterDefinition> xi(new ParameterDefinition(0, 1, "*.node[*].scenario.caccOmegaN", "Hz"));
    vector<shared_ptr<Parameter>> v1 = {shared_ptr<Parameter>(new ContinuousParameter(c1, 0.5)),
                                        shared_ptr<Parameter>(new ContinuousParameter(xi, 0.5))};
    vector<shared_ptr<Parameter>> v2 = {shared_ptr<Parameter>(new ContinuousParameter(c1, 1)),
                                        shared_ptr<Parameter>(new ContinuousParameter(xi, 1))};
    editor.createConfig(v1, 0, 2);
    editor.createConfig(v2, 1, 5);
    cout << "Configs erstellt - löschen?";
    cin.get();
    editor.deleteConfig(0);
    editor.deleteConfig(1);
}

void plexeRunnerTest() {
    ConfigEditor editor = ConfigEditor("/home/petern/src/plexe/examples/platooning");
    unique_ptr<SimulationRunner> runner(
            new PlexeSimulationRunner(3, 5, {"BrakingNoGui", "SinusoidalNoGui"}, editor));
    shared_ptr<ParameterDefinition> c1(new ParameterDefinition(0, 1, "*.node[*].scenario.caccC1"));
    shared_ptr<ParameterDefinition> xi(new ParameterDefinition(0, 1, "*.node[*].scenario.caccOmegaN", "Hz"));
    set<vector<shared_ptr<Parameter>>, CmpVectorSharedParameter> set;
    for (int i = 0; i < 5; ++i) {
        set.insert({shared_ptr<Parameter>(new ContinuousParameter(c1, pow(0.75, i))),
                    shared_ptr<Parameter>(new ContinuousParameter(xi, pow(0.5, i)))});
    }
    auto result = runner->runSimulations(set);
}

int main() {
    //pipelineTest();
    //hyRectStructureTest();
    //grahamScanTest();
    //valueMapTest();
    //directTest();
    //configEditorTest();
    //plexeRunnerTest();

    return 0;
}

#pragma clang diagnostic pop