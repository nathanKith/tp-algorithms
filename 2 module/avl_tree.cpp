#include <iostream>
#include <stack>

template <typename T>
class AvlTree
{
    struct Node
    {
        Node(const T &data) : data(data), left(nullptr), right(nullptr), height(1), nodesQuantity(1) {}
        T data;
        Node *left, *right;
        size_t height;
        size_t nodesQuantity;
    };

public:
    AvlTree() : root(nullptr) {}
    ~AvlTree()
    {
        destroyTree(root);
    }

    bool Has(const T &data)
    {
        Node *tmp = root;
        while (tmp)
        {
            if (tmp->data == data)
                return true;
            else if (tmp->data < data)
                tmp = tmp->right;
            else
                tmp = tmp->left;
        }
        return false;
    }

    void Add(const T &data)
    {
        root = addInternal(root, data);
    }

    void Remove(const T &data)
    {
        root = removeInternal(root, data);
    }

    size_t GetStatistic(const T &data) {
        Node* node = root;
        size_t kth = 0;
        while (node) {
            if (node->data == data) {
                return kth + getNodesQuantity(node->left);
            }

            if (data < node->data) {
                kth += getNodesQuantity(node->left) + 1;
                node = node->left;
            } else if (data > node->data){
                node = node->right;
            }
        }
        return 0;
    }

private:
    Node *root;

    size_t getNodesQuantity(Node* node)
    {
        return node ? node->nodesQuantity : 0;
    }

    void setNodesQuantity(Node* node)
    {
        node->nodesQuantity = getNodesQuantity(node->left) + getNodesQuantity(node->right) + 1;
    }

    void fixHeight(Node *node)
    {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    size_t getHeight(Node *node)
    {
        return node ? node->height : 0;
    }

    int getBalance(Node *node)
    {
        return getHeight(node->right) - getHeight(node->left);
    }

    Node* rotateLeft(Node *node)
    {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;

        fixHeight(node);
        fixHeight(tmp);
        setNodesQuantity(node);
        setNodesQuantity(tmp);

        return tmp;
    }

    Node* rotateRight(Node *node)
    {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;

        fixHeight(node);
        fixHeight(tmp);
        setNodesQuantity(node);
        setNodesQuantity(tmp);

        return tmp;
    }

    Node* doBalance(Node *node)
    {
        fixHeight(node);
        setNodesQuantity(node);
        switch (getBalance(node))
        {
            case 2:
            {
                if (getBalance(node->right) < 0)
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2:
            {
                if (getBalance(node->left) > 0)
                    node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

    Node* findMin(Node *node)
    {
        while (node->left)
            node = node->left;
        return node;
    }

    Node* removeMin(Node *node)
    {
        if (!node->left)
            return node->right;
        node->left = removeMin(node->left);
        return doBalance(node);
    }

    Node* findMax(Node* node)
    {
        while (node->right)
            node = node->right;
        return node;
    }

    Node* removeMax(Node* node)
    {
        if (!node->right)
            return node->left;
        node->right = removeMax(node->right);
        return doBalance(node);
    }

    // не работает
//    Node* findAndRemoveMin(Node* node) {
//        Node* resNode = node;
//        std::stack<Node*> s;
//        while (!node->left) {
//            s.push(node);
//            node = node->left;
//        }
//
//        node = node->right;
//
//        while (!s.empty()) {
//            doBalance(s.top());
//            s.pop();
//        }
//
//        // не работает
//
//        return resNode;
//    }


    Node* addInternal(Node *node, const T &data)
    {
        if (!node)
            return new Node(data);
        if (node->data <= data)
            node->right = addInternal(node->right, data);
        else
            node->left = addInternal(node->left, data);
        return doBalance(node);
    }

    Node* removeInternal(Node *node, const T &data)
    {
        if (!node)
            return nullptr;
        if (getNodesQuantity(node->left) < data)
        {
            node->right = removeInternal(node->right, data - getNodesQuantity(node->left) - 1);
        }
        else if (getNodesQuantity(node->left) > data)
        {
            node->left = removeInternal(node->left, data);
        } else {
            Node *left = node->left;
            Node *right = node->right;
            delete node;

            if (!right && !left)
                return nullptr;

            if (getHeight(left) > getHeight(right)) {
                Node *max = findMax(left);
                max->left = removeMax(left);
                max->right = right;
                return doBalance(max);
            } else {
                Node *min = findMin(right);
                min->right = removeMin(right);
                min->left = left;
                return doBalance(min);
            }

        }
        return doBalance(node);
    }

    void destroyTree(Node *node)
    {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};

int main(int argc, const char * argv[]) {
    AvlTree<int> avlTree;

    int n = 0;
    std::cin >> n;
    int op, value;

    for (int i = 0; i < n; ++i) {
        std::cin >> op >> value;
        if (op == 1) {
            avlTree.Add(value);
            std::cout << avlTree.GetStatistic(value);
        } else if (op == 2) {
            avlTree.Remove(value);
        }
    }
    return 0;
}