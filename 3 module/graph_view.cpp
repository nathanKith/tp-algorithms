#include <iostream>
#include <vector>
#include <cassert>
#include <functional>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iterator>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph
{
public:
    ListGraph(int size) : adjacencyLists(size)
    {
    }

    ListGraph(const IGraph& graph) : adjacencyLists(graph.VerticesCount())
    {
        for (int i = 0; i < graph.VerticesCount(); i++)
        {
            adjacencyLists[i] = graph.GetNextVertices(i);
        }
    }

    ~ListGraph()
    {
    }

    void AddEdge(int from, int to) override
    {
        //assert(0 <= from && from < adjacencyLists.size());
        //assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(to);
    }

    int VerticesCount() const override
    {
        return (int)adjacencyLists.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override
    {
        //assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const override
    {
        //assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); from++)
        {
            for (int to : adjacencyLists[from])
            {
                if (to == vertex)
                {
                    prevVertices.push_back(from);
                }
            }
        }

        return prevVertices;
    }

private:
    std::vector<std::vector<int>> adjacencyLists;
};

class MatrixGraph : public IGraph {
public:
    MatrixGraph(int size) : adjacencyMatrix(size) {}

    MatrixGraph(const IGraph& graph) : adjacencyMatrix(graph.VerticesCount()) {
        for (int i = 0; i < adjacencyMatrix.size(); ++i) {
            adjacencyMatrix[i] = graph.GetNextVertices(i);
        }
    }

    ~MatrixGraph() {

    }

    void AddEdge(int from, int to) {
        adjacencyMatrix[from][to] = 1;
    }

    int VerticesCount() const override {
        return adjacencyMatrix.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> nextVertices;
        for (auto v : adjacencyMatrix[vertex]) {
            nextVertices.push_back(v);
        }
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override	{
        std::vector<int> prevVertices;
        for (int i = 0; i < adjacencyMatrix.size(); ++i) {
            if (adjacencyMatrix[i][vertex] == 1) {
                prevVertices.push_back(i);
            }
        }
        return prevVertices;
    }
private:
    std::vector<std::vector<int>> adjacencyMatrix;
};

class SetGraph : public IGraph {
public:
    SetGraph(int size) : vertexSet(size) {}

    SetGraph(const IGraph& graph) : vertexSet(graph.VerticesCount()) {
        for (int i = 0; i < vertexSet.size(); ++i) {
            std::vector<int> v = GetNextVertices(i);
            std::copy(v.begin(), v.end(),
                      std::inserter(vertexSet[i], vertexSet[i].end()));
        }
    }

    ~SetGraph() {

    }

    void AddEdge(int from, int to) override {
        vertexSet[from].insert(to);
    }

    int VerticesCount() const override {
        return (int)vertexSet.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override	{
        std::vector<int> nextVertices;
        for (auto v : vertexSet[vertex]) {
            nextVertices.push_back(v);
        }
        return nextVertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override	{
        std::vector<int> prevVertices;
        for (int i = 0; i < vertexSet.size(); ++i) {
            if (vertexSet[i].find(vertex) == vertexSet[i].end()) {
                prevVertices.push_back(i);
            }
        }
        return prevVertices;
    }
private:
    std::vector<std::unordered_set<int>> vertexSet;
};

class ArcGraph : public IGraph {
public:
    ArcGraph(int size) {

    }

    ArcGraph(const IGraph& graph) {
        for (int i = 0; i < graph.VerticesCount(); ++i) {
            std::vector<int> nextVertices = graph.GetNextVertices(i);
            for (auto v : nextVertices) {
                vertexPair.push_back(std::pair<int, int>(i, v));
            }
        }
    }

    void AddEdge(int from, int to) override {
        vertexPair.push_back(std::pair<int, int>(from, to));
    }

    int VerticesCount() const override {
        std::unordered_set<int> vertices;
        for (auto edge : vertexPair) {
            vertices.insert(edge.first);
            vertices.insert(edge.second);
        }
        return vertices.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override {
        std::vector<int> vertices;
        for (auto edge : vertexPair) {
            if (edge.first == vertex) {
                vertices.push_back(edge.second);
            }
        }
        return vertices;
    }

    std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> vertices;
        for (auto edge : vertexPair) {
            if (edge.second == vertex) {
                vertices.push_back(edge.first);
            }
        }
        return vertices;
    }
private:
    std::vector<std::pair<int, int>> vertexPair;
};

void BFS(const IGraph& graph, int vertex, std::vector<bool>& visited, std::function<void(int)>& func)
{
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();

        func(currentVertex);

        for (int nextVertex : graph.GetNextVertices(currentVertex))
        {
            if (!visited[nextVertex])
            {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph& graph, std::function<void(int)> func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            BFS(graph, i, visited, func);
    }
}

void DFS(const IGraph& graph, int vertex, std::vector<bool>& visited, std::function<void(int)>& func)
{
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex : graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
            DFS(graph, nextVertex, visited, func);
    }

}

void mainDFS(const IGraph& graph, std::function<void(int)> func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            DFS(graph, i, visited, func);
    }
}

void topologicalSortInternal(const IGraph& graph, int vertex, std::vector<bool>& visited, std::deque<int>& sorted)
{
    visited[vertex] = true;

    for (int nextVertex : graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
            topologicalSortInternal(graph, nextVertex, visited, sorted);
    }

    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph& graph)
{
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
            topologicalSortInternal(graph, i, visited, sorted);
    }

    return sorted;
}


int main(int argc, const char* argv[]) {
    ListGraph graph(7);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 5);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);
    graph.AddEdge(1, 5);
    graph.AddEdge(1, 6);
    graph.AddEdge(3, 2);
    graph.AddEdge(3, 4);
    graph.AddEdge(3, 6);
    graph.AddEdge(5, 4);
    graph.AddEdge(5, 6);
    graph.AddEdge(6, 4);

    mainBFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    mainDFS(graph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    for (int vertex : topologicalSort(graph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl << std::endl;

    MatrixGraph mGraph(graph);
    mainBFS(mGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    mainDFS(mGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    for (int vertex : topologicalSort(mGraph))
    {
        std::cout << vertex << " ";
    }

    std::cout << std::endl << std::endl;

    SetGraph sGraph(graph);
    mainBFS(sGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    mainDFS(sGraph, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;

    for (int vertex : topologicalSort(sGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl << std::endl;

    ArcGraph arcGraph(graph);

    mainBFS(arcGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    mainBFS(arcGraph, [](int vertex){ std::cout << vertex << " ";});
    std::cout << std::endl;

    for (int vertex : topologicalSort(arcGraph))
    {
        std::cout << vertex << " ";
    }

    return 0;
}
