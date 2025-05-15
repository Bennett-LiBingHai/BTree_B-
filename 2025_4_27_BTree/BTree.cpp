#define _CRT_SECURE_NO_WARNINGS
#include"BTree.h"

//��ʼ����
void InitBTree(BTree* tree)
{
	tree->root = CreatNode();
	tree->size = 0;
}

//�����ڵ�,�����һ���ؼ���
BNode* CreatNode(int key)
{
	//�ڵ�
	BNode* newnode = (BNode*)malloc(sizeof(BNode));
	assert(newnode);
	//��������
	//newnode->children = (BNode**)malloc(sizeof(BNode*) * (LEVEL + 1));
	newnode->children = (BNode**)malloc(sizeof(BNode*) * (LEVEL+1));
	assert(newnode->children);
	for (int i = 0; i < LEVEL + 1; ++i)
	{
		newnode->children[i] = NULL;
	}
	newnode->childNum = 0;
	//�ؼ�������
	//newnode->keys = (int*)malloc(sizeof(int) * LEVEL);
	newnode->keys = (int*)malloc(sizeof(int) * (LEVEL + 1));
	assert(newnode->keys);
	for (int i = 0; i < LEVEL+1; ++i)
	{
		newnode->keys[i] = -1;
	}
	newnode->keys[1] = key;
	newnode->keyNum = 1;
	//���ڵ�
	newnode->parent = NULL;
	//����
	return newnode;
}

//�����ڵ�
BNode* CreatNode()
{
	//�ڵ�
	BNode* newnode = (BNode*)malloc(sizeof(BNode));
	assert(newnode);
	//��������
	//newnode->children = (BNode**)malloc(sizeof(BNode*) * (LEVEL + 1));
	newnode->children = (BNode**)malloc(sizeof(BNode*) * (LEVEL + 1));
	assert(newnode->children);
	for (int i = 0; i < LEVEL + 1; ++i)
	{
		newnode->children[i] = NULL;
	}
	newnode->childNum = 0;
	//�ؼ�������
	//newnode->keys = (int*)malloc(sizeof(int) * LEVEL);
	newnode->keys = (int*)malloc(sizeof(int) * (LEVEL+1));
	assert(newnode->keys);
	for (int i = 0; i < LEVEL+1; ++i)
	{
		newnode->keys[i] = -1;
	}
	newnode->keyNum = 0;
	//���ڵ�
	newnode->parent = NULL;
	//����
	return newnode;
}

//Ѱ����������,�����±�
int FindIndex(BNode* node, int key)
{
	int i = 1;
	for (; i <= node->keyNum; ++i)
	{
		if (key <= node->keys[i])
		{
			break;
		}
	}
	return i;
}

//B�����ң����ؽڵ�,keyֵ�ڷ��ؽڵ�������У����ܲ��������keyֵ
BNode* Search_BT(BTree* tree, int key)
{
	assert(tree->root&&key>0);
	BNode* curr = tree->root;
	//BNode* parent = tree->root;
	int index = 0;
	while (curr->childNum)
	{
		////��û����������Ϳ�
		//for (int i = curr->childNum; i < LEVEL; ++i)
		//{
		//	curr->children[i] = NULL;
		//}
		index = FindIndex(curr, key);
		if ((index<=curr->keyNum&&key == curr->keys[index])|| !curr->children[index - 1])//Ҫ��֤���û�������key���ܺ���Ҫ���ҵ�key��ͬ
		{
			return curr;
		}
		else
		{
			/*parent = curr;*/
			curr = curr->children[index-1];
		}
	}
	return curr;
}

//�ؼ��ֲ��룬˳����ɷ���,����ʧ�ܷ���0���ɹ�����key
int Insert_Key(BTree* tree, BNode* node, int key)
{
	assert(key > 0&&node);
	//����key
	int index = FindIndex(node, key);//Ҫȷ��key��������ͬ
	if (node->keys[index] == key)//����ͬ��key��
	{
		return 0;
	}
	for (int i = node->keyNum; i >= index; --i)
	{
		node->keys[i + 1] = node->keys[i];
	}
	node->keys[index] = key;
	node->keyNum++;
	//����
	if (node->keyNum == LEVEL)
	{
		int mid = LEVEL / 2 + LEVEL % 2;
		BNode* rchild = CreatNode();
		int j = 1;
		int i = mid + 1;
		int keyNum = node->keyNum;
		for (; i <= keyNum; ++i,++j)
		{
			//��key
			rchild->keys[j] = node->keys[i];
			rchild->keyNum++;
			node->keyNum--;
			node->keys[i] = -1;
			//��child
			rchild->children[j - 1] = node->children[i - 1];
			if (node->children[i - 1])
			{
				rchild->children[j - 1]->parent = rchild;
				rchild->childNum++;
				node->childNum--;
				node->children[i-1] = NULL;
			}
		}
		//�����һ��child
		rchild->children[j - 1] = node->children[i - 1];
		if (node->children[i - 1])
		{
			rchild->children[j - 1]->parent = rchild;
			rchild->childNum++;
			node->childNum--;
			node->children[i - 1] = NULL;
		}

		if (node->parent == NULL)//û�и��ڵ�
		{
			//��mid
			BNode* parent = CreatNode(node->keys[mid]);
			tree->root = parent;
			//������
			parent->children[0] = node;
			node->parent = parent;
			parent->children[1] = rchild;
			rchild->parent = parent;
			parent->childNum=2;
		}
		else
		{
			//��������
			int rchildPos = 0;
			for (i = 0; node->parent->children[i] != node; ++i)
			{
			}
			rchildPos = i+1;
			for (i = node->parent->childNum; i > rchildPos; --i)
			{
				node->parent->children[i] = node->parent->children[i - 1];
			}
			node->parent->children[rchildPos] = rchild;
			rchild->parent = node->parent;
			node->parent->childNum++;
			//��mid
			Insert_Key(tree, node->parent, node->keys[mid]);
		}
		node->keyNum--;
		node->keys[mid] = -1;
	}
	return key;
}

//B���ڵ���룬����ʧ�ܷ���0���ɹ�����key
int Insert_BT(BTree* tree, int key)
{
	assert(key > 0);
	BNode* node = Search_BT(tree, key);
	int rt=Insert_Key(tree, node, key);
	if (rt)
	{
		tree->size++;
	}
	return rt;
}

//B���������
void InOrder_Traversal(BNode* root)
{
	if (!root)return;//Ϊ����ֹ�ݹ�
	//��ӡÿ��key,���ݹ�����
	int i = 1;
	//for (int i = root->childNum; i < LEVEL; ++i)
	//{
	//	root->children[i] = NULL;
	//}
	for (; i <= root->keyNum; ++i)
	{
		InOrder_Traversal(root->children[i-1]);
		printf("%d ", root->keys[i]);
	}
	InOrder_Traversal(root->children[i-1]);
}

//B��ɾ��,�ɹ�����key��ʧ�ܷ���0
int Delete_BT(BTree* tree, int key)
{
	assert(tree&&key>0);
	//�ҵ��ڵ�
	BNode* curr = Search_BT(tree, key);
	//Ѱ��key
	int index = -1;
	for (int i = 1; i <= curr->keyNum; ++i)
	{
		if (key == curr->keys[i])
		{
			index = i;
			break;
		}
	}
	//û���ҵ�key
	if (index == -1)
	{
		return 0;
	}
	//�Ƿ���Ҷ�ڵ�
	if (!IsLeaf(curr))//����Ҷ�ڵ�
	{
		//��ǰ����������ת���ɶ�ǰ����ɾ��
		//�ҵ�ǰ��
		BNode* Pre = curr->children[index-1];
		while (!IsLeaf(Pre))
		{
			Pre = Pre->children[Pre->childNum-1];
		}
		curr->keys[index] = Pre->keys[Pre->keyNum];
		curr = Pre;
		index = Pre->keyNum;
	}
	//ɾ��key
	Erase_Key(curr,index);
	//�Ƿ������
	while (curr&&curr!=tree->root&&curr->keyNum < KEYS_MIN)//�����
	{
		BNode* LeftBrother = NULL;
		BNode* RightBrother = NULL;
		for (int i = 0; i < curr->parent->childNum; ++i)
		{
			if (curr->parent->children[i] == curr)
			{
				if (i > 0)LeftBrother = curr->parent->children[i - 1];
				if (i < curr->parent->childNum - 1)RightBrother = curr->parent->children[i + 1];
			}
		}
		//�ֵܹ���
		if (LeftBrother && LeftBrother->keyNum> KEYS_MIN)//���ֵܹ���
		{
			//Ѱ�Ҹ�keyλ��
			/*int posParent = 1;
			for (; curr->keys[1] > curr->parent->keys[posParent]; ++posParent);*///��key<����ĵ�һ��key,posParent-1���Ǹ�keyλ��
			int posParent = FindIndex_Parent(LeftBrother);
			Insert_Key(tree, curr, curr->parent->keys[posParent]);//����key��������
			curr->parent->keys[posParent]=LeftBrother->keys[LeftBrother->keyNum];//����ڵ���key(���һ��key)���븸�ڵ�
			LeftBrother->keyNum--;
			/*�����left������̬
				  key key
				  /  |   |    \
			child  child child child*/
			if (LeftBrother->keyNum == LeftBrother->childNum+2)//������������ڵ������
			{
				for (int i = curr->childNum; i > 0; --i)
				{
					curr->children[i] = curr->children[i - 1];
				}
				curr->children[0] = LeftBrother->children[LeftBrother->childNum-1];
				curr->children[0]->parent = curr;
				LeftBrother->children[LeftBrother->childNum - 1] = NULL;
				LeftBrother->childNum--;
				curr->childNum++;
			}
		}
		else if (RightBrother && RightBrother->keyNum > KEYS_MIN)//���ֵܹ���
		{
			//Ѱ�Ҹ�keyλ��
			/*int posParent = curr->parent->keyNum;
			for (; curr->keys[curr->keyNum] < curr->parent->keys[posParent]; --posParent);*///��key>��������һ��key,posParent+1���Ǹ�keyλ��
			int posParent = FindIndex_Parent(curr);
			Insert_Key(tree, curr, curr->parent->keys[posParent]);//����key��������
			curr->parent->keys[posParent] = RightBrother->keys[1];//���ҽڵ���key(��һ��key)���븸�ڵ�
			for (int i = 2; i <= RightBrother->keyNum; ++i)
			{
				RightBrother->keys[i - 1] = RightBrother->keys[i];
			}
			RightBrother->keyNum--;
			if (RightBrother->children[0])//������������ڵ������
			{
				curr->children[curr->childNum] = RightBrother->children[0];
				for (int i = 0; i < RightBrother->childNum-1; ++i)
				{
					curr->children[i] = curr->children[i + 1];
				}
				RightBrother->children[RightBrother->childNum - 1]->parent = curr;
				RightBrother->children[RightBrother->childNum-1] = NULL;
				RightBrother->childNum--;
				curr->childNum++;
			}
		}
		else//������
		{
			//�ڵ�ϲ�
			if (LeftBrother)//�����Ӿͺ����Ӻϲ�
			{
				CombineNode(tree,LeftBrother,curr);
				curr = curr->parent;
			}
			else
			{
				CombineNode(tree,curr,RightBrother);
				curr = RightBrother->parent;
			}
			
		}
	}
	tree->size--;
	return key;
}

//ɾ��key(����Ҳ��ɾ��������ǰ�ϲ��������ҵ�key���ұ�),��ִ�� childNum-- ����
void Erase_Key(BNode* curr, int index)
{
	assert(index > 0 && index <= curr->keyNum);
	int i = index;
	int flag = 0;//�Ƿ�������ɾ��
	for (; i < curr->keyNum; ++i)
	{
		curr->keys[i] = curr->keys[i + 1];
		curr->children[i-1] = curr->children[i];
		curr->children[i] = NULL;
		if (curr->children[i-1])
		{
			//curr->children[i-1]->parentKeyPos--;
			flag = 1;
		}
	}
	curr->keys[i] = -1;
	curr->children[i - 1] = curr->children[i];
	curr->children[i] = NULL;
	if (curr->children[i - 1])
	{
		//curr->children[i - 1]->parentKeyPos--;
		flag = 1;
	}

	curr->keyNum--;
	if (flag)
	{
		curr->childNum--;
	}
}

//�ж��Ƿ���Ҷ�ڵ�
bool IsLeaf(BNode* curr)
{
	if (curr->childNum == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Ѱ�Ҹ�keyλ��(����������key),��������,û�з���-1
int FindIndex_Parent(BNode* lchild)
{
	assert(lchild && lchild->parent);
	int posParent = 1;
	for (; posParent <= lchild->parent->keyNum&&lchild->parent->keys[posParent]<lchild->keys[lchild->keyNum]; ++posParent);
	if (posParent > lchild->parent->keyNum)return -1;
	return posParent;
}

//�ڵ�ϲ�
void CombineNode(BTree* tree,BNode* LeftNode, BNode* RightNode)
{
	assert(LeftNode && RightNode);
	//��key����right
	BNode* parent = RightNode->parent;
	int posParent = FindIndex_Parent(LeftNode);
	Insert_Key(NULL, RightNode, parent->keys[posParent]);
	//left��key����right
	//RightNode->children[RightNode->keyNum + LeftNode->keyNum] = RightNode->children[RightNode->keyNum];
	for (int i=RightNode->keyNum;i>=1;--i)
	{
		RightNode->keys[i + LeftNode->keyNum] = RightNode->keys[i];
	}
	for (int i = 1; i <= LeftNode->keyNum; ++i)
	{
		RightNode->keys[i] = LeftNode->keys[i];
	}
	//left��children����right
	for (int i = RightNode->childNum; i > 0; --i)
	{
		RightNode->children[i + LeftNode->childNum-1] = RightNode->children[i-1];
	}
	for (int i=0;i<LeftNode->childNum;++i)
	{
		RightNode->children[i] = LeftNode->children[i];
		RightNode->children[i]->parent = RightNode;
	}
	//����Num
	RightNode->childNum += LeftNode->childNum;
	RightNode->keyNum += LeftNode->keyNum;
	free(LeftNode);
	//ɾ��key
	Erase_Key(parent, posParent);
	//������ڵ���ֻ��һ������ʱ�����Ӵ��游�ڵ�
	if (!parent->keyNum && parent == tree->root)
	{
		assert(parent->childNum == 1);
		tree->root = RightNode;
		RightNode->parent = NULL;
		delete parent;
	}
}

//��սڵ�(���ڵ㲻��),�������ӽڵ�����������ڴ�й©
void ClearNode(BNode* node)
{
	assert(node);
	for (int i = 0; i < node->childNum; ++i)
	{
		node->children[i] = NULL;
	}
	node->childNum = 0;
	for (int i = 1; i <= node->keyNum; ++i)
	{
		node->keys[i] = -1;
	}
	node->keyNum = 0;
}

//�������û�г�ʼ��������գ�
void Clear(BTree* tree)
{
	assert(tree&& tree->root);
	std::queue<BNode*> q;
	for (int i = 0; i < tree->root->childNum; ++i)
	{
		q.push(tree->root->children[i]);
	}
	while (!q.empty())
	{
		BNode* temp = q.front();
		q.pop();
		for (int i = 0; i < temp->childNum; ++i)
		{
			q.push(temp->children[i]);
		}
		free(temp);
	}
	ClearNode(tree->root);
	tree->size = 0;
}

//������
void Destroy(BTree* tree)
{
	assert(tree);
	if (tree->root)
	{
		std::queue<BNode*> q;
		q.push(tree->root);
		while (!q.empty())
		{
			BNode* temp = q.front();
			q.pop();
			for (int i = 0; i < temp->childNum; ++i)
			{
				q.push(temp->children[i]);
			}
			free(temp);
		}
	}
	tree->size = 0;
	tree->root = NULL;
}

//B������,�ɹ�����key,ʧ�ܷ���0
int Find_BT(BTree* tree, int key)
{
	BNode* node=Search_BT(tree,key);
	int i = 1;
	for (; i <= node->keyNum && node->keys[i] != key; ++i);
	if (i > node->keyNum)return 0;
	else return key;
}