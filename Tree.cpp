//
// Created by yunsu Jung on 2018. 8. 29..
//
#include <iostream>
#include <string>

using namespace std;
//char notUsed = (char)255;
class Node
{
public:
    Node()
    {

    }
    Node(int key, char data){
        this -> key = key;
        this -> data = data;
        left = NULL;
        right = NULL;
    }

    int getItem() const
    {
        return key;
    }
    char getData()
    {
        return data;
    }
    Node* getLeft()
    {
        return left;
    }
    Node* getRight()
    {
        return right;
    }
    void setItem(int x)
    {
        key = x;
    }
    void setData(char x){
        data = x;
    }
    void setLeft(Node* nd)
    {
        left = nd;
    }
    void setRight(Node* nd)
    {
        right = nd;
    }

    bool operator <(const Node &b) {
        return this->getItem() < b.getItem();
    }

private:
    int key;
    char data;
    Node* left;
    Node* right;
};



class Tree
{
public:
    Tree()
    {

    }
    Tree(Node* rootNode){
        this->root = rootNode;
    }
    Node* getRoot() const
    {
        return root;
    }
    void setRoot(Node* A)
    {
        root = A;
    }
    void setNum(int x){
        numItem = x;
    }
    int size()
    {
        return numItem;
    }

    Tree* merge(Tree* otherTree)
    {
        Node* A = this->getRoot();
        Node* B = otherTree->getRoot();

        int keyA = A->getItem();
        int keyB = B->getItem();
        Node *newNode = new Node(keyA + keyB, (char)255);

        if(keyA >= keyB){
            newNode->setLeft(A);
            newNode->setRight(B);
        }
        else{
            newNode->setLeft(B);
            newNode->setRight(A);
        }

        Tree* merged = new Tree();
        merged->setRoot(newNode);
        merged->setNum(this->size() + otherTree->size());
        return merged;

    }
private:
    Node *root = NULL;
    int numItem = 0;
};

