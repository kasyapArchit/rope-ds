#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

int MAX_NODE_LENGTH = 5;

class Node {
public:
    virtual ~Node() = default;

    virtual bool empty() = 0;

    virtual int size() = 0; // total size inclusive of left and right children
    virtual char charAt(int index) = 0;

    virtual string to_string() = 0;

    virtual void display(string from, int level) = 0; // for debugging
};

class LeafNode : public Node {
private:
    int weight;
    string str;

public:
    explicit LeafNode(string s) {
        str = s;
        weight = s.length();
    }

    bool empty() override {
        return weight != 0;
    }

    int size() override {
        return weight;
    }

    char charAt(int index) override {
        if (index < weight)
            return str[index];
        return '\0';
    }

    string to_string() override {
        return str;
    }

    void display(string from, int level) override {
        cout << "\n Leaf from " + from + " ,level " << level << " :" + str;
    }
};

class InternalNode : public Node {
private:
    unique_ptr<Node> left, right;
    int weight;

public:
    ~InternalNode() override {
        vector<unique_ptr<Node> > unvisitedNodes, garbageNodes;

        unvisitedNodes.push_back(std::move(left));
        unvisitedNodes.push_back(std::move(right));

        while (!unvisitedNodes.empty()) {
            unique_ptr<Node> currNode = std::move(unvisitedNodes.back());
            unvisitedNodes.pop_back();

            auto *tmp = dynamic_cast<InternalNode *>(currNode.get());
            if (tmp != nullptr) {
                if (tmp->left)
                    unvisitedNodes.push_back(std::move(tmp->left));
                if (tmp->right)
                    unvisitedNodes.push_back(std::move(tmp->right));

                garbageNodes.push_back(std::move(currNode));
            } else
                garbageNodes.push_back(std::move(currNode));
        }
    }

    InternalNode(unique_ptr<Node> left, unique_ptr<Node> right, int wt) : left{std::move(left)}, right{std::move(right)},
                                                                          weight{wt} {}

    InternalNode(unique_ptr<Node> left, unique_ptr<Node> right) : left{std::move(left)}, right{std::move(right)} {
        weight = left->size();
    }

    explicit InternalNode(const string& s) {
        int mid = ceil(s.length() / (2*1.0));
        cout << "mid: " << mid << '\n';
        if (mid < MAX_NODE_LENGTH)
            left = make_unique<LeafNode>(s.substr(0, mid));
        else
            left = make_unique<InternalNode>(s.substr(0, mid));
        if (s.length() - mid < MAX_NODE_LENGTH && mid < s.length())
            right = make_unique<LeafNode>(s.substr(mid));
        else if(mid<s.length())
            right = make_unique<InternalNode>(s.substr(mid));

        weight = mid;
    }

    bool empty() override {
        return size() != 0;
    }

    int size() override {
        int res = weight;
        if (right)
            res += right->size();
        return res;
    }

    char charAt(int index) override {
        if (left && index < weight)
            return left->charAt(index);
        else if (right)
            return right->charAt(index - weight);
        else
            return '\0';
    }

    string to_string() override {
        if (right)
            return left->to_string() + right->to_string();
        return left->to_string();
    }

    void display(string from, int level) override {
        cout << "\n Internal from " + from + " ,level: " << level << " ,weight :" << weight;
        if(left) left->display("left", level+1);
        if(right) right->display("right", level+1);
    }
};

class Rope {
private:
    unique_ptr<InternalNode> root;
    int length; // size of rope => length of string

public:
    ~Rope() {
        root.reset();
    }

    explicit Rope(string s) {
        root = make_unique<InternalNode>(s);
        length = s.length();
    }

    bool empty() const {
        return !length;
    }

    int size() const {
        return length;
    }

    int calculateSize() {
        return root->size();
    }

    void clear() {
        root.reset();
        root = make_unique<InternalNode>("");
        length = 0;
    }

    bool insert(int idx, const string& s) {
        // update size
        length += s.length();
    }

    bool erase(int first, int last) {
        // update size
        length -= last - first + 1;
    }

    char charAt(int index) {
        return root->charAt(index);
    }

    Rope *subrope(int first, int last) {
    }

    Rope *concat(Rope *r2) {
        // update size
        length += r2->size();
    }

    Rope *push_back(string s) {
        // update size
        length += s.length();
    }

    string to_string() {
        return root->to_string();
    }

    pair<Rope *, Rope *> split(int index) {
        // update size
        length = index;
    }

    void display() {
        root->display("root", 0);
    }
};

int main() {
    string s = "How would code outputs look without line breaks?";
    Rope *rope = new Rope("");
    rope->display();
    cout << '\n' << s.size() << ' ' << rope->size() << ' ' << rope->calculateSize() << '\n';
    cout << rope->to_string() << '\n';
    cout << rope->empty();
    rope->clear();
    return 0;
}
