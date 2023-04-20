#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include"head.h"
#include"goods.h"

//商品界面PrePage进行选择前往购物车还是看商品，管理员直接进商品界面
void GoodsFindAndChange() {
	int n;
	while (1) {
		system("cls");
		printf("\t***********************************商品浏览界面*********************************\n\n");
		if (pLoggedInUser->grade == 0) {//判断用户等级，管理员只能看商品，不能购买，没有购物车
			GoodsList();
			return;
		}
		else {
			printf("\n\t----------------------------------操  作  菜  单--------------------------------\n\n");
			printf("\t***********************-0--返回                  返回--0-***********************\n\n");
			printf("\t***********************-1--浏览商品            购物车--2-***********************\n\n");
			printf("\t请输入操作序号：");
			n = -1;
			scan(&n);
			if (n == 0)
				return;
			else if (n == 1)
				GoodsList();
			else if (n == 2)
				GoodsTrolley();
			else
				printf("输入代码错误\n\t");
		}
	}
}

//释放商品链表
static void releaseGoods(struct CGoods* head) {
	struct CGoods* p1 = head;
	while (p1 != NULL) {
		head = head->next;
		free(p1);
		p1 = head;
	}
	head = NULL;
}
//复制链表
static struct CGoods* copyGoods(struct CGoods* copied) {
	if (copied == NULL)return NULL;
	struct CGoods* pNewHead = (struct CGoods*)malloc(sizeof(struct CGoods));
	*pNewHead = *copied;
	pNewHead->next = NULL;
	copied = copied->next;
	struct CGoods* pNew1 = pNewHead;
	struct CGoods* pNew2 = NULL;
	while (copied != NULL) {
		pNew2 = (struct CGoods*)malloc(sizeof(struct CGoods));
		*pNew2 = *copied;
		pNew2->next = NULL;
		pNew1->next = pNew2;
		copied = copied->next;
		pNew1 = pNew1->next;
		pNew2 = pNew2->next;
	}
	return pNewHead;
}

//打印指定的单个商品
static const void GoodsSinglePrint(struct CGoods* pNow) {
	printf("%-4d", pNow->goodsNum);
	printf("%24s", pNow->brand);
	printf("%20s", pNow->kind);
	printf("%9d°", pNow->concentration);
	printf("%11d", pNow->netContent);
	printf("%13d", pNow->boxesNumber);
	printf("%13.2f", pNow->price);
	printf("%13d箱余%03d瓶", pNow->stock / pNow->boxesNumber, pNow->stock % pNow->boxesNumber);
	printf("%10d\n\t", pNow->sales);
	printf("-------------------------------------------------------------------------------------------------------------------------------------\n\t");
}
//打印指定的商品列表
static const void GoodsPrint(struct CGoods* head) {
	struct CGoods* pNow = head;
	printf("\t编号               商品名称               商品类别   酒精浓度   每瓶净含量(ml)  每箱瓶数  价格(元)/瓶      库存(箱+瓶)       销量(瓶)\n");
	printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
	pNow = head;
	if (pNow == NULL)//列表防空
		printf("商品列表为空\n\t");
	while (pNow != NULL) {//输出商品列表
		GoodsSinglePrint(pNow);
		pNow = pNow->next;
	}

}

//商品模糊搜索
static struct CGoods* GoodsSearch(struct CGoods* head, const char* keyword) {
	struct CGoods* pNow = head;
	struct CGoods* result = NULL;
	int len = strlen(keyword);

	while (pNow != NULL) {
		if (strstr(pNow->brand, keyword) != NULL||strstr(pNow->kind,keyword)!=NULL) {
			// 找到匹配的商品
			if (result == NULL) {
				result = (struct CGoods*)malloc(sizeof(struct CGoods));
				*result = *pNow;
				result->next = NULL;
			}
			else {
				struct CGoods* newResult = (struct CGoods*)malloc(sizeof(struct CGoods));
				*newResult = *pNow;
				newResult->next = result;
				result = newResult;
			}
		}
		pNow = pNow->next;
	}
	if (result == NULL) {
		printf("\n\t未找到匹配商品，即将返回");
		Sleep(1000);
		return head;
	}
	else
		return result;
}

//商品浏览界面
static void GoodsList() {
	struct CGoods* pNow;
	struct CGoods* head = copyGoods(headGoods);//拷贝商品链表，提供多次筛选和筛选排序混合功能，防止伤害商品链表
	int goodsnum = 1;
	int n = 10;
	int units = 1, boxesnum = 0;
	char keystr[40];
	while (1)
	{
		system("cls");
		printf("\t************************************************************商品浏览界面************************************************************\n\n");
		printf("\t-----------------------------------------------------------商  品  列  表-----------------------------------------------------------\n\n");
		GoodsPrint(head);
		if (pLoggedInUser->grade == 0) {//管理员操作
			printf("\n\t-----------------------------------------------------------操  作  菜  单-----------------------------------------------------------\n\n");
			printf("\t                      ********************-0--返回                              返回--0-********************\n\n");
			printf("\t                      ********************-1--筛选此商品列表          搜索此商品列表--2-********************\n\n");
			printf("\t                      ********************-3--重置为初始列表          价格由低至高序--4-********************\n\n");
			printf("\t                      ********************-5--销量由高至低序              添加新商品--6-********************\n\n");
			printf("\t                      ********************-7--修改商品信息                  删除商品--8-********************\n\n");
			printf("\t                      --------------------------------------------------------------------------------------\n\n");
			printf("\n\t请输入操作序号：");
			n = -1;
			scan(&n);
			if (n == 0)
				return;
			else if (n == 1)
				head = GoodsFilter(head);
			else if (n == 2) {
				printf("\t输入关键词(直接回车返回)：");
				gets(keystr);
				if (keystr[0] == '\0') {
					continue;
				}
				head = GoodsSearch(head, keystr);
			}
			else if (n == 3) {
				releaseGoods(head);
				head = copyGoods(headGoods);
			}
			else if (n == 4)
				head = ascendSortGoodsByPrice(head);
			else if (n == 5)
				head = decendSortGoodsBySales(head);
			else if (n == 6) {
				GoodsAdd();
				head = copyGoods(headGoods);
			}
			else if (n == 7) {
				printf("\t修改商品信息请输入商品编号[编号]  返回[0]\n\t");
				scan(&goodsnum);
				if (goodsnum == 0)continue;
				pNow = headGoods;
				while (pNow != NULL && pNow->goodsNum != goodsnum)
					pNow = pNow->next;
				if (pNow != NULL) {
					printf("\t修改  库存[1]  品名[2]  种类[3]  净含量[4]  每箱瓶数[5]  价格[6]  销量[7]  返回[0]\n\t");
					n = -1;
					scan(&n);
					if (n == 0)break;
					if (n <= 7 && n >= 1) {
						printf("\t");
						GoodsChange(pNow, n);
						SaveGoodsData();//保存信息到文件
					}
					else {
						printf("\t代码错误\n");
						Sleep(500);
					}
					head = copyGoods(headGoods);
				}
				else {
					printf("\t商品不存在\n\t");
					Sleep(500);
				}
			}
			else if (n == 8) {
				printf("\t删除商品信息请输入商品编号[编号]  返回[0]\n\t");
				scan(&goodsnum);
				if (goodsnum == 0)continue;
				//只能删除所指的下一个节点，故生成哨兵使其可删头节点
				pNow = (struct CGoods*)malloc(sizeof(struct CGoods));
				pNow->next = headGoods;
				while (pNow->next != NULL && pNow->next->goodsNum != goodsnum)
					pNow = pNow->next;
				if (pNow->next != NULL) {
					while (1)
					{
						printf("\t确认删除[1]  输入任意键返回\n\t");
						scanf_s("%d", &n);
						getchar();
						if (n == 1) {
							GoodsDelete(pNow);
							SaveGoodsData();//保存信息到文件
						}
						head = copyGoods(headGoods);
						break;
					}
				}
				else {
					printf("\t商品不存在\n\t");
					Sleep(500);
				}
			}
			else {
				printf("\t输入代码错误\n\t");
				Sleep(500);
			}
		}
		else {//用户操作
			printf("\n\t-----------------------------------------------------------操  作  菜  单-----------------------------------------------------------\n\n");
			printf("\t                      ********************-0--返回                              返回--0-********************\n\n");
			printf("\t                      ********************-1--筛选此商品列表          搜索此商品列表--2-********************\n\n");
			printf("\t                      ********************-3--重置为初始列表          价格由低至高序--4-********************\n\n");
			printf("\t                      ********************-5--销量由高至低序        添加商品至购物车--6-********************\n\n");
			printf("\t                      --------------------------------------------------------------------------------------\n\n");
			printf("\n\t请输入操作序号：");
			n = -1;
			scan(&n);
			if (n == 0)
				return;
			else if (n == 1)
				head = GoodsFilter(head);
			else if (n == 2) {
				printf("\t输入关键词(直接回车返回)：");
				gets(keystr);
				if (keystr[0] == '\0') {
					continue;
				}
				head = GoodsSearch(head, keystr);
			}
			else if (n == 3) {
				releaseGoods(head);
				head = copyGoods(headGoods);
			}
			else if (n == 4)
				head = ascendSortGoodsByPrice(head);
			else if (n == 5)
				head = decendSortGoodsBySales(head);
			else if (n == 6)
			{
				printf("\t添加商品至购物车请输入商品编号[编号]  返回[0]\n\t");
				scan(&goodsnum);
				if (goodsnum == 0)
					continue;
				else {
					pNow = headGoods;
					while (pNow != NULL && pNow->goodsNum != goodsnum)
						pNow = pNow->next;
					if (pNow == NULL) {
						printf("\t商品编号不存在，请检查输入编号！\n\t");
						Sleep(1000);
					}
					else
						GoodsTrolleyAdd(pNow);
				}
			}
			else {
				printf("\t输入代码错误\n\t");
				Sleep(1000);
			}
		}
	}
}

//商品筛选界面
static struct CGoods* GoodsFilter(struct CGoods* head) {
	float minprice = -1, maxprice = -1;
	int n = -1;//保存选择号码
	char kind[20];//暂存用户输入的商品类别
	struct CGoods* pNow = head;//pNow在原列表遍历寻找符合条件的商品
	struct CGoods* pNewhead = NULL;//链表新头
	struct CGoods* pNew1 = NULL;//用于链接新商品和链表的后指针
	//输入最低最高价
	printf("\t进入价格筛选[1]  返回[0]  输入其他数跳过价格筛选\n\t");
	scan(&n);
	if (n == 0)
		return head;
	else if (n == 1)
	{
		while (1)
		{
			while (1)//输入最高价，排除负数
			{
				printf("\t请输入想要的每瓶最高价(大于0)： ");
				scanf_s("%f", &maxprice);
				getchar();
				if (maxprice > 0)
					break;
				else
					printf("\t输入最高价非法\n\t");
			}
			while (1)//输入最低价，排除-1以外的负数
			{
				printf("\t请输入想要的每瓶最低价(大于等于0)：");
				scanf_s("%f", &minprice);
				getchar();
				if (minprice >= 0)
					break;
				else
					printf("\t输入最低价非法\n\t");
			}
			if (minprice > maxprice)//最低价大于最高价，重新输入
			{
				printf("\t最低价不可大于最高价\n\t");
				continue;
			}
			else
				break;
		}
	}
	//输入商品种类
	printf("\t请输入酒水种类(0返回)(1不限)：");
	gets(kind);
	if (strlen(kind) == 1 && kind[0] == '0')
		return head;
	system("cls");
	while (pNow != NULL) {
		if (((n != 1) || ((pNow->price >= minprice) && (pNow->price <= maxprice))) && ((!strcmp(kind, "1")) || (!strcmp(kind, pNow->kind)))) {
			//满足筛选条件进入链接部分
			if (pNewhead == NULL) {//链表为空的特殊情况
				pNewhead = pNow;
				pNew1 = pNewhead;
			}
			else {
				pNew1->next = pNow;
				pNew1 = pNow;
			}
		}
		pNow = pNow->next;
	}
	//筛选需要新建链表
	if (pNewhead == NULL) {
		printf("\t没有符合条件的商品，即将返回\n\t");
		Sleep(1000);
		return head;//筛选结果无商品则返回原商品链表
	}
	else {
		pNew1->next = NULL;
		return pNewhead;//返回筛选链表
	}
}

//按照商品价格递增排序
static struct CGoods* ascendSortGoodsByPrice(struct CGoods* head) {
	if (head == NULL || head->next == NULL)return head;//空链表和单数据链表的情况排除
	struct CGoods* p, * p0, * r, * r0, * q;
	p = p0 = r = r0 = q = NULL;
	int flag = 1;
	while (flag) {
		flag = 0;
		r = head;
		p = head->next;
		if (head->price > head->next->price) {
			head->next = p->next;
			p->next = head;
			head = p;
			p = head->next;
			r = head;
			flag = 1;
		}
		while (p->next != NULL) {
			if (r->next->price > p->next->price) {
				r->next = p->next;
				p->next = p->next->next;
				r->next->next = p;
				flag = 1;
				r = r->next;
			}
			else {
				p = p->next;
				r = r->next;
			}
		}
	}

	return head;
}
//按照商品销量递减排序
static struct CGoods* decendSortGoodsBySales(struct CGoods* head) {
	if (head == NULL || head->next == NULL)return head;//空链表和单数据链表的情况排除
	struct CGoods* p, * p0, * r, * r0, * q;
	p = p0 = r = r0 = q = NULL;
	int flag = 1;
	while (flag) {
		flag = 0;
		r = head;
		p = head->next;
		if (head->sales < head->next->sales) {
			head->next = p->next;
			p->next = head;
			head = p;
			p = head->next;
			r = head;
			flag = 1;
		}
		while (p->next != NULL) {
			if (r->next->sales < p->next->sales) {
				r->next = p->next;
				p->next = p->next->next;
				r->next->next = p;
				flag = 1;
				r = r->next;
			}
			else {
				p = p->next;
				r = r->next;
			}
		}
	}
	return head;
}

//新增商品
static void GoodsAdd() {
	int n = -1;//保存用户选择代码
	char brand[40] = { '\0' };//暂存要添加的商品名称
	struct CGoods* pNow = headGoods;
	printf("\t请输入要添加的商品名称，返回[0]\n\t");
	gets(brand);
	if (strlen(brand) == 1 && brand[0] == '0')//比对是否为0
		return;
	while (pNow != NULL && strcmp(pNow->brand, brand))
		pNow = pNow->next;
	if (pNow == NULL) {
		printf("\t是否确认添加%s至商品列表，确认请按[1]，输入其他任意键返回\n\t", brand);//防手误的缓冲与提示
		scan(&n);
		if (n == 1) {
			struct CGoods* pNew = (struct CGoods*)malloc(sizeof(struct CGoods));
			pNew->next = NULL;
			if (headGoods == NULL)
				headGoods = tailGoods = pNew;
			else {
				tailGoods->next = pNew;
				tailGoods = pNew;
			}
			//初始值设定
			int max = 0;
			struct CGoods* p = headGoods;
			while (p != NULL) {
				if (p->goodsNum > max)
					max = p->goodsNum;
				p = p->next;
			}
			pNew->goodsNum = max + 1;//依次自动初始化商品信息
			strcpy_s(pNew->brand, 40, brand);
			strcpy_s(pNew->kind, 20, "Not set");
			pNew->netContent = 0;
			pNew->boxesNumber = 1;
			pNew->concentration = 0;
			pNew->price = 0;
			pNew->stock = 0;
			pNew->sales = 0;
			system("cls");
			printf("\t编号               商品名称               商品类别   酒精浓度   每瓶净含量(ml)  每箱瓶数  价格(元)/瓶      库存(箱+瓶)       销量(瓶)\n");
			printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
			GoodsSinglePrint(pNew);
			GoodsChange(pNew, 5);//依次人工初始化商品信息

			system("cls");
			printf("\t编号               商品名称               商品类别   酒精浓度   每瓶净含量(ml)  每箱瓶数  价格(元)/瓶      库存(箱+瓶)       销量(瓶)\n");
			printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
			GoodsSinglePrint(pNew);
			GoodsChange(pNew, 1);

			system("cls");
			printf("\t编号               商品名称               商品类别   酒精浓度   每瓶净含量(ml)  每箱瓶数  价格(元)/瓶      库存(箱+瓶)       销量(瓶)\n");
			printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
			GoodsSinglePrint(pNew);
			GoodsChange(pNew, 3);

			system("cls");
			printf("\t编号               商品名称               商品类别   酒精浓度   每瓶净含量(ml)  每箱瓶数  价格(元)/瓶      库存(箱+瓶)       销量(瓶)\n");
			printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
			GoodsSinglePrint(pNew);
			GoodsChange(pNew, 4);

			system("cls");
			printf("\t编号               商品名称               商品类别   酒精浓度   每瓶净含量(ml)  每箱瓶数  价格(元)/瓶      库存(箱+瓶)       销量(瓶)\n");
			printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
			GoodsSinglePrint(pNew);
			GoodsChange(pNew, 6);

			SaveGoodsData();//保存商品数据到文件
			printf("\t添加商品成功\n");
			Sleep(500);
			return;
			//添加成功后回到添加界面继续添加
		}
		else
			return;
	}
	else {
		printf("\t商品已存在，请检查输入商品！\n\t");
		Sleep(1000);
	}
}

//删除商品，删除所给的指针的下一个商品
static void GoodsDelete(struct CGoods* pNow) {
	struct CGoods* p = pNow->next;
	pNow->next = p->next;
	if (headGoods == p)//如果删除的是头节点
		headGoods = p->next;
	else if (p->next == NULL)//如果删除的是尾节点
		tailGoods = pNow;
	free(p);
	printf("删除成功");
}

//修改商品信息，1改库存stock，2改名brand，3改类别kind，4改每瓶净含量，5改每箱瓶数，6改每箱价格，7改销量
static void GoodsChange(struct CGoods* pNow, int slectNum) {
	int n, unit;//n保存选择代码，unit保存库存的操作单位
	float fudianshu;//专用于保存浮点数据
	char cache[40];//对用户输入进行缓存
	//各分支操作不同数据，根循环在操作成功后break，输入错误则continue
	if (slectNum == 1) {
		while (1)
		{
			printf("%s现在的库存为%d箱余%d瓶\n\t", pNow->brand, pNow->stock / pNow->boxesNumber, pNow->stock % pNow->boxesNumber);
			printf("添加[1]  减少[2]  修改[3]  返回[0]\n\t");
			n = -1;
			scan(&n);
			if (n == 0)
				return;
			if (n == 1) {
				printf("\t请输入操作的单位 箱[1] 或 瓶[2]\n\t");
				unit = -1;
				scan(&unit);
				if ((unit != 1) && (unit != 2)) {//输入只能为1或2
					printf("\t输入单位错误\n\t");
					Sleep(500);
					continue;
				}
				printf("\t请输入库存添加的数量：");
				n = -1;
				scan(&n);
				if (n < 0) {
					printf("\t输入非法\n\t");//数量需为正数
					Sleep(500);
					continue;
				}
				if (unit == 1)pNow->stock += n * pNow->boxesNumber;
				else pNow->stock += n;
				break;
			}
			else if (n == 2) {
				printf("\t请输入操作的单位 箱[1] 或 瓶[2]\n\t");
				unit = -1;
				scan(&unit);
				if ((unit != 1) && (unit != 2)) {
					printf("\t输入单位错误\n\t");
					Sleep(500);
					continue;
				}
				printf("\t请输入库存减少的数量：");
				n = -1;
				scan(&n);
				if (unit == 1) {//以箱为单位操作
					if (pNow->stock < n * pNow->boxesNumber) {
						printf("\t库存不足\n\t");//减少数量需有足够库存以供操作
						Sleep(500);
						continue;
					}
					else
						pNow->stock -= n * pNow->boxesNumber;
				}
				else {//以瓶为单位操作
					if (pNow->stock < n) {
						printf("\t库存不足\n\t");
						Sleep(500);
						continue;
					}
					else pNow->stock -= n;
				}
				break;
			}
			else if (n == 3) {
				printf("\t请输入操作的单位 箱[1] 或 瓶[2]\n\t");
				unit = -1;
				scan(&unit);
				if ((unit != 1) && (unit != 2)) {
					printf("\t输入单位错误\n\t");
					Sleep(500);
					continue;
				}
				printf("\t请输入库存的新数量：");
				n = -1;
				scan(&n);
				if (n <= 0) {
					printf("\t数量非法\n\t");//排除负数
					Sleep(500);
					continue;
				}
				if (unit == 1)
					pNow->stock = n * pNow->boxesNumber;
				else
					pNow->stock = n;
				break;
			}
			else {
				printf("\t输入错误代码！\n\t");
				Sleep(500);
				continue;
			}
		}
	}
	else if (slectNum == 2) {
		printf("请输入新商品名(0返回)：");
		gets(cache);
		if (strlen(cache) == 1 && cache[0] == '0')
			return;
		strcpy_s(pNow->brand, 40, cache);
	}
	else if (slectNum == 3) {
		printf("请输入新商品类别(0返回)：");
		gets(cache);
		if ((strlen(cache) == 1) && (cache[0] == '0'))return;
		strcpy_s(pNow->kind, 20, cache);
		while (1)
		{
			printf("\t如含有酒精请输入度数，不含则为0：");
			n = -1;
			scan(&n);
			if ((n < 0) || (n >= 100)) {
				printf("\t输入非法\n");
				Sleep(500);
				continue;
			}
			else {
				pNow->concentration = n;
				break;
			}
		}
	}
	else if (slectNum == 4) {
		while (1)
		{
			printf("请输入新的每瓶净含量(ml)(0返回)：");
			n = -1;
			scan(&n);
			if (n == 0)return;
			if (n < 0) {
				printf("\t净含量输入非法\n\t");
				Sleep(500);
				continue;
			}
			else {
				pNow->netContent = n;
				break;
			}
		}
	}
	else if (slectNum == 5) {
		while (1)
		{
			printf("请输入新的每箱瓶数(0返回)：");
			n = -1;
			scan(&n);
			if (n == 0)return;
			if (n < 0) {
				printf("\t每箱瓶数输入非法\n\t");
				Sleep(500);
				continue;
			}
			else {
				pNow->boxesNumber = n;
				break;
			}
		}
	}
	else if (slectNum == 6) {
		while (1)
		{
			printf("请输入新的每瓶价格(0返回)：");
			scanf_s("%f", &fudianshu);
			getchar();
			if ((fudianshu <= 0.000001) && (fudianshu >= -0.000001))return;
			if (fudianshu < 0) {
				printf("\t每瓶价格不可为负数\n\t");
				Sleep(500);
				continue;
			}
			else {
				pNow->price = fudianshu;
				break;
			}
		}
	}
	else if (slectNum == 7) {
		while (1)
		{
			printf("请输入修改后的销量(0返回)：");
			n = -1;
			scan(&n);
			if (n == 0)return;
			if (n < 0) {
				printf("\t销量不可为负数\n\t");
				Sleep(500);
				continue;
			}
			else {
				pNow->sales = n;
				break;
			}
		}
	}
	printf("\t数据操作成功\n\n");
	Sleep(500);
}