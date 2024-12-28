#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <limits>
#include <ctime>
#include <algorithm>
#include <chrono> 

using namespace std;

// Структура, представляющая узел графа
struct Node {
    string lon, lat;  // Долгота и широта хранятся как строки (O(1) по памяти на каждую строку)
    vector<pair<Node*, double>> neighbors;  // Соседи узла и вес ребра до них (O(E) по памяти для всех соседей)
};

// Функция для измерения времени выполнения произвольного вызываемого объекта
// Асимптотика зависит от передаваемой функции T.
template <typename T, typename... Args>
double measureTime(T func, Args&&... args) {
    auto start = chrono::high_resolution_clock::now(); // Засекаем время начала (O(1))
    func(forward<Args>(args)...);  // Вызываем функцию (зависит от функции T)
    auto end = chrono::high_resolution_clock::now(); // Засекаем время окончания (O(1))
    chrono::duration<double> duration = end - start; // Вычисляем разницу во времени (O(1))
    return duration.count(); // Возвращаем время выполнения в секундах (O(1))
}

// Структура графа
struct Graph {
    vector<Node*> nodes; // Вектор всех узлов графа (O(V) по памяти)
    unordered_map<string, Node*> nodeMap;  // Хеш-таблица для быстрого доступа к узлам (O(V) по памяти)

    // Конструктор, загружающий граф из файла
    explicit Graph(const string& filename) {
        loadGraphFromFile(filename); // O(V + E) по времени, где V - количество узлов, E - количество рёбер
    }

    // Загрузка графа из файла
    void loadGraphFromFile(const string& filename) {
        ifstream file(filename); // Открытие файла (O(1))
        if (!file.is_open()) {
            cerr << "Ошибка открытия файла: " << filename << endl; // O(1)
            return;
        }

        string line;
        while (getline(file, line)) { // O(V + E) по времени, так как обрабатываем строки
            stringstream ss(line);
            string token;

            // Парсинг родительского узла
            getline(ss, token, ':');
            string parentKey = token;  // Ключ - исходная строка координат (O(1))
            Node* parentNode;

            if (nodeMap.find(parentKey) == nodeMap.end()) { // O(1) на поиск в хеш-таблице
                stringstream parentCoordSS(token);
                getline(parentCoordSS, token, ',');
                string parentLon = token; // O(1)
                getline(parentCoordSS, token, ',');
                string parentLat = token; // O(1)

                parentNode = new Node{parentLon, parentLat}; // O(1)
                nodes.push_back(parentNode); // O(1)
                nodeMap[parentKey] = parentNode; // O(1)
            } else {
                parentNode = nodeMap[parentKey]; // O(1)
            }

            // Парсинг дочерних узлов и ребер
            while (getline(ss, token, ';')) { // O(E) для всех рёбер
                stringstream childSS(token);

                getline(childSS, token, ',');
                string childLon = token; // O(1)
                getline(childSS, token, ',');
                string childLat = token; // O(1)
                string childKey = childLon + "," + childLat; // O(1)

                childLon.erase(remove(childLon.begin(), childLon.end(), ' '), childLon.end()); // O(L), где L - длина строки
                childLat.erase(remove(childLat.begin(), childLat.end(), ' '), childLat.end()); // O(L)

                getline(childSS, token, ','); // O(1)
                double weight = stod(token); // O(1)

                Node* childNode;
                if (nodeMap.find(childKey) == nodeMap.end()) { // O(1)
                    childNode = new Node{childLon, childLat}; // O(1)
                    nodes.push_back(childNode); // O(1)
                    nodeMap[childKey] = childNode; // O(1)
                } else {
                    childNode = nodeMap[childKey]; // O(1)
                }

                parentNode->neighbors.push_back({childNode, weight}); // O(1)
            }
        }
        file.close(); // O(1)
    }

    // Поиск ближайшего узла к заданным координатам
    Node* findClosestNode(double lat, double lon) const {
        Node* closestNode = nullptr; // O(1)
        double minDistance = numeric_limits<double>::infinity(); // O(1)

        for (auto* node : nodes) { // O(V)
            double nodeLat = stod(node->lat); // O(1)
            double nodeLon = stod(node->lon); // O(1)
            double distance = sqrt(pow(lat - nodeLat, 2) + pow(lon - nodeLon, 2)); // O(1)

            if (distance < minDistance) { // O(1)
                minDistance = distance; // O(1)
                closestNode = node; // O(1)
            }
        }
        return closestNode; // O(1)
    }

private:
    // Метод для создания или получения узла по его координатам (строка "долгота,широта")
    Node* createOrGetNode(const string& key) {
        if (nodeMap.find(key) == nodeMap.end()) { // O(1)
            stringstream ss(key);
            string lon, lat;

            getline(ss, lon, ',');
            getline(ss, lat, ',');

            Node* newNode = new Node{lon, lat}; // O(1)
            nodes.push_back(newNode); // O(1)
            nodeMap[key] = newNode; // O(1)
        }
        return nodeMap[key]; // O(1)
    }
};

// Алгоритм Дейкстры
unordered_map<Node*, double> dijkstra(const vector<Node*>& nodes, Node* startNode) {
    unordered_map<Node*, double> distances; // O(V) по памяти
    priority_queue<pair<double, Node*>, vector<pair<double, Node*>>, greater<>> pq; // O(V) по памяти

    for (auto* node : nodes) { // O(V)
        distances[node] = numeric_limits<double>::infinity(); // O(1)
    }
    distances[startNode] = 0.0; // O(1)
    pq.push({0.0, startNode}); // O(log V)

    while (!pq.empty()) { // O((V + E) log V) из-за приоритетной очереди
        double currentDist = pq.top().first; // O(1)
        Node* currentNode = pq.top().second; // O(1)
        pq.pop(); // O(log V)

        if (currentDist > distances[currentNode]) continue; // O(1)

        for (const auto& [neighbor, weight] : currentNode->neighbors) { // O(E)
            double newDist = currentDist + weight; // O(1)
            if (newDist < distances[neighbor]) { // O(1)
                distances[neighbor] = newDist; // O(1)
                pq.push({newDist, neighbor}); // O(log V)
            }
        }
    }
    return distances; // O(1)
}

// Эвристическая функция для алгоритма A*
double heuristic(Node* a, Node* b) {
    double lat1 = stod(a->lat), lon1 = stod(a->lon); // O(1)
    double lat2 = stod(b->lat), lon2 = stod(b->lon); // O(1)
    return sqrt(pow(lat1 - lat2, 2) + pow(lon1 - lon2, 2)); // O(1)
}

// Алгоритм A*
unordered_map<Node*, double> aStar(const vector<Node*>& nodes, Node* startNode, Node* endNode) {
    unordered_map<Node*, double> distances, fScore; // O(V) по памяти
    priority_queue<pair<double, Node*>, vector<pair<double, Node*>>, greater<>> pq; // O(V) по памяти

    for (auto* node : nodes) { // O(V)
        distances[node] = numeric_limits<double>::infinity(); // O(1)
        fScore[node] = numeric_limits<double>::infinity(); // O(1)
    }
    distances[startNode] = 0.0; // O(1)
    fScore[startNode] = heuristic(startNode, endNode); // O(1)
    pq.push({fScore[startNode], startNode}); // O(log V)

    while (!pq.empty()) { // O((V + E) log V)
        Node* current = pq.top().second; // O(1)
        pq.pop(); // O(log V)

        if (current == endNode) break; // O(1)

        for (const auto& [neighbor, weight] : current->neighbors) { // O(E)
            double tentativeGScore = distances[current] + weight; // O(1)
            if (tentativeGScore < distances[neighbor]) { // O(1)
                distances[neighbor] = tentativeGScore; // O(1)
                fScore[neighbor] = tentativeGScore + heuristic(neighbor, endNode); // O(1)
                pq.push({fScore[neighbor], neighbor}); // O(log V)
            }
        }
    }
    return distances; // O(1)
}

// BFS (поиск в ширину)
double bfs(const vector<Node*>& nodes, Node* startNode, Node* endNode) {
    queue<pair<Node*, double>> q; // O(V) по памяти
    unordered_set<Node*> visited; // O(V) по памяти

    q.push({startNode, 0.0}); // O(1)
    visited.insert(startNode); // O(1)

    while (!q.empty()) { // O(V + E)
        Node* current = q.front().first; // O(1)
        double currentDist = q.front().second; // O(1)
        q.pop(); // O(1)

        if (current == endNode) return currentDist; // O(1)

        for (const auto& [neighbor, weight] : current->neighbors) { // O(E)
            if (visited.insert(neighbor).second) { // O(1)
                q.push({neighbor, currentDist + weight}); // O(1)
            }
        }
    }
    return -1.0; // O(1)
}

// DFS (поиск в глубину)
double dfs(Node* startNode, Node* endNode, unordered_set<Node*>& visited) {
    if (startNode == endNode) return 0.0; // O(1)

    visited.insert(startNode); // O(1)

    for (const auto& [neighbor, weight] : startNode->neighbors) { // O(E)
        if (visited.find(neighbor) == visited.end()) { // O(1)
            double result = dfs(neighbor, endNode, visited); // Рекурсивный вызов (глубина рекурсии <= V)
            if (result != -1.0) return result + weight; // O(1)
        }
    }
    return -1.0; // O(1)
}

#ifndef TESTING

int main() {
    Graph graph("spb_graph.txt"); // Инициализация графа (O(V + E))

    double myLat = 59.884972, myLon = 30.368072; // O(1)
    double finLat = 59.956248, finLon = 30.309215; // O(1)

    // Находим ближайшие узлы к заданным координатам
    Node* startNode = graph.findClosestNode(myLat, myLon); // O(V)
    Node* endNode = graph.findClosestNode(finLat, finLon); // O(V)

    if (!startNode || !endNode) {
        cerr << "Ошибка: начальная или конечная вершина не найдена." << endl; // O(1)
        return 1; // O(1)
    }

    unordered_set<Node*> visited; // O(V) по памяти

    // cout << "DFS Distance: " << dfs(startNode, endNode, visited) << endl; // O(V + E)
    // double dfsTime = measureTime(dfs, startNode, endNode, ref(visited)); // O(V + E)
    // cout << "DFS Time: " << dfsTime << " seconds" << endl; // O(1)

    // double bfsTime = measureTime(bfs, graph.nodes, startNode, endNode);
    // cout << "BFS Distance: " << bfs(graph.nodes, startNode, endNode) << endl;
    // cout << "BFS Time: " << bfsTime << " seconds" << endl;

    // double dijkstraTime = measureTime(dijkstra, graph.nodes, startNode);
    // auto distancesDijkstra = dijkstra(graph.nodes, startNode);
    // cout << endl << "Dijkstra Distance: " << distancesDijkstra[endNode] << endl;
    // cout << "Dijkstra Time: " << dijkstraTime << " seconds" << endl;

    double aStarTime = measureTime(aStar, graph.nodes, startNode, endNode);
    auto distancesAStar = aStar(graph.nodes, startNode, endNode);
    cout << endl << "A* Distance: " << distancesAStar[endNode] << endl;
    cout << "A* Time: " << aStarTime << " seconds" << endl;

    return 0; // O(1)
}

#endif
