#define _CRT_SECURE_NO_WARNINGS
#include"BTree.h"

//初始化树
void InitBTree(BTree* tree)
{
	tree->root = CreatNode();
	tree->size = 0;
}

//创建节点,插入第一个关键字
BNode* CreatNode(int key)
{
	//节点
	BNode* newnode = (BNode*)malloc(sizeof(BNode));
	assert(newnode);
	//孩子数组
	//newnode->children = (BNode**)malloc(sizeof(BNode*) * (LEVEL + 1));
	newnode->children = (BNode**)malloc(sizeof(BNode*) * (LEVEL+1));
	assert(newnode->children);
	for (int i = 0; i < LEVEL + 1; ++i)
	{
		newnode->children[i] = NULL;
	}
	newnode->childNum = 0;
	//关键字数组
	//newnode->keys = (int*)malloc(sizeof(int) * LEVEL);
	newnode->keys = (int*)malloc(sizeof(int) * (LEVEL + 1));
	assert(newnode->keys);
	for (int i = 0; i < LEVEL+1; ++i)
	{
		newnode->keys[i] = -1;
	}
	newnode->keys[1] = key;
	newnode->keyNum = 1;
	//父节点
	newnode->parent = NULL;
	//返回
	return newnode;
}

//创建节点
BNode* CreatNode()
{
	//节点
	BNode* newnode = (BNode*)malloc(sizeof(BNode));
	assert(newnode);
	//孩子数组
	//newnode->children = (BNode**)malloc(sizeof(BNode*) * (LEVEL + 1));
	newnode->children = (BNode**)malloc(sizeof(BNode*) * (LEVEL + 1));
	assert(newnode->children);
	for (int i = 0; i < LEVEL + 1; ++i)
	{
		newnode->children[i] = NULL;
	}
	newnode->childNum = 0;
	//关键字数组
	//newnode->keys = (int*)malloc(sizeof(int) * LEVEL);
	newnode->keys = (int*)malloc(sizeof(int) * (LEVEL+1));
	assert(newnode->keys);
	for (int i = 0; i < LEVEL+1; ++i)
	{
		newnode->keys[i] = -1;
	}
	newnode->keyNum = 0;
	//父节点
	newnode->parent = NULL;
	//返回
	return newnode;
}

//寻找子树索引,返回下标
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

//B树查找，返回节点,key值在返回节点的区间中，可能不存在这个key值
BNode* Search_BT(BTree* tree, int key)
{
	assert(tree->root&&key>0);
	BNode* curr = tree->root;
	//BNode* parent = tree->root;
	int index = 0;
	while (curr->childNum)
	{
		////给没意义的子树滞空
		//for (int i = curr->childNum; i < LEVEL; ++i)
		//{
		//	curr->children[i] = NULL;
		//}
		index = FindIndex(curr, key);
		if ((index<=curr->keyNum&&key == curr->keys[index])|| !curr->children[index - 1])//要保证其后没有意义的key不能和需要查找的key相同
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

//关键字插入，顺便完成分裂,插入失败返回0，成功返回key
int Insert_Key(BTree* tree, BNode* node, int key)
{
	assert(key > 0&&node);
	//插入key
	int index = FindIndex(node, key);//要确保key不会有相同
	if (node->keys[index] == key)//有相同的key了
	{
		return 0;
	}
	for (int i = node->keyNum; i >= index; --i)
	{
		node->keys[i + 1] = node->keys[i];
	}
	node->keys[index] = key;
	node->keyNum++;
	//分裂
	if (node->keyNum == LEVEL)
	{
		int mid = LEVEL / 2 + LEVEL % 2;
		BNode* rchild = CreatNode();
		int j = 1;
		int i = mid + 1;
		int keyNum = node->keyNum;
		for (; i <= keyNum; ++i,++j)
		{
			//移key
			rchild->keys[j] = node->keys[i];
			rchild->keyNum++;
			node->keyNum--;
			node->keys[i] = -1;
			//移child
			rchild->children[j - 1] = node->children[i - 1];
			if (node->children[i - 1])
			{
				rchild->children[j - 1]->parent = rchild;
				rchild->childNum++;
				node->childNum--;
				node->children[i-1] = NULL;
			}
		}
		//移最后一个child
		rchild->children[j - 1] = node->children[i - 1];
		if (node->children[i - 1])
		{
			rchild->children[j - 1]->parent = rchild;
			rchild->childNum++;
			node->childNum--;
			node->children[i - 1] = NULL;
		}

		if (node->parent == NULL)//没有父节点
		{
			//插mid
			BNode* parent = CreatNode(node->keys[mid]);
			tree->root = parent;
			//插子树
			parent->children[0] = node;
			node->parent = parent;
			parent->children[1] = rchild;
			rchild->parent = parent;
			parent->childNum=2;
		}
		else
		{
			//插右子树
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
			//插mid
			Insert_Key(tree, node->parent, node->keys[mid]);
		}
		node->keyNum--;
		node->keys[mid] = -1;
	}
	return key;
}

//B树节点插入，插入失败返回0，成功返回key
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

//B树中序遍历
void InOrder_Traversal(BNode* root)
{
	if (!root)return;//为空终止递归
	//打印每个key,并递归子树
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

//B树删除,成功返回key，失败返回0
int Delete_BT(BTree* tree, int key)
{
	assert(tree&&key>0);
	//找到节点
	BNode* curr = Search_BT(tree, key);
	//寻找key
	int index = -1;
	for (int i = 1; i <= curr->keyNum; ++i)
	{
		if (key == curr->keys[i])
		{
			index = i;
			break;
		}
	}
	//没有找到key
	if (index == -1)
	{
		return 0;
	}
	//是否是叶节点
	if (!IsLeaf(curr))//不是叶节点
	{
		//用前驱代替自身，转换成对前驱的删除
		//找到前驱
		BNode* Pre = curr->children[index-1];
		while (!IsLeaf(Pre))
		{
			Pre = Pre->children[Pre->childNum-1];
		}
		curr->keys[index] = Pre->keys[Pre->keyNum];
		curr = Pre;
		index = Pre->keyNum;
	}
	//删除key
	Erase_Key(curr,index);
	//是否下溢出
	while (curr&&curr!=tree->root&&curr->keyNum < KEYS_MIN)//下溢出
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
		//兄弟够借
		if (LeftBrother && LeftBrother->keyNum> KEYS_MIN)//左兄弟够借
		{
			//寻找父key位置
			/*int posParent = 1;
			for (; curr->keys[1] > curr->parent->keys[posParent]; ++posParent);*///父key<自身的第一个key,posParent-1就是父key位置
			int posParent = FindIndex_Parent(LeftBrother);
			Insert_Key(tree, curr, curr->parent->keys[posParent]);//将父key插入自身
			curr->parent->keys[posParent]=LeftBrother->keys[LeftBrother->keyNum];//将左节点借的key(最后一个key)插入父节点
			LeftBrother->keyNum--;
			/*借完后left可能形态
				  key key
				  /  |   |    \
			child  child child child*/
			if (LeftBrother->keyNum == LeftBrother->childNum+2)//处理自身与左节点的子树
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
		else if (RightBrother && RightBrother->keyNum > KEYS_MIN)//右兄弟够借
		{
			//寻找父key位置
			/*int posParent = curr->parent->keyNum;
			for (; curr->keys[curr->keyNum] < curr->parent->keys[posParent]; --posParent);*///父key>自身的最后一个key,posParent+1就是父key位置
			int posParent = FindIndex_Parent(curr);
			Insert_Key(tree, curr, curr->parent->keys[posParent]);//将父key插入自身
			curr->parent->keys[posParent] = RightBrother->keys[1];//将右节点借的key(第一个key)插入父节点
			for (int i = 2; i <= RightBrother->keyNum; ++i)
			{
				RightBrother->keys[i - 1] = RightBrother->keys[i];
			}
			RightBrother->keyNum--;
			if (RightBrother->children[0])//处理自身与左节点的子树
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
		else//不够借
		{
			//节点合并
			if (LeftBrother)//有左孩子就和左孩子合并
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

//删除key(子树也会删除，请提前合并子树并挂到key的右边),会执行 childNum-- 操作
void Erase_Key(BNode* curr, int index)
{
	assert(index > 0 && index <= curr->keyNum);
	int i = index;
	int flag = 0;//是否有子树删除
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

//判断是否是叶节点
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

//寻找父key位置(大于其所有key),返回索引,没有返回-1
int FindIndex_Parent(BNode* lchild)
{
	assert(lchild && lchild->parent);
	int posParent = 1;
	for (; posParent <= lchild->parent->keyNum&&lchild->parent->keys[posParent]<lchild->keys[lchild->keyNum]; ++posParent);
	if (posParent > lchild->parent->keyNum)return -1;
	return posParent;
}

//节点合并
void CombineNode(BTree* tree,BNode* LeftNode, BNode* RightNode)
{
	assert(LeftNode && RightNode);
	//父key插入right
	BNode* parent = RightNode->parent;
	int posParent = FindIndex_Parent(LeftNode);
	Insert_Key(NULL, RightNode, parent->keys[posParent]);
	//left的key插入right
	//RightNode->children[RightNode->keyNum + LeftNode->keyNum] = RightNode->children[RightNode->keyNum];
	for (int i=RightNode->keyNum;i>=1;--i)
	{
		RightNode->keys[i + LeftNode->keyNum] = RightNode->keys[i];
	}
	for (int i = 1; i <= LeftNode->keyNum; ++i)
	{
		RightNode->keys[i] = LeftNode->keys[i];
	}
	//left的children插入right
	for (int i = RightNode->childNum; i > 0; --i)
	{
		RightNode->children[i + LeftNode->childNum-1] = RightNode->children[i-1];
	}
	for (int i=0;i<LeftNode->childNum;++i)
	{
		RightNode->children[i] = LeftNode->children[i];
		RightNode->children[i]->parent = RightNode;
	}
	//调整Num
	RightNode->childNum += LeftNode->childNum;
	RightNode->keyNum += LeftNode->keyNum;
	free(LeftNode);
	//删父key
	Erase_Key(parent, posParent);
	//如果父节点且只有一个孩子时，孩子代替父节点
	if (!parent->keyNum && parent == tree->root)
	{
		assert(parent->childNum == 1);
		tree->root = RightNode;
		RightNode->parent = NULL;
		delete parent;
	}
}

//清空节点(父节点不变),可能与子节点断连，导致内存泄漏
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

//清空树（没有初始化不能清空）
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

//销毁树
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

//B树查找,成功返回key,失败返回0
int Find_BT(BTree* tree, int key)
{
	BNode* node=Search_BT(tree,key);
	int i = 1;
	for (; i <= node->keyNum && node->keys[i] != key; ++i);
	if (i > node->keyNum)return 0;
	else return key;
}