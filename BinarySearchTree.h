// STUDENT CODE
// San Diego State University
// CS210/CS496: Data Structures
// Yuta Nakano
// File: BinarySearchTree.h
// A concrete implementation of a basic binary search tree. This structure is
// useful only for education and illustration of the BAD things about growth.
// Additional Reading:
// https://en.wikipedia.org/wiki/Binary_search_tree

#ifndef PROG2_BINARYSEARCHTREE_H
#define PROG2_BINARYSEARCHTREE_H

#include <memory>
#include "Map.h"

namespace sdsu {
    template<typename KEY, typename VALUE>
    class BinarySearchTree : public Map<KEY, VALUE> {

        // The BST links together BSTNode objects. The outside world never
        // needs to use these, but this data structure will use them to build
        // the search tree.
        struct BSTNode {

            // The BST structures itself around the KEY's ranking. The key is
            // so important, it must be unique in the BST.
            KEY key;

            // The value will almost certainly not be a void*, but this allows
            // us to store ANYTHING, for we can cast a void* into something else
            // void* is an address to anything . . . not nothingness. Values
            // in a map may be repeated, and values don't appear in a Set.
            VALUE value;

            std::shared_ptr<BSTNode> childL;
            std::shared_ptr<BSTNode> childR;

            BSTNode() {};

            BSTNode(KEY item) : key(item), childL(nullptr), childR(nullptr) {};

            std::pair<std::shared_ptr<KEY *> &, int> keyset(std::shared_ptr<KEY *> &arr) {
                std::pair<std::shared_ptr<KEY *> &, int> toRet(arr);
            };

            // This is something like Java's toString method.
            // This is an IN-ORDER traversal.
            friend std::ostream &operator<<(std::ostream &os, BSTNode const &node) {
                if (node.childL != nullptr) os << *node.childL;
                os << std::setw(8) << node.key << " ";
                if (node.childR != nullptr) os << *node.childR;
                return os;
            }
        };

        std::shared_ptr<BSTNode> root;
        int nodes;

        // this function may help when you write other functions. Sometimes you
        // want to literally work with the node holding the key, and not
        // just the keys and values themselves. Your design will decide if you
        // need something like this or not.
        BSTNode &getNode(const KEY &key) {
            BSTNode *current = root.get();

            while (current != nullptr) {
                if (key == current->key) {
                    return *current;
                } else if (key < current->key) {
                    current = current->childL.get();
                } else {
                    current = current->childR.get();
                }
            }
            return NULL;
        }

        // This is a PRIVATE version of teh insert function that no one on
        // the outside can call. I find it useful for something like
        // the public version to kick things off, and this does the real
        // work.
        bool insert(const KEY &key, std::shared_ptr<BSTNode> &start) {

            if (this->isEmpty()) {
                root = std::make_shared<BSTNode>(key);
                nodes++;
                return true;
            }

            //if inserting key is less than its root.
            if (key < start->key) {

                //if there's already childL, start again from that child.
                if (start->childL) {
                    return insert(key, start->childL);
                } else {
                    start->childL = std::make_shared<BSTNode>(key);
                    nodes++;
                    return true;
                }

                //if inserting key is grater than its root
            } else if (key > start->key) {
                //if there's already childR, start again from that childR.
                if (start->childR) {
                    return insert(key, start->childR);
                } else {
                    start->childR = std::make_shared<BSTNode>(key);
                    nodes++;
                    return true;
                }
            }
            return false;
        }

        bool remove(const KEY &key, std::shared_ptr<BSTNode> parent, std::shared_ptr<BSTNode> current, bool wasLeft) {
            if (current == nullptr) {
                return false;
            }

            if (current->key == key) {

                //no child.leaf.
                if (current->childL == nullptr && current->childR == nullptr) {
                    if (wasLeft) {
                        parent->childL = nullptr;
                    } else {
                        parent->childR = nullptr;
                    }
                    // two children.
                } else if (current->childL && current->childR) {
                    //IOS
                    std::shared_ptr<BSTNode> temp = current->childR;
                    std::shared_ptr<BSTNode> tempParent = current;

                    while (temp->childL != nullptr) {
                        tempParent = temp;
                        temp = temp->childL;
                    }

                    if (tempParent == current) {
                        tempParent->childR = temp->childR;
                    } else {
                        tempParent->childL = temp->childR;
                    }

                    //tempParent->childL = temp->childR;
                    temp->childL = current->childL;
                    temp->childR = current->childR;

                    if (wasLeft) {
                        parent->childL = temp;
                    } else {
                        parent->childR = temp;
                    }

                    //one child
                } else {
                    std::shared_ptr<BSTNode> temp;

                    if (current->childL) {
                        temp = current->childL;
                    } else {
                        temp = current->childR;
                    }

                    if (wasLeft) {
                        parent->childL = temp;
                    } else {
                        parent->childR = temp;
                    }

                }
            } else if (key < current->key) {
                return remove(key, current, current->childL, true);
            } else {
                return remove(key, current, current->childR, false);
            }

            return true;
        }




        /*       KEY getInOrderPredecessor(std::shared_ptr<BSTNode> current) const {

                   std::shared_ptr<BSTNode> temp = current->childL;

                   while (temp->childR != nullptr) {
                       temp = temp->childR;
                   }

                   return temp->key;
               }

               KEY getInOrderSuccessor(std::shared_ptr<BSTNode> current) const {
                   std::shared_ptr<BSTNode> temp = current->childR;

                   while (temp->childL != nullptr) {
                       temp = temp->childL;
                   }

                   return temp->key;
               }*/

    public:
        BinarySearchTree() {
            nodes = 0;
        }

        BinarySearchTree(const BinarySearchTree<KEY, VALUE> &other) {

        }

        ~BinarySearchTree() override {

        }

        bool contains(const KEY &key) const override {

            std::shared_ptr<BSTNode> temp = root;

            while (temp != nullptr) {
                if (key == temp->key) {
                    return true;
                } else if (key < temp->key) {
                    temp = temp->childL;
                } else {
                    temp = temp->childR;
                }
            }
            return false;

        }

        void clear() override {
            root = nullptr;
            nodes = 0;
        }

        virtual VALUE &get(const KEY &key) override {

            std::shared_ptr<BSTNode> temp = root;
            while (temp->key != key) {
                if (key < temp->key) {
                    temp = temp->childL;
                } else {
                    temp = temp->childR;
                }
            }
            VALUE &val = (temp->value);
            return val;

        }

        bool insert(const KEY &key) override {
            return insert(key, root);
        }

        VALUE insert(const KEY &key, const VALUE &val) override {

            if (contains(key)) {
                //storing the value associated with the key to return later.
                VALUE t = get(key);
                //changing the value associated with the key.
                get(key) = val;

                //returning the old value.
                return t;
            } else {
                //adding a new key.
                insert(key);
                //setting the value associated with the key.
                get(key) = val;
                return NULL;
            }
        }

        // pre order: deal with self, deal with left children, deal with right children
        // post order: deal with left children, deal with right children, deal with self
        // level-order: deal with all nodes at the same depth before moving on

        void extraKeys(std::shared_ptr<BSTNode> node, KEY *arr, int &pos) {
        // in order: deal with left children, deal with self, deal with right children

            if (node != nullptr) {
                extraKeys(node->childL, arr, pos);
                arr[pos] = node->key;
                pos++;
                extraKeys(node->childR, arr, pos);
            }
            // return to parent node.(can get rid of it.)
            return;
        }

        void extraValues(std::shared_ptr<BSTNode> node, VALUE *arr, int &pos) {

            if (node != nullptr) {
                extraValues(node->childL, arr, pos);
                arr[pos] = node->value;
                pos++;
                extraValues(node->childR, arr, pos);
            }
            return;
        }

        std::pair<std::shared_ptr<KEY *>, int> keys() override {
            KEY *raw = new KEY[size()];
            std::shared_ptr<KEY *> arr = std::make_shared<KEY *>(raw);
            // Todo: Extra Credit Students fill up the arr[] with the keys in-order

            int posi = 0;
            extraKeys(root, raw, posi);

            std::pair<std::shared_ptr<KEY *>, int> toRet(arr, size());
            return toRet;
        };

        virtual std::pair<std::shared_ptr<VALUE *>, int> values() override {
            VALUE *raw = new VALUE[size()];
            std::shared_ptr<VALUE *> arr = std::make_shared<VALUE *>(raw);
            // Todo: Students fill up the arr[] with the values in-order
            // Todo: with respect to their keys (not each other). Extra Credit

            int posi = 0;
            extraValues(root, raw, posi);

            std::pair<std::shared_ptr<VALUE *>, int> vals(arr, size());
            return vals;
        };

        bool remove(const KEY &key) override {
            //empty
            if (this->isEmpty()) return false;

            //if key is in tree.
            if (!contains(key)) return false;

            if (key == root->key) {
                std::shared_ptr<BSTNode> temp = std::make_shared<BSTNode>(key);
                temp->childL = root;
                bool rmv = remove(key, temp, root, true);
                root = temp->childL;
                nodes--;
                return rmv;
            } else {
                std::shared_ptr<BSTNode> temp = std::make_shared<BSTNode>(key);
                bool rmv = remove(key, temp, root, true);
                nodes--;
                return rmv;
            }
        }

        int size() const override {
            return nodes;
        }

        VALUE &operator[](std::size_t idx) {
            return get(idx);
        }

        friend std::ostream &operator<<(std::ostream &os, sdsu::BinarySearchTree<KEY, VALUE> const &bst) {
            if (bst.root != nullptr) return os << "[" << (*bst.root) << "]";
            return os;
        }
    };
}
#endif //PROG2_BINARYSEARCHTREE_H
