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
			printf("\n\t**************************�˺Ű��**************************\n");
			printf("\n\t-----------------------------�˵�---------------------------\n");
			printf("\t**************-0--�˳�                �˳�--0-**************\n");
			printf("\t********-1--�޸��˺�����          �޸�ע�ᰲȫ��--2-********\n");
			printf("\t********-3--��ѯ�ͻ���Ϣ            ���ĵȼ�����--4-********\n");
			printf("\n\t-------------------------------------------------------------\n");
			printf("\n\t�����������ţ�");
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
				printf("\n\t����Ĵ�����������������:");
			}//��������
		}
	}
	else
	{
		while(1)
		{
			system("cls");
			printf("\n\t**************************�˺Ű��**************************\n");
			printf("\n\t-----------------------------�˵�---------------------------\n");
			printf("\t**************-0--�˳�                �˳�--0-**************\n");
			printf("\t********-1--�޸��˺�����             �޸ĵ绰����--2-********\n");
			printf("\t********-3--�޸��ͻ���ַ             �޸��ͻ���ַ--3-********\n");
			printf("\n\t-------------------------------------------------------------\n");
			printf("\n\t�����������ţ�");
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
				printf("\n\t����Ĵ�����������������:");
			}
		}
	}
}


void PassWordChange()//�������뺯��
{
	char newpassword[40];
	char PassAccount[40];//�û���������
	int n1;
	while (1)
	{
		system("cls");
		printf("\n\t**************************�����޸�**************************\n");
		printf("\n\t-----------------------------�˵�---------------------------\n");
		printf("\t********-0--�˳�                          �˳�--0-********\n");
		printf("\t********-1--�޸��˺�����            �޸��˺�����--1-*********\n");
		printf("\n\t------------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
		scan(&n1);
		if (n1 == 0){
			return;
		}
		else if (n1 == 1)
		{
			while(1)
			{
				printf("\n\t������ԭ����(����0����)��");

				gets_s(PassAccount, 40);
				if (strlen(PassAccount) == 1 && PassAccount[0] == '0')
					break;
				else if (strcmp(pLoggedInUser->password, PassAccount)) //����ȶԴ���
					printf("\n\t�������");
				else {
					printf("\n\t������������(����0����)��");
					while (1)
					{
						gets_s(newpassword, 40);
						if (strlen(newpassword) > 20)
							printf("\n\t�������������������:");
						else if (strlen(newpassword) == 1 && newpassword[0] == '0')
							continue;
						else if (strlen(newpassword) < 6)
							printf("\n\t�������,����������:");
						else
						{
							strcpy(pLoggedInUser->password, newpassword);
							SaveAccountData();
							printf("\n\t�����޸ĳɹ���");
							getchar();
							return;
						}
					}
				}
			}
		}
		else{
			printf("\t�������ָ��������������룺");
			Sleep(500);
		}
	}
}


void SecureWordChange() // ��ȫ����ĺ���
{
	char secureNum[20];
	char secureWord[20];
	char newsecureNum[20];
	int n1;
	FILE* fp;
	fp = fopen("secureWord.dat", "rb");
	if (fp == NULL) {
		system("cls");
		printf("\n\t�޷����ļ� secureWord.dat");
		Sleep(500);
		return;
	}
	fread(secureWord, sizeof(char), 20, fp);
	fclose(fp); // �ļ���ȡ��Ϻ�ر��ļ�ָ��

	while (1)
	{
		system("cls");
		printf("\n\t**************************��ȫ���޸�**************************\n");
		printf("\n\t-----------------------------�˵�----------------------------\n");
		printf("\t********-0--�˳�                          �˳�--0-********\n");
		printf("\t********-1--�޸İ�ȫ��               �޸İ�ȫ��--1-*********\n");
		printf("\n\t--------------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
		scan(&n1);
		if (n1 == 0)
			return;
		else if (n1 == 1)
		{
			printf("\n\t������ԭ��ȫ��(����0����)��");
			while (1)
			{
				gets(secureNum);

				if (strcmp(secureNum, "0") == 0)
					break;
				else if (strcmp(secureWord, secureNum)) // ��ȫ��ȶԴ���
					printf("\n\t��ȫ�����,���������룺");
				else{
					printf("\n\t�������°�ȫ��(����0����)��");
					gets(newsecureNum);
					if (strcmp(newsecureNum, "0") == 0)
						break;

					int len = strlen(newsecureNum);
					if (len > 20) len = 20; // ���洢20���ַ��İ�ȫ��
					memset(secureWord, 0, sizeof(secureWord)); // ��հ�ȫ������
					strncpy(secureWord, newsecureNum, len); // �����°�ȫ�뵽��ȫ������
					fp = fopen("secureWord.dat", "wb"); // �Զ�����д��ģʽ���ļ�
					if (fp == NULL) {
						printf("\n\t�޷����ļ� secureWord.dat\n");
						return;
					}
					fwrite(secureWord, sizeof(char), 20, fp); // д��20���ַ������ļ������洢20���ַ��İ�ȫ��
					fclose(fp);

					printf("\t��ȫ���޸ĳɹ��������������\n");
					getchar();
					return;
				}
			}
		}
		else{
			printf("\t����������������룺");
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
		printf("\n\t**********************�ͻ���Ϣ��ѯ************************\n");
		printf("\n\t---------------------------�˵�---------------------------\n");
		printf("\t********-0--�˳�                           �˳�--0-********\n");
		printf("\t********-1--�����˻��û���     ��ʾ���пͻ���Ϣ--2-********\n");
		printf("\n\t----------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
		scan(&n1);
		if (n1 == 0)
			return;
		else if (n1 == 1) {
			printf("\n\t������Ҫ��ѯ���˻��û���(����0����)��");
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
					printf("\n\t�˺Ų����ڣ�\n");
					Sleep(500);
				}
				else {
					printf("\t%-15s %-15s %-15s %-15s %-15s\n", "�û���", "����", "�ֻ���", "�����Ѷ�", "���ѵȼ�");
					printf("\t--------------------------------------------------------------------------\n");
					printf("\t%-15s %-15s %-15lld %-15.2f %-15d\n", pNow->userName, pNow->RealName, pNow->phoneNumber, pNow->totalAmount, pNow->grade);
				}
			}
			printf("\n\t����ʾ�˻���Ϣ������������أ�");
			getchar();
			continue;
		}
		else if (n1 == 2) {
			system("cls");
			printf("\t%-15s %-15s %-15s %-15s %-15s\n", "�û���", "����", "�ֻ���", "�����Ѷ�", "���ѵȼ�");
			printf("\t--------------------------------------------------------------------------\n");
			pNow = headAccount;
			while (pNow != NULL) {
				printf("\t%-15s %-15s %-15lld %-15.2f %-15d\n", pNow->userName, pNow->RealName, pNow->phoneNumber, pNow->totalAmount, pNow->grade);
				pNow = pNow->next;
			}
			printf("\n\t����ʾ�˻���Ϣ������������أ�");
			getchar();
			continue;
		}
		else {
			printf("\n\t�����������������:");
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
		printf("\n\t***********************���ĵȼ�����************************\n");
		printf("\n\t---------------------------�˵�---------------------------\n");
		printf("\t********-0--�˳�                        �˳�--0-********\n");
		printf("\t********-1--���õȼ���ֵ          ���õȼ���ֵ--1-*********\n");
		printf("\n\t----------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
		scan(&n1);
		if (n1 == 0)
			return;
		else if (n1 == 1)
		{
			while (1)
			{
				if (headGrade->stageOfGrade == 1 && headGrade->price[0] == 10)
				{
					printf("\n\t����ǰ��δ�����κεȼ���ֵ������[0]���أ������������������õȼ���ֵ��");
					int n1 = -1;
					scan(&n1);
					if (n1 == 0)
						break;
				}
				else
				{
					printf("\n\t����ǰ������%d���ۿ۽��ݡ�ÿ���ۿ۽��ݶ�Ӧ��������Ѷ���ۿ��ʷֱ�Ϊ��\n", headGrade->stageOfGrade);
					int i = 0;
					while (i != headGrade->stageOfGrade)
					{
						printf("\t%.2fԪ%.2f��", headGrade->price[i], headGrade->discount[i]);
						i++;
					}
					printf("\n\t����[0]���أ������������������õȼ���ֵ��");
					int n1 = -1;
					scan(&n1);
					if (n1 == 0)
						break;
				}
				printf("\n\t������Ҫ���õĵȼ����������֧��5���û��ȼ�����������[0]���أ�");
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
						printf("\n\t��������������������룺");
				}
				while (1)
				{
					for (i = 0; i < headGrade->stageOfGrade; i++)
					{
						printf("\n\t�������%d���ۿ۵�������Ѷ����Ӧ�ۿۣ��ÿո��������\"800 9\"����ʾ�ۿ�ǰ�����ܼ���800Ԫ������9�ۣ�", i + 1);
						valid = 1;
						while (valid) {
							if (scanf("%f %f", &price[i], &discount[i]) != 2) {
								printf("\t������Ч�����������룺");
								while (getchar() != '\n');
							}
							else {
								if (price[i] <= 0 || discount[i] <= 0 || discount[i] >= 10) {
									printf("\t��͹�������ۿ۱������0��С��10�����������룺");
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
						printf("\n\t��n���ۿ����ȱ�����ڵ�n-1�㣬��n���ۿ�������Ѷ������ڵ�n-1�㣡\n");
						printf("\n\t���������ã�");
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
				printf("\n\t���ѳɹ�����%d���ۿ۽��ݡ�ÿ���ۿ۽��ݶ�Ӧ��������Ѷ���ۿ��ʷֱ�Ϊ��", headGrade->stageOfGrade);
				i = 0;
				while (i != headGrade->stageOfGrade)
				{
					printf("\t%.2fԪ%.2f��", headGrade->price[i], headGrade->discount[i]);
					i++;
				}

				FILE* fp = fopen("grade.dat", "wb");
				fwrite(headGrade, sizeof(struct CGrade), 1, fp);
				fclose(fp);
				getchar();
				printf("\n\t���óɹ����������������");
				getchar();
				return;
			}
		}
		else{
			printf("\n\t������� ���������룺");
			Sleep(500);
		}
	}
}



void PhoneNumberChange()
{
	long long newPhoneNumber;
	char PassAccount[40]; // �û���������
	int n1;

	while (1)
	{
		system("cls");
		printf("\n\t************************�绰�����޸�***********************\n");
		printf("\n\t----------------------------�˵�---------------------------\n");
		printf("\t********-0--�˳�                        �˳�--0-********\n");
		printf("\t********-1--�޸ĵ绰����        �޸ĵ绰����--1-********\n");
		printf("\n\t----------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
		scan(&n1);
		if (n1 == 0){
			return;
		}
		else if (n1 == 1)
		{
			printf("\n\t����������(����0����)��");

			gets_s(PassAccount, 20);

			if (strlen(PassAccount) == 1 && PassAccount[0] == '0') {
				continue;
			}
			else if (strcmp(pLoggedInUser->password, PassAccount)){ // ����ȶԴ���
				printf("\n\t�������,��������ȷ����:");
				Sleep(500);
			}
			else {
				printf("\n\t�������µ绰����(����0����)��");
				while (1)
				{
					scanf_s("%lld", &newPhoneNumber);
					getchar();
					char str[40];
					sprintf(str, "%lld", newPhoneNumber);
					int len = strlen(str);
					int flag = 1; // ��־λ����ʼֵΪ1����ʾ�绰�������Ҫ��
					if (len != 11) // �жϵ绰����λ���Ƿ�Ϊ11
					{
						flag = 0;
						if (newPhoneNumber == 0)
						{
							system("cls");
							break;
						}
					}
					else{
						for (int i = 0; i < len; i++) // �жϵ绰�����Ƿ�������
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

						printf("\n\t�绰�����޸ĳɹ��������������");
						getchar();
						break;
					}
					else
						printf("\n\t�绰���벻����Ҫ������������:");
				}
			}
		}
		else{
			printf("\n\t�������ָ��������������룺");
			Sleep(500);
		}
	}
}


void PositionChange()
{
	char newPosition[100];
	char PassAccount[20]; // �û���������
	struct CAccount* temp = headAccount;
	int n1;

	while (1)
	{
		system("cls");
		printf("\n\t************************�޸��ջ���ַ***********************\n");
		printf("\n\t----------------------------�˵�---------------------------\n");
		printf("\t********-0--�˳�                        �˳�--0-*********\n");
		printf("\t********-1--�޸��ջ���ַ         �޸��ջ���ַ--1-*********\n");
		printf("\n\t----------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
		scan(&n1);
		if (n1 == 0){
			return;
		}
		else if (n1 == 1)
		{
			printf("\t����������(����0����)��");
			gets_s(PassAccount, 20);

			if (strlen(PassAccount) == 1 && PassAccount[0] == '0')
				continue;
			else if (strcmp(pLoggedInUser->password, PassAccount)) // ����ȶԴ���
				printf("\n\t�������,��������ȷ����:");
			else {
				printf("\n\t�������µ��ջ���ַ(����0����)��");
				gets_s(newPosition, 100);
				if (strcmp(newPosition, "0") == 0)
					continue;
				else {
					strcpy(pLoggedInUser->Position, newPosition);
					SaveAccountData();
					printf("\n\t�ջ���ַ�޸ĳɹ��������������");
					getchar();
					return;
				}
			}
		}
		else{
			printf("\n\t�������ָ��������������룺");
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