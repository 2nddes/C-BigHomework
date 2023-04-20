#define _CRT_SECURE_NO_WARNINGS
#include"head.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int scan(int* n) {
	*n = -1;
	char str[100];
	int i = 0;
	while (1) {
		gets_s(str, 100);
		if (str[0] != '\0')//输入为空
			break;
		printf("\t");
	}
	while (str[i] != '\0') {
		if ((str[i] < '0') || (str[i] > '9')) {
			return 0;
		}
		i++;
	}
	*n = atoi(str);
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
//加载数据到文件
void LoadAccountData() {
	FILE* fp;
	struct CAccount* pNew;
	headAccount = NULL;
	fp = fopen("account.dat", "rb");
	if (fp == NULL) {
		printf("账号文件打开失败！\n");
		return;
	}
	while (1) {
		pNew = (struct CAccount*)malloc(sizeof(struct CAccount));
		if (fread(pNew, sizeof(struct CAccount), 1, fp) != 1) {
			free(pNew);
			break;
		}
		if (headAccount == NULL) {
			headAccount = pNew;
			tailAccount = pNew;
		}
		else {
			tailAccount->next = pNew;
			tailAccount = pNew;
		}
	}
	fclose(fp);
}
void LoadGoodsData() {
	struct CGoods* pNew;
	headGoods = NULL;
	FILE* fp;
	fp = fopen("goods.dat", "rb");
	if (fp == NULL) {
		printf("商品文件打开失败！\n");
		return;
	}
	int count = 0;
	while (1) {
		pNew = (struct CGoods*)malloc(sizeof(struct CGoods));
		if (fread(pNew, sizeof(struct CGoods), 1, fp) != 1) {
			free(pNew);
			break;
		}
		pNew->goodsNum = ++count;
		if (headGoods == NULL) {
			headGoods = pNew;
			tailGoods = pNew;
		}
		else {
			tailGoods->next = pNew;
			tailGoods = pNew;
		}
	}
	fclose(fp);
}
void LoadOrderData() {
	struct COrder* pNew;
	headOrder = NULL;
	FILE* fp;
	fp = fopen("order.dat", "rb");
	if (fp == NULL) {
		printf("订单文件打开失败！\n");
		return;
	}
	int count = 0;
	while (1) {
		pNew = (struct COrder*)malloc(sizeof(struct COrder));
		if (fread(pNew, sizeof(struct COrder), 1, fp) != 1) {
			free(pNew);
			break;
		}
		pNew->orderNum = ++count;
		if (headOrder == NULL) {
			headOrder = pNew;
			tailOrder = pNew;
		}
		else {
			tailOrder->next = pNew;
			tailOrder = pNew;
		}
	}
	fclose(fp);
}
void LoadTrolleyData() {
	struct CTrolley* pNew;
	
	FILE* fp;
	fp = fopen("trolley.dat", "rb");
	if (fp == NULL) {
		printf("购物车文件打开失败！\n");
		return;
	}
	while (1) {
		pNew = (struct CTrolley*)malloc(sizeof(struct CTrolley));
		if (fread(pNew, sizeof(struct CTrolley), 1, fp) != 1) {
			free(pNew);
			break;
		}
		if (headTrolley==NULL) {
			headTrolley = pNew;
			tailTrolley = pNew;
		}
		else {
			tailTrolley->next = pNew;
			tailTrolley = pNew;
		}
	}
	fclose(fp);
}
void LoadFullDiscountData() {

	FILE* fp;
	fp = fopen("fullDiscount.dat", "rb");
	if (fp == NULL) {
		printf("\n\t折扣文件打开失败！\n");
		return;
	}
	headFullDiscount = (struct CFullDiscount*)malloc(sizeof(struct CFullDiscount));
	if (fread(headFullDiscount, sizeof(struct CFullDiscount), 1, fp) != 1) {
		
		headFullDiscount->discount[0] = 10;
		headFullDiscount->price[0] = 1;
		headFullDiscount->stageOfDiscoiunt = 1;
	}
	fclose(fp);

}
void LoadFestivalDiscountData() {
	FILE* fp;
	fp = fopen("festivalDiscount.dat", "rb");
	if (fp == NULL) {
		printf("\n\t折扣文件打开失败！\n");
		return;
	}
	headFestivalDiscount = (struct CFestivalDiscount*)malloc(sizeof(struct CFestivalDiscount));
	if (fread(headFestivalDiscount, sizeof(struct CFestivalDiscount), 1, fp) != 1) {
		headFestivalDiscount->discountSetted = 10;
	}
	fclose(fp);

}
void LoadGiftData() {
	FILE* fp;
	//从文件读取gift字符串到char gift[40]
	fp = fopen("gift.dat", "rb");
	if (fp == NULL) {
		printf("\n\t礼品文件打开失败！\n");
		return;
	}
	if (fread(&Gift, sizeof(struct CGift), 1, fp) != 1) {
		strcpy(Gift.gift, "无");
		Gift.nums = 0;
		Gift.threshold = 0;
	}
	fclose(fp);
}
void LoadGradeData() {
	FILE* fp;
	fp = fopen("grade.dat", "rb");
	if (fp == NULL) {
		printf("\n\t折扣文件打开失败！\n");
		return;
	}
	headGrade = (struct CGrade*)malloc(sizeof(struct CGrade));
	if (fread(headGrade, sizeof(struct CGrade), 1, fp) != 1) {
		//在这里添加需要初始化的信息
		headGrade->discount[0] = 10;
		headGrade->stageOfGrade = 1;
		headGrade->price[0] = 1;
	}
	fclose(fp);
}
/////////////////////////////////////////////////////////////////////////////
//保存商品信息
void SaveGoodsData() {
	FILE* fp;
	struct CGoods* pNow;
	fp = fopen("goods.dat", "wb");
	if (fp == NULL)
		return;
	pNow = headGoods;
	while (pNow != NULL) {
		fwrite(pNow, sizeof(struct CGoods), 1, fp);
		pNow = pNow->next;
	}
	fclose(fp);
}
void SaveGoodsTrolleyData() {
	FILE* fp;
	struct CTrolley* pNow;
	fp = fopen("trolley.dat", "wb");
	if (fp == NULL)
		return;
	pNow = headTrolley;
	while (pNow != NULL) {
		fwrite(pNow, sizeof(struct CTrolley), 1, fp);
		pNow = pNow->next;
	}
	fclose(fp);
}
void SaveAccountData() {
	FILE* fp;
	struct CAccount* pNow;
	fp = fopen("account.dat", "wb");
	if (fp == NULL)
		return;
	pNow = headAccount;
	while (pNow != NULL) {
		fwrite(pNow, sizeof(struct CAccount), 1, fp);
		pNow = pNow->next;
	}
	fclose(fp);
}
void SaveOrderData() {
	FILE* fp;
	struct COrder* pNow;
	fp = fopen("order.dat", "wb");
	if (fp == NULL)
		return;
	pNow = headOrder;
	while (pNow != NULL) {
		fwrite(pNow, sizeof(struct COrder), 1, fp);
		pNow = pNow->next;
	}
	fclose(fp);
}
void SaveGiftData() {
	//将gift字符串保存到文件
	FILE* fp;
	fp = fopen("gift.dat", "wb");
	if (fp == NULL)
		return;
	fwrite(&Gift, sizeof(struct CGift), 1, fp);
	fclose(fp);
}
void SaveGradeData() {
	FILE* fp;
	fp = fopen("grade.dat", "wb");
	if (fp == NULL)
		return;
	fwrite(headGrade, sizeof(struct CGrade), 1, fp);
	fclose(fp);
}
/////////////////////////////////////////////////////////////////////////////
//释放空间
void releaseAccount() {
	struct CAccount* p = headAccount;
	while (p != NULL) {
		headAccount = headAccount->next;
		free(p);
		p = headAccount;
	}
	headAccount = NULL;
}
void releaseData() {
	struct CGoods* p1 = headGoods;
	while (p1 != NULL) {
		headGoods = headGoods->next;
		free(p1);
		p1 = headGoods;
	}
	headGoods = NULL;

	struct COrder* p2 = headOrder;
	while (p2 != NULL) {
		headOrder = headOrder->next;
		free(p2);
		p2 = headOrder;
	}
	headOrder = NULL;

	struct CTrolley* p3 = headTrolley;
	while (p3 != NULL) {
		headTrolley = headTrolley->next;
		free(p3);
		p3 = headTrolley;
	}
	headTrolley = NULL;

	free(headFullDiscount);
	headFullDiscount = NULL;

	free(headFestivalDiscount);
	headFestivalDiscount = NULL;
}
/////////////////////////////////////////////////////////////////////////////


