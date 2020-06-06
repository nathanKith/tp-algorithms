#include <iostream>
#include <queue>
#include <stack>

template <typename T>
struct Node {
    Node(T value)
        : data(value)
        , parent(nullptr)
        , left(nullptr)
        , right(nullptr)
        {}

    ~Node() {}

    T data;
    Node* right;
    Node* left;
    Node* parent;
};

template <typename T, typename Comparator = std::less_equal<T>>
class BinaryTree {
public:
    explicit BinaryTree() : root(nullptr) {}

    explicit BinaryTree(T value) : root(new Node<T>(value)) {}

    void insert(T value, Comparator cmp = Comparator()) {
        if (!root) {
            root = new Node<T>(value);
            return;
        }
        Node<T>* iterNode = root;
        Node<T>* lastNode = nullptr;
        while (iterNode) {
            lastNode = iterNode;
            iterNode = cmp(value, iterNode->data)
                     ? iterNode->left
                     : iterNode->right;
        }
        if (cmp(value, lastNode->data)) {
            lastNode->left = new Node<T>(value);
            lastNode->left->parent = lastNode;
        } else {
            lastNode->right = new Node<T>(value);
            lastNode->right->parent = lastNode;
        }
    }

    ~BinaryTree() {
        while (!delStack.empty()) {
            delStack.pop();
        }
        levelOrder(true); // собрать стек удаления, получится что у нас в стеке всегда листы
        Node<T>* node = delStack.top();
        delStack.pop();
        while (!delStack.empty()) {
            delete node;
            node = delStack.top();
            delStack.pop();
        }
    }

    void levelOrder(bool destructorFlag = false) {
        if (root == nullptr) {
            return;
        }
        std::queue<Node<T>*> nodeQueue;
        nodeQueue.push(root);
        delStack.push(root);
        while (!nodeQueue.empty()) {
            Node<T>* node = nodeQueue.front();
            nodeQueue.pop();
            if (destructorFlag) {
                delStack.push(node);
            } else {
                visit(node);
            }
            if (node->left != nullptr) {
                nodeQueue.push(node->left);
            }
            if (node->right != nullptr) {
                nodeQueue.push(node->right);
            }
        }
    }
private:
    Node<T>* root;
    std::stack<Node<T>*> delStack;

    void visit(Node<T>*& node) {
        std::cout << node->data << " ";
    }
};

int main() {
    int n = 0;
    std::cin >> n;
    BinaryTree<int> binaryTree;
    for (int i = 0; i < n; ++i) {
        int value = 0;
        std::cin >> value;
        binaryTree.insert(value);
    }
    binaryTree.levelOrder();
    return 0;
}