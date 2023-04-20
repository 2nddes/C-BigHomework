#define _CRT_SECURE_NO_WARNINGS
#include "head.h"
#include "trolley.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void GradeChange();
double GetPriceAfterActivaiyDiscountAndFullDiscount(double totalPrice);

void InformationChange(int numPurchaseGoods, int goodsSerialNumber) {
	//goodsSerialNumber为商品编号 
	//numPurchaseGoods为购买的商品数量

	struct CGoods* pGood = headGoods; struct CGoods* pGood0 = NULL;
	struct CTrolley* pTro = headTrolley; struct CTrolley* pTro0 = NULL;
	struct CAccount* pAccou = headAccount; struct CAccount* pAccou0 = NULL;
	struct COrder* pOrder = headOrder; struct COrder* pOrder0 = NULL;
	//需要改变的量 用户的总销售额,用户会员等级，购物车商品，商品库存,商品销量,新增订单以及可能的赠品订单
	while (pTro != NULL) {
		if (pTro->goodsNumber == goodsSerialNumber && strcmp(pTro->userName, pLoggedInUser->userName) == 0) break;
		pTro0 = pTro;
		pTro = pTro->next;
	}
	while (pGood != NULL && pGood->goodsNum != goodsSerialNumber) {
		pGood0 = pGood;
		pGood = pGood->next;
	}

	//用户等级、销售更改
	float totalCost = (float)(TotalCostMoney(goodsSerialNumber, pTro));
	pLoggedInUser->totalAmount += totalCost;
	////GradeChange(pLoggedInUser);

	//商品库存、销量更改
	pGood->stock -= pTro->trolleyGoodsNumber;
	pGood->sales += pTro->trolleyGoodsNumber;


	//新增购物商品订单以及赠品订单
	OrderAdd(totalCost, goodsSerialNumber, pTro->trolleyGoodsNumber);

	//在购物车中删除该商品
	GoodsTrolleyDeleteNoRemind(goodsSerialNumber);


	//将修改后的各项信息存储到文件当中去

	SaveGoodsTrolleyData(headTrolley);
	SaveGoodsData(headGoods);
	SaveOrderData(headOrder);
	SaveAccountData(headAccount);

}

//购物车界面
void GoodsTrolley() {
	system("cls");
	struct CTrolley* pTro = headTrolley;
	struct CGoods* pGood = NULL;

	while (1)
	{
		system("cls");
		pTro = headTrolley;

		printf("\n\t*******************************************************   购物车   *********************************************\n\n");
		char* arrName[8] = { "商品编号","酒水名称","酒水类别","酒精浓度","酒水净含量","酒水单价(瓶)","酒水数量","酒水库存" };
		printf("\n\t%8s %16s %12s %10s %12s %12s %17s %17s\n\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5], arrName[6], arrName[7]);

		int numTrolleyGoods = 0;//用于记录该用户购物车中商品总数量

		while (pTro != NULL) {

			//所有人的购物车信息都存储于同一个文件中，先依据用户名找到当前用户购物车中的所有商品信息
			if (strcmp(pTro->userName, pLoggedInUser->userName) == 0) {
				//找到了当前用户的一个商品，现要将该商品信息输出
				pGood = headGoods;
				int stateFindGoods = 0;//是否能够在现今商铺中依据商品名称找到该商品
				while (pGood != NULL) {
					//输出购物车中商品信息
					if (strcmp(pGood->brand, pTro->brand) == 0) {
						stateFindGoods = 1;
						numTrolleyGoods++;
						printf("\n\t%-8d %16s %12s %9d° %12d %12.2f",
							pGood->goodsNum, pGood->brand, pGood->kind, pGood->concentration, pGood->netContent
							, pGood->price);
						printf("%8d箱余%04d瓶", pTro->trolleyGoodsNumber / pGood->boxesNumber, pTro->trolleyGoodsNumber % pGood->boxesNumber);
						printf("%8d箱余%04d瓶\n", pGood->stock / pGood->boxesNumber, pGood->stock % pGood->boxesNumber);
					}
					pGood = pGood->next;
				}
				if (pGood == NULL && stateFindGoods == 0) {
					printf("\n\t您的购物车中名称为\"%s\"的商品已被下架！请等待商品重新上架或者删除该商品。\n\t输入0删除该商品，输入其他数字继续浏览您购物车中其它商品：", pTro->brand);
					int n1 = 0;
					scan(&n1);
					if (n1 == 0) {
						struct CTrolley* p = pTro->next;
						GoodsTrolleyDeleteNoRemind(pTro->goodsNumber);
						pTro = p;
						numTrolleyGoods--;
					}
					else {

						pTro = pTro->next;
					}
				}
				else pTro = pTro->next;
			}
			else pTro = pTro->next;

		}


		if (numTrolleyGoods == 0) {
			printf("\n\n\t您的购物车中暂无商品！\n");
		}
		else {
			printf("\n\n\t目前，您的购物车中共有%d件商品\n", numTrolleyGoods);
		}


		printf("\n\t----------------------------------------------------------------------------------------------------------------\n");
		printf("\n\t-----------------------------------------------------操  作  菜  单---------------------------------------------\n\n");
		printf("\t               ********************-0--返回                              返回--0-********************\n\n");
		printf("\t               ********************-1--一键删除商品               一键购买商品--2-********************\n\n");
		printf("\t               ********************-3--更改商品数量               逐个购买商品--3-********************\n\n");
		printf("\t               -------------------------------------------------------------------------------------\n\n");
		printf("\n\t请输入操作序号：");

		int n1;
		n1 = -1;
		scan(&n1);

		if (n1 == 0) {
			system("cls");
			return;
		}
		else if (n1 == 1) {
			GoodsTrolleyClear();
			system("cls");
			break;
		}
		else if (n1 == 2) {
			TotalGoodsTrolleyBuy();

		}
		else if (n1 == 3) {
			int n2 = -1;
			while (1) {
				n2 = -1;
				printf("\n\t请输入商品编号,输入[0]可返回：");
				scan(&n2);
				if (n2 == 0) {
					system("cls");
					break;
				}
				else if (n2 >= 1 && n2 <= 100000) {
					int state = 0;
					struct CTrolley* p = headTrolley;
					while (p != NULL) {
						if (p->goodsNumber == n2 && strcmp(p->userName, pLoggedInUser->userName) == 0) {
							state = 1;
						}
						p = p->next;
					}
					if (state == 0) {
						printf("\n\t您的购物车中不含您输入的编号对应的商品，请按任意键后重新输入！");
						getchar();
					}
					else {
						int num0 = -1;
						while (1) {
							num0 = -1;
							printf("\n\t输入[1]下单，输入[2]删除商品，输入[3]修改商品数量，输入[0]返回,输入其他类型字符无效:");
							scan(&num0);
							if (num0 == 1) {
								GoodsTrolleyBuy(n2);
								break;
							}
							else if (num0 == 2) {
								GoodsTrolleyDelete(n2);
								break;
							}
							else if (num0 == 3) {
								GoodsTrolleyChange(n2);
								break;
							}
							else if (num0 == 0) {
								break;
							}
							else {}
						}
					}
				}
				else {
					printf("\n\t编号不合规定，请重新输入！\n");
				}
			}

		}
	}
}

/******-----------------------------------------------------------*/
/*购物车需求*/
 /******-----------------------------------------------------------*/
//--添加商品至购物车
void GoodsTrolleyAdd(struct CGoods* pNow) {
	struct CTrolley* pTro = NULL;
	while (pTro == NULL) {
		pTro = (struct CTrolley*)malloc(sizeof(struct CTrolley));
	}//保证ptro不会指向NULL
	pTro->next = NULL;
	strcpy(pTro->userName, pLoggedInUser->userName);
	strcpy(pTro->brand, pNow->brand);
	pTro->goodsNumber = pNow->goodsNum;
	pTro->boxesNumber = pNow->boxesNumber;
	pTro->trolleyGoodsNumber = 0;

	int numDW = -1;//单位   箱为单位添加商品至购物车请输入1，需要以瓶为单位添加商品至购物车请输入2，按0可返回
	while (1) {
		numDW = -1;
		printf("\n\t需要以箱为单位添加商品至购物车请输入1，需要以瓶为单位添加商品至购物车请输入2，按0可返回：");
		scan(&numDW);
		if (numDW == 1 || numDW == 2) {
			break;
		}
		if (numDW == 0) {
			system("cls");
			return;
		}
		else {
			printf("\n\t您输入的代码有误，请重新输入！");
		}
	}

	int num = -1;
	while (1) {
		num = -1;
		printf("\n\t请输入需要添加到购物车的商品数量（大于等于1且小于等于1000）,按0可返回：");
		scan(&num);
		if (num >= 1 && num <= 1000) {
			if (numDW == 1) {
				pTro->trolleyGoodsNumber = num * (pNow->boxesNumber);
			}
			else if (numDW == 2) { pTro->trolleyGoodsNumber = num; }
			break;
		}
		if (num == 0) {
			system("cls");
			return;
		}
		else {
			printf("\n\t您输入的代码有误，请重新输入！\n");
		}
	}

	//如果购物车中已有该商品，商品数量在原来的基础上继续添加
	struct CTrolley* p = headTrolley;
	struct CTrolley* p0 = NULL;
	if (p == NULL) {
		headTrolley = pTro;
		pTro->next = NULL;
	}
	else {
		int stateExisted = 0;
		while (p != NULL) {
			if (strcmp(p->brand, pTro->brand) == 0 && strcmp(p->userName, pLoggedInUser->userName) == 0) {
				stateExisted = 1;
				p->trolleyGoodsNumber += pTro->trolleyGoodsNumber;
				break;
			}
			p0 = p;
			p = p->next;
		}
		if (stateExisted == 0) {
			p0->next = pTro;
			pTro->next = NULL;
		}
	}

	SaveGoodsTrolleyData();

	printf("\n\t添加成功，按任意键键返回\n");
	getchar();
	system("cls");

	return;

}
//--删除购物车中的某件商品
void GoodsTrolleyDeleteNoRemind(int goodsSerialNumber) {//将商品从购物车中删除
	struct CTrolley* p = headTrolley;
	struct CTrolley* p0 = NULL;
	while (p != NULL) {
		if (p->goodsNumber == goodsSerialNumber && strcmp(p->userName, pLoggedInUser->userName) == 0) {
			if (p0 == NULL) {
				if (headTrolley == p) {

				}
				headTrolley = headTrolley->next;
				free(p);
				p = headTrolley;
			}
			else {
				p0->next = p->next;
				free(p);
				p = p0->next;
				break;
			}
		}
		else {
			p0 = p;
			p = p->next;
		}
	}
	SaveGoodsTrolleyData(headTrolley);

}
void GoodsTrolleyDelete(int goodsSerialNumber) {//将商品从购物车中删除
	int num_ = -1;
	while (1) {
		num_ = -1;
		printf("\n\t您确定要删除该商品吗？按[1]确认，按[0]返回: ");
		scanf_s("%d", &num_);
		getchar();
		if (num_ == 1) {
			struct CTrolley* p = headTrolley;
			struct CTrolley* p0 = NULL;
			while (p != NULL) {
				if (p->goodsNumber == goodsSerialNumber && strcmp(p->userName, pLoggedInUser->userName) == 0) {
					if (p0 == NULL) {
						headTrolley = headTrolley->next;
						free(p);
						p = headTrolley;
					}
					else {
						p0->next = p->next;
						free(p);
						p = p0->next;
					}
				}
				else {
					p0 = p;
					p = p->next;
				}
			}
			SaveGoodsTrolleyData(headTrolley);
			printf("\n\t成功移除该商品！\n");
			break;
		}
		else if (num_ == 0) {
			//releaseTrolley(headTrolley);
			return;
		}
		else {
			printf("\n\t您输入的代码有误，请重新输入！\n");
		}
	}

}
//--修改购物车中商品数量
void GoodsTrolleyChange(int goodsSerialNumber) {
	//修改购物车中的商品数量

	//修改商品信息的主要思路：删除原节点，将新节点从链头插入（这样新修改后的商品就会显示在第一排）

	struct CTrolley* pTro = headTrolley;//pTro记录该商品在购物车中的地址，是之后需要被删除的商品

	while (pTro != NULL) {
		if (pTro->goodsNumber == goodsSerialNumber && strcmp(pTro->userName, pLoggedInUser->userName) == 0) {
			break;
		}
		pTro = pTro->next;
	}
	if (pTro == NULL) {
		printf("\n\t您输入的商品编号有误！\n");
		return;
	}

	//pNew是之后需要被新加的购物车商品节点
	struct CTrolley* pNew = (struct CTrolley*)malloc(sizeof(struct CTrolley));
	*pNew = *pTro;

	while (1)
	{
		int indexA = 0;
		printf("\n\t增加商品数量[1], 减少商品数量[2], 删除商品[3], 返回[0]:");
		scan(&indexA);
		if (indexA == 0) {
			system("cls");
			return;
		}
		else if (indexA == 3) {
			GoodsTrolleyDelete(goodsSerialNumber);
			system("cls");
			break;
		}
		else if (indexA == 1) {
			int numXiang = 0;
			int numPing = 0;
			while (1) {
				printf("\n\t提示：当前商品1箱含%d瓶酒水\n", pTro->boxesNumber);
				printf("\n\t请依次输入您要增加的箱数和瓶数（均不超过10000），数据之间以空格隔开：");

				int i = 0;
				int valid = 0;
				int num[2];
				for (i = 0; i < 2; i++) {
					valid = 0;
					while (!valid) {
						if (scanf("%d", &num[i]) != 1) {
							printf("\t输入无效，请重新输入：");
							while (getchar() != '\n');
						}
						else {
							valid = 1;

						}
					}
				}

				numXiang = num[0];
				numPing = num[1];
				if (numXiang >= 0 && numXiang <= 10000 && numPing >= 0 && numPing <= 10000) {
					break;
				}
				else if (numXiang > 10000) {
					printf("\n\t输入的商品箱数过多，请重新输入！\n");
				}
				else {
					printf("\n\t输入的商品箱数格式错误，请重新输入！\n");
				}
				if (numPing >= 0 && numPing <= 10000);
				else if (numPing > 10000) {
					printf("\n\t输入的商品瓶数过多，请重新输入！\n");
				}
				else {
					printf("\n\t输入的商品瓶数格式错误，请重新输入！\n");
				}
				getchar();
			}
			pNew->trolleyGoodsNumber += (numPing + numXiang * pTro->boxesNumber);

			GoodsTrolleyDeleteNoRemind(goodsSerialNumber);//删除旧商品
			pNew->next = headTrolley;
			headTrolley = pNew;//添加新商品

			SaveGoodsTrolleyData(headTrolley);
			printf("\n\t您的购物车当前含有该商品%d箱%d瓶。按任意键返回。\n", pNew->trolleyGoodsNumber / pNew->boxesNumber, pNew->trolleyGoodsNumber % pNew->boxesNumber);
			getchar();
			getchar();
			system("cls");
			return;
		}
		else if (indexA == 2) {
			int numXiang = 0;
			int numPing = 0;
			while (1) {
				printf("\n\t提示：当前商品1箱含%d瓶酒水\n", pTro->boxesNumber);
				printf("\n\t请依次输入您要减少的箱数和瓶数（均不超过10000），数据之间以空格隔开：");

				int i = 0;
				int valid = 0;
				int num[2];
				for (i = 0; i < 2; i++) {
					valid = 0;
					while (!valid) {
						if (scanf("%d", &num[i]) != 1) {
							printf("\t输入无效，请重新输入：");
							while (getchar() != '\n');
						}
						else {
							valid = 1;

						}
					}
				}

				numXiang = num[0];
				numPing = num[1];
				if (numXiang >= 0 && numXiang <= 100000 && numPing >= 0 && numPing <= 100000) {
					break;
				}
				else if (numXiang > 100000) {
					printf("\n\t输入的商品箱数过多，请重新输入！\n");
				}
				else {
					printf("\n\t输入的商品箱数格式错误，请重新输入！\n");
				}
				if (numPing >= 0 && numPing <= 100000);
				else if (numPing > 100000) {
					printf("\n\t输入的商品瓶数过多，请重新输入！\n");
				}
				else {
					printf("\n\t输入的商品瓶数格式错误，请重新输入！\n");
				}
			}
			if (pNew->trolleyGoodsNumber >= (numPing + numXiang * pTro->boxesNumber)) {
				pNew->trolleyGoodsNumber -= (numPing + numXiang * pTro->boxesNumber);
			}
			else {
				int n2 = 0;
				while (1) {
					printf("\n\t您的购物车中该商品总数小于您需要减少的数量！输入 1 可删除该商品，输入 0 可取消减少商品数量的操作并返回:");
					scan(&n2);
					if (n2 == 1) {
						GoodsTrolleyDeleteNoRemind(goodsSerialNumber);
						//system("cls");
						return;
					}
					else if (n2 == 0) {
						//system("cls");
						return;
					}
					else {
						printf("\n\t输入错误，请重新输入！\n");
					}
				}
			}

			GoodsTrolleyDeleteNoRemind(goodsSerialNumber);//删除旧商品
			pNew->next = headTrolley;
			headTrolley = pNew;//添加新商品

			SaveGoodsTrolleyData(headTrolley);
			printf("\n\t您的购物车当前含有该商品%d箱%d瓶。按任意键返回。\n", pNew->trolleyGoodsNumber / pNew->boxesNumber, pNew->trolleyGoodsNumber % pNew->boxesNumber);
			getchar();
			getchar();
			//	system("cls");
			return;
		}
	}
}
//--清除购物车中所有商品
void GoodsTrolleyClear() {//清空购物车
	//LoadTrolleyData();
	struct CTrolley* pTro = headTrolley;
	struct CTrolley* pTro_ = NULL;//pTro的前置节点
	while (pTro != NULL) {
		pTro_ = pTro;
		pTro = pTro->next;
		if (strcmp(pTro_->userName, pLoggedInUser->userName) == 0) {
			GoodsTrolleyDeleteNoRemind(pTro_->goodsNumber);
		}
	}
	SaveGoodsTrolleyData(headTrolley);
	printf("\n\t已删除您的购物车中所有商品！输入任意字符返回！\n");
	getchar();
}

/******-----------------------------------------------------------*/
/*下单*/
/******-----------------------------------------------------------*/
//--购买购物车中某件商品


void GoodsTrolleyBuy(int goodsSerialNumber) {//购买购物车中的商品
	//LoadTrolleyData();
	struct CTrolley* pTro = headTrolley;
	struct CGoods* pGood = headGoods;
	while (pTro != NULL) {
		if (pTro->goodsNumber == goodsSerialNumber && strcmp(pTro->userName, pLoggedInUser->userName) == 0) {
			pGood = headGoods;
			while (pGood != NULL) {
				if (pGood->goodsNum == goodsSerialNumber) {
					if (pGood->stock < pTro->trolleyGoodsNumber) {
						printf("\n\t编号为%d的商品库存不足！请更改购物车商品数量后再下单！输入0可返回，输入其他正整数可前往更改商品数量", pTro->goodsNumber);
						int n1;
						scan(&n1);
						if (n1 == 0) return;
						else {
							GoodsTrolleyChange(goodsSerialNumber);
							return;
						}
					}
					else {
						double cost = TotalCostMoney(goodsSerialNumber, pTro);
						printf("\n\t提示：当前商品1箱含%d瓶酒水\n", pTro->boxesNumber);
						printf("\n\t购买该商品共需花费 %.2lf 元，确认购买吗？\n", cost);
						int num = -1;
						while (1) {
							num = -1;
							printf("\n\t输入1确认购买，输入0返回:");
							scan(&num);
							if (num == 0) {
								return;
							}
							else if (num == 1) {
								if (ModeOfPayment() == 0) {
									printf("\n\t支付取消，输入任意字符可返回。\n");
									printf("\n\t");
									getchar();
									return;
								}
								else {
									InformationChange(pTro->trolleyGoodsNumber, pTro->goodsNumber);//更改因商品购买造成的信息改变
									printf("\n\t您已成功购买该商品！\n");
									printf("\n\t");
									getchar();
									return;
								}
							}
							else {
								printf("\n\t您输入的代码有误，请重新输入！\n");
							}
						}
					}
				}
				pGood = pGood->next;
			}
		}
		pTro = pTro->next;
		if (pGood == NULL) {
			printf("\n\t该商品已下架！\n");
			GoodsTrolleyDelete(pTro->goodsNumber);
			return;
		}
	}
}
void GoodsTrolleyBuyNoRemind(struct CTrolley* pTro) {
	struct CGoods* pGood = headGoods;
	while (pGood != NULL && (pGood->goodsNum != pTro->goodsNumber || pGood->stock < pTro->trolleyGoodsNumber)) {
		pGood = pGood->next;
	}
	if (pGood == NULL);//该商品已被下架
	else {
		InformationChange(pTro->trolleyGoodsNumber, pGood->goodsNum);
	}

}
//--购买购物车中所有商品
void TotalGoodsTrolleyBuy() {
	double cost = 0;
	struct CTrolley* pTro = headTrolley;
	while (pTro != NULL) {
		if (strcmp(pTro->userName, pLoggedInUser->userName) == 0) {
			AllGoodsTotalCost(pTro->goodsNumber, &cost);
		}
		pTro = pTro->next;
	}
	if (fabs(cost) < 1e-6) {
		printf("\n\t您的购物车中暂无库存足够的商品！按任意键返回。\n");
		getchar();
		return;
	}

	printf("\n\t购买购物车中所有库存充足的商品共需花费%.2lf元\n", cost);
	printf("\n\t输入[1]确认购买，输入[0]取消购买:");
	int n1 = -1;
	while (1) {
		n1 = -1;
		scan(&n1);
		if (n1 == 1) {
			if (ModeOfPayment() == 0) {
				printf("\n\t您已取消购买！\n");
				getchar();
				return;
			}
			else {
				printf("\n\t购买成功！\n");

				pTro = headTrolley;
				struct CTrolley* pTro_ = NULL;
				while (pTro != NULL) {
					pTro_ = pTro;
					pTro = pTro->next;
					if (strcmp(pTro_->userName, pLoggedInUser->userName) == 0) {
						GoodsTrolleyBuyNoRemind(pTro_);
					}
				}

				if (strcmp(pTro_->userName, pLoggedInUser->userName) == 0) {
					GoodsTrolleyBuyNoRemind(pTro_);
				}

				SaveGoodsTrolleyData();
				getchar();

				return;
			}
		}
		else if (n1 == 0) {
			return;
		}
		else {
			printf("\t输入有误，请重新输入！\n");
		}
	}
}
//支付金额与支付方式
//--计算购物车中某件商品花费总金额
double TotalCostMoney(int goodsSerialNumber, struct CTrolley* pTro) {

	struct CGoods* pGood = headGoods;
	while (pGood != NULL && pGood->goodsNum != goodsSerialNumber) {
		pGood = pGood->next;
	}
	double allMoney = -1;
	allMoney = pLoggedInUser->discount * 0.1 * GetPriceAfterActivaiyDiscountAndFullDiscount(pGood->price * pTro->trolleyGoodsNumber * pLoggedInUser->discount / 10);

	return allMoney;
}
void AllGoodsTotalCost(int goodsSerialNumber, double* totalCost) {//购买购物车中的商品
	struct CTrolley* pTro = headTrolley;
	struct CGoods* pGood = headGoods;
	while (pTro != NULL) {
		if (pTro->goodsNumber == goodsSerialNumber && strcmp(pTro->userName, pLoggedInUser->userName) == 0) {
			pGood = headGoods;
			while (pGood != NULL) {
				if (pGood->goodsNum == goodsSerialNumber) {
					if (pGood->stock < pTro->trolleyGoodsNumber) {}
					else {
						*totalCost += TotalCostMoney(goodsSerialNumber, pTro);
					}
				}
				pGood = pGood->next;
			}
		}
		pTro = pTro->next;
	}
}
//--选择支付方式
int ModeOfPayment() {
	int num_ = -1;
	while (1) {
		printf("\n\t请选择您的付款方式 输入1使用支付宝支付，输入2使用微信支付,输入0返回：");
		num_ = -1;
		scan(&num_);
		if (num_ == 0) {
			return 0;
		}
		if (num_ == 1) {
			printf("\n\t使用支付宝付款成功！\n");
			return 1;
		}
		if (num_ == 2) {
			printf("\n\t使用微信付款成功！\n");
			return 1;
		}
		else {
			printf("\n\t您输入的代码有误，请重新输入！\n");
		}
	}
}