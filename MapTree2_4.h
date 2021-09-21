/*Name: Colton Mulkey
  Class: CSCE 121-506
  date: 01/25/2019
  project: quadratic
*/
//
// Created by ColtM on 11/11/2019.
//

#ifndef SEARCHTREES_MAPTREE2_4_H
#define SEARCHTREES_MAPTREE2_4_H

#include "TwoFourNode.h"

using namespace std;

template<class key, class value, class compare = std::less<key>>
class MapTree2_4 {
    compare lessThan;
    unsigned int length;
    TwoFourNode<key, value, compare> *root;
    void recursivePrint(TwoFourNode<key,value,compare>* a, int level = 0);
public:
    MapTree2_4();

    void print();

    std::pair<key, value> *search(key k);

    void insert(key k,
                value v); //- insert the key-value pair (k,x); if there is already a pair in the tree with key k then return an error message
    int
    erase(key k); //- delete the key-value pair (k,x); if there is no pair in the tree with key k then return an error message
    unsigned int size(); //- return the number of key-value pairs in the tree
    void removeNode(node<key, value, compare> *leaf, TwoFourNode<key, value, compare> *whole,
                TwoFourNode<key, value, compare> *childReplacement = nullptr);

    void underflow(TwoFourNode<key, value, compare> *remove); //remove is length of 1, fixes underflow
    void merge(TwoFourNode<key, value, compare> *big,
               node<key, value, compare> *add, TwoFourNode<key,value,compare>* child = nullptr); //merges add and big into add, assumes add(left and right are the same)
    void
    rotate(TwoFourNode<key, value, compare> *borrow, TwoFourNode<key, value, compare> *borrower); //rotates to borrower
};

template<class key, class value, class compare>
MapTree2_4<key, value, compare>::MapTree2_4() {
    length = 0;
    root = nullptr;
}

template<class key, class value, class compare>
pair<key, value> *MapTree2_4<key, value, compare>::search(key k) {
    //returns pointer to key value pair with key k(nullptr if not found)
    if(root == nullptr){
        return nullptr;
    }
    node<key, value, compare> *point = root->search(k);
    if(point == nullptr){
        return nullptr;
    }
    return point->keyValue;
}

template<class key, class value, class compare>
void MapTree2_4<key, value, compare>::insert(key k, value v) {
    if (root == nullptr) {
        //empty case
        root = new TwoFourNode<key, value, compare>(k, v);
        length++;
    } else {
        //gets insert TwoFourNode
        TwoFourNode<key, value, compare> *insertNode = root->findInsert(k);
        if (insertNode == nullptr) {
            cout << "Error: Duplicate key" << endl;
            exit(1);
        }
        //inserts key into insertNode
        insertNode->insert(k, v);
        if (insertNode->length == 4) {
            //overflow case
            TwoFourNode<key, value, compare> *split = insertNode->split();
            if (split != nullptr) {
                root = split;
            }
        }
        length++;
    }
}

template<class key, class value, class compare>
unsigned int MapTree2_4<key, value, compare>::size() {
    return length;
}

template<class key, class value, class compare>
int MapTree2_4<key, value, compare>::erase(key k) {
    if (length == 1) {
        //root case
        delete root;
        root = nullptr;
        length = 0;
        return 0;
    }
    TwoFourNode<key, value, compare> *removeNode24 = root->findRemove(k);
    node<key, value, compare> *era = removeNode24->search(k);

    //finds replacement node and swaps (if swap era and removeNode are updated)
    if (era->left != nullptr) {
        node<key, value, compare> *curr = era->left->root;
        removeNode24 = era->left;
        while (curr != nullptr) {
            if (curr->rightSibling == nullptr) {
                if (curr->right == nullptr) {
                    break;
                }
                removeNode24 = curr->right;
                curr = curr->right->root;
            } else {
                curr = curr->rightSibling;
            }
        }
        //swap curr and era
        era->keyValue->first = curr->keyValue->first;
        era->keyValue->second = curr->keyValue->second;
        era = curr;
    }

    length--;
    //checks for underflow
    if (removeNode24->length >= 2) {
        removeNode(era, removeNode24);
    } else {
        underflow(removeNode24);
    }

    return 0;
}

template<class key, class value, class compare>
void MapTree2_4<key, value, compare>::underflow(TwoFourNode<key, value, compare> *remove) {
    //remove is length 1 of node that is supposed to be removed
    if (remove->length > 1) {
        cout << "Error remove is greater than length 1" << endl;
        exit(2);
    }


    if (remove->leftSib && remove->leftSib->length >= 2) {
        //rotate with leftSib
        rotate(remove->leftSib, remove);
    } else if (remove->rightSib && remove->rightSib->length >= 2) {
        //rotate with rightSib
        rotate(remove->rightSib, remove);
    } else {
        //merge case
        node<key, value, compare> *curr = remove->parent->root;
        bool isRight = true;
        //find replacement node in parent
        if(remove->leftSib != nullptr){
            while(curr->right != remove){
                curr = curr->rightSibling;
            }
            isRight = false;
        }
        else{
            while(curr->left != remove){
                curr = curr->rightSibling;
            }
        }

        node<key, value, compare> *add = new node<key, value, compare>(curr->keyValue->first, curr->keyValue->second);

        //merges with either right or left sibling
        if (isRight) {
            merge(remove->rightSib, add, remove->root->left);
            remove->rightSib->leftSib = remove->leftSib;
            //pointers
            removeNode(curr, remove->parent, remove->rightSib);
            if(remove->leftSib){
                remove->leftSib->rightSib = remove->rightSib;
            }

        } else {
            merge(remove->leftSib, add, remove->root->left);
            removeNode(curr, remove->parent, remove->leftSib);
            //pointers
            remove->leftSib->rightSib = remove->rightSib;
            if(remove->rightSib){
                remove->rightSib->leftSib = remove->leftSib;
            }
        }

        if (remove->parent->length == 0) {
            //case where parent's last node was removed
            remove->parent->root = new node<key, value, compare>(remove->root->keyValue->first,remove->root->keyValue->second); //key and value doesn't matter
            remove->parent->length = 1;

            auto parentRoot = remove->parent->root;
            //both children point to merged node.
            if (isRight) {
                parentRoot->right = remove->rightSib;
                parentRoot->left = remove->rightSib;
            } else {
                parentRoot->right = remove->leftSib;
                parentRoot->left = remove->leftSib;
            }

            if (remove->parent->parent != nullptr) {
                //fix underflow on parent
                underflow(remove->parent);
            } else {
                root = root->root->right;
                delete remove->parent;
                root->parent = nullptr;

            }
        }

        delete remove;

    }


}

template<class key, class value, class compare>
void MapTree2_4<key, value, compare>::merge(TwoFourNode<key, value, compare> *big, node<key, value, compare> *add, TwoFourNode<key,value,compare>* child) {
    //inserts add node into big in merge operation
    auto curr = big->root;
    if (lessThan(add->keyValue->first, curr->keyValue->first)) {
        add->rightSibling = curr;
        curr->leftSibling = add;
        add->right = curr->left;
        add->left = child;
        big->root = add;
        if(add->left != nullptr){
            child->parent = big;
            add->left->rightSib = add->right;
            add->right->leftSib = add->left;
        }
    } else {
        //gets end node of big
        while (curr->rightSibling != nullptr) {
            curr = curr->rightSibling;
        }
        add->leftSibling = curr;
        curr->rightSibling = add;
        add->left = curr->right;
        add->right = child;
        if(add->right != nullptr){
            child->parent = big;
            add->right->leftSib = add->left;
            add->left->rightSib = add->right;
        }
    }
    big->length++;
}

template<class key, class value, class compare>
void MapTree2_4<key, value, compare>::rotate(TwoFourNode<key, value, compare> *borrow,
                                             TwoFourNode<key, value, compare> *borrower) {
    //borrow is left or right silbing of borrower
    auto curr = borrow->root;
    auto replace = borrower->root;
    if (borrower->leftSib == borrow) {
        //borrow is left sib
        while (curr->rightSibling != nullptr) {
            curr = curr->rightSibling;
        }

        auto T1 = curr->right;

        //find replacement node in parent
        auto replacement = borrow->parent->root;
        while (replacement->left != borrow) {
            replacement = replacement->rightSibling;
        }
        //rotating values
        replace->keyValue->first = replacement->keyValue->first;
        replace->keyValue->second = replacement->keyValue->second;
        replacement->keyValue->first = curr->keyValue->first;
        replacement->keyValue->second = curr->keyValue->second;
        replace->left = T1;
        //fixing pointers on T1
        if(T1 != nullptr) {
            T1->parent = borrower;
            if (T1->leftSib != nullptr) {
                T1->leftSib->rightSib = nullptr;
                T1->leftSib = nullptr;
            }
            T1->rightSib = replace->right;
            replace->right->leftSib = T1;
        }

        removeNode(curr, borrow, curr->left);

    } else {
        auto T1 = curr->left;

        //find replacement node in parent
        auto replacement = borrow->parent->root;
        while (replacement->right != borrow) {
            replacement = replacement->rightSibling;
        }
        //rotate values
        replace->keyValue->first = replacement->keyValue->first;
        replace->keyValue->second = replacement->keyValue->second;
        replacement->keyValue->first = curr->keyValue->first;
        replacement->keyValue->second = curr->keyValue->second;
        replace->right = T1;

        //fixing pointers on T1
        if(T1 != nullptr) {
            T1->parent = borrower;
            if (T1->rightSib != nullptr) {
                T1->rightSib->leftSib = nullptr;
                T1->rightSib = nullptr;
            }
            T1->leftSib = replace->left;
            replace->left->rightSib = T1;
        }


        removeNode(curr, borrow, curr->right);
    }


}


template<class key, class value, class compare>
void MapTree2_4<key, value, compare>::removeNode(node<key, value, compare> *leaf, TwoFourNode<key, value, compare> *whole,
                                             TwoFourNode<key, value, compare> *childReplacement) {
    //removes leaf node rem, and sets child pointers of left and right siblings too childReplacement(nullptr default)
    if (leaf->leftSibling != nullptr) {
        leaf->leftSibling->rightSibling = leaf->rightSibling;
        leaf->leftSibling->right = childReplacement;
    }
    if (leaf->rightSibling != nullptr) {
        leaf->rightSibling->leftSibling = leaf->leftSibling;
        leaf->rightSibling->left = childReplacement;
    }
    //checks if root needs to be changed
    if (leaf->leftSibling == nullptr) {
        whole->root = leaf->rightSibling;
    }

    whole->length--;
    delete leaf;
}


template<class key, class value, class compare>
void MapTree2_4<key, value, compare>::print() {
    recursivePrint(root);
}

template<class key, class value, class compare>
void MapTree2_4<key, value, compare>::recursivePrint(TwoFourNode<key, value, compare> *a, int level) {
    //prints Tree out a then a's children then prints a's right sibling
    if(a == nullptr){}
    else{
        cout << "LEVEL: " << level << " NODE: ";
        auto curr = a->root;
        while(curr != nullptr){
            cout << "Key: " << curr->keyValue->first << " ";
            curr = curr->rightSibling;
        }
        if(a->root != nullptr) {
            cout << endl;
            recursivePrint(a->root->left, level+1);
        }
        if(a->rightSib != nullptr){
            recursivePrint(a->rightSib, level);
        }

    }
}

#endif //SEARCHTREES_MAPTREE2_4_H
