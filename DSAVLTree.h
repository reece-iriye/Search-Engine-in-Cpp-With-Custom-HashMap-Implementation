#ifndef AVL_TREE_H
#define AVL_TREE_H

#define DEBUG

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <functional>

using namespace std;

// AvlTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x (with rebalancing)
// void remove( x )       --> Remove x (with rebalancing)
// bool contains( x )     --> Return true if x is present
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************

template <typename Key, typename Value>
class DSAVLTree
{
private:
    struct AvlNode
    {
        Key key;
        Value value;
        AvlNode *left;
        AvlNode *right;
        int height;

        AvlNode(const Key &k, const Value &v, AvlNode *lt, AvlNode *rt, int h = 0) {
            key = k;
            value = v;
            left = lt;
            right = rt;
            height = h;
        }


    };

    AvlNode *root;

public:
    /**
     * @brief Construct a new Avl Tree object
     */
    DSAVLTree() : root{nullptr}
    {
    }

    /**
     * @brief Copy constructor
     */
    DSAVLTree(const DSAVLTree &rhs) : root{nullptr}
    {
        root = clone(rhs.root);
    }

    ~DSAVLTree()
    {
        makeEmpty();
    }

    /**
     * Copy assignment operator
     */
    DSAVLTree &operator=(const DSAVLTree &rhs)
    {
        makeEmpty();
        root = clone(rhs.root);
        return *this;
    }

    /**
     * Returns true if x is found in the tree.
     */
    bool contains(const Key &x) const
    {
        return contains(x, root);
    }

    Value get(const Key &x) {
        return get(x, root);
    }

    void modify(const Key &x, const Value &newValue) {
        modify_value(x, root, newValue);
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty() const
    {
        return root == nullptr;
    }

    /**
     * Print the tree contents in sorted order.
     */
    void printTree() const
    {
        if (isEmpty())
            cout << "Empty tree" << endl;
        else
            printTree(root);
    }

    /**
     * Print the tree structure.
     */
    void prettyPrintTree() const
    {
        prettyPrintTree("", root, false);
    }

    /**
     * Make the tree empty.
     */
    void makeEmpty()
    {
        makeEmpty(root);
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert(const Key &x, const Value &y)
    {
        insert(x, y, root);
    }

    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove(const Key &x)
    {
        remove(x, root);
    }

private:
    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */

    Value get(const Key &x, AvlNode *&t) {

       if (t == nullptr) {
           throw std::runtime_error("Key doesn't exist!");
       }


        if (x < t->key) {
            return get(x, t->left);
        }

       else if (t->key < x) {
            return get(x, t->right);
        }

       else {
            return t->value;
        }


    }


    void insert(const Key &x, const Value &y, AvlNode *&t)
    {

        if (t == nullptr)
        {
            t = new AvlNode{x, y, nullptr, nullptr, 0};
            return; // a single node is always balanced
        }

        if (x < t->key)
            insert(x, y, t->left);
        else if (t->key < x)
            insert(x, y, t->right);
        else
        {
        } // Duplicate; do nothing

        // This will call balance on the way back up the tree. It will only balance once where
        // the tree got imbalanced.
        balance(t);
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void remove(const Key &x, AvlNode *&t)
    {
        /*
         *
         * Recursion is intended to find the node to delete
         *
         *
        */

        //Implemented from https://www.geeksforgeeks.org/deletion-in-an-avl-tree/


        // Empty case
        if (t == nullptr) { return; }

        // Recursive through left subtree
        if (x < t->key) { remove(x, t->left); }

        // Recursive through right subtree
        else if (t->key < x) { remove (x, t->right); }

        else {
            // Non-2-child cases
            if (t->left == nullptr || t->right == nullptr) {
                AvlNode* temp = t->left ? t->left : t->right;

                if (temp == nullptr) {
                    temp = t;
                    t = nullptr;
                }

                else {
                    *t = *temp;
                }

                delete temp;


            } // Big if statement closes

            else {
                AvlNode* temp = findMin(t->right);
                t->key = temp->key;
                t->value = temp->value;
                remove(temp->key, t->right);
            }

        }

        if (t == nullptr) {
            return;
        }
        balance(t);

    }

    /**
     * Internal method to find the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    AvlNode *findMin(AvlNode *t) const
    {
        if (t == nullptr)
            return nullptr;

        if (t->left == nullptr)
            return t;

        return findMin(t->left);
    }

    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the tree.
     */
    bool contains(const Key &x, AvlNode *t) const
    {
        if (t == nullptr)
            return false;

        else if (x < t->key)
            return contains(x, t->left);
        else if (t->key < x)
            return contains(x, t->right);
        else
            return true; // Match
    }
    /****** NONRECURSIVE VERSION*************************
        bool contains( const Comparable & x, AvlNode *t ) const
        {
            while( t != nullptr )
                if( x < t->element )
                    t = t->left;
                else if( t->element < x )
                    t = t->right;
                else
                    return true;    // Match
            return false;   // No match
        }
    *****************************************************/

    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty(AvlNode *&t)
    {
        if (t == nullptr)
            return;

        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }

    /**
     * Internal method to clone subtree.
     */
    AvlNode *clone(AvlNode *t) const
    {
        if (t == nullptr)
            return nullptr;

        return new AvlNode{t->key, t->value, clone(t->left), clone(t->right), t->height};
    }

    // Modified from: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
    void prettyPrintTree(const std::string &prefix, const AvlNode *node, bool isRight) const
    {
        if (node == nullptr)
            return;

        std::cout << prefix;
        std::cout << (isRight ? "├──" : "└──");
        // print the value of the node
        std::cout << node->value << std::endl;

        // enter the next tree level - left and right branch
        prettyPrintTree(prefix + (isRight ? "│   " : "    "), node->right, true);
        prettyPrintTree(prefix + (isRight ? "│   " : "    "), node->left, false);
    }

    // Avl manipulations

    /**
     * Return the height of node t or -1 if nullptr.
     */
    int height(AvlNode *t) const
    {
        return t == nullptr ? -1 : t->height;
    }

    static const int ALLOWED_IMBALANCE = 1; // 1 is the default; more will make balancing cheaper
    // but the search less efficient.

    // Assume t is balanced or within one of being balanced since we check this after each manipulation
    void balance(AvlNode *&t)
    {
        if (t == nullptr)
            return;

        if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)  // unbalancing insertion was left
        {
            if (height(t->left->left) >= height(t->left->right))
                rotateWithLeftChild(t); // case 1 (outside)
            else
                doubleWithLeftChild(t); // case 2 (inside)
        }
        else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) // unbalancing insertion was right
        {
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t); // case 4 (outside)
            else
                doubleWithRightChild(t); // case 3 (inside)
        }

        // update height
        t->height = max(height(t->left), height(t->right)) + 1;
    }

    int max(int lhs, int rhs) const
    {
        return lhs > rhs ? lhs : rhs;
    }

    /**
     * Rotate binary tree node with left child.
     * For AVL trees, this is a single rotation for case 1.
     * Update heights, then set new root.
     */
    void rotateWithLeftChild(AvlNode *&k2)
    {
#ifdef DEBUG
        cout << "need to rotateWithLeftChild for node " << k2->value << endl;
        cout << "Before:" << endl;
        prettyPrintTree();
#endif

        AvlNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
#ifdef DEBUG
        cout << "After:" << endl;
        prettyPrintTree();
#endif
    }

    /**
     * Rotate binary tree node with right child.
     * For AVL trees, this is a single rotation for case 4.
     * Update heights, then set new root.
     */
    void rotateWithRightChild(AvlNode *&k1)
    {
#ifdef DEBUG
        cout << "need to rotateWithRightChild for node " << k1->value << endl;
        cout << "Before:" << endl;
        prettyPrintTree();

#endif

        AvlNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;
        k1 = k2;
#ifdef DEBUG
        cout << "After:" << endl;
        prettyPrintTree();
#endif
    }

    /**
     * Double rotate binary tree node: first left child.
     * with its right child; then node k3 with new left child.
     * For AVL trees, this is a double rotation for case 2.
     * Update heights, then set new root.
     */
    void doubleWithLeftChild(AvlNode *&k3)
    {
#ifdef DEBUG
        cout << "doubleWithLeftChild" << endl;
#endif
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    /**
     * Double rotate binary tree node: first right child.
     * with its left child; then node k1 with new right child.
     * For AVL trees, this is a double rotation for case 3.
     * Update heights, then set new root.
     */
    void doubleWithRightChild(AvlNode *&k1)
    {
#ifdef DEBUG
        cout << "doubleWithRightChild" << endl;
#endif
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }
};

#endif
#ifndef ASSIGNMENT_4_OFFLINE_DSAVLTREE_H
#define ASSIGNMENT_4_OFFLINE_DSAVLTREE_H

#endif //ASSIGNMENT_4_OFFLINE_DSAVLTREE_H
