#include <iostream>
#include <cassert>
#include "lab_8_graph.cpp" 

using namespace std;

void testLoadGraph() {
    Graph graph("test_graph.txt"); // Загрузка тестового графа

    assert(!graph.nodes.empty());
    assert(!graph.nodeMap.empty());
    cout << "Graph loaded successfully. Nodes count: " << graph.nodes.size() << endl;
}

void testFindClosestNode() {
    Graph graph("test_graph.txt");

    Node* closest = graph.findClosestNode(59.884972, 30.368072);
    assert(closest != nullptr);
    cout << "Closest node found: (" << closest->lat << ", " << closest->lon << ")" << endl;
}

void testDijkstra() {
    Graph graph("test_graph.txt");

    Node* start = graph.findClosestNode(59.884972, 30.368072);
    Node* goal = graph.findClosestNode(59.956248, 30.309215);

    assert(start != nullptr);
    assert(goal != nullptr);

    auto distances = dijkstra(graph.nodes, start);
    assert(distances[goal] != numeric_limits<double>::infinity());

    cout << "Dijkstra Distance: " << distances[goal] << endl;
}

void testAStar() {
    Graph graph("test_graph.txt");

    Node* start = graph.findClosestNode(59.884972, 30.368072);
    Node* goal = graph.findClosestNode(59.956248, 30.309215);

    assert(start != nullptr);
    assert(goal != nullptr);

    auto distances = aStar(graph.nodes, start, goal);
    assert(distances[goal] != numeric_limits<double>::infinity());

    cout << "A* Distance: " << distances[goal] << endl;
}

void testDFS() {
    Graph graph("test_graph.txt");

    Node* start = graph.findClosestNode(59.884972, 30.368072);
    Node* goal = graph.findClosestNode(59.956248, 30.309215);

    assert(start != nullptr);
    assert(goal != nullptr);

    unordered_set<Node*> visited;
    double distance = dfs(start, goal, visited);

    assert(distance >= 0);
    cout << "DFS Distance: " << distance << endl;
}

void testBFS() {
    Graph graph("test_graph.txt");

    Node* start = graph.findClosestNode(59.884972, 30.368072);
    Node* goal = graph.findClosestNode(59.956248, 30.309215);

    assert(start != nullptr);
    assert(goal != nullptr);

    double distance = bfs(graph.nodes, start, goal);

    assert(distance >= 0);
    cout << "BFS Distance: " << distance << endl;
}

int main() {
    testLoadGraph();
    testFindClosestNode();
    testDijkstra();
    testAStar();
    testDFS();
    testBFS();

    cout << "All tests passed successfully!" << endl;
    return 0;
}