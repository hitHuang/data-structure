#include <iostream>
#include <utility>
#include <functional>
#include <memory>
using namespace std;

template<class Key, class Value>
class BSTNode{

public:
    typedef Key key_type;
    typedef Value value_type;
    typedef pair<key_type, value_type>  data_type;
    typedef BSTNode<Key, Value> node_type;
    typedef shared_ptr<node_type> link_type;
    BSTNode(key_type key = key_type(), value_type value = value_type())
        :key_(key), value_(value), left_(nullptr), right_(nullptr){ }

    BSTNode(data_type& data)
        :key_(data.first), value_(data.second), left_(nullptr), right_(nullptr){ }

public:
    key_type    key_;
    value_type  value_;
    link_type   left_;
    link_type   right_;
private:
    BSTNode& operator=(const BSTNode&);
};

template<class Key, class Value>
class BSTree{
public:
    typedef Key     key_type;
    typedef Value   value_type;
    typedef pair<key_type, value_type>  data_type;
    typedef BSTNode<key_type, value_type> node_type;
    typedef shared_ptr<node_type>  link_type;
    typedef pair<link_type, link_type> father_son_link_pair;

public:
    BSTree():root_(nullptr), size_(0){ }
    link_type find(const Key& key) const;
    link_type find(const data_type& data) const;
    link_type findMin();
    link_type findMax();
    bool insert(const key_type& key);
    bool insert(const data_type& data);
    void erase(const key_type& key);


public:
    bool empty() const {return size() == 0;};
    size_t size() const {return size_;};
    void print() const ;

private:
    link_type find_aux(link_type root, const key_type key) const ;
    father_son_link_pair find_aux_t(link_type& root, key_type key) const ;
    father_son_link_pair findMin_t(link_type& root);
    father_son_link_pair findMax_t(link_type& root);

    void print_t(const link_type& root) const ;
public:
    link_type   root_;
    size_t      size_;

private:
    BSTree(const BSTree&);
    BSTree& operator=(const BSTree&);
};

template<class Key, class Value>
typename BSTree<Key, Value>::link_type
BSTree<Key, Value>::find_aux(link_type root, key_type key) const {
    while (nullptr != root){
        if (key < root->key_){
            root = root->left_;
        }else if (root->key_ < key){
            root = root->right_;
        }else{
            return root;
        }
    }
    return nullptr;
}

template<class Key, class Value>
typename BSTree<Key, Value>::father_son_link_pair
BSTree<Key, Value>::find_aux_t(link_type& root, const key_type key) const {
    link_type pp = nullptr;
    link_type p = root;
    while (nullptr != p && p->key_ != key){
        pp = p;
        if (key < p->key_){
            p = p->left_;
        }
        else {
            p = p->right_;
        }
    }
    return father_son_link_pair(pp, p);
}

template<class Key, class Value>
typename BSTree<Key, Value>::link_type
BSTree<Key, Value>::find(const key_type& key) const {
    return find_aux(root_, key);
}

template<class Key, class Value>
typename BSTree<Key, Value>::link_type
BSTree<Key, Value>::find(const data_type& data) const {
    return find(data.first);
}

template<class Key, class Value>
typename BSTree<Key, Value>::father_son_link_pair
BSTree<Key, Value>::findMin_t(link_type& root)  {
    link_type pp = nullptr;
    link_type p = root;
    while (nullptr != p->left_){
        pp = p;
        p = p->left_;
    }
    return father_son_link_pair(pp, p);
}

template<class Key, class Value>
typename BSTree<Key, Value>::link_type
BSTree<Key, Value>::findMin() {
    return findMin_t(root_).second;
}

template<class Key, class Value>
typename BSTree<Key, Value>::father_son_link_pair
BSTree<Key, Value>::findMax_t(link_type& root) {
    link_type pp = nullptr;
    link_type p = root;
    while (nullptr != p->right_){
        pp = p;
        p = p->right_;
    }
    return father_son_link_pair(pp, p);
}

template<class Key, class Value>
typename BSTree<Key, Value>::link_type
BSTree<Key, Value>::findMax() {
    return findMax_t(root_).second;
}

template<class Key, class Value>
bool BSTree<Key, Value>::insert(const key_type& key){
    father_son_link_pair link_pair = find_aux_t(root_, key);
    link_type pp = link_pair.first;
    link_type p = link_pair.second;
    if (nullptr != p)    return false;
    link_type newNode = make_shared<node_type>(key);
    if (nullptr == pp) {
        root_ = newNode;
    }
    else {
        if (key < pp->key_){
            pp->left_ = newNode;
        }
        else {
            pp->right_ = newNode;
        }
    }
    ++size_;
    return true;
}

template<class Key, class Value>
bool BSTree<Key, Value>::insert(const data_type& data){
    father_son_link_pair link_pair = find_aux_t(root_, data.first);
    link_type pp = link_pair.first;
    link_type p = link_pair.second;
    if (nullptr != p) {
        p->value_ = data.second;
        return true;
    }
    link_type newNode = make_shared<node_type>(const_cast<data_type&>(data));
    if (nullptr == pp) {
        root_ = newNode;
    }
    else {
        if (data.first < pp->key_){
            pp->left_ = newNode;
        }
        else {
            pp->right_ = newNode;
        }
    }
    ++size_;
    return true;
}

template<class Key, class Value>
void BSTree<Key, Value>::erase(const key_type& key){
    father_son_link_pair link_pair = find_aux_t(root_, key);
    link_type pp = link_pair.first;
    link_type p = link_pair.second;
    if (nullptr == p)   return ;
    if (nullptr != p->left_ && nullptr != p->right_){
        father_son_link_pair link_pair = findMax_t(p->left_);
        link_type s = link_pair.second;
        link_type ps = link_pair.first;
        p->key_ = s->key_;
        p->value_ = s->value_;
        pp = ps;
        p = s;
    }
    link_type c;
    if (nullptr != p->left_){
        c = p->left_;
    }
    else {
        c = p->right_;
    }
    if (p == root_) root_ = c;
    else {
        if (pp->left_ == p) {
            pp->left_ = c;
        }
        else {
            pp->right_ = c;
        }
    }
    --size_;
}

template<class Key, class Value>
void BSTree<Key, Value>::print() const {
    print_t(root_);
}

template<class Key, class Value>
void BSTree<Key, Value>::print_t(const link_type& root) const {
    if (nullptr == root)    return ;
    print_t(root->left_);
    cout << "(" << root->key_  << ", " << root->value_ << ")" << "  ";
    print_t(root->right_);
}

int main()
{
    BSTree<int, string> bst;
    bst.insert(pair<int, string>(1, "hello"));
    bst.insert(pair<int, string>(2, "world"));
    bst.print();
    return 0;
}
