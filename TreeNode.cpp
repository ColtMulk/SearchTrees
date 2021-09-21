//
// Created by ColtM on 10/30/2019.
//
#include <utility>
template <class key, class value>
struct TreeNode {
    std::pair<key,value>* keyValue;
    TreeNode<key,value>* left;
    TreeNode<key,value>* right;
    TreeNode<key,value>* parent;

    TreeNode(): left(nullptr), right(nullptr),  parent(nullptr){}
    TreeNode(key a, value b): left(nullptr), right(nullptr),  parent(nullptr){
        keyValue = new std::pair<key,value>(a,b);
    }
    void setKeyValue(std::pair<key,value>* a){
        delete keyValue;
        keyValue = a;
    }


};