#include <iostream>
#include <string>
#include <memory>
#include <vector>
using namespace std;

class Node {
  public:
    virtual int weight() const = 0;
    virtual int size() const = 0;
    virtual char charAt(int) const = 0;
    virtual string to_string() const = 0;
    // using unique_ptr for automatic cleanup to prevent memory leaks
    virtual pair<unique_ptr<Node>, unique_ptr<Node>> split(int index) = 0;
};

class LeafNode: public Node {
  private:
    string str;

  public:
    LeafNode(string s) : str{s} {}
    
    int weight() const override {
      return size();
    }

    int size() const override {
      return str.length();
    }

    char charAt(int pos) const override
    {
      if (pos < size())
      {
        return str[pos];
      }
      return '\0'; // returning null charcter if not a valid position
    }

    string to_string() const override {
      return str;
    }

    pair<unique_ptr<Node>, unique_ptr<Node>> split(int index) override {
      unique_ptr<LeafNode> root1 = make_unique<LeafNode>(str.substr(0, index));
      unique_ptr<LeafNode> root2 = make_unique<LeafNode>(str.substr(index));
      return make_pair(root1, root2);
    }
};


class InternalNode: public Node {
  private:
    unique_ptr<Node> left, right;

    static string getString(Node* node) {
      string res = "";
      if (node == nullptr)
        return res;

      vector<Node *> unvisitedNodes;
      // inorder traversal
      while(node != nullptr || unvisitedNodes.size()) {
        if(node != nullptr) {
          unvisitedNodes.push_back(node);

          InternalNode *tmp = dynamic_cast<InternalNode *>(node);
          if(tmp != nullptr)
            node = tmp->left.get();
          else
            node = nullptr;
        } else {
          node = unvisitedNodes.back();
          unvisitedNodes.pop_back();

          InternalNode *tmp = dynamic_cast<InternalNode *>(node);
          if(tmp != nullptr)
            node = tmp->right.get();
          else {
            res += node->to_string();
            node = nullptr;
          }
        }
      }
      return res;
    }

  public:
    InternalNode(unique_ptr<Node> left, unique_ptr<Node> right) : left{move(left)}, right{move(right)} {}
    // InternalNode(unique_ptr<Node> left, unique_ptr<Node> right) {
    //   this->left = make_unique<Node>(left);
    //   this->right = make_unique<Node>(right);
    // }

    ~InternalNode() {
      // dfs to collect all nodes in garbageNodes
      // since this is a vector<unique_ptr> all of these will be destroyed at the end.
      vector<unique_ptr<Node>> unvisitedNodes, garbageNodes;

      unvisitedNodes.push_back(move(left));
      unvisitedNodes.push_back(move(right));

      while(unvisitedNodes.size()) {
        unique_ptr<Node> currNode = move(unvisitedNodes.back());
        unvisitedNodes.pop_back();

        InternalNode *tmp = dynamic_cast<InternalNode *>(currNode.get());
        if(tmp != nullptr) {
          if(tmp->left)
            unvisitedNodes.push_back(move(tmp->left));
          if (tmp->right)
            unvisitedNodes.push_back(move(tmp->right));

          garbageNodes.push_back(move(currNode));
        } else
          garbageNodes.push_back(move(currNode));
      }
    }

    int weight() const override {
      return left->size();
    }

    int size() const override {
      vector<Node *> unvisitedNodes;
      unvisitedNodes.push_back(left.get());
      unvisitedNodes.push_back(right.get());

      int sz = 0;
      while (unvisitedNodes.size()) {
        Node* currNode = unvisitedNodes.back();
        unvisitedNodes.pop_back();

        // if InternalNode
        // else return size from LeafNode
        InternalNode *tmp = dynamic_cast<InternalNode *>(currNode);

        if(tmp != nullptr) {
          if(tmp->left)
            unvisitedNodes.push_back(tmp->left.get());
          if(tmp->right)
            unvisitedNodes.push_back(tmp->right.get());
        } else
          sz += currNode->size();
      }
      return sz;
    }

    char charAt(int pos) const override {
      int currWeight = weight();
      if(pos < currWeight)
        return left->charAt(pos);
      else
        return right->charAt(pos - currWeight);
    }

    string to_string() const override {
      return getString(left.get()) + getString(right.get());
    }

    pair<unique_ptr<Node>, unique_ptr<Node>> split(int index) override {
      
    }
};
