#pragma once
#include <string>
#include <iostream>

enum Color { BLACK, RED };

namespace DM852 {
class Tree {
public:
    class Node {
    private:
        friend class Tree;
        Color _color = BLACK;
    public:
        const int key;
        
        std::string value;
        
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
        
        /**
         * Returns the next node with key greater than this node.
         */
        Node *next();
        
        /**
         * Returns the next node with key greater than this node.
         */
        const Node *next() const;
        
        /**
         * Returns the next node with key less than this node.
         */
        Node *prev();
        
        /**
         * Returns the next node with key less than this node.
         */
        const Node *prev() const;

        /**
         * This method frees the memory used by this node and it's children.
         */
        void clear();

    public:

        Node() = delete;
    

        Node( int key, const std::string str ) : key(key) {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
            std::string wtf(str);
            value = wtf;
        }

        Node( const Node &other ) : key(other.key), value(other.value) {
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            if ( other.left != nullptr ) {
                left = new Node(*other.left);
                left->parent = this;
            } if ( other.right != nullptr ) {
                right = new Node(*other.right);
                right->parent = this;
            }
        }

        ~Node() = default;
    };
private:
    int _size = 0;

    Node *_root = nullptr;

    void left_rotate( Node *node );
    void right_rotate( Node *node );
    void transplant( Node *u, Node *v );
    void insert_fixup( Node *node );

public:
    friend bool operator==(const Tree &l, const Tree &r);

    /**
     * Constructs a new empty Red Black Tree
     */
    Tree() = default;

    /**
     * Constructs a new copy of the given Red Black Tree
     */
    Tree( const Tree &other ) {
        _size = other._size;
        if ( other._root == nullptr )
            _root = nullptr;
        else
            _root = new Node( *other._root );
    }
    
    ~Tree() {
        clear();
    }

    /**
     * @return: A copy of the given Tree
     * Runtime: O(n)
     */
    Tree &operator=( const Tree &other ) {
        Tree *t = new Tree;
        t->_root = new Node( *other._root );
        t->_size = other._size;
        return *t;
    }

    
    /**
     * @return: The size of the Tree
     * Runtime: O(1)
     */
    int size() const;

    /**
     * @return: True if this collection is empty
     * Runtime: O(1)
     */
    bool empty() const;

    /**
     * Inserts the value with the given key.
     * @return: The pointer to the Node, and true if it inserted a new Node into the Tree
     * Runtime: O(log(n))
     */
    std::pair<Node *, bool> insert( int key, const std::string &value );

    /**
     * Returns the Node containing the key if it exists, otherwise nullptr
     * @return: The Node containing the key or nullptr if it's not contained
     * Runtime: O(log(n))
    */
    Node *find( int key );

    /**
     * Returns the Node containing the key if it exists, otherwise nullptr
     * @return: The Node containing the key or nullptr if it's not contained
     * Runtime: O(log(n))
    */
    const Node *find( int key ) const;

    /**
     * Clears this collection
     * Runtime: O(n)
     */
    void clear();

    /**
     * @return: The element with the lowest key
     * Runtime: O(log(n))
     */
    std::string &front();

    /**
     * @return: The element with the lowest key
     * Runtime: O(log(n))
     */
    const std::string &front() const;

    /**
     * @return: The element with the highest key
     * Runtime: O(log(n))
     */
    std::string &back();

    /**
     * @return: The element with the highest key
     * Runtime: O(log(n))
     */
    const std::string &back() const;

    /**
     * @return: The Node with the lowest key
     * Runtime: O(log(n))
     */
    Node *begin();

    /**
     * @return: The Node with the lowest key
     * Runtime: O(log(n))
     */
    const Node *begin() const;

    /**
     * @return: nullptr
     */
    Node *end();

    /**
     * @return: nullptr
     */
    const Node *end() const;

};
}
