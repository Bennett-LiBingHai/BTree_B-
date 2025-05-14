#define _CRT_SECURE_NO_WARNINGS
#include"BTree.h"

void test01()
{
	BTree t;
	InitBTree(&t);
	printf("%d", Insert_BT(&t, 10));
	printf("%d", Insert_BT(&t, 15));
	printf("%d", Insert_BT(&t, 5));
	printf("%d", Insert_BT(&t, 1));
	printf("%d", Insert_BT(&t, 4));
	printf("%d", Insert_BT(&t, 11));
	printf("%d", Insert_BT(&t, 7));
	printf("%d", Insert_BT(&t, 8));
	InOrder_Traversal(t.root);
}

void test02()
{
	BTree t;
	InitBTree(&t);
	for (int i = 100; i > 0; --i)
	{
		printf("%d ", Insert_BT(&t, i));
	}
	printf("\n");
	InOrder_Traversal(t.root);
	//printf("%d\n", Delete_BT(&t, 11));
	//printf("%d\n", Delete_BT(&t, 8));
	//printf("%d\n", Delete_BT(&t, 5));
	//InOrder_Traversal(t.root);
	printf("\n");
	for (int i = 2; i <= 100; ++i)
	{
		printf("%d ", Delete_BT(&t, i));
	}
	printf("\n");
	InOrder_Traversal(t.root);

}

void test03()
{
	BNode* parent = CreatNode(8);
	BNode* left = CreatNode();
	BNode* right = CreatNode();
	parent->children[0] = left;
	parent->children[1] = right;
	parent->childNum = 2;
	left->keys[1] = 1;
	//left->keys[2] = 2;
	left->keyNum = 1;
	left->parent = parent;
	right->keys[1] = 9;
	right->keys[2] = 10;
	right->keyNum = 2;
	right->parent = parent;
	/*CombineNode(left, right);*/
}

int main()
{
	//test01();
	test02();
	//test03();
	return 0;
}