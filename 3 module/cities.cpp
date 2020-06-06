#include <iostream>
#include <vector>
#include <queue>
#include <set>

struct Relation {
    Relation() {}
    Relation(int v, int w) : vertex(v), weight(w){}
    int vertex;
    int weight;

    Relation(const Relation& relation)
    : vertex(relation.vertex)
    , weight(relation.weight) {}

    bool operator<(Relation const& rhs) const {
        if (weight == rhs.weight) {
            return vertex < rhs.vertex;
        }
        return weight < rhs.weight;
    }
};

class ListGraph
{
public:
    ListGraph(int size) : adjacencyLists(size)
    {
    }

    ~ListGraph()
    {
    }

    void AddEdge(int from, int to, int weight)
    {
        //assert(0 <= from && from < adjacencyLists.size());
        //assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(Relation(to, weight));
    }

    int VerticesCount() const
    {
        return (int)adjacencyLists.size();
    }

    std::vector<Relation> GetNextVertices(int vertex) const
    {
        return adjacencyLists[vertex];
    }

    std::vector<Relation> GetPrevVertices(int vertex) const
    {
        //assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<Relation> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); from++)
        {
            for (auto v : adjacencyLists[from])
            {
                if (v.vertex == vertex)
                {
                    prevVertices.push_back(Relation(from, v.weight));
                }
            }
        }

        return prevVertices;
    }

private:
    std::vector<std::vector<Relation>> adjacencyLists;
};

bool Relax(int from, int to, int weight, std::vector<int>& r, std::vector<int>& pi) {
    if (r[to] > r[from] + weight) {
        r[to] = r[from] + weight;
        pi[to] = from;
        return true;
    }
    return false;
}

int Dijkstra(int vertex, const ListGraph& graph, int destination) {
    std::vector<int> r(graph.VerticesCount(), std::numeric_limits<int>::max());
    std::vector<int> pi(graph.VerticesCount(), -1);
    std::set<Relation> q; // замена очереди с приоритетом
    r[vertex] = 0;
    pi[vertex] = -1;
    q.insert(Relation(vertex, r[vertex]));
    while (!q.empty()) {
        auto v = q.begin();
        auto value = *v;
        q.erase(*q.begin());
        for (const auto& w : graph.GetNextVertices(value.vertex)) {
            if (r[w.vertex] == std::numeric_limits<int>::max()) {
                r[w.vertex] = r[value.vertex] + w.weight;
                pi[w.vertex] = value.vertex;
                q.insert(Relation(w.vertex, r[w.vertex]));
            }
            int decrKey = r[w.vertex];
            if (Relax(value.vertex, w.vertex, w.weight, r, pi)) {
                q.erase(Relation(w.vertex, decrKey));
                q.insert(Relation(w.vertex, r[w.vertex]));
            }
        }
    }
    return r[destination];
}

int main() {
    int n, m;
    std::cin >> n >> m;
    int from, to, weight;
    ListGraph graph(n);
    for (int i = 0; i < m; ++i) {
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
        graph.AddEdge(to, from, weight);
    }
    std::cin >> from >> to;
    int result = Dijkstra(from, graph, to);
    std::cout << result;
    return 0;
}