#include <stdio.h>
#include<iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace ::std;

class TreeNode {
public:
    string val;
    TreeNode *parent;
    TreeNode *left;
    TreeNode *right;
    bool colorIsBlack;
public: TreeNode(){

    }
public:
    TreeNode(TreeNode *parent, string &val) {
        this->parent = parent;
        this->val = val.c_str();
        right = NULL;
        left = NULL;
        colorIsBlack = false;
    };
public: bool hasUncle(){
        if(this->parent == NULL)
            return false;
        else if(this->parent->parent == NULL)
            return false;
        else return true;
    }
public: TreeNode* getUncle(){
        if(this->parent == parent->parent->left)
            return parent->parent->right;
        else if(this->parent == parent->parent->right)
            return parent->parent->left;
    }
public: bool isBlack(){
        if(this==NULL)
            return true;
        else
            return colorIsBlack;
    }
public: bool isLeftChild(){
        //assuming that node->parent != NULL
        TreeNode* p = this->parent;
       if(p->left == this)
           return true;
       else return false;

    }
};

class RBTree {
public: TreeNode* root;
public:
    RBTree() {
     root=NULL;
    }

public:
    int getHeight() {
        return getHeight(root);
    }

private:
    int getHeight(TreeNode *node) {
        if (node == NULL)
            return -1;
        else
            return 1 + max(getHeight(node->right), getHeight(node->left));
    }

public:
    void insert(string word) {
        TreeNode *temp = this->root;
        TreeNode *parent = NULL;
        //TreeNode* newNode=
        if(this->root==NULL){
            this->root = new TreeNode(parent,word);
            this->root->colorIsBlack=true;
        }
        else{
            while (temp != NULL) {
                parent = temp;
                int comp = temp->val.compare(word);
                if (comp == 0) {
                    cout << "Error: Word already in the dictionary!!" << endl;
                    return;
                } else if (comp < 0)
                    temp = temp->right;
                else
                    temp = temp->left;
            }
            TreeNode* newNode=new TreeNode(parent,word);
            if (parent->val.compare(word) < 0)
                parent->right = newNode;
            else
                parent->left = newNode;
           // newNode->parent = parent;
            validate(newNode);
        }
    }
private: void validate(TreeNode* node) {
        if (this->root == node)
            node->colorIsBlack = true;
        else if(this->root == node->parent)
            node->parent->colorIsBlack=true;
        else if(node->parent!=NULL){
            TreeNode *p = node->parent;
            TreeNode *g = p->parent;
            if (!p->isBlack()) {
                // Case 1: uncle is red, change colors.
                if (!node->getUncle()->isBlack()) {
                    p->colorIsBlack = true;
                    node->getUncle()->colorIsBlack = true;
                    g->colorIsBlack = false;
                    validate(g);
                }
                //Case 2: Uncle is black.
                else if (node->getUncle()->isBlack()) {
                    if(node->isLeftChild()){
                        //left left
                        if(node->parent->isLeftChild())
                            handleLeftLeft(node);
                        //right left
                        else{
                            rotateRight(node);
                            handleRightRight(node->right);
                        }
                    }else if(!node->isLeftChild()){
                        //right right
                        if(!node->parent->isLeftChild())
                            handleRightRight(node);
                            else {
                                // left right
                                rotateLeft(node);
                                handleLeftLeft(node->left);
                        }
                    }
                }
            }
        }
    }
private: void rotateRight(TreeNode* node){
        TreeNode* p = node->parent;
        TreeNode* g = p->parent;
        if(g!=NULL){
            if(p->isLeftChild())
                g->left = node;
            else
                g->right = node;
        } else
            this->root=node;
        TreeNode* temp = node->right;
        node->parent = g;
        p->parent = node;
        node->right = p;
        if(temp!=NULL)
        temp->parent = p;
        p->left = temp;
    }

private: void handleLeftLeft(TreeNode* node){
        TreeNode* p = node->parent;
        TreeNode* g = p->parent;
        bool c = g->colorIsBlack;
        g->colorIsBlack=p->colorIsBlack;
        p->colorIsBlack=c;
        rotateRight(p);
    }
private: void handleRightRight(TreeNode* node){
        TreeNode* p = node->parent;
        TreeNode* g = p->parent;
        bool c = g->colorIsBlack;
        g->colorIsBlack=p->colorIsBlack;
        p->colorIsBlack=c;
        rotateLeft(p);
    }
private: void rotateLeft(TreeNode* node){
        TreeNode* p = node->parent;
        TreeNode* temp = node->left;
        TreeNode* g = p->parent;
        if(g!=NULL){
            if(p->isLeftChild())
                g->left = node;
            else
                g->right = node;
        } else
            this->root=node;
        node->parent = g;
        node->left = p;
        p->parent = node;
        p->right = temp;
        if(temp!=NULL)
            temp->parent = p;
        }
public: int size(){
        if(root==NULL)
            return 0;
        else
            return 1+ size(root->left)+size(root->right);
    }
private: int size(TreeNode* node){
        if(node==NULL)
            return 0;
        else
            return 1+size(node->left)+size(node->right);
    }
public: bool contains(string word){
        TreeNode* temp = this->root;
        while(temp!=NULL){
            int comp = temp->val.compare(word);
            if(comp==0)
                return true;
            else if (comp<0)
                temp = temp->right;
            else
                temp = temp->left;
        }
        return false;
    }
public: void print(){
        print(root);
    }
private: void print(TreeNode* node){
        if(node!=NULL){
            print(node->left);
            cout<<node->val<<endl;
            print(node->right);
        }
    }
};

vector<string> readInput(char *filename) {
    ifstream inputFile;
    vector<string> input;
    inputFile.open(filename);
    string x;
    while (inputFile >> x)
        input.push_back(x);
    inputFile.close();
    return input;
}

int main(int args, char *argv[]) {
    if (args != 2) {
        cout << "Please enter the input file!!" << endl;
        return 0;
    }
    vector<string> arr = readInput(argv[1]);
   // sort(arr.begin(),arr.end());
    RBTree myTree = RBTree();
    for (int i = 0; i < arr.size(); i++){
        myTree.insert(arr[i]);
       // cout<<"After inserting: "<<arr[i]<< " , Height = " << myTree.getHeight() << endl;
    }
  //  myTree.print();
    cout<< "height: " << myTree.getHeight() << endl;
    cout<< "size: "<<myTree.size() <<endl;
    cout<< "root: " <<myTree.root->val<<endl;
    return 0;
}