#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include<math.h>
#include"account.h"
#include"head.h"


void AccountFindAndChange()
{
	int n1;
	if (pLoggedInUser->grade == 0)
	{
		while (1)
		{
			system("cls");
			printf("\n\t**************************账号板块**************************\n");
			printf("\n\t-----------------------------菜单---------------------------\n");
			printf("\t**************-0--退出                退出--0-**************\n");
			printf("\t********-1--修改账号密码          修改注册安全码--2-********\n");
			printf("\t********-3--查询客户信息            更改等级阶梯--4-********\n");
			printf("\n\t-------------------------------------------------------------\n");
			printf("\n\t请输入操作序号：");
			scan(&n1);
			if (n1 == 1)
			{
				PassWordChange();
			}
			else if (n1 == 2)
			{
				SecureWordChange();
			}
			else if (n1 == 3)
			{
				UserNameFind();
			}
			else if (n1 == 4)
			{
				GradeSet();
			}
			else if (n1 == 0)
			{
				return;
			}
			else{
				printf("\n\t输入的代码有误，请重新输入:");
			}//输入代码错
		}
	}
	else
	{
		while(1)
		{
			system("cls");
			printf("\n\t**************************账号板块**************************\n");
			printf("\n\t-----------------------------菜单---------------------------\n");
			printf("\t**************-0--退出                退出--0-**************\n");
			printf("\t********-1--修改账号密码             修改电话号码--2-********\n");
			printf("\t********-3--修改送货地址             修改送货地址--3-********\n");
			printf("\n\t-------------------------------------------------------------\n");
			printf("\n\t请输入操作序号：");
			n1 = -1;
			scan(&n1);
			if (n1 == 0)
			{
				return;
			}
			else if (n1 == 1)
			{
				PassWordChange();
			}
			else if (n1 == 2)
			{
				PhoneNumberChange();
			}
			else if (n1 == 3)
			{
				PositionChange();
			}
			else
			{
				printf("\n\t输入的代码有误，请重新输入:");
			}
		}
	}
}


void PassWordChange()//更改密码函数
{
	char newpassword[40];
	char PassAccount[40];//用户输入密码
	int n1;
	while (1)
	{
		system("cls");
		printf("\n\t**************************密码修改**************************\n");
		printf("\n\t-----------------------------菜单---------------------------\n");
		printf("\t********-0--退出                          退出--0-********\n");
		printf("\t********-1--修改账号密码            修改账号密码--1-*********\n");
		printf("\n\t------------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		scan(&n1);
		if (n1 == 0){
			return;
		}
		else if (n1 == 1)
		{
			while(1)
			{
				printf("\n\t请输入原密码(输入0返回)：");

				gets_s(PassAccount, 40);
				if (strlen(PassAccount) == 1 && PassAccount[0] == '0')
					break;
				else if (strcmp(pLoggedInUser->password, PassAccount)) //密码比对错误
					printf("\n\t密码错误");
				else {
					printf("\n\t请输入新密码(输入0返回)：");
					while (1)
					{
						gets_s(newpassword, 40);
						if (strlen(newpassword) > 20)
							printf("\n\t密码过长，请重新输入:");
						else if (strlen(newpassword) == 1 && newpassword[0] == '0')
							continue;
						else if (strlen(newpassword) < 6)
							printf("\n\t密码过短,请重新输入:");
						else
						{
							strcpy(pLoggedInUser->password, newpassword);
							SaveAccountData();
							printf("\n\t密码修改成功！");
							getchar();
							return;
						}
					}
				}
			}
		}
		else{
			printf("\t输入操作指令错误，请重新输入：");
			Sleep(500);
		}
	}
}


void SecureWordChange() // 安全码更改函数
{
	char secureNum[20];
	char secureWord[20];
	char newsecureNum[20];
	int n1;
	FILE* fp;
	fp = fopen("secureWord.dat", "rb");
	if (fp == NULL) {
		system("cls");
		printf("\n\t无法打开文件 secureWord.dat");
		Sleep(500);
		return;
	}
	fread(secureWord, sizeof(char), 20, fp);
	fclose(fp); // 文件读取完毕后关闭文件指针

	while (1)
	{
		system("cls");
		printf("\n\t**************************安全码修改**************************\n");
		printf("\n\t-----------------------------菜单----------------------------\n");
		printf("\t********-0--退出                          退出--0-********\n");
		printf("\t********-1--修改安全码               修改安全码--1-*********\n");
		printf("\n\t--------------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		scan(&n1);
		if (n1 == 0)
			return;
		else if (n1 == 1)
		{
			printf("\n\t请输入原安全码(输入0返回)：");
			while (1)
			{
				gets(secureNum);

				if (strcmp(secureNum, "0") == 0)
					break;
				else if (strcmp(secureWord, secureNum)) // 安全码比对错误
					printf("\n\t安全码错误,请重新输入：");
				else{
					printf("\n\t请输入新安全码(输入0返回)：");
					gets(newsecureNum);
					if (strcmp(newsecureNum, "0") == 0)
						break;

					int len = strlen(newsecureNum);
					if (len > 20) len = 20; // 最多存储20个字符的安全码
					memset(secureWord, 0, sizeof(secureWord)); // 清空安全码数组
					strncpy(secureWord, newsecureNum, len); // 复制新安全码到安全码数组
					fp = fopen("secureWord.dat", "wb"); // 以二进制写入模式打开文件
					if (fp == NULL) {
						printf("\n\t无法打开文件 secureWord.dat\n");
						return;
					}
					fwrite(secureWord, sizeof(char), 20, fp); // 写入20个字符，即文件中最多存储20个字符的安全码
					fclose(fp);

					printf("\t安全码修改成功！按任意键返回\n");
					getchar();
					return;
				}
			}
		}
		else{
			printf("\t输入错误，请重新输入：");
			Sleep(500);
		}
	}
}


void UserNameFind()
{
	char NameAccount[20];
	struct CAccount* pNow = headAccount;
	int n1;

	while (1)
	{
		system("cls");
		printf("\n\t**********************客户信息查询************************\n");
		printf("\n\t---------------------------菜单---------------------------\n");
		printf("\t********-0--退出                           退出--0-********\n");
		printf("\t********-1--查找账户用户名     显示所有客户信息--2-********\n");
		printf("\n\t----------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		scan(&n1);
		if (n1 == 0)
			return;
		else if (n1 == 1) {
			printf("\n\t请输入要查询的账户用户名(输入0返回)：");
			fgets(NameAccount, sizeof(NameAccount), stdin);

			NameAccount[strcspn(NameAccount, "\n")] = '\0';
			system("cls");
			if (strcmp(NameAccount, "0") == 0)
				continue;
			if (NameAccount[0] != '\0') {
				pNow = headAccount;
				while (pNow != NULL && strcmp(pNow->userName, NameAccount) != 0) {
					pNow = pNow->next;
				}
				if (pNow == NULL) {
					printf("\n\t账号不存在！\n");
					Sleep(500);
				}
				else {
					printf("\t%-15s %-15s %-15s %-15s %-15s\n", "用户名", "姓名", "手机号", "总消费额", "消费等级");
					printf("\t--------------------------------------------------------------------------\n");
					printf("\t%-15s %-15s %-15lld %-15.2f %-15d\n", pNow->userName, pNow->RealName, pNow->phoneNumber, pNow->totalAmount, pNow->grade);
				}
			}
			printf("\n\t已显示账户信息，按任意键返回：");
			getchar();
			continue;
		}
		else if (n1 == 2) {
			system("cls");
			printf("\t%-15s %-15s %-15s %-15s %-15s\n", "用户名", "姓名", "手机号", "总消费额", "消费等级");
			printf("\t--------------------------------------------------------------------------\n");
			pNow = headAccount;
			while (pNow != NULL) {
				printf("\t%-15s %-15s %-15lld %-15.2f %-15d\n", pNow->userName, pNow->RealName, pNow->phoneNumber, pNow->totalAmount, pNow->grade);
				pNow = pNow->next;
			}
			printf("\n\t已显示账户信息，按任意键返回：");
			getchar();
			continue;
		}
		else {
			printf("\n\t输入错误，请重新输入:");
			Sleep(500);
		}
	}
}


void GradeSet()
{
	int n1 = -1, num = -1;
	float price[5] = { -1 };
	float discount[5] = { -1, -1, -1, -1, -1 };
	int grade = 0;
	int valid = 0;
	int i = 0;

	while (1)
	{
		system("cls");
		printf("\n\t***********************更改等级阶梯************************\n");
		printf("\n\t---------------------------菜单---------------------------\n");
		printf("\t********-0--退出                        退出--0-********\n");
		printf("\t********-1--设置等级阈值          设置等级阈值--1-*********\n");
		printf("\n\t----------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		scan(&n1);
		if (n1 == 0)
			return;
		else if (n1 == 1)
		{
			while (1)
			{
				if (headGrade->stageOfGrade == 1 && headGrade->price[0] == 10)
				{
					printf("\n\t您当前还未设置任何等级阈值！输入[0]返回，输入其他正整数设置等级阈值：");
					int n1 = -1;
					scan(&n1);
					if (n1 == 0)
						break;
				}
				else
				{
					printf("\n\t您当前设置了%d个折扣阶梯。每个折扣阶梯对应的最低消费额和折扣率分别为：\n", headGrade->stageOfGrade);
					int i = 0;
					while (i != headGrade->stageOfGrade)
					{
						printf("\t%.2f元%.2f折", headGrade->price[i], headGrade->discount[i]);
						i++;
					}
					printf("\n\t输入[0]返回，输入其他正整数设置等级阈值：");
					int n1 = -1;
					scan(&n1);
					if (n1 == 0)
						break;
				}
				printf("\n\t请输入要设置的等级次数，最多支持5个用户等级次数，输入[0]返回：");
				while (1)
				{
					scan(&num);
					if (num >= 1 && num <= 5)
					{
						headGrade->stageOfGrade = num;
						break;
					}
					else if (num == 0)
						break;
					else
						printf("\n\t您输入的有误，请重新输入：");
				}
				while (1)
				{
					for (i = 0; i < headGrade->stageOfGrade; i++)
					{
						printf("\n\t请输入第%d层折扣的最低消费额和相应折扣，用空格隔开，如\"800 9\"，表示折扣前订单总价满800元可享受9折：", i + 1);
						valid = 1;
						while (valid) {
							if (scanf("%f %f", &price[i], &discount[i]) != 2) {
								printf("\t输入无效！请重新输入：");
								while (getchar() != '\n');
							}
							else {
								if (price[i] <= 0 || discount[i] <= 0 || discount[i] >= 10) {
									printf("\t最低购买金额和折扣必须大于0且小于10！请重新输入：");
								}
								else{
									valid = 0;
								}
							}
						}
					}
					i--;
					while (i != 0)
					{
						if (price[i] - price[i - 1] < 0 || discount[i] - discount[i - 1] > 0)
						{
							break;
						}
						i--;
					}
					if (i != 0) {
						printf("\n\t第n层折扣力度必须大于第n-1层，第n层折扣最低消费额必须大于第n-1层！\n");
						printf("\n\t请重新设置：");
						continue;
					}
					for (i = 0; i < num; i++)
					{
						headGrade->discount[i] = discount[i];
						headGrade->price[i] = price[i];
					}
					break;
				}
				system("cls");
				printf("\n\t您已成功设置%d个折扣阶梯。每个折扣阶梯对应的最低消费额和折扣率分别为：", headGrade->stageOfGrade);
				i = 0;
				while (i != headGrade->stageOfGrade)
				{
					printf("\t%.2f元%.2f折", headGrade->price[i], headGrade->discount[i]);
					i++;
				}

				FILE* fp = fopen("grade.dat", "wb");
				fwrite(headGrade, sizeof(struct CGrade), 1, fp);
				fclose(fp);
				getchar();
				printf("\n\t设置成功！输入任意键返回");
				getchar();
				return;
			}
		}
		else{
			printf("\n\t输入错误 请重新输入：");
			Sleep(500);
		}
	}
}



void PhoneNumberChange()
{
	long long newPhoneNumber;
	char PassAccount[40]; // 用户输入密码
	int n1;

	while (1)
	{
		system("cls");
		printf("\n\t************************电话号码修改***********************\n");
		printf("\n\t----------------------------菜单---------------------------\n");
		printf("\t********-0--退出                        退出--0-********\n");
		printf("\t********-1--修改电话号码        修改电话号码--1-********\n");
		printf("\n\t----------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		scan(&n1);
		if (n1 == 0){
			return;
		}
		else if (n1 == 1)
		{
			printf("\n\t请输入密码(输入0返回)：");

			gets_s(PassAccount, 20);

			if (strlen(PassAccount) == 1 && PassAccount[0] == '0') {
				continue;
			}
			else if (strcmp(pLoggedInUser->password, PassAccount)){ // 密码比对错误
				printf("\n\t密码错误,请输入正确密码:");
				Sleep(500);
			}
			else {
				printf("\n\t请输入新电话号码(输入0返回)：");
				while (1)
				{
					scanf_s("%lld", &newPhoneNumber);
					getchar();
					char str[40];
					sprintf(str, "%lld", newPhoneNumber);
					int len = strlen(str);
					int flag = 1; // 标志位，初始值为1，表示电话号码符合要求
					if (len != 11) // 判断电话号码位数是否为11
					{
						flag = 0;
						if (newPhoneNumber == 0)
						{
							system("cls");
							break;
						}
					}
					else{
						for (int i = 0; i < len; i++) // 判断电话号码是否都是数字
						{
							if (str[i] < '0' || str[i] > '9')
							{
								flag = 0;
								break;
							}
						}
					}
					if (flag){
						pLoggedInUser->phoneNumber = newPhoneNumber;
						SaveAccountData();

						printf("\n\t电话号码修改成功！按任意键返回");
						getchar();
						break;
					}
					else
						printf("\n\t电话号码不符合要求，请重新输入:");
				}
			}
		}
		else{
			printf("\n\t输入操作指令错误，请重新输入：");
			Sleep(500);
		}
	}
}


void PositionChange()
{
	char newPosition[100];
	char PassAccount[20]; // 用户输入密码
	struct CAccount* temp = headAccount;
	int n1;

	while (1)
	{
		system("cls");
		printf("\n\t************************修改收货地址***********************\n");
		printf("\n\t----------------------------菜单---------------------------\n");
		printf("\t********-0--退出                        退出--0-*********\n");
		printf("\t********-1--修改收货地址         修改收货地址--1-*********\n");
		printf("\n\t----------------------------------------------------------\n");
		printf("\n\t请输入操作序号：");
		scan(&n1);
		if (n1 == 0){
			return;
		}
		else if (n1 == 1)
		{
			printf("\t请输入密码(输入0返回)：");
			gets_s(PassAccount, 20);

			if (strlen(PassAccount) == 1 && PassAccount[0] == '0')
				continue;
			else if (strcmp(pLoggedInUser->password, PassAccount)) // 密码比对错误
				printf("\n\t密码错误,请输入正确密码:");
			else {
				printf("\n\t请输入新的收货地址(输入0返回)：");
				gets_s(newPosition, 100);
				if (strcmp(newPosition, "0") == 0)
					continue;
				else {
					strcpy(pLoggedInUser->Position, newPosition);
					SaveAccountData();
					printf("\n\t收货地址修改成功！按任意键返回");
					getchar();
					return;
				}
			}
		}
		else{
			printf("\n\t输入操作指令错误，请重新输入：");
			Sleep(500);
		}
	}
}


void GradeChange()
{
	int gradeLevel = 1;
	while (gradeLevel < headGrade->stageOfGrade && pLoggedInUser->totalAmount >= headGrade->price[gradeLevel])
	{
		gradeLevel++;
	}
	pLoggedInUser->grade = gradeLevel;
	pLoggedInUser->discount = headGrade->discount[gradeLevel - 1];
	SaveAccountData();
}