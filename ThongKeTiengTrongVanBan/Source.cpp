#include <iostream>
#pragma warning(disable:4996)

struct AVLNODE
{
	char Key[30];
	int number = 1;
	int bal; // thuộc tính cho biết giá trị cân bằng
	// 0: cân bằng, 1: lệch trái, 2: lệch phải
	AVLNODE* pLeft;
	AVLNODE* pRight;
};

void CopyChuoi(char* dst, char* src)
{
	int length = 0;
	while (src[length] != '\0')
	{
		dst[length] = src[length];
		length++;
	}
	dst[length] = '\0';
}

AVLNODE* CreateNode(char Data[])
{
	AVLNODE* pNode;
	pNode = new AVLNODE; //Xin cấp phát bộ nhớ động để tạo một phần tử (node) mới
	if (pNode == NULL) {
		return NULL;
	}
	CopyChuoi(pNode->Key, Data);
	pNode->pLeft = NULL;
	pNode->pRight = NULL;
	pNode->bal = 0; //Ghi chú: khởi tạo thuộc tính cân bằng hiện tại đang cân bằng và có giá trị bằng 0
	return pNode;
}

void LeftRotate(AVLNODE* &P)
{
	AVLNODE *Q;
	Q = P->pRight;
	P->pRight = Q->pLeft;
	Q->pLeft = P;
	P = Q;
}

void RightRotate(AVLNODE* &P)
{
	AVLNODE *Q;
	Q = P->pLeft;
	P->pLeft = Q->pRight;
	Q->pRight = P;
	P = Q;
}

void LeftBalance(AVLNODE* &P)
{
	switch (P->pLeft->bal) {
	case 1: //Ghi chú: trường hợp mất cân bằng trái trái
		RightRotate(P);
		P->bal = 0;
		P->pRight->bal = 0;
		break;
	case 2: //Ghi chú: trường hợp mất cân bằng trái phải
		LeftRotate(P->pLeft);
		RightRotate(P);
		switch (P->bal) {
		case 0:
			P->pLeft->bal = 0;
			P->pRight->bal = 0;
			break;
		case 1:
			P->pLeft->bal = 0;
			P->pRight->bal = 2;
			break;
		case 2:
			P->pLeft->bal = 1;
			P->pRight->bal = 0;
			break;
		}
		P->bal = 0;
		break;
	}
}

void RightBalance(AVLNODE* &P)
{
	switch (P->pRight->bal) {
	case 1: //Ghi chú: trường hợp mất cân bằng phải trái
		RightRotate(P->pRight);
		LeftRotate(P);
		switch (P->bal) {
		case 0:
			P->pLeft->bal = 0;
			P->pRight->bal = 0;
			break;
		case 1:
			P->pLeft->bal = 1;
			P->pRight->bal = 0;
			break;
		case 2:
			P->pLeft->bal = 0;
			P->pRight->bal = 2;
			break;
		}
		P->bal = 0;
		break;
	case 2: //Ghi chú: trường hợp mất cân bằng phải phải
		LeftRotate(P);
		P->bal = 0;
		P->pLeft->bal = 0;
		break;
	}
}

int InsertNode(AVLNODE* &tree, char* x)
{
	int res;
	if (tree == NULL) { //Ghi chú: xử lý trường hợp tại vị trí trỏ tới chưa có node cần thêm vào node
		tree = CreateNode(x);
		if (tree == NULL) {
			return -1; //thêm ko thành công vì thiếu bộ nhớ
		}
		return 2;//thêm thành công và làm tăng chiều cao cây
	}
	else {
		if (strcmp(tree->Key, x) == 0 ) {
			tree->number++;
			return 0; //khóa này đã tồn tại trong cây
		}
		//thêm node vào bên trái
		else if (strcmp(tree->Key, x) > 0) {
			res = InsertNode(tree->pLeft, x);
			if (res < 2) {   //xét trường hợp không phêm thành công
				return res;	 //trả về -1 khi không thêm thành công
			}
			switch (tree->bal) { //Ghi chú: xét các trường hợp thay đổi chỉ số bal khi thêm node vào nếu gây mất cân bằng trái thì phải xử lý 
			case 0:	//đang cân bằng sau khi thêm vào bị lêch trái
				tree->bal = 1;
				return 2;
			case 1: //đang lệch trái tại node đang xét sau khi thêm vào mất cân bằng trái                     
				LeftBalance(tree);
				return 1;
			case 2: //đang lệch phải sau khi thêm vào thành cân bằng										 				                                                                                             
				tree->bal = 0;
				return 1;
			}
		}
		//thêm node vào bên phải
		else
		{
			res = InsertNode(tree->pRight, x);
			if (res < 2) {
				return res;
			}
			switch (tree->bal) { //Ghi chú: xét các trường hợp thay đổi chỉ số bal khi thêm node vào nếu gây mất cân bằng phải thì phải xử lý
			case 0: //đang cân bằng sau khi thêm vào bị lệch phải
				tree->bal = 2;
				return 2;
			case 1: //đang lệch phải sau khi thêm vào thành cân bằng
				tree->bal = 0;
				return 1;
			case 2: //đang lệch phải tại node đang xét sau thêm vào mất cân bằng phải
				RightBalance(tree);
				return 1;
			}
		}
	}
}

AVLNODE* SearchNode(AVLNODE* pNode, char* key)
{
	if (pNode == NULL)
	{
		return NULL;
	}
	if (strcmp(pNode->Key, key) > 0)
	{
		SearchNode(pNode->pLeft, key);
	}
	else if (strcmp(pNode->Key, key) < 0)
	{
		SearchNode(pNode->pRight, key);
	}
	else
	{
		return pNode;
	}
}

void ReadFileCreateTree(AVLNODE *&pRoot, char* filename)
{
	FILE* fi = fopen(filename, "rt");
	char temp[30];
	while (!feof(fi))
	{
		if (fscanf(fi, "%s", temp) > 0)
		{
			printf("%s ", temp);
			InsertNode(pRoot, temp);
		}
		else
			break;
	}
	printf("\n");
	fclose(fi);
}

void RemoveAll(AVLNODE* &t)
{
	if (t != NULL) {
		RemoveAll(t->pLeft);
		RemoveAll(t->pRight);
		delete t;
	}
}

int searchStandFor(AVLNODE* &pNode, AVLNODE* &q)
{
	int res;
	if (q->pLeft != NULL)
	{
		res = searchStandFor(pNode, q->pLeft);
		if (res < 2)
			return res;
		switch (q->bal)
		{
		case 1:
			q->bal = 0;
			return 2;
		case 0:
			q->bal = 2;
			return 1;
		case 2:
			RightBalance(pNode);
			return 1;
		}
	}
	else {
		CopyChuoi(pNode->Key, q->Key);
		pNode = q;
		q = q->pRight;
		return 2;
	}
}

int RemoveNode(AVLNODE* &pTree, char* x)
{
	int res;
	if (pTree == NULL)
		return 0;
	else
	{
		if (strcmp(pTree->Key, x) > 0)  //sang con trái
		{
			res = RemoveNode(pTree->pLeft, x);
			if (res < 2)
				return res;
			switch (pTree->bal)
			{
			case 1:   //trường hợp con bên trái bị xóa thì đang lệch trái thành không lệch
				pTree->bal = 0;
				return 2;
			case 0:   //trường hợp đang cân hai cây con xóa con bên trái thì bị lệch phải 
				pTree->bal = 2;
				return 1;
			case 2:
				RightBalance(pTree);
				return 1;
			}
		}
		else if (strcmp(pTree->Key, x) < 0)  //sang con phải
		{
			res = RemoveNode(pTree->pRight, x);
			if (res < 2)
				return res;
			switch (pTree->bal)
			{
			case 2:  //trường hợp đang lệch phải sau khi xóa thì thành không lệch 
				pTree->bal = 0;
				return 2;
			case 0:  //trường hợp hai con không lệch sau khi xóa con phải thì lệch trái
				pTree->bal = 1;
				return 1;
			case 1:	 //trường hợp hợp đang lệch trái thì cháu của con trái bị xóa gây mất cân bằng trái
				LeftBalance(pTree);
				return 1;
			}
		}
		else
		{
			AVLNODE* p = pTree;
			if (pTree->pLeft == NULL)
			{
				pTree = pTree->pRight;
				res = 2;
			}
			else if (pTree->pRight == NULL)
			{
				pTree = pTree->pLeft;
				res = 2;
			}
			else // if (T-> pLeft != NULL && T-> pRight != NULL) 
			{
				res = searchStandFor(p, pTree->pRight);  //tìm con trái nhất của con phải
				if (res < 2)
					return res;
				switch (pTree->bal)
				{
				case 2:
					pTree->bal = 0;
					return 2;
				case 0: pTree->bal = 1;
					return 1;
				case 1:
					LeftBalance(pTree);
					return 1;
				}
			}
			delete p;
			return res;
		}
	}
}

void ShowNumOfString(AVLNODE* pTree)
{
	if (pTree != NULL)
	{
		ShowNumOfString(pTree->pLeft);
		printf("tu \"%s\" co so lan xuat hien la %d\n", pTree->Key, pTree->number);
		ShowNumOfString(pTree->pRight);
	}
}



int main(int argc, char* argv[])
{
	AVLNODE *tree;
	tree = NULL;


	//đọc dữ liệu từ file input
	ReadFileCreateTree(tree, (char*)"input.txt");
	ShowNumOfString(tree);
	

	system("pause");
	return 0;
}

