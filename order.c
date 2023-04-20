#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include"order.h"
#include"head.h"

//新增订单
void OrderAdd(double totalCost, int goodsNum, int nums) {
	//指向新添加的订单
	struct COrder* pNew = (struct COrder*)malloc(sizeof(struct COrder));
	pNew->next = NULL;
	if (headOrder == NULL) {
		headOrder = pNew;
		tailOrder = pNew;
	}
	else {
		pNew->next = headOrder;
		headOrder = pNew;
	}
	//订单编号初始化
	int max = 0;
	struct COrder* p = headOrder;
	while (p != NULL) {
		if (p->orderNum > max)
			max = p->orderNum;
		p = p->next;
	}
	//指向被购买的商品
	struct CGoods* pNow = headGoods;
	while (pNow->goodsNum != goodsNum) {
		pNow = pNow->next;
	}
	//订单的商品名，商品数量初始化
	strcpy(pNew->goodsName, pNow->brand);
	pNew->bottleNums = nums;
	//赠品相关初始化
	if (totalCost >= Gift.threshold && Gift.nums > 0) {
		strcpy(pNew->giftName, Gift.gift);
		pNew->giftNum = Gift.nums;
	}
	else {
		strcpy(pNew->giftName, "无");
		pNew->giftNum = 0;
	}
	//商品编号初始化
	pNew->orderNum = max + 1;
	//根据系统时间为订单时间初始化
	time_t now;
	struct tm* tm_now;
	time(&now);
	tm_now = localtime(&now);
	pNew->time = *tm_now;
	//其他初始化
	pNew->orderStatus = 1;//订单状态
	pNew->StatementOFChangingOrRefunding = 0;//退换货状态
	pNew->singleMoney = (float)totalCost;//订单金额
	strcpy(pNew->userName, pLoggedInUser->userName);//订单用户名
	//送货信息初始化
	while (1) {//分为默认地址和手动输入
		printf("\n\t当前默认收货信息：\n\t");
		printf("收货地址：%s\n\t", pLoggedInUser->Position);
		printf("收货人姓名：%s\n\t", pLoggedInUser->RealName);
		printf("收货人手机号：%lld\n\t", pLoggedInUser->phoneNumber);	
		printf("\n\t%s的订单使用 默认收货信息[1]  输入收货信息[2]\n\t",pNow->brand);
		int n1;
		scan(&n1);
		if (n1 == 1) {
			strcpy(pNew->address, pLoggedInUser->Position);
			strcpy(pNew->realName, pLoggedInUser->RealName);
			pNew->teleNum = pLoggedInUser->phoneNumber;
			break;
		}
		else if (n1 == 2) {
			printf("\t请输入收货地址：");
			gets(pNew->address);

			printf("\n\t请输入收货人手机号：");
			scanf("%lld", &pNew->teleNum);
			getchar();

			printf("\n\t请输入收货人姓名：");
			gets(pNew->realName);

			break;
		}
		else {
			printf("\t输入的代码有误，请重新输入！");
			Sleep(500);
		}
	}
}

void OrderStatusChangeAutomatic() {
	time_t now = time(NULL);
	struct tm nowTime = *localtime(&now);
	nowTime.tm_wday -= 15;//15天前的日期
	time_t timeBefore_15_Day = mktime(&nowTime);

	struct COrder* p = headOrder;
	while (p != NULL) {
		//orderStatus 为 1 表示订单处于未完成状态
		if (p->orderStatus == 1) {
			time_t tempTime = mktime(&p->time);
			//自动将距离下单时间超过15*24h的订单赋为已完成
			if (difftime(timeBefore_15_Day, tempTime) <= 0) {
				p->orderStatus = 0;
			}
		}
		p = p->next;
	}

	SaveOrderData();
}
//超过15天，订单状态自动变为已完成


//订单主菜单
void OrderFindAndChange() {
	OrderStatusChangeAutomatic();
	while (1) {
		system("cls");
		printf("\n\t************************  订单系统  ****************************\n");
		printf("\n\t---------------------------主菜单-------------------------------\n");
		printf("\n\t**************-0--返回                  返回--0-****************\n");
		printf("\n\t**************-1--查询订单          修改订单--2-****************\n");
		printf("\n\t**************-3--删除订单          商品退换--4-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		int n1;
		n1 = -1;
		scan(&n1);
		if (n1 == 0) {
			system("cls");
			return;
		}
		else if (n1 == 1) {
			OrderSeek();
		}
		else if (n1 == 2) {
			OrderChange();
		}

		else if (n1 == 3) {
			OrderDelete();
		}
		else if (n1 == 4) {
			GoodsChangingAndRefunding();
		}
		else {
			system("cls");
			printf("\n\t您输入的代码有误，请重新输入！\n");
		}
	}
}

//设置赠品
void GiftSet() {
	int n;
	while (1)
	{
		system("cls");
		printf("\t**************************赠品管理******************************\n");
		printf("\n\t---------------------------主菜单-------------------------------\n");
		printf("\n\t**************-0--返回                  返回--0-****************\n");
		printf("\n\t**************-1--设置赠品          删除赠品--2-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t已设置的赠品：%s\t", Gift.gift);
		printf("赠送数量：%d\t", Gift.nums);
		printf("消费门槛：%.2f\n", Gift.threshold);
		printf("\n\t请输入操作序号：");
		scan(&n);
		if (n == 0) {
			return;
		}
		else if (n == 1) {
			printf("\n\t请输入要设置的赠品名(0返回)：");
			gets(Gift.gift);
			if (!strcmp(Gift.gift, "0")) {
				strcpy(Gift.gift, "无");
				continue;
			}
			else {//继续设置数量和门槛
				Gift.nums = 0;
				Gift.threshold = 0;
				while (1)
				{
					printf("\n\t输入赠送数量\n\t");
					scanf("%d", &n);
					getchar();
					if (n <= 0) {
						printf("\n\t输入非法！\n\t");
						Sleep(500);
						continue;
					}
					printf("\n\t输入赠送赠品的消费额门槛：");
					float f = -1;
					scanf("%f", &f);
					getchar();
					if (f <= 0) {
						printf("\n\t输入非法！\n\t");
						Sleep(500);
						continue;
					}
					Gift.nums = n;
					Gift.threshold = f;
					SaveGiftData();
					break;
				}
			}
		}
		else if (n == 2) {
			strcpy(Gift.gift, "无");
			Gift.nums = 0;
			Gift.threshold = 0;
			SaveGiftData();
			printf("\n\t删除成功\n");
			Sleep(500);
			continue;
		}
		else {
			printf("\n\t代码错误\n\t");
		}
	}
}

// 获取一个月前的时间
struct tm GetOneMonthAgoTimestamp() {
	time_t now = time(NULL);
	struct tm one_month_ago_tm = *localtime(&now);
	one_month_ago_tm.tm_mon -= 1;
	mktime(&one_month_ago_tm);
	return one_month_ago_tm;
}

//打印订单
void OrderPrint(int fullOrder) {
	struct COrder* p = headOrder;
	//管理员用户
	if (pLoggedInUser->grade == 0) {
		// 获取一个月前的时间戳
		struct tm oneMonthAgo = GetOneMonthAgoTimestamp();

		// 遍历订单链表，打印最近一个月的订单
		struct COrder* currOrder = headOrder;
		if (fullOrder == 1) {
			system("cls");
			printf("\n\t*******************************************************   历史订单   *********************************************\n\n");
			printf("\n\t现在打印的是最近一个月内订单信息!\n");
			printf("\n\t-----------------------------------------------------------------------------------------------------------------");
			char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
			printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
			printf("\t-----------------------------------------------------------------------------------------------------------------");
			while (currOrder != NULL) {
				if (difftime(mktime(&(currOrder->time)), mktime(&oneMonthAgo)) >= 0) {
					// 打印订单信息
					printf("\n\t%8d", currOrder->orderNum);
					printf("\t%4d/%2d/%2d %2d:%2d:%2d", currOrder->time.tm_year + 1900, currOrder->time.tm_mon + 1, currOrder->time.tm_mday, currOrder->time.tm_hour, currOrder->time.tm_min, currOrder->time.tm_sec);
					printf("%20s", currOrder->realName);
					printf("%18lld", currOrder->teleNum);
					if (currOrder->orderStatus == 1) printf("\t未完成");
					else printf("\t已完成");
					printf("\t\t%.2f\n", currOrder->singleMoney);
					printf("\n\t                商品：%d瓶%-30s\t赠品：%d %30s", currOrder->bottleNums,currOrder->goodsName, currOrder->giftNum, currOrder->giftName);

					printf("\n\t                收货地址：%s", currOrder->address);
					printf("\n\t-----------------------------------------------------------------------------------------------------------------");
				}
				currOrder = currOrder->next;
			}
		}
		else if (fullOrder == 2) {
			system("cls");
			printf("\n\t*******************************************************   历史订单   *********************************************\n\n");
			printf("\n\t现在打印的是完整订单信息!\n");
			printf("\n\t-----------------------------------------------------------------------------------------------------------------");
			char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
			printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
			printf("\t-----------------------------------------------------------------------------------------------------------------");
			while (currOrder != NULL) {
				// 打印订单信息

				printf("\n\t%8d", currOrder->orderNum);
				printf("\t%4d/%2d/%2d %2d:%2d:%2d", currOrder->time.tm_year + 1900, currOrder->time.tm_mon + 1, currOrder->time.tm_mday, currOrder->time.tm_hour, currOrder->time.tm_min, currOrder->time.tm_sec);
				printf("%20s", currOrder->realName);
				printf("%18lld", currOrder->teleNum);
				if (currOrder->orderStatus == 1) printf("\t未完成");
				else printf("\t已完成");
				printf("\t\t%.2f\n", currOrder->singleMoney);
				printf("\n\t                商品：%d瓶%-30s\t赠品：%d %30s", currOrder->bottleNums, currOrder->goodsName, currOrder->giftNum, currOrder->giftName);

				printf("\n\t                收货地址：%s", currOrder->address);
				printf("\n\t-----------------------------------------------------------------------------------------------------------------");
				currOrder = currOrder->next;
			}
		}

	}
	//普通用户
	else {
		struct COrder* currOrder = headOrder;
		char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
		printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
		printf("\t-----------------------------------------------------------------------------------------------------------------");
		while (currOrder != NULL) {

			if (strcmp(pLoggedInUser->userName, currOrder->userName) == 0) {
				// 打印订单信息
				printf("\n\t%8d", currOrder->orderNum);
				printf("\t%4d/%2d/%2d %2d:%2d:%2d", currOrder->time.tm_year + 1900, currOrder->time.tm_mon + 1, currOrder->time.tm_mday, currOrder->time.tm_hour, currOrder->time.tm_min, currOrder->time.tm_sec);
				printf("%20s", currOrder->realName);
				printf("%18lld", currOrder->teleNum);
				if (currOrder->orderStatus == 1) printf("\t未完成");
				else printf("\t已完成");
				printf("\t\t%.2f\n", currOrder->singleMoney);
				printf("\n\t                商品：%d瓶%-30s\t赠品：%d %30s", currOrder->bottleNums, currOrder->goodsName, currOrder->giftNum, currOrder->giftName);

				printf("\n\t                收货地址：%s", currOrder->address);
				printf("\n\t-----------------------------------------------------------------------------------------------------------------");
			}
			currOrder = currOrder->next;
		}
	}
}

//查询订单
void OrderSeek() {
	int beginMonth;//开始的订单月份
	int beginDay;//开始的订单日
	int beginHour;//开始的时刻
	int endMonth;//结束的订单月份
	int endDay;//结束的订单日
	int endHour;//结束的时刻
	int orderNum;//订单编号
	while (1) {
		system("cls");
		printf("\t**************************订单查询******************************\n");
		printf("\n\t---------------------------主菜单-------------------------------\n");
		printf("\n\t**************-0--返回                  返回--0-****************\n");
		printf("\n\t**************-1--按时间查询      按编号查询--2-****************\n");
		printf("\n\t**************-3--按状态筛选    用户所有订单--4-****************\n");
		printf("\n\t**************-5--查看退货订单  查看换货订单--6-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		int n = -1;
		scan(&n);
		if (n == 0)
			return;
		else if (n == 1) {
			printf("\n\t请输入起始月份：");
			scanf("%d", &beginMonth);
			getchar();

			printf("\n\t请输入起始日：");
			scanf("%d", &beginDay);
			getchar();

			printf("\n\t请输入起始小时：");
			scanf("%d", &beginHour);
			getchar();

			printf("\n\t请输入截至月份：");
			scanf("%d", &endMonth);
			getchar();

			printf("\n\t请输入截至日：");
			scanf("%d", &endDay);
			getchar();

			printf("\n\t请输入截至小时：");
			scanf("%d", &endHour);
			getchar();

			struct COrder* p = headOrder;
			int orderNumble = 0;
			system("cls");
			char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
			printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
			printf("\t-----------------------------------------------------------------------------------------------------------------");
			while (p != NULL) {
				if (p->time.tm_mon >= beginMonth && p->time.tm_mon <= endMonth) {
					if (p->time.tm_mday >= beginDay && p->time.tm_mday <= endDay) {
						if (p->time.tm_hour >= beginHour && p->time.tm_hour) {
							if (pLoggedInUser->grade == 0) {
								printf("\n\t%8d", p->orderNum);
								printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
								printf("%20s", p->realName);
								printf("%18lld", p->teleNum);
								if (p->orderStatus == 1) printf("\t未完成");
								else printf("\t已完成");
								printf("\t\t%.2f\n", p->singleMoney);
								printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
								printf("\n\t                收货地址：%s", p->address);
								printf("\n\t-----------------------------------------------------------------------------------------------------------------");
								orderNumble++;
							}
							else {
								if (!strcmp(pLoggedInUser->userName, p->userName)) {
									printf("\n\t%8d", p->orderNum);
									printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
									printf("%20s", p->realName);
									printf("%18lld", p->teleNum);
									if (p->orderStatus == 1) printf("\t未完成");
									else printf("\t已完成");
									printf("\t\t%.2f\n", p->singleMoney);
									printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
									printf("\n\t                收货地址：%s", p->address);
									printf("\n\t-----------------------------------------------------------------------------------------------------------------");
									orderNumble++;
								}
							}
						}
					}
				}
				p = p->next;
			}
			printf("\n\t找到%d个订单\n\t", orderNumble);
			printf("\n\t回车键返回\n\t");
			while (getchar() != '\n');
		}
		else if (n == 2) {
			printf("\n\t请输入订单编号：");
			orderNum = -1;
			scan(&orderNum);
			struct COrder* p = headOrder;
			while (p != NULL) {
				if (p->orderNum == orderNum) {
					if (pLoggedInUser->grade == 0) {
						system("cls");
						char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
						printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
						printf("\t-----------------------------------------------------------------------------------------------------------------");
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\t未完成");
						else printf("\t已完成");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                收货地址：%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						break;
					}
					else {
						if (strcmp(pLoggedInUser->userName, p->userName)) {
							printf("\n\t账户内不存在此订单！\n\t");
							break;
						}
						else {
							system("cls");
							char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
							printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
							printf("\t-----------------------------------------------------------------------------------------------------------------");
							printf("\n\t%8d", p->orderNum);
							printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
							printf("%20s", p->realName);
							printf("%18lld", p->teleNum);
							if (p->orderStatus == 1) printf("\t未完成");
							else printf("\t已完成");
							printf("\t\t%.2f\n", p->singleMoney);
							printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
							printf("\n\t                收货地址：%s", p->address);
							printf("\n\t-----------------------------------------------------------------------------------------------------------------");
							break;
						}
					}
				}
				p = p->next;
			}
			if (p == NULL)printf("\n\t未找到订单\n\t");
			printf("\n\t回车键返回\n\t");
			while (getchar() != '\n');
		}
		else if (n == 3) {
			while (1)
			{
				printf("\n\t查询已完成订单[1]  查询未完成订单[2]  返回[0]\n\t");
				n = -1;
				scan(&n);
				if (n == 0)
					return;
				else if (n == 1) {
					struct COrder* p = headOrder;
					int orderNumble = 0;
					system("cls");
					char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
					printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
					printf("\t-----------------------------------------------------------------------------------------------------------------");
					while (p != NULL) {
						if (pLoggedInUser->grade == 0) {
							if (p->orderStatus == 0) {
								printf("\n\t%8d", p->orderNum);
								printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
								printf("%20s", p->realName);
								printf("%18lld", p->teleNum);
								if (p->orderStatus == 1) printf("\t未完成");
								else printf("\t已完成");
								printf("\t\t%.2f\n", p->singleMoney);
								printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
								printf("\n\t                收货地址：%s", p->address);
								printf("\n\t-----------------------------------------------------------------------------------------------------------------");
								orderNumble++;
							}
						}
						else {
							if (p->orderStatus == 0 && !strcmp(pLoggedInUser->userName, p->userName)) {
								printf("\n\t%8d", p->orderNum);
								printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
								printf("%20s", p->realName);
								printf("%18lld", p->teleNum);
								if (p->orderStatus == 1) printf("\t未完成");
								else printf("\t已完成");
								printf("\t\t%.2f\n", p->singleMoney);
								printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
								printf("\n\t                收货地址：%s", p->address);
								printf("\n\t-----------------------------------------------------------------------------------------------------------------");
								orderNumble++;
							}
						}
						p = p->next;
					}
					printf("\n\t找到%d个订单\n\t", orderNumble);
					printf("\n\t回车键返回\n\t");
					while (getchar() != '\n');
					break;
				}
				else if (n == 2) {
					struct COrder* p = headOrder;
					int orderNumble = 0;
					system("cls");
					char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
					printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
					printf("\t-----------------------------------------------------------------------------------------------------------------");
					while (p != NULL) {
						if (pLoggedInUser->grade == 0) {
							if (p->orderStatus == 1) {
								printf("\n\t%8d", p->orderNum);
								printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
								printf("%20s", p->realName);
								printf("%18lld", p->teleNum);
								if (p->orderStatus == 1) printf("\t未完成");
								else printf("\t已完成");
								printf("\t\t%.2f\n", p->singleMoney);
								printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
								printf("\n\t                收货地址：%s", p->address);
								printf("\n\t-----------------------------------------------------------------------------------------------------------------");
								orderNumble++;
							}
						}
						else {
							if (p->orderStatus == 1 && !strcmp(pLoggedInUser->userName, p->userName)) {
								printf("\n\t%8d", p->orderNum);
								printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
								printf("%20s", p->realName);
								printf("%18lld", p->teleNum);
								if (p->orderStatus == 1) printf("\t未完成");
								else printf("\t已完成");
								printf("\t\t%.2f\n", p->singleMoney);
								printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
								printf("\n\t                收货地址：%s", p->address);
								printf("\n\t-----------------------------------------------------------------------------------------------------------------");
								orderNumble++;
							}
						}
						p = p->next;
					}
					printf("\n\t找到%d个订单\n\t", orderNumble);
					printf("\n\t回车键返回\n\t");
					while (getchar() != '\n');
					break;
				}
				else {
					printf("\n\t代码错误！\n\t");
					Sleep(500);
					continue;
				}
			}
		}
		else if (n == 4) {
			if (pLoggedInUser->grade == 0) {
				//管理员查看任意用户订单
				printf("\n\t请输入查询的用户名：");
				char userName[40];
				gets(userName);
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (!strcmp(p->userName, userName)) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\t未完成");
						else printf("\t已完成");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                收货地址：%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t找到%d个订单\n\t", orderNumble);
				printf("\n\t回车键返回\n\t");
				while (getchar() != '\n');
			}
			else {
				//输出当前用户的订单
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (!strcmp(p->userName, pLoggedInUser->userName)) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\t未完成");
						else printf("\t已完成");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                收货地址：%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t找到%d个订单\n\t", orderNumble);
				printf("\n\t回车键返回\n\t");
				while (getchar() != '\n');
			}
		}
		else if (n == 5) {
			if (pLoggedInUser->grade == 0) {
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (p->StatementOFChangingOrRefunding == 1) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\t未完成");
						else printf("\t已完成");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                收货地址：%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t找到%d个订单\n\t", orderNumble);
				printf("\n\t回车键返回\n\t");
				while (getchar() != '\n');

			}
			else {
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (strcmp(p->userName, pLoggedInUser->userName) == 0 && p->StatementOFChangingOrRefunding == 1) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\t未完成");
						else printf("\t已完成");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                收货地址：%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t找到%d个订单\n\t", orderNumble);
				getchar();
			}
		}
		else if (n == 6) {
			if (pLoggedInUser->grade == 0) {
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (p->StatementOFChangingOrRefunding == 2) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\t未完成");
						else printf("\t已完成");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                收货地址：%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t找到%d个订单\n\t", orderNumble);
				printf("\n\t回车键返回\n\t");
				while (getchar() != '\n');

			}
			else {
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (strcmp(p->userName, pLoggedInUser->userName) == 0 && p->StatementOFChangingOrRefunding == 2) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\t未完成");
						else printf("\t已完成");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                商品名：%30s\t赠品：%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                收货地址：%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t找到%d个订单\n\t", orderNumble);
				printf("\n\t回车键返回\n\t");
				while (getchar() != '\n');

			}
		}
		else {
			printf("\n\t输入代码错误！\n\t");
			Sleep(500);
			continue;
		}
	}
}

//修改订单
void OrderChange() {
	system("cls");
	printf("\n\t*******************************************************   修改订单   *********************************************\n\n");
	int num_ = -1;
	while (pLoggedInUser->grade == 0) {
		printf("\n\t输出近1个月的订单 [1]    输出历史完整订单 [2]");
		printf("\n\t在此输入：");
		num_ = -1;
		scan(&num_);
		if (num_ == 1)break;
		else if (num_ == 2)break;
		else printf("\n\t输入有误！\n");
	}
	OrderPrint(num_);

	struct COrder* p = headOrder;
	printf("\n\n\t----------------------------------------------------------------------------------------------------------------\n\n");
	//找到需要被修改的订单的地址
	int num = -1;
	while (1) {
		p = headOrder;
		printf("\n\t请输入需要进行修改的订单的编号,输入[0]可返回：");
		scan(&num);
		if (num == 0) {
			system("cls");
			return;
		}
		else if (num >= 1 && num <= 100000) {
			//非管理员用户
			if (pLoggedInUser->grade != 0) {
				int state = 0;
				p = headOrder;
				while (p != NULL) {
					if (p->orderNum == num && strcmp(p->userName, pLoggedInUser->userName) == 0) {
						state = 1;
						break;
					}
					p = p->next;
				}

				if (state == 0) {
					printf("\n\t不存在您输入的编号对应的订单，请按任意键后重新输入！");
					getchar();
				}
				else break;//找到了输入的订单
			}
			//管理员用户
			else if (pLoggedInUser->grade == 0) {
				int state = 0;
				p = headOrder;
				while (p != NULL) {
					if (p->orderNum == num) {
						state = 1;
						break;
					}
					p = p->next;
				}

				if (state == 0) {
					printf("\n\t不存在您输入的编号对应的订单，请按任意键后重新输入！");
					getchar();
				}
				else break;//找到了输入的订单
			}
		}
	}
	printf("\n\t-----------------------------------------------------操  作  菜  单---------------------------------------------\n\n");
	printf("\t               ********************-0--返回                              返回--0-********************\n\n");
	printf("\t               ********************-1--修改收货地址              修改收货人姓名--2-********************\n\n");
	printf("\t               ********************-3--修改收货人电话           修改订单完成状态--4-********************\n\n");
	printf("\t               -------------------------------------------------------------------------------------\n\n");

	while (1) {
		printf("\n\t请输入\"操作菜单\"中的操作序号：");
		int n1 = -1;
		scan(&n1);
		if (n1 == 0) {
			system("cls");
			return;
		}
		else if (n1 == 1) {
			while (1) {
				printf("\n\t您当前的收货地址为%s", p->address);
				printf("\n\t修改该商品收货地址请输入[1],返回请输入[0]:");
				int n2 = -1;
				scan(&n2);
				if (n2 == 1) {
					printf("\n\t请输入新的收货地址(不超过25个汉字)：");
					char tempPlace[100];
					gets(tempPlace);
					if (strlen(tempPlace) > 50) {
						printf("\n\t地址过长，请重新输入！\n");
					}
					else {
						strcpy(p->address, tempPlace);
						printf("\n\t该订单新的收货地址为%s\n", tempPlace);
						printf("\n\t按任意键返回。");
						SaveOrderData();
						getchar();
						break;
					}
					break;
				}
				else if (n2 == 0) {
					break;
				}
				else printf("\n\t输入有误，请重新输入！\n");
			}
			//break;
		}
		else if (n1 == 2) {
			while (1) {
				printf("\n\t该订单当前的收货人为%s", p->realName);
				printf("\n\t修改订单当前的收货人请输入[1],返回请输入[0]:");
				int n2 = -1;
				scan(&n2);
				if (n2 == 1) {
					printf("\n\t请输入新的收货人姓名(不超过10个汉字)：");
					char tempName[40];
					gets(tempName);
					if (strlen(tempName) > 20) {
						printf("\n\t姓名过长，请重新输入！\n");
					}
					else {
						strcpy(p->realName, tempName);
						printf("\n\t该订单新的收货人为%s\n", tempName);
						printf("\n\t按任意键返回。");
						SaveOrderData();
						getchar();
						break;
					}
					break;
				}
				else if (n2 == 0) {
					break;
				}
				else printf("\n\t输入有误，请重新输入！\n");
			}
			break;
		}
		else if (n1 == 3) {
			int n2 = -1;
			while (1) {
				n2 = -1;
				printf("\n\t当前收货人的电话为：%lld\n", p->teleNum);
				printf("\t输入[0]返回,输入[1]修改收货人电话：");
				scan(&n2);
				if (n2 == 0) {
					break;
				}
				if (n2 == 1) {
					while (1) {
						printf("\n\t请输入新的收货人电话：");
						long long tempTeleNum = -1;
						scanf("%lld", &tempTeleNum);
						getchar();
						if (tempTeleNum >= 10000000000 && tempTeleNum <= 100000000000) {
							printf("\n\t收货人新电话号码%lld\n", tempTeleNum);
							printf("\n\t输入任意键返回。");
							SaveOrderData();
							getchar();
							p->teleNum = tempTeleNum;
							break;
						}
						else printf("\n\t输入有误，请重新输入！\n");
					}
					break;
				}
				else printf("\n\t输入有误，请重新输入！\n");
			}
			break;
		}
		else if (n1 == 4) {
			if (p->orderStatus == 1) {
				printf("\n\t编号为%d的订单暂未完成\n", p->orderNum);
				printf("\n\t注意：距离下单时间超过15天的订单的状态会自动转变为\"已完成\"！");
				while (1) {
					printf("\n\t输入[0]可返回，输入[1]将订单状态改为\"已完成\"");
					int n2 = -1;
					scan(&n2);
					if (n2 == 0) {
						break;
					}
					else if (n2 == 1) {
						int n3 = -1;
						while (1) {
							n3 = -1;
							printf("\n\t该操作不可撤销，是否确认修改？输入[0]返回，输入[1]确认更改：");
							scan(&n3);
							if (n3 == 0) break;
							else if (n3 == 1) {
								p->orderStatus = 0;
								printf("\n\t您已成功更改！按任意键返回");
								SaveOrderData();
								getchar();
								break;
							}
							else printf("\n\t您的输入有误！请重新输入！\n");
						}
						break;
					}
					else printf("\n\t您的输入有误！请重新输入！\n");
				}
				break;
			}
			else {
				printf("\n\t编号为%d的订单已完成，订单状态不可再更改！输入任意键返回。", p->orderNum);
				getchar();
				break;
			}
		}
		else if (n1 == 5) {
			
			printf("\n\t请输入修改后时间：\n");

			struct tm start;
			int timeStart[6]; // 定义int型数组time，用于存储时间
			int timeEnd[6];
			const int month_days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // 每个月的天数（暂时不考虑闰年）
			// 从键盘中读入时间，依次为年月日时分秒
			
			
				printf("\n\t请输入活动开始时间，格式为 yyyy mm dd hh mm ss（分别对应年、月、日、时、分、秒）：");
				printf("\n\t");
				int i = 0;
				int valid = 0;
				for (i = 0; i < 6; i++) {
					valid = 0;
					while (!valid) {
						if (scanf("%d", &timeStart[i]) != 1) {
							printf("\t输入无效，请重新输入：");
							while (getchar() != '\n');
						}
						else {
							valid = 1;
							getchar();
						}
					}

				}
				
			
			start.tm_year = timeStart[0] - 1900;
			start.tm_mon = timeStart[1] - 1;
			start.tm_mday = timeStart[2];
			start.tm_hour = timeStart[3];
			start.tm_min = timeStart[4];
			start.tm_sec = timeStart[5];

			printf("\n\t活动开始时间为%4d年%2d月%2d日%2d时%2d分%2d秒\n", start.tm_year + 1900, start.tm_mon + 1,
				start.tm_mday, start.tm_hour, start.tm_min, start.tm_sec);

			p->time = start;
			SaveOrderData();
			getchar();
			
		}
		else if (n1 == 6) {
			printf("\n\t请输入修改后瓶数");
			int n3 = -1;
			scan(&n3);
			p->bottleNums = n3;
			SaveOrderData();
			getchar();
		}
		else printf("\n\t输入有误，请重新输入！\n");
	}
	SaveOrderData();
}

//删除订单
void OrderDelete() {
	system("cls");
	printf("\n\t*******************************************************   订单删除   *********************************************\n\n");
	int num_ = -1;
	while (pLoggedInUser->grade == 0) {
		printf("\n\t输出近1个月的订单[1], 输出历史完整订单[2]");
		printf("\n\t在此输入：");
		num_ = -1;
		scan(&num_);
		if (num_ == 1)break;
		else if (num_ == 2)break;
		else printf("\n\t输入有误！\n");
	}
	OrderPrint(num_);

	printf("\n\n\t----------------------------------------------------------------------------------------------------------------\n");
	printf("\n\t*******************************************************   订单删除   *********************************************\n\n");
	int num = -1;
	struct COrder* p = headOrder;
	struct COrder* p0 = NULL;
	//找到需要被删除的订单的编号
	while (1) {
		p = headOrder;
		p0 = NULL;
		printf("\n\t请输入需要删除的订单的编号,输入[0]可返回：");
		scan(&num);
		if (num == 0) {
			system("cls");
			return;
		}
		else if (num >= 1 && num <= 100000) {
			//非管理员用户
			if (pLoggedInUser->grade != 0) {
				int state = 0;
				p = headOrder;
				p0 = NULL;
				while (p != NULL) {
					if (p->orderNum == num && strcmp(p->userName, pLoggedInUser->userName) == 0) {
						state = 1;
						break;
					}
					p0 = p;
					p = p->next;
				}

				if (state == 0) {
					printf("\n\t不存在您输入的编号对应的订单，请按任意键后重新输入！");
					getchar();
				}
				else break;//找到了输入的订单
			}
			//管理员用户
			else if (pLoggedInUser->grade == 0) {
				int state = 0;
				p = headOrder;
				p0 = NULL;
				while (p != NULL) {
					if (p->orderNum == num) {
						state = 1;
						break;
					}
					p0 = p;
					p = p->next;
				}

				if (state == 0) {
					printf("\n\t不存在您输入的编号对应的订单，请按任意键后重新输入！");
					getchar();
				}
				else break;//找到了输入的订单
			}
		}
	}


	if (p->orderStatus == 1) {
		printf("\n\t该订单暂未完成，不能被删除。输入任意键返回！");
		getchar();
	}
	else if (p->orderStatus == 0) {
		int n2 = -1;
		while (1) {
			n2 = -1;
			printf("\n\t输入[1]删除该订单，输入[0]返回：");
			scan(&n2);
			if (n2 == 0) {
				break;
			}
			else if (n2 == 1) {
				if (p0 == NULL) {
					headOrder = headOrder->next;
					free(p);
					p = headOrder;
				}
				else {
					p0->next = p->next;
					free(p);
					p = p0->next;
				}
				printf("\n\t您已成功删除该订单，按任意键返回！");
				SaveOrderData();
				getchar();
				break;
			}
			else printf("\n\t输入有误，请重新输入！\n");
		}
	}
}

//退换货
void GoodsChangingAndRefunding() {
	//第一步 输出所有正在申请退换货的订单信息
	struct COrder* currOrder = headOrder;
	//管理员用户
	if (pLoggedInUser->grade == 0) {
		system("cls");
		printf("\n\t*******************************************************   历史订单   *********************************************\n\n");
		printf("\n\t现在打印的是所有正在申请退换货的订单!\n");
		printf("\n\t-----------------------------------------------------------------------------------------------------------------");
		char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
		printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
		printf("\t-----------------------------------------------------------------------------------------------------------------");
		while (currOrder != NULL) {
			// 打印处于退换货状态订单的信息
			if ((currOrder->StatementOFChangingOrRefunding == 1 || currOrder->StatementOFChangingOrRefunding == 2)) {
				printf("\n\t%8d", currOrder->orderNum);
				printf("\t%4d/%2d/%2d %2d:%2d:%2d", currOrder->time.tm_year + 1900, currOrder->time.tm_mon + 1, currOrder->time.tm_mday, currOrder->time.tm_hour, currOrder->time.tm_min, currOrder->time.tm_sec);
				printf("%20s", currOrder->realName);
				printf("%18lld", currOrder->teleNum);
				if (currOrder->orderStatus == 1) printf("\t未完成");
				else printf("\t已完成");
				printf("\t\t%.2f\n", currOrder->singleMoney);
				printf("\n\t                退换货状态：");
				if (currOrder->StatementOFChangingOrRefunding == 1)printf("正在退货");
				else printf("正在换货");
				printf("\n\t-----------------------------------------------------------------------------------------------------------------");
			}
			currOrder = currOrder->next;
		}


		//找到需要修改的订单
		struct COrder* p = headOrder;
		int num = -1;
		while (1) {
			p = headOrder;
			printf("\n\t请输入需要进行操作的的订单的编号,输入[0]可返回：");
			scan(&num);
			if (num == 0) {
				system("cls");
				return;
			}
			else if (num >= 1) {

				int state = 0;
				p = headOrder;
				while (p != NULL) {
					if (p->orderNum == num && (p->StatementOFChangingOrRefunding == 1 || p->StatementOFChangingOrRefunding == 2)) {
						state = 1;
						break;
					}
					p = p->next;
				}

				if (state == 0) {
					printf("\n\t不存在您输入的编号对应的订单，请按任意键后重新输入！");
					getchar();
				}
				else break;//找到了输入的订单

			}
			else printf("\n\t输入有误，请重新输入!\n");
		}

		while (1) {
			num = -1;
			if (p->StatementOFChangingOrRefunding == 1)printf("\n\t当前订单正在退货");
			else printf("\n\t当前订单正在换货");
			printf("\n\t批准申请请输入[1], 驳回申请请输入[2], 取消操作请输入[0]:");
			scan(&num);
			if (num == 0) {
				system("cls");
				break;
			}
			else if (num == 1) {
				p->StatementOFChangingOrRefunding = 3;
				SaveOrderData();
				if (p->StatementOFChangingOrRefunding != 1)
					InformationChangeForRefunding(p);
				printf("\n\t批准成功！\n");
				break;
			}
			else if (num == 2) {
				p->StatementOFChangingOrRefunding = 0;
				SaveOrderData();
				printf("\n\t成功驳回申请！\n");
				break;
			}
			else printf("\n\t不存在与您输入的数字对应的编号！请重新输入！\n");
		}
	}
	//非管理员用户
	else if (pLoggedInUser->grade > 0) {
		system("cls");
		printf("\n\t*******************************************************   历史订单   *********************************************\n\n");
		printf("\n\t现在打印的是所有处于未完成状态的订单!\n");
		printf("\n\t-----------------------------------------------------------------------------------------------------------------");
		char* arrName[6] = { "订单编号", "下单时间", "收件人姓名", "收件人手机号" ,"订单完成状态", "订单金额" };
		printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
		printf("\t-----------------------------------------------------------------------------------------------------------------");
		while (currOrder != NULL) {
			// 打印订单信息
			if (currOrder->orderStatus == 1 && currOrder->StatementOFChangingOrRefunding != 3 && strcmp(currOrder->userName, pLoggedInUser->userName) == 0) {
				printf("\n\t%8d", currOrder->orderNum);
				printf("\t%4d/%2d/%2d %2d:%2d:%2d", currOrder->time.tm_year + 1900, currOrder->time.tm_mon + 1, currOrder->time.tm_mday, currOrder->time.tm_hour, currOrder->time.tm_min, currOrder->time.tm_sec);
				printf("%20s", currOrder->realName);
				printf("%18lld", currOrder->teleNum);
				if (currOrder->orderStatus == 1) printf("\t未完成");
				else printf("\t已完成");
				printf("\t\t%.2f\n", currOrder->singleMoney);
				printf("\n\t                退换货状态：");
				if (currOrder->StatementOFChangingOrRefunding == 1)printf("正在退货");
				else if (currOrder->StatementOFChangingOrRefunding == 2) printf("正在换货");
				else printf("未申请过退换货！");
				printf("\n\t-----------------------------------------------------------------------------------------------------------------");
			}
			currOrder = currOrder->next;
		}


		//找到需要修改的订单
		struct COrder* p = headOrder;
		int num = -1;
		while (1) {
			p = headOrder;
			printf("\n\t请输入需要进行操作的的订单的编号,输入[0]可返回：");
			scan(&num);
			if (num == 0) {
				system("cls");
				return;
			}
			else if (num >= 1) {

				int state = 0;
				p = headOrder;
				while (p != NULL) {
					if (p->orderNum == num && p->orderStatus == 1 && p->StatementOFChangingOrRefunding != 3 && strcmp(p->userName, pLoggedInUser->userName) == 0) {
						state = 1;
						break;
					}
					p = p->next;
				}

				if (state == 0) {
					printf("\n\t不存在您输入的编号对应的订单，请按任意键后重新输入！");
					getchar();
				}
				else break;//找到了输入的订单

			}
			else printf("\n\t输入有误，请重新输入!\n");
		}

		while (1) {
			num = -1;
			//订单处于退换货状态
			if (p->StatementOFChangingOrRefunding == 1 || p->StatementOFChangingOrRefunding == 2) {
				if (p->StatementOFChangingOrRefunding == 1) printf("\n\t当前订单正在退货");
				else if (p->StatementOFChangingOrRefunding == 2) printf("\n\t当前订单正在换货");
				int n1 = -1;
				while (1) {
					n1 = -1;
					printf("\n\t取消申请请输入[1], 返回请输入[0]:");
					scan(&n1);
					if (n1 == 0) {
						
						break;
					}
					else if (n1 == 1) {
						p->StatementOFChangingOrRefunding = 0;
						printf("\n\t您已成功取消申请！\n");
						break;
					}
				}
			}
			//订单未处于退换货状态
			else {
				printf("\n\t当前订单未处于退换货状态\n");
				printf("\n\t申请退货请输入[1], 申请换货请输入[2], 取消操作请输入[0]:");
				scan(&num);
				if (num == 0) {
					system("cls");
					break;
				}
				else if (num == 1) {
					p->StatementOFChangingOrRefunding = 1;
					SaveOrderData();
					printf("\n\t申请成功！请耐心等待商家处理！\n");
					getchar();
					break;
				}
				else if (num == 2) {
					p->StatementOFChangingOrRefunding = 2;
					SaveOrderData();
					printf("\n\t申请成功！请耐心等待商家处理！\n");
					getchar();
					break;
				}
				else printf("\n\t不存在与您输入的数字对应的编号！请重新输入！\n");
			}
		}
	}
}

//由于退货造成的信息改变
void InformationChangeForRefunding(struct COrder* p) {

	struct CGoods* pGood = headGoods; struct CGoods* pGood0 = NULL;
	struct CAccount* pAccou = headAccount; struct CAccount* pAccou0 = NULL;
	//需要改变的量 用户的总销售额,用户会员等级，商品库存,商品销量

	while (pGood != NULL && strcmp(pGood->brand, p->realName) == 0) {
		pGood0 = pGood;
		pGood = pGood->next;
	}

	//用户等级、销售更改
	float totalCost = p->singleMoney;
	pLoggedInUser->totalAmount += totalCost;
	//*********GradeChange(pLoggedInUser);

	//商品库存、销量更改
	pGood->stock += p->bottleNums;
	pGood->sales -= p->bottleNums;

	//将修改后的各项信息存储到文件当中去

	SaveGoodsData(headGoods);
	SaveAccountData(headAccount);

}

