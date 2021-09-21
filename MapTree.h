/*Name: Colton Mulkey
  Class: CSCE 121-506
  date: 01/25/2019
  project: quadratic
*/
//
// Created by ColtM on 10/26/2019.
//

#ifndef HASHTABLES_MAPTREE_H
#define HASHTABLES_MAPTREE_H


#include <cstdlib>
#include "TreeNode.cpp"
#include <iostream>
#include <utility>

using namespace std;

template<class key, class value, class compare = std::less<key>>
class MapTree {
    TreeNode<key, value> *root;
    unsigned int length;
    compare lessThan;
    int del(TreeNode<key,value>* T);

    TreeNode<key,value> *find(key k);


public:
    MapTree();

    ~MapTree();

    unsigned int size();

    bool empty();



    pair<key,value> *search(key k);

    pair<key,value> *insert(key k, value v);

    void erase(key k);
};

template<class key, class value, class compare>
MapTree<key, value, compare>::MapTree(): length(0), root(nullptr) {
}

template<class key, class value, class compare>
MapTree<key, value, compare>::~MapTree() {
    del(root);
}

template<class key, class value, class compare>
int MapTree<key, value, compare>::del(TreeNode<key, value>* T) {
    if(T == nullptr){
        return 0;
    }
    if(T->left != nullptr){
        del(T->left);
    }
    if(T->right!= nullptr){
        del(T->right);
    }
    delete T;
    return 0;
}

template<class key, class value, class compare>
unsigned int MapTree<key, value, compare>::size() {
    return length;
}

template<class key, class value, class compare>
bool MapTree<key, value, compare>::empty() {
    return length == 0;
}

template<class key, class value, class compare>
TreeNode<key, value> *MapTree<key, value, compare>::find(key k) {
    TreeNode<key, value> *curr = root;
    //empty case
    if (root == nullptr) {
        return nullptr;
    } else {
        //loop through tree
        while (curr != nullptr) {
            if (curr->keyValue->first == k)
                return curr;
            if (lessThan(k, curr->keyValue->first))
                curr = curr->left;
            else
                curr = curr->right;
        }
    }
    return nullptr;
}

template<class key, class value, class compare>
pair<key, value> *MapTree<key, value, compare>::insert(key k, value v) {
    TreeNode<key, value> *curr = root;
    TreeNode<key, value> *element = new TreeNode<key, value>(k, v);

    length++;
    while (curr != nullptr) {
        //iterate through tree
            if (curr->keyValue->first == k){
                cout << "Error duplicate key" << endl;
                length--;
                exit(1);
            }
            if (lessThan(k, curr->keyValue->first)) {
                //less than case
                if(curr->left == nullptr){
                    curr->left = element;
                    element->parent = curr;
                    return element->keyValue;
                }
                curr = curr->left;
            }
            else {
                //greater than case
                if(curr->right == nullptr){
                    curr->right = element;
                    element->parent = curr;
                    return element->keyValue;
                }
                curr = curr->right;
            }
    }
    //empty tree case
    root = element;
    return root->keyValue;
}

template<class key, class value, class compare>
void MapTree<key, value, compare>::erase(key k) {
    TreeNode<key, value> *w = find(k);
    if (w == nullptr) {
        std::cout << "Invalid erase key";
        exit(1);
    }
    //checks for root case
    if(length == 1) {
        delete root;
        root = nullptr;
        length = 0;
    }
    else{
        length--;
        TreeNode<key, value> *parent = w->parent;
        if (w->left == nullptr && w->right == nullptr) {
            //leaf case
            if (parent == nullptr) {}
            else if ((parent->left == w)) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            delete w;
        } else if (w->left != nullptr) {
            //left node case
            TreeNode<key, value> *curr = w->left;
            //getting replacement
            while (curr->right != nullptr) {
                curr = curr->right;
            }
            parent = curr->parent;
            w->keyValue->first = curr->keyValue->first;
            w->keyValue->second = curr->keyValue->second;
            TreeNode<key, value> *T1 = curr->left;
            //fixing pointers
            if (parent->left == curr) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            //assigning pointers left on hanging on replacement node curr
            if (T1 != nullptr) {
                if (lessThan(T1->keyValue->first, parent->keyValue->first)) {
                    parent->left = T1;
                    T1->parent = parent;
                } else {
                    parent->right = T1;
                    T1->parent = parent;
                }
            }
            delete curr;
        } else {
            //right node case
            TreeNode<key, value> *curr = w->right;
            //finding replacement
            while (curr->left != nullptr) {
                curr = curr->left;
            }
            parent = curr->parent;
            //swap
            w->setKeyValue(curr->keyValue);

            if (parent->left == curr) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            //reassiging hanging Tree T1 that was on curr
            TreeNode<key, value> *T1 = curr->right;
            if (T1 != nullptr) {
                if (lessThan(T1->keyValue->first, parent->keyValue->first)) {
                    parent->left = T1;
                    T1->parent = parent;
                } else {
                    parent->right = T1;
                    T1->parent = parent;
                }
            }
            delete curr;
        }
    }
}

template<class key, class value, class compare>
pair<key, value>* MapTree<key, value, compare>::search(key k) {
    TreeNode<key,value>* temp = find(k);
    if(temp != nullptr){
        return temp->keyValue;
    }
    return nullptr;
}


#endif //HASHTABLES_MAPTREE_H
