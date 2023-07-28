#pragma once
#include <string>
#include <iostream>
#include <cassert>

enum Color { BLACK, RED };

namespace DM852 {
    
template <typename Key, typename Value, typename Comp = std::less<Key>>
class Tree {
public:
    class iterator;
    class const_iterator;

    using value_type = std::pair<const Key, Value>;
private:

    class Node {
    private:
        friend class Tree;
        friend class iterator;
        friend class const_iterator;
        using value_type = std::pair<const Key, Value>;

        Color _color = BLACK;
    // public:
        value_type key_val;

        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
        
        /**
         * Returns the next node with key greater than this node.
         */
        Node *next() {
            const Node *t = this;
            const Node *n = t->next();
            return const_cast<Node *>(n);
        }
        
        /**
         * Returns the next node with key greater than this node.
         */
        const Node *next() const {
            if ( right == nullptr ) {
                if ( parent != nullptr && parent->key() > key() )
                    return parent;
                Node *ret = parent;
                while ( ret != nullptr && ret->key() < key() )
                    ret = ret->parent;
                return ret;
            }

            Node *ret = right;
            while ( ret->left != nullptr )
                ret = ret->left;
            return ret;
        }
        
        /**
         * Returns the next node with key less than this node.
         */
        Node *prev() {
            const Node *t = this;
            const Node *n = t->prev();
            return const_cast<Node *>(n);
        }
        
        /**
         * Returns the next node with key less than this node.
         */
        const Node *prev() const {
            if ( left == nullptr ) {
                if ( parent != nullptr && parent->key() < key() )
                    return parent;
                Node *ret = parent;
                while ( ret != nullptr && ret->key() > key() )
                    ret = ret->parent;
                return ret;
            }

            Node *ret = left;
            while ( ret->right != nullptr ) {
                ret = ret->right;
            }
            return ret;
        }

        /**
         * This method frees the memory used by this node and it's children.
         */
        void clear() {
            if ( left != nullptr )
                left->clear();
            if ( right != nullptr )
                right->clear();
            delete this;
        }

        const Key key() const {
            return key_val.first;
        }

        Key key() {
            return key_val.first;
        }

        Value value() {
            return key_val.second;
        }

    // public:

        Node() = delete;
    

        Node( Key key, Value val ) : key_val( std::pair<Key, Value>(key, val) ) {
            parent = nullptr;
            left = nullptr;
            right = nullptr;
        }

        Node( const Node &other ) :  key_val( std::pair<Key, Value>(other.key_val.first, other.key_val.second) ) {
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

        Node( Key &&key, Value &&value ) : 
            key_val(std::pair<Key, Value>(std::move(key), std::move(value))) {}

        /**
         * @return: whether or not other is equal to this
         */
        friend bool operator==(const Node &l, const Node &r) {
            return l.key == r.key && l.value == r.value;
        }

        /**
         * @return: whether or not other is not equal to this
         */
        friend bool operator!=(const Node &l, const Node &r) {
            return !(l == r);
        }

        ~Node() = default;
    };
    int _size = 0;

    Comp comparator;
    Node *_root = nullptr;
    Node *_smallest = nullptr;
    Node *_biggest = nullptr;
    void left_rotate( Node *node ) {
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
    void right_rotate( Node *node ) {
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
    void transplant( Node *u, Node *v ) {
        if ( u->parent == nullptr )
            _root = v;
        else if ( u == u->parent->left )
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }
    void insert_fixup( Node *node ) {
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

public:
    class iterator {
    public:    
        using value_type = std::pair<const Key, Value>;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::bidirectional_iterator_tag;
    private:
        friend class Tree;
    public:
        iterator() = default;
        iterator(Tree *tree, Node *p) : tree(tree), p(p) {}
        iterator(const iterator *it) : tree(it->tree), p(it->p) {}
        
        ~iterator() = default;

        reference operator*() const {
			assert(p); // this must be dereferencable
			return p->key_val;
		}

        pointer operator->() const {
			assert(p); // this must be dereferencable
			return &p->key_val;
		}

		iterator &operator++() {
			assert(p); // this must be incrementable
            if ( p != nullptr )
                p = p->next();
			return *this;
		}

		iterator operator++(int) {
			assert(p); // this must be incrementable
            iterator old(this);
            if ( p != nullptr )
                p = p->next();
			return old;
		}

		iterator &operator--() {
			assert(p); // this must be decrementable
            if ( p == nullptr )
                p = tree->_biggest;
            else
                p = p->prev();
			return *this;
		}

		iterator operator--(int) {
			assert(p); // this must be incrementable
            iterator old(this);
            if ( p == nullptr )
                p = tree->_biggest;
            else
                p = p->prev();
			return old;
		}

		friend bool operator==(iterator a, iterator b) {
			return a.p == b.p;
		}
		friend bool operator!=(iterator a, iterator b) {
			return !(a == b);
		}
	private:
		Tree *tree = nullptr;
        Node *p = nullptr;
    };
    class const_iterator {
    public:    
        using value_type = std::pair<const Key, Value>;
        using difference_type = std::ptrdiff_t;
        using reference = const value_type&;
        using pointer = const value_type*;
        using iterator_category = std::bidirectional_iterator_tag;
    private:
        friend class Tree;
        
    public:
        const_iterator() = default;
        const_iterator(const Tree *tree, Node *p) : tree(tree), p(p) {}
        const_iterator(const const_iterator *it) : tree(it->tree), p(it->p) {}

        const_iterator(const iterator &other) : tree(other.tree), p(other.p) {}
        ~const_iterator() = default;


        reference operator*() const {
			assert(p); // this must be dereferencable
			return p->key_val;
		}

        pointer operator->() const {
			assert(p); // this must be dereferencable
			return &p->key_val;
		}

		const_iterator &operator++() {
			assert(p); // this must be incrementable
            if ( p != nullptr )
                p = p->next();
			return *this;
		}

		const_iterator operator++(int) {
			assert(p); // this must be incrementable
            const_iterator old(this);
            if ( p != nullptr )
                p = p->next();
			return old;
		}

		const_iterator &operator--() {
			assert(p); // this must be decrementable
            if ( p == nullptr )
                p = tree->_biggest;
            else
                p = p->prev();
			return *this;
		}

		const_iterator operator--(int) {
			assert(p); // this must be incrementable
            const_iterator old(this);
            if ( p == nullptr )
                p = tree->_biggest;
            else
                p = p->prev();
			return old;
		}

		friend bool operator==(const_iterator a, const_iterator b) {
			return a.p == b.p;
		}
		friend bool operator!=(const_iterator a, const_iterator b) {
			return !(a == b);
		}
	private:
        const Tree *tree = nullptr;
		Node *p = nullptr;
    };

    // friend bool operator==(const Tree &l, const Tree &r);

    /**
     * Constructs a new empty Red Black Tree
     */
    Tree() {
        comparator = std::less<Key>{};
    };

    /**
     * Constructs a new copy of the given Red Black Tree
     * Runtime: O(n)
     */
    Tree( const Tree<Key, Value, Comp> &other ) {
        _size = other._size;
        if ( other._root == nullptr )
            _root = nullptr;
        else {
            _root = new Node( *other._root );
            _smallest = _begin();
            _biggest = _root;
            while ( _biggest->right != nullptr ) {
                _biggest = _biggest->right;
            }
        }
    }

    /**
     * Moves the given Red Black Tree
     * Runtime: O(1)
     */
    Tree(Tree<Key, Value, Comp> &&other) noexcept : 
        _root(other._root), 
        _size(other._size), 
        _smallest(other._smallest), 
        _biggest(other._biggest)
    {
        other._root = nullptr;
        other._size = 0;
        other._smallest = nullptr;
        other._biggest = nullptr;
    }
    

    Tree( Comp comp ) : comparator(comp) {}

    ~Tree() {
        clear();
    }


    /**
     * @return: whether or not other is equal to this
     */
    friend bool operator==(const Tree &l, const Tree &r) {
        
        if ( l.size() != r.size() ) return false;
        
        const_iterator le = l.begin();
        const_iterator re = r.begin();

        while ( le != l.end() ) {
            if ( *le != *re ) return false;
            le++;
            re++;
        }
        return true;
    }

    /**
     * @return: whether or not other is not equal to this
     */
    friend bool operator!=(const Tree &l, const Tree &r) {
        return !(l == r);
    }


    /**
     * @return: A copy of the given Tree
     * Runtime: O(n)
     */
    Tree &operator=(const Tree<Key, Value, Comp> &other) {
        if (this != &other) {
            Tree tmp(other);
            std::swap(_root, tmp._root);
            std::swap(_size, tmp._size);
            std::swap(_smallest, tmp._smallest);
            std::swap(_biggest, tmp._biggest);
        }
        return *this;
    }

    Tree& operator=( Tree<Key, Value, Comp> &&other ) noexcept {
        if (this != &other) {
            delete _root; // release the old resources
            _root = other._root;
            _size = other._size;
            _smallest = other._smallest;
            _biggest = other._biggest;
            other._root     = nullptr;
            other._smallest = nullptr;
            other._biggest  = nullptr;
        }
        return *this;
    }

    
    /**
     * @return: The size of the Tree
     * Runtime: O(1)
     */
    int size() const {
        return _size;
    }

    /**
     * @return: True if this collection is empty
     * Runtime: O(1)
     */
    bool empty() const {
        return _size == 0;
    }

    /**
     * Inserts the value with the given key.
     * @return: The pointer to the Node, and true if it inserted a new Node into the Tree
     * Runtime: O(log(n))
     */
    std::pair<iterator, bool> insert( const Key &key, const Value &value ) {

        Node *current = _root;
        Node *parent = nullptr;

        while ( current != nullptr ) {
            parent = current;
            if ( !comparator(key, current->key()) && !comparator(current->key(), key) ) {
                current->key_val.second = value;
                return {iterator(this, current), false};
            } else if ( comparator(key, current->key()) )
                current = current->left;
            else
                current = current->right;
        }
        Node *new_node = new Node(key, value);
        new_node->parent = parent;
        if ( parent == nullptr ) {
            _root = new_node;
            _smallest = new_node; // The tree was empty. Thus this new node is both the smallest and the biggest
            _biggest = new_node;
        } else if ( comparator(key, parent->key()) ) {
            parent->left = new_node;
            if ( comparator(key, _smallest->key()) )
                _smallest = new_node; // We have found a new smallest
        } else {
            parent->right = new_node;
            if ( comparator(_biggest->key(), key) ) 
                _biggest = new_node; // We have found a new biggest
        }
        new_node->_color = RED;

        insert_fixup( new_node );
        ++_size;
        return {iterator(this, new_node), true};
    }

    std::pair<iterator, bool>insert( Key &&key, Value &&value ) {
        Node *current = _root;
        Node *parent = nullptr;

        while ( current != nullptr ) {
            parent = current;
            if ( !comparator(key, current->key()) && !comparator(current->key(), key) ) {
                current->key_val.second = std::move(value);
                return {iterator(this, current), false};
            } else if ( comparator(key, current->key()) )
                current = current->left;
            else
                current = current->right;
        }
        Node *new_node = new Node(key, value);
        new_node->parent = parent;
        if ( parent == nullptr ) {
            _root = new_node;
            _smallest = new_node; // The tree was empty. Thus this new node is both the smallest and the biggest
            _biggest = new_node;
        } else if ( comparator(key, parent->key()) ) {
            parent->left = new_node;
            if ( comparator(key, _smallest->key()) )
                _smallest = new_node; // We have found a new smallest
        } else {
            parent->right = new_node;
            if ( comparator(_biggest->key(), key) ) 
                _biggest = new_node; // We have found a new biggest
        }
        new_node->_color = RED;

        insert_fixup( new_node );
        ++_size;
        return {iterator(this, new_node), true};
    }

    /**
     * Returns the Node containing the key if it exists, otherwise nullptr
     * @return: The Node containing the key or nullptr if it's not contained
     * Runtime: O(log(n))
    */
    iterator find( Key key ) {
        Node *ret = _root;

        while ( ret != nullptr && key != ret->key() ) {
            if ( comparator(key, ret->key()) ) ret = ret->left;
            else                               ret = ret->right;
        }

        return iterator(this, ret);
    }

    /**
     * Returns the Node containing the key if it exists, otherwise nullptr
     * @return: The Node containing the key or nullptr if it's not contained
     * Runtime: O(log(n))
    */
    const_iterator find( Key key ) const {
        Node *ret = _root;

        while ( ret != nullptr && key != ret->key() ) {
            if ( comparator(key, ret->key()) ) ret = ret->left;
            else                               ret = ret->right;
        }

        return const_iterator(this, ret);    
    }

    /**
     * Clears this collection
     * Runtime: O(n)
     */
    void clear() {
        if ( _root != nullptr )
            _root->clear();
        _root = nullptr;
        _smallest = nullptr;
        _biggest = nullptr;
        _size = 0;
    }

    /**
     * @return: The element with the lowest key
     * Runtime: O(log(n))
     */
    value_type &front() {
        const Tree *t = this;
        const value_type &s = t->front();
        return const_cast<value_type &>(s);
    }

    /**
     * @return: The element with the lowest key
     * Runtime: O(log(n))
     */
    const value_type &front() const {
        return _begin()->key_val;
    }

    /**
     * @return: The element with the highest key
     * Runtime: O(log(n))
     */
    value_type &back() {
        const Tree *t = this;
        const value_type &s = t->back();
        return const_cast<value_type &>(s);
    }

    /**
     * @return: The element with the highest key
     * Runtime: O(log(n))
     */
    const value_type &back() const {
        Node *ret = _root;
        while ( ret->right != nullptr ) 
            ret = ret->right;
        return ret->key_val;
    }

private:
    /**
     * @return: The Node with the lowest key
     * Runtime: O(log(n))
     */
    Node *_begin() {
        const Tree *t = this;
        const Node *s = t->_begin();
        return const_cast<Node *>(s);
    }

    /**
     * @return: The Node with the lowest key
     * Runtime: O(log(n))
     */
    const Node *_begin() const {
        Node *ret = _root;
        while ( ret != nullptr && ret->left != nullptr ) {
            ret = ret->left;
        }
        return ret;
    }
public:

    /**
     * @return: Iterator with the lowest key
     * Runtime: O(1)
     */
    iterator begin() {
        // Node *ret = _begin();
        return iterator(this, _smallest);
    }

    /**
     * @return: Iterator with the lowest key
     * Runtime: O(1)
     */
    const_iterator begin() const {
        return const_iterator(this, _smallest);
    }

    /**
     * @return: End of iterator
     * Runtime: O(1)
     */
    iterator end() {
        return Tree::iterator(this, nullptr);
    }

    /**
     * @return: End of iterator
     * Runtime: O(1)
     */
    const_iterator end() const {
        return Tree::const_iterator(this, nullptr);
    }

};

}
