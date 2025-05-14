#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define LEVEL 5//��������֧����
#define KEYS_MIN ((LEVEL+1)/2-1)//���ٹؼ�������

//�ڵ㣨�ؼ���Ϊ����
typedef struct BNode
{
	int keyNum;//ӵ�йؼ�������
	int childNum;//ӵ�к�������
	int* keys;//�ؼ�������(����0),������Ϊ1�ĵط���ʼ��
	struct BNode** children;//�ڵ�ָ�������
	struct BNode* parent;//���ڵ�
	//int parentKeyPos;//���ؼ���λ��(�����������йؼ���)
}BNode;

//B��
typedef struct BTree
{
	BNode* root;//���ڵ�
	int size;//���ݸ������ؼ���������
}BTree;

//��ʼ����
void InitBTree(BTree* tree);
//�����ڵ�
BNode* CreatNode();
//�����ڵ�,�����һ���ؼ���
BNode* CreatNode(int key);
//Ѱ����������,�����±�
int FindIndex(BNode* node, int key);
//B�����ң����ؽڵ�,keyֵ�ڷ��ؽڵ�������У����ܲ��������keyֵ
BNode* Search_BT(BTree* tree, int key);
//�ؼ��ֲ��룬˳����ɷ���,����ʧ�ܷ���0���ɹ�����key
int Insert_Key(BTree* tree, BNode* node, int key);
//B���ڵ���룬����ʧ�ܷ���0���ɹ�����key
int Insert_BT(BTree* tree,int key);
//B���������
void InOrder_Traversal(BNode* root);

//B��ɾ��,�ɹ�����1��ʧ�ܷ���0
int Delete_BT(BTree* tree, int key);
//ɾ��key(����Ҳ��ɾ��������ǰ�ϲ��������ҵ�key���ұ�)
void Erase_Key(BNode* curr, int index);
//�ж��Ƿ���Ҷ�ڵ�
bool IsLeaf(BNode* curr);
//�ڵ�ϲ�
void CombineNode(BTree* tree,BNode* LeftNode, BNode* RightNode);
//Ѱ�Ҹ�keyλ��(����������key),��������
int FindIndex_Parent(BNode* lchild);

