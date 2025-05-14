#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define LEVEL 5//阶数（分支数）
#define KEYS_MIN ((LEVEL+1)/2-1)//最少关键字数量

//节点（关键字为正）
typedef struct BNode
{
	int keyNum;//拥有关键字数量
	int childNum;//拥有孩子数量
	int* keys;//关键字数组(大于0),从索引为1的地方开始存
	struct BNode** children;//节点指针的数组
	struct BNode* parent;//父节点
	//int parentKeyPos;//父关键字位置(大于自身所有关键字)
}BNode;

//B树
typedef struct BTree
{
	BNode* root;//根节点
	int size;//数据个数（关键字数量）
}BTree;

//初始化树
void InitBTree(BTree* tree);
//创建节点
BNode* CreatNode();
//创建节点,插入第一个关键字
BNode* CreatNode(int key);
//寻找子树索引,返回下标
int FindIndex(BNode* node, int key);
//B树查找，返回节点,key值在返回节点的区间中，可能不存在这个key值
BNode* Search_BT(BTree* tree, int key);
//关键字插入，顺便完成分裂,插入失败返回0，成功返回key
int Insert_Key(BTree* tree, BNode* node, int key);
//B树节点插入，插入失败返回0，成功返回key
int Insert_BT(BTree* tree,int key);
//B树中序遍历
void InOrder_Traversal(BNode* root);

//B树删除,成功返回1，失败返回0
int Delete_BT(BTree* tree, int key);
//删除key(子树也会删除，请提前合并子树并挂到key的右边)
void Erase_Key(BNode* curr, int index);
//判断是否是叶节点
bool IsLeaf(BNode* curr);
//节点合并
void CombineNode(BTree* tree,BNode* LeftNode, BNode* RightNode);
//寻找父key位置(大于其所有key),返回索引
int FindIndex_Parent(BNode* lchild);

