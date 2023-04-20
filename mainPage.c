#define _CRT_SECURE_NO_WARNINGS
#include "head.h"
#include"mainPage.h"
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void GoodsFindAndChange();
void AccountFindAndChange();
void OrderFindAndChange();
void OtherSetting();

void ReSetTrolleyInformation() {
	struct CTrolley* pTro = headTrolley;
	struct CGoods* pGood = headGoods;

	while (pTro != NULL) {
		pGood = headGoods;
		while (pGood != NULL && strcmp(pGood->brand, pTro->brand)) {
			pGood = pGood->next;
		}
		if (pGood == NULL);
		else {

			pTro->boxesNumber = pGood->boxesNumber;
			pTro->goodsNumber = pGood->goodsNum;
		}
		pTro = pTro->next;
	}

	SaveGoodsTrolleyData();
}

int accountLogin() {
	char NameAccount[40];//用户输入的用户名
	char PassAccount[40];//用户输入的密码
	struct CAccount* pNow = headAccount;//用于比对以登陆账号
	system("cls");
	while (1) {
		//输入用户名阶段
		printf("\n\t************************  登录系统  ****************************\n");
		printf("\n\t---------------------------主菜单-------------------------------\n");
		printf("\n\t**************-0--          返回           --0-****************\n");
		printf("\n\t**************----   输入账号名以继续登录   ----****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t请输入(不超过20个字符)：");
		gets(NameAccount);
		if (strlen(NameAccount) == 1 && NameAccount[0] == '0') {
			system("cls");
			return 0;
		}
		pNow = headAccount;
		while (pNow != NULL && strcmp(pNow->userName, NameAccount))//遍历用户链表
			pNow = pNow->next;
		if (pNow == NULL) {//链表到尾即不存在此用户
			system("cls");
			printf("\n\t用户名不存在！\n");
		}
		else {//用户名比对成功，进入输密码阶段
			while (1) {
				printf("\n\t请输入正确密码（按0回到主界面）：");
				gets(PassAccount);
				if (strlen(PassAccount) == 1 && PassAccount[0] == '0') {
					system("cls");
					return 0;
				}
				if (strcmp(pNow->password, PassAccount)) //密码比对错误
					printf("\n\t密码错误\n");
				else {
					pLoggedInUser = pNow; //登录成功返回，并将全局指针指向登录者
					return 1;
				}
			}
		}
	}
}

void accountSignup() {
	int g = 1;//表示账号等级
	char NewAccount[40];
	char SecureNum[40];
	char NewPassword[40];
	char secureWord[40];
	struct CAccount* pNew = headAccount;
	//读取安全码
	FILE* fp_ = fopen("secureWord.dat", "rb");
	fread(secureWord, sizeof(char), 10, fp_);
	fclose(fp_);
	system("cls");
	while (1)//接收用户名并比对是否存在重复用户名
	{
		printf("\n\t************************  注册系统  ****************************\n");
		printf("\n\t---------------------------主菜单-------------------------------\n");
		printf("\n\t**************-0--          返回           --0-****************\n");
		printf("\n\t**************-1--   输入账号名以继续注册   --1-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t请输入：");
		int n1;
		n1 = -1;
		scan(&n1);
		if (n1 == 1) {
			printf("\n\t请在下方输入您要注册的用户名(小于等于20个字符，用户名不能为0)(按0返回)\n\t");
			char arrCharTemp[40] = { 0 };
			while (1) {
				gets(arrCharTemp);
				arrCharTemp[39] = '\0';
				if (strlen(arrCharTemp) > 20) {
					printf("\n\t用户名过长，请重新输入:");
				}
				else if (strlen(arrCharTemp) == 1 && arrCharTemp[0] == '0') {
					system("cls");
					return;
				}
				else
					break;
			}
			strcpy(NewAccount, arrCharTemp);

			if (!strcmp(NewAccount, "\0")) {//用户名非空
				system("cls");
				printf("\n\t不能输入空字符！请重新输入\n");
				continue;
			}
			else {
				while (pNew != NULL && strcmp(NewAccount, pNew->userName))pNew = pNew->next;
				if (pNew == NULL)break;
				else {
					system("cls");
					printf("\n\t用户名重复，请提交新用户名\n");
					continue;//循环输入用户名
				}
			}
		}
		else if (n1 == 0) {
			system("cls");
			return;
		}
		else {
			system("cls");
			printf("\n\t输入的代码有误，请重新输入！\n");
		}
	}
	while (1)//注册管理员账户则需要安全码，注册普通账户按0跳过
	{
		printf("\n\t注册管理员账号请输入安全码(初始为5个1),注册普通账号请输入0：");
		gets(SecureNum);
		if (!strcmp(SecureNum, "0")) {//检测是否为0
			break;
		}
		else if (!strcmp(SecureNum, "\0"))//安全码非空
		{
			printf("\n\t不能输入空字符！请重新输入。\n");
			continue;
		}
		else if (!strcmp(SecureNum, secureWord)) {
			g = 0;//管理员账号等级为0级别
			break;
		}
		else printf("\n\t安全码错误！\n");
	}

	//创建密码并存储注册的新账号
	while (1) {
		printf("\n\t请在下方输入您要注册的密码(大于等于6个字符，小于等于20个字符)(按0返回)\n\t");
		char arrCharTemp[1000] = { 0 };
		gets(arrCharTemp);
		arrCharTemp[39] = '\0';
		if (strlen(arrCharTemp) > 20) {
			printf("\n\t密码过长，请重新输入。\n");
		}
		else if (strlen(arrCharTemp) == 1 && arrCharTemp[0] == '0') {
			system("cls");
			return;
		}
		else if (strlen(arrCharTemp) < 6)
			printf("\n\t密码过短,请重新输入。\n");
		else {
			strcpy(NewPassword, arrCharTemp);
			break;
		}
	}

	//均符合要求，接下来进行账号存储
	//账户信息初始化
	pNew = (struct CAccount*)malloc(sizeof(struct CAccount));//创建账户结构体
	pNew->next = NULL;
	strcpy_s(pNew->userName, 20, NewAccount);
	strcpy_s(pNew->password, 20, NewPassword);
	pNew->phoneNumber = 0;
	strcpy_s(pNew->Position, 100, "Not set");
	strcpy_s(pNew->RealName, 20, "Not set");
	pNew->grade = g;
	pNew->totalAmount = 0;
	pNew->discount = 10;
	//将链表连起来
	if (tailAccount == NULL) {//链表为空的特殊情况
		headAccount = pNew;
		tailAccount = pNew;
	}
	else {
		tailAccount->next = pNew;//链接
		tailAccount = pNew;
	}
	//将新建的账户结构体存储至账户文件

	FILE* fp = fopen("account.dat", "ab");
	fwrite(pNew, sizeof(struct CAccount), 1, fp);
	fclose(fp);

	printf("\n\t注册成功！按任意键键回到主界面\n");
	getchar();
	system("cls");
	return;//返回PrePage
}

const void InformationNotion() {
	system("cls");
	if (pLoggedInUser->grade != 0) {
		printf("\n\t***********************************   通   知   ***************************************\n");

		//
		printf("\n\t用户等级通知-------------------------------------");
		printf("\n\t您当前为%d级用户，购物可享受%.2f折优惠！\n\n", pLoggedInUser->grade + 1, pLoggedInUser->discount);

		//满减活动通知

		if (headFullDiscount->stageOfDiscoiunt == 1 && fabs(headFullDiscount->discount[0] - 10) < 1e-6) {}
		else {
			printf("\n\t满减活动通知-------------------------------------");
			printf("\n\t当前设置了%d个折扣阶梯。每个折扣阶梯对应的最低消费额和折扣率分别为：\n", headFullDiscount->stageOfDiscoiunt);
			int i = 0;
			while (i != headFullDiscount->stageOfDiscoiunt) {
				printf("\t%.2f元%.2f折\n", headFullDiscount->price[i], headFullDiscount->discount[i]);
				i++;
			}
			printf("\n\n");
		}

		//节假日活动通知
		if (headFestivalDiscount->discountSetted * 100 < 1000) {
			printf("\n\t节假日活动通知-----------------------------------");
			printf("\n\t当前已设置折扣活动！活动期间用户购买任意商品均可享受%.2f折", headFestivalDiscount->discountSetted);
			printf("\n\t活动开始时间为%4d年%2d月%2d日%2d时%2d分%2d秒\n", headFestivalDiscount->startTime.tm_year + 1900, headFestivalDiscount->startTime.tm_mon + 1,
				headFestivalDiscount->startTime.tm_mday, headFestivalDiscount->startTime.tm_hour, headFestivalDiscount->startTime.tm_min, headFestivalDiscount->startTime.tm_sec);
			printf("\t活动截止时间为%4d年%2d月%2d日%2d时%2d分%2d秒\n\n", headFestivalDiscount->deadline.tm_year + 1900, headFestivalDiscount->deadline.tm_mon + 1,
				headFestivalDiscount->deadline.tm_mday, headFestivalDiscount->deadline.tm_hour, headFestivalDiscount->deadline.tm_min, headFestivalDiscount->deadline.tm_sec);
		}

		//
		printf("\n\t---------------------------------------------------------------------------------------\n");
		printf("\n\t按任意键进入或返回主界面！");

		getchar();
	}
}

int MainPage() {
	LoadFestivalDiscountData();
	LoadFullDiscountData();
	LoadGoodsData();
	LoadOrderData();
	LoadTrolleyData();
	LoadGiftData();
	LoadGradeData();

	InformationNotion();

	int n;//用于保存用户的选择代码
	while (pLoggedInUser->grade == 0)
	{
		system("cls");
		//输出与接收代码
		printf("\n\t************************酒水销售管理****************************\n");
		printf("\n\t---------------------------主菜单-------------------------------\n");
		printf("\n\t**************-0--退出系统          退出登录--1-****************\n");
		printf("\n\t**************-2--商品浏览          账号管理--3-****************\n");
		printf("\n\t**************-4--订单管理          折扣管理--5-****************\n");
		printf("\n\t**************-5--赠品管理          销量预测--5-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		n = -1;
		scan(&n);

		ReSetTrolleyInformation();

		if (n == 2) {//进入商品界面
			GoodsFindAndChange();
			continue;
		}
		else if (n == 3)//进入账户管理界面
			AccountFindAndChange();
		else if (n == 4)//进入订单界面
			OrderFindAndChange();
		else if (n == 1) {//登出，即返回PrePage
			system("cls");
			releaseData();
			pLoggedInUser = NULL;
			return 0;
		}
		else if (n == 5) {
			OtherSetting();
		}
		else if (n == 0) {//退出系统
			releaseData();
			return 1;
		}
		else {
			system("cls");
			printf("\n\t输入的代码有误，请重新输入！\n");
		}
	}
	while (pLoggedInUser->grade > 0) {
		system("cls");
		//输出与接收代码
		printf("\n\t************************酒水销售管理****************************\n");
		printf("\n\t---------------------------主菜单-------------------------------\n");
		printf("\n\t**************-0--退出系统          退出登录--1-****************\n");
		printf("\n\t**************-2--商品浏览          账号管理--3-****************\n");
		printf("\n\t**************-4--订单管理          查看通知--5-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		scan(&n);

		ReSetTrolleyInformation();

		if (n == 2) {//进入商品界面
			GoodsFindAndChange();
			continue;
		}
		else if (n == 3)//进入账户管理界面
			AccountFindAndChange();
		else if (n == 4)//进入订单界面
			OrderFindAndChange();
		else if (n == 1) {//登出，即返回PrePage
			system("cls");
			releaseData();
			pLoggedInUser = NULL;
			return 0;
		}
		else if (n == 5) {
			InformationNotion();
		}
		else if (n == 0) {//退出系统
			releaseData();
			return 1;
		}
		else {
			system("cls");
			printf("\n\t输入的代码有误，请重新输入！\n");
		}
	}
	releaseData();
	return 1;
}

int PrePage() {
	system("color 70");
	int n1;//用于保存用户的选择代码
	while (1)
	{


		printf("\n\t************************酒水销售管理****************************\n");
		printf("\n\t---------------------------主菜单-------------------------------\n");
		printf("\n\t**************-0--退出                 退出--0-****************\n");
		printf("\n\t**************-1--登录账号          注册账号--2-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		n1 = -1;
		scan(&n1);
		if (n1 == 1) {
			if (accountLogin())//登录成功则break跳出循环，未登录则继续循环PrePage
				break;
		}
		else if (n1 == 2) {
			accountSignup();//注册账号
		}
		else if (n1 == 0) {
			return 1;
		}
		else {
			system("cls");
			printf("\n\t输入的代码有误，请重新输入！\n");
		}
	}
	system("cls");
	printf("\n\t登陆成功！\n");//登录成功退出PrePage，进入MainPage
	return 0;
}