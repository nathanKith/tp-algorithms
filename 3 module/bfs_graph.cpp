#include <iostream>
#include <vector>
#include <functional>
#include <queue>

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

int BFS(int from, int to, const IGraph& graph) {
    std::vector<int> r(graph.VerticesCount(), std::numeric_limits<int>::max());
    std::vector<int> pi(graph.VerticesCount(), -1);
    std::queue<int> q;
    q.push(from);
    r[from] = 0;
    pi[from] = -1;
    std::vector<int> count(graph.VerticesCount(), 0);
    count[from] = 1;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (auto w : graph.GetNextVertices(v)) {
            if (r[w] == r[v] + 1) {
                count[w] += count[v];
            }
            if (r[w] > r[v] + 1) {
                r[w] = r[v] + 1;
                pi[w] = v;
                count[w] = count[v];
                q.push(w);
            }
        }
    }
    return count[to];
}

int main() {
    int v, n, u, w;
    std::cin >> v >> n;
    ListGraph graph(v);
    for (int i = 0; i < n; ++i) {
        std::cin >> u >> w;
        graph.AddEdge(u, w);
        graph.AddEdge(w, u);
    }
    std::cin >> u >> w;
    std::vector<int> a(v), b(v);
    int number = BFS(u, w, graph);
    std::cout << number;
    return 0;
}

