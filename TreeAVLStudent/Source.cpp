#include <iostream>

#pragma warning(disable:4996)

struct SV
{
	int mssv;
	char tensv[30];
};

struct AVLNODE
{
	SV Key;
	int bal; // thuộc tính cho biết giá trị cân bằng
	// 0: cân bằng, 1: lệch trái, 2: lệch phải
	AVLNODE* pLeft;
	AVLNODE* pRight;
};

AVLNODE* CreateNode(SV Data)
{
	AVLNODE* pNode;
	pNode = new AVLNODE; //Xin cấp phát bộ nhớ động để tạo một phần tử (node) mới
	if (pNode == NULL) {
		return NULL;
	}
	pNode->Key = Data;
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

int InsertNode(AVLNODE* &tree, SV x)
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
		if (tree->Key.mssv == x.mssv) {
			return 0; //khóa này đã tồn tại trong cây
		}
		//thêm node vào bên trái
		else if (tree->Key.mssv > x.mssv) {
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

void Traverse(AVLNODE* t)
{
	if (t != NULL)
	{
		Traverse(t->pLeft);
		printf("mssv: %d hoten: %s, can bang: %d\n", t->Key.mssv, t->Key.tensv, t->bal);
		Traverse(t->pRight);

	}
}

AVLNODE* SearchNode(AVLNODE* pNode, SV key)
{
	if (pNode == NULL)
	{
		return NULL;
	}
	if (pNode->Key.mssv > key.mssv)
	{
		SearchNode(pNode->pLeft, key);
	}
	else if (pNode->Key.mssv < key.mssv)
	{
		SearchNode(pNode->pRight, key);
	}
	else
	{
		return pNode;
	}
}

void RemoveAll(AVLNODE* &t)
{
	if (t != NULL) {
		RemoveAll(t->pLeft);
		RemoveAll(t->pRight);
		delete t;
	}
}

int main(int argc, char* argv[])
{
	AVLNODE *tree;

	tree = NULL;
	SV Data;
	printf("nhap vao so luong sv ");
	int soluong;
	scanf("%d", &soluong);
	for (int i = 0; i < soluong; i++)
	{
		printf("\nnhap thong tin sv ");
		SV tempsv;
		printf("\nnhap vao mssv ");
		scanf("%d", &tempsv.mssv);
		printf("nhap vao ten sv ");
		scanf("%s", tempsv.tensv);
		InsertNode(tree, tempsv);
	}


	printf("\nCay AVL vua tao: \n");
	Traverse(tree);



	RemoveAll(tree);
	system("pause");
	return 0;
}

