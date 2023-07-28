#include "Tree.hpp"

namespace DM852 {
using Node = Tree::Node;



void Node::clear() {
    if ( left != nullptr )
        left->clear();
    if ( right != nullptr )
        right->clear();
    delete this;
}

Node *Node::next() {
    const Node *t = this;
    const Node *n = t->next();
    return const_cast<Node *>(n);
}

const Node *Node::next() const {
    if ( right == nullptr ) {
        if ( parent != nullptr && parent->key > key )
            return parent;
        Node *ret = parent;
        while ( ret != nullptr && ret->key < key )
            ret = ret->parent;
        return ret;
    }

    Node *ret = right;
    while ( ret->left != nullptr )
        ret = ret->left;
    return ret;
}

Node *Node::prev() {
    const Node *t = this;
    const Node *n = t->prev();
    return const_cast<Node *>(n);
}

const Node *Node::prev() const {
    if ( left == nullptr ) {
        if ( parent != nullptr && parent->key < key )
            return parent;
        Node *ret = parent;
        while ( ret != nullptr && ret->key > key )
            ret = ret->parent;
        return ret;
    }

    Node *ret = left;
    while ( ret->right != nullptr ) {
        ret = ret->right;
    }
    return ret;
}

int Tree::size() const {
    return _size;
}

bool Tree::empty() const {
    return _size == 0;
}

void Tree::left_rotate( Node *node ) {
    Node *y = node->right;
    node->right = y->left;
    if ( y->left != nullptr )
        y->left->parent = node;
    y->parent = node->parent;
    if ( node->parent == nullptr )
        _root = y;
    else if ( node == node->parent->left )
        node->parent->left = y;
    else
        node->parent->right = y;
    y->left = node;
    node->parent = y;
}
void Tree::right_rotate( Node *node ) {
    Node *y = node->left;
    node->left = y->right;
    if ( y->right != nullptr ) 
        y->right->parent = node;
    y->parent = node->parent;
    if ( node->parent == nullptr )
        _root = y;
    else if ( node == node->parent->right )
        node->parent->right = y;
    else
        node->parent->left = y;
    y->right = node;
    node->parent = y;
}
void Tree::transplant( Node *u, Node *v ) {
    if ( u->parent == nullptr )
        _root = v;
    else if ( u == u->parent->left )
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}
void Tree::insert_fixup( Node *node ) {
    while ( node->parent != nullptr && node->parent->_color == RED ) {
        if ( node->parent->parent != nullptr && node->parent == node->parent->parent->right ) {
            Node *y = node->parent->parent->right;
            if ( y->_color == RED ) {
                node->parent->_color = BLACK;
                y->_color = BLACK;
                node->parent->parent->_color = RED;
                node = node->parent->parent;
            } else {
                if ( node == node->parent->right ) {
                    node = node->parent;
                    left_rotate( node );
                }
                node->parent->_color = BLACK;
                node->parent->parent->_color = RED;
                right_rotate( node->parent->parent );
            }
        } else if ( node->parent->parent != nullptr ) {
            Node *y = node->parent->parent->left;
            if ( y->_color == RED ) {
                node->parent->_color = BLACK;
                y->_color = BLACK,
                node->parent->parent->_color = RED;
                node = node->parent->parent;
            } else {
                if ( node == node->parent->left ) {
                    node = node->parent;
                    right_rotate( node );
                }
                node->parent->_color = BLACK;
                node->parent->parent->_color = RED;
                left_rotate( node->parent->parent );
            }
        }
    }
    _root->_color = BLACK;
}

std::pair<Node *, bool> Tree::insert( int key, const std::string &value ) {

    Node *current = _root;
    Node *parent = nullptr;

    while ( current != nullptr ) {
        parent = current;
        if ( key == current->key ) {
            current->value = value;
            return {current, false};
        } else if ( key < current->key )
            current = current->left;
        else
            current = current->right;
    }
    Node *new_node = new Node(key, value);
    new_node->parent = parent;
    if ( parent == nullptr ) {
        _root = new_node;
    } else if ( key < parent->key ) {
        parent->left = new_node;
    } else
        parent->right = new_node;
    
    new_node->_color = RED;

    insert_fixup( new_node );
    ++_size;
    return {new_node, true};
}

Node *Tree::find( int key ) {
    const Tree *t = this;
    const Node *n = t->find( key );
    return const_cast<Node *>(n);
}

const Node *Tree::find( int key ) const {
    Node *ret = _root;

    while ( ret != nullptr && key != ret->key ) {
        if ( key < ret->key ) ret = ret->left;
        else                  ret = ret->right;
    }

    return ret;    
}

void Tree::clear() {
    if ( _root != nullptr )
        _root->clear();
    _root = nullptr;
    _size = 0;
}

std::string &Tree::front() {
    const Tree *t = this;
    const std::string &s = t->front();
    return const_cast<std::string &>(s);
}

const std::string &Tree::front() const {
    return begin()->value;
}

std::string &Tree::back() {
    const Tree *t = this;
    const std::string &s = t->back();
    return const_cast<std::string &>(s);

}

const std::string &Tree::back() const {
    Node *ret = _root;
    while ( ret->right != nullptr ) 
        ret = ret->right;
    return ret->value;
}

Node *Tree::begin() {
    const Tree *t = this;
    const Node *s = t->begin();
    return const_cast<Node *>(s);
}

const Node *Tree::begin() const {
    Node *ret = _root;
    while ( ret != nullptr && ret->left != nullptr ) {
        ret = ret->left;
    }
    return ret;
}

Node *Tree::end() {
    return nullptr;
}

const Node *Tree::end() const {
    return nullptr;
}

/**
 * @return: whether or not other is equal to this
 */
bool operator==(const Node &l, const Node &r) {
    return l.key == r.key && l.value == r.value;
}

/**
 * @return: whether or not other is not equal to this
 */
bool operator!=(const Node &l, const Node &r) {
    return !(l == r);
}

/**
 * @return: whether or not other is equal to this
 */
bool operator==(const Tree &l, const Tree &r) {
    if ( l.size() != r.size() ) return false;
    
    const Node *le = l.begin();
    const Node *re = r.begin();
    while ( le != nullptr ) {
        if ( *le != *re ) return false;
        le = le->next();
        re = re->next();
    }
    return true;
}

/**
 * @return: whether or not other is not equal to this
 */
bool operator!=(const Tree &l, const Tree &r) {
    return !(l == r);
}

}