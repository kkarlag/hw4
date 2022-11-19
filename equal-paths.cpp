#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
//helper function:
//counts the largest num of nodes 
/*int countNode(Node* node)
{
    
    int height(Node* root) {
	if (root == nullptr) {
		return 1;
	}
	if (root->left != nullptr) {
		return 1 + height(root->left);
	}
	else {
		return 1 + height(root->right);
	}	
*/
int countNode(Node* node)
{

int largest=0; 
    int leftNode, rightNode;
    //Base Case: no nodes
    if(node== nullptr)
    {
        return 0; 
    }
    leftNode=countNode(node->left); 
    rightNode=countNode(node->right);
    if(leftNode<= rightNode) largest= rightNode; 
    else if(rightNode <= leftNode) largest =leftNode; 

    largest++; //accounting for nodes its on
    return largest; 
}



bool equalPaths(Node * root)
{
    // Add your code below
    //base Case
    if(root == nullptr)
    {
        return true; 
    }
    //have to take into account a tree thats one sided (all left or all right)
    else if ((root->right != nullptr)&& (root-> left ==nullptr))
    {
        return equalPaths(root->right);
    }
    else if((root->left != nullptr)&& (root->right == nullptr))
    {
        return equalPaths(root->left);
    }
    else //a normal tree
    {
      int rightNodes =countNode(root->right);
      int leftNodes= countNode(root->left);
      if(rightNodes != leftNodes) return false; 
    }
    return true;
}

