#ifndef FUNDAMENTAL_ALGORITHMS_2_BST_H
#define FUNDAMENTAL_ALGORITHMS_2_BST_H

#include <functional>
#include <stack>
#include <cassert>

template <typename T>
struct DefaultComparator {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <typename Key, typename Value, typename Comparator = DefaultComparator<Key>>
class BST {
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;

        Node(const Key& k, const Value& v)
                : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* root;
    size_t tree_size;
    Comparator cmp;
public:
    BST() : root(nullptr), tree_size(0) {}
    ~BST() { clear(); }

    void insert(const Key& key, const Value& value) {
        if (!root) {
            root = new Node(key, value);
            tree_size++;
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current) {
            parent = current;
            if (cmp(key, current->key)) {
                current = current->left;
            } else if (cmp(current->key, key)) {
                current = current->right;
            } else {
                current->value = value;
                return;
            }
        }

        Node* newNode = new Node(key, value);
        if (cmp(key, parent->key)) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        tree_size++;
    }

    Value* find(const Key& key) {
        Node* current = root;
        while (current) {
            if (cmp(key, current->key))
                current = current->left;

            else if (cmp(current->key, key))
                current = current->right;

            else
                return &current->value;
        }

        return nullptr;
    }

    bool remove(const Key& key) {
        Node* parent = nullptr;
        Node* current = root;

        while (current && current->key != key) {
            parent = current;
            if (cmp(key, current->key)) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (!current)
            return false;

        if (!current->left || !current->right) {
            Node* child = current->left ? current->left : current->right;

            if (!parent) {
                root = child;
            } else if (parent->left == current) {
                parent->left = child;
            } else {
                parent->right = child;
            }

            delete current;
        } else {
            Node* successor_parent = current;
            Node* successor = current->right;

            while (successor->left) {
                successor_parent = successor;
                successor = successor->left;
            }

            current->key = successor->key;
            current->value = successor->value;

            if (successor_parent->left == successor) {
                successor_parent->left = successor->right;
            } else {
                successor_parent->right = successor->right;
            }

            delete successor;
        }

        tree_size--;

        return true;
    }

    void clear() {
        std::stack<Node*> stack;
        if (root) stack.push(root);

        while (!stack.empty()) {
            Node* node = stack.top();
            stack.pop();

            if (node->left)
                stack.push(node->left);
            if (node->right)
                stack.push(node->right);

            delete node;
        }

        root = nullptr;
        tree_size = 0;
    }

    size_t size() const { return tree_size; }
    bool contains(const Key& key) { return find(key) != nullptr; }

    void inorder(std::function<void(const Key&, Value&)> callback) {
        std::stack<Node*> stack;
        Node* current = root;

        while (current || !stack.empty()) {
            while (current) {
                stack.push(current);
                current = current->left;
            }

            current = stack.top();
            stack.pop();

            callback(current->key, current->value);
            current = current->right;
        }
    }

    void preorder(std::function<void(const Key&, Value&)> callback) {
        if (!root)
            return;

        std::stack<Node*> stack;
        stack.push(root);

        while (!stack.empty()) {
            Node* node = stack.top();
            stack.pop();

            callback(node->key, node->value);

            if (node->right)
                stack.push(node->right);
            if (node->left)
                stack.push(node->left);
        }
    }

    void postorder(std::function<void(const Key&, Value&)> callback) {
        if (!root)
            return;

        std::stack<Node*> stack1, stack2;
        stack1.push(root);

        while (!stack1.empty()) {
            Node* node = stack1.top();
            stack1.pop();
            stack2.push(node);

            if (node->left)
                stack1.push(node->left);

            if (node->right)
                stack1.push(node->right);
        }

        while (!stack2.empty()) {
            callback(stack2.top()->key, stack2.top()->value);
            stack2.pop();
        }
    }

    class Iterator {
        std::stack<Node*> stack;
        Node* current;

        void push_left(Node* node) {
            while (node) {
                stack.push(node);
                node = node->left;
            }
        }

    public:
        Iterator(Node* root = nullptr) {
            push_left(root);
            current = stack.empty() ? nullptr : stack.top();
        }

        std::pair<const Key&, Value&> operator*() {
            return {current->key, current->value};
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Iterator& operator++() {
            if (stack.empty()) {
                current = nullptr;
                return *this;
            }

            Node* node = stack.top();
            stack.pop();
            push_left(node->right);

            current = stack.empty() ? nullptr : stack.top();

            return *this;
        }

        bool operator==(const Iterator& other) const { return current == other.current; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    Iterator begin() { return Iterator(root); }
    Iterator end() { return Iterator(); }
};

template <typename Key,
        typename Value,
        template<typename, typename, typename> class TreeType,
        typename Comparator = DefaultComparator<Key>>
class MyMap {
    TreeType<Key, Value, Comparator> tree;

public:
    void insert(const Key& key, const Value& value) { tree.insert(key, value); }
    Value* find(const Key& key) { return tree.find(key); }
    bool remove(const Key& key) { return tree.remove(key); }
    bool contains(const Key& key) { return tree.contains(key); }
    void clear() { tree.clear(); }
    size_t size() const { return tree.size(); }
    auto begin() { return tree.begin(); }
    auto end() { return tree.end(); }

    void inorder(std::function<void(const Key&, Value&)> callback) { tree.inorder(callback); }
    void preorder(std::function<void(const Key&, Value&)> callback) { tree.preorder(callback); }
    void postorder(std::function<void(const Key&, Value&)> callback) { tree.postorder(callback); }
};
#endif //FUNDAMENTAL_ALGORITHMS_2_BST_H
