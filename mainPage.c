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
	char NameAccount[40];//�û�������û���
	char PassAccount[40];//�û����������
	struct CAccount* pNow = headAccount;//���ڱȶ��Ե�½�˺�
	system("cls");
	while (1) {
		//�����û����׶�
		printf("\n\t************************  ��¼ϵͳ  ****************************\n");
		printf("\n\t---------------------------���˵�-------------------------------\n");
		printf("\n\t**************-0--          ����           --0-****************\n");
		printf("\n\t**************----   �����˺����Լ�����¼   ----****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t������(������20���ַ�)��");
		gets(NameAccount);
		if (strlen(NameAccount) == 1 && NameAccount[0] == '0') {
			system("cls");
			return 0;
		}
		pNow = headAccount;
		while (pNow != NULL && strcmp(pNow->userName, NameAccount))//�����û�����
			pNow = pNow->next;
		if (pNow == NULL) {//����β�������ڴ��û�
			system("cls");
			printf("\n\t�û��������ڣ�\n");
		}
		else {//�û����ȶԳɹ�������������׶�
			while (1) {
				printf("\n\t��������ȷ���루��0�ص������棩��");
				gets(PassAccount);
				if (strlen(PassAccount) == 1 && PassAccount[0] == '0') {
					system("cls");
					return 0;
				}
				if (strcmp(pNow->password, PassAccount)) //����ȶԴ���
					printf("\n\t�������\n");
				else {
					pLoggedInUser = pNow; //��¼�ɹ����أ�����ȫ��ָ��ָ���¼��
					return 1;
				}
			}
		}
	}
}

void accountSignup() {
	int g = 1;//��ʾ�˺ŵȼ�
	char NewAccount[40];
	char SecureNum[40];
	char NewPassword[40];
	char secureWord[40];
	struct CAccount* pNew = headAccount;
	//��ȡ��ȫ��
	FILE* fp_ = fopen("secureWord.dat", "rb");
	fread(secureWord, sizeof(char), 10, fp_);
	fclose(fp_);
	system("cls");
	while (1)//�����û������ȶ��Ƿ�����ظ��û���
	{
		printf("\n\t************************  ע��ϵͳ  ****************************\n");
		printf("\n\t---------------------------���˵�-------------------------------\n");
		printf("\n\t**************-0--          ����           --0-****************\n");
		printf("\n\t**************-1--   �����˺����Լ���ע��   --1-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t�����룺");
		int n1;
		n1 = -1;
		scan(&n1);
		if (n1 == 1) {
			printf("\n\t�����·�������Ҫע����û���(С�ڵ���20���ַ����û�������Ϊ0)(��0����)\n\t");
			char arrCharTemp[40] = { 0 };
			while (1) {
				gets(arrCharTemp);
				arrCharTemp[39] = '\0';
				if (strlen(arrCharTemp) > 20) {
					printf("\n\t�û�������������������:");
				}
				else if (strlen(arrCharTemp) == 1 && arrCharTemp[0] == '0') {
					system("cls");
					return;
				}
				else
					break;
			}
			strcpy(NewAccount, arrCharTemp);

			if (!strcmp(NewAccount, "\0")) {//�û����ǿ�
				system("cls");
				printf("\n\t����������ַ�������������\n");
				continue;
			}
			else {
				while (pNew != NULL && strcmp(NewAccount, pNew->userName))pNew = pNew->next;
				if (pNew == NULL)break;
				else {
					system("cls");
					printf("\n\t�û����ظ������ύ���û���\n");
					continue;//ѭ�������û���
				}
			}
		}
		else if (n1 == 0) {
			system("cls");
			return;
		}
		else {
			system("cls");
			printf("\n\t����Ĵ����������������룡\n");
		}
	}
	while (1)//ע�����Ա�˻�����Ҫ��ȫ�룬ע����ͨ�˻���0����
	{
		printf("\n\tע�����Ա�˺������밲ȫ��(��ʼΪ5��1),ע����ͨ�˺�������0��");
		gets(SecureNum);
		if (!strcmp(SecureNum, "0")) {//����Ƿ�Ϊ0
			break;
		}
		else if (!strcmp(SecureNum, "\0"))//��ȫ��ǿ�
		{
			printf("\n\t����������ַ������������롣\n");
			continue;
		}
		else if (!strcmp(SecureNum, secureWord)) {
			g = 0;//����Ա�˺ŵȼ�Ϊ0����
			break;
		}
		else printf("\n\t��ȫ�����\n");
	}

	//�������벢�洢ע������˺�
	while (1) {
		printf("\n\t�����·�������Ҫע�������(���ڵ���6���ַ���С�ڵ���20���ַ�)(��0����)\n\t");
		char arrCharTemp[1000] = { 0 };
		gets(arrCharTemp);
		arrCharTemp[39] = '\0';
		if (strlen(arrCharTemp) > 20) {
			printf("\n\t������������������롣\n");
		}
		else if (strlen(arrCharTemp) == 1 && arrCharTemp[0] == '0') {
			system("cls");
			return;
		}
		else if (strlen(arrCharTemp) < 6)
			printf("\n\t�������,���������롣\n");
		else {
			strcpy(NewPassword, arrCharTemp);
			break;
		}
	}

	//������Ҫ�󣬽����������˺Ŵ洢
	//�˻���Ϣ��ʼ��
	pNew = (struct CAccount*)malloc(sizeof(struct CAccount));//�����˻��ṹ��
	pNew->next = NULL;
	strcpy_s(pNew->userName, 20, NewAccount);
	strcpy_s(pNew->password, 20, NewPassword);
	pNew->phoneNumber = 0;
	strcpy_s(pNew->Position, 100, "Not set");
	strcpy_s(pNew->RealName, 20, "Not set");
	pNew->grade = g;
	pNew->totalAmount = 0;
	pNew->discount = 10;
	//������������
	if (tailAccount == NULL) {//����Ϊ�յ��������
		headAccount = pNew;
		tailAccount = pNew;
	}
	else {
		tailAccount->next = pNew;//����
		tailAccount = pNew;
	}
	//���½����˻��ṹ��洢���˻��ļ�

	FILE* fp = fopen("account.dat", "ab");
	fwrite(pNew, sizeof(struct CAccount), 1, fp);
	fclose(fp);

	printf("\n\tע��ɹ�������������ص�������\n");
	getchar();
	system("cls");
	return;//����PrePage
}

const void InformationNotion() {
	system("cls");
	if (pLoggedInUser->grade != 0) {
		printf("\n\t***********************************   ͨ   ֪   ***************************************\n");

		//
		printf("\n\t�û��ȼ�֪ͨ-------------------------------------");
		printf("\n\t����ǰΪ%d���û������������%.2f���Żݣ�\n\n", pLoggedInUser->grade + 1, pLoggedInUser->discount);

		//�����֪ͨ

		if (headFullDiscount->stageOfDiscoiunt == 1 && fabs(headFullDiscount->discount[0] - 10) < 1e-6) {}
		else {
			printf("\n\t�����֪ͨ-------------------------------------");
			printf("\n\t��ǰ������%d���ۿ۽��ݡ�ÿ���ۿ۽��ݶ�Ӧ��������Ѷ���ۿ��ʷֱ�Ϊ��\n", headFullDiscount->stageOfDiscoiunt);
			int i = 0;
			while (i != headFullDiscount->stageOfDiscoiunt) {
				printf("\t%.2fԪ%.2f��\n", headFullDiscount->price[i], headFullDiscount->discount[i]);
				i++;
			}
			printf("\n\n");
		}

		//�ڼ��ջ֪ͨ
		if (headFestivalDiscount->discountSetted * 100 < 1000) {
			printf("\n\t�ڼ��ջ֪ͨ-----------------------------------");
			printf("\n\t��ǰ�������ۿۻ����ڼ��û�����������Ʒ��������%.2f��", headFestivalDiscount->discountSetted);
			printf("\n\t���ʼʱ��Ϊ%4d��%2d��%2d��%2dʱ%2d��%2d��\n", headFestivalDiscount->startTime.tm_year + 1900, headFestivalDiscount->startTime.tm_mon + 1,
				headFestivalDiscount->startTime.tm_mday, headFestivalDiscount->startTime.tm_hour, headFestivalDiscount->startTime.tm_min, headFestivalDiscount->startTime.tm_sec);
			printf("\t���ֹʱ��Ϊ%4d��%2d��%2d��%2dʱ%2d��%2d��\n\n", headFestivalDiscount->deadline.tm_year + 1900, headFestivalDiscount->deadline.tm_mon + 1,
				headFestivalDiscount->deadline.tm_mday, headFestivalDiscount->deadline.tm_hour, headFestivalDiscount->deadline.tm_min, headFestivalDiscount->deadline.tm_sec);
		}

		//
		printf("\n\t---------------------------------------------------------------------------------------\n");
		printf("\n\t�����������򷵻������棡");

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

	int n;//���ڱ����û���ѡ�����
	while (pLoggedInUser->grade == 0)
	{
		system("cls");
		//�������մ���
		printf("\n\t************************��ˮ���۹���****************************\n");
		printf("\n\t---------------------------���˵�-------------------------------\n");
		printf("\n\t**************-0--�˳�ϵͳ          �˳���¼--1-****************\n");
		printf("\n\t**************-2--��Ʒ���          �˺Ź���--3-****************\n");
		printf("\n\t**************-4--��������          �ۿ۹���--5-****************\n");
		printf("\n\t**************-5--��Ʒ����          ����Ԥ��--5-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
		n = -1;
		scan(&n);

		ReSetTrolleyInformation();

		if (n == 2) {//������Ʒ����
			GoodsFindAndChange();
			continue;
		}
		else if (n == 3)//�����˻��������
			AccountFindAndChange();
		else if (n == 4)//���붩������
			OrderFindAndChange();
		else if (n == 1) {//�ǳ���������PrePage
			system("cls");
			releaseData();
			pLoggedInUser = NULL;
			return 0;
		}
		else if (n == 5) {
			OtherSetting();
		}
		else if (n == 0) {//�˳�ϵͳ
			releaseData();
			return 1;
		}
		else {
			system("cls");
			printf("\n\t����Ĵ����������������룡\n");
		}
	}
	while (pLoggedInUser->grade > 0) {
		system("cls");
		//�������մ���
		printf("\n\t************************��ˮ���۹���****************************\n");
		printf("\n\t---------------------------���˵�-------------------------------\n");
		printf("\n\t**************-0--�˳�ϵͳ          �˳���¼--1-****************\n");
		printf("\n\t**************-2--��Ʒ���          �˺Ź���--3-****************\n");
		printf("\n\t**************-4--��������          �鿴֪ͨ--5-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
		scan(&n);

		ReSetTrolleyInformation();

		if (n == 2) {//������Ʒ����
			GoodsFindAndChange();
			continue;
		}
		else if (n == 3)//�����˻��������
			AccountFindAndChange();
		else if (n == 4)//���붩������
			OrderFindAndChange();
		else if (n == 1) {//�ǳ���������PrePage
			system("cls");
			releaseData();
			pLoggedInUser = NULL;
			return 0;
		}
		else if (n == 5) {
			InformationNotion();
		}
		else if (n == 0) {//�˳�ϵͳ
			releaseData();
			return 1;
		}
		else {
			system("cls");
			printf("\n\t����Ĵ����������������룡\n");
		}
	}
	releaseData();
	return 1;
}

int PrePage() {
	system("color 70");
	int n1;//���ڱ����û���ѡ�����
	while (1)
	{


		printf("\n\t************************��ˮ���۹���****************************\n");
		printf("\n\t---------------------------���˵�-------------------------------\n");
		printf("\n\t**************-0--�˳�                 �˳�--0-****************\n");
		printf("\n\t**************-1--��¼�˺�          ע���˺�--2-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
		n1 = -1;
		scan(&n1);
		if (n1 == 1) {
			if (accountLogin())//��¼�ɹ���break����ѭ����δ��¼�����ѭ��PrePage
				break;
		}
		else if (n1 == 2) {
			accountSignup();//ע���˺�
		}
		else if (n1 == 0) {
			return 1;
		}
		else {
			system("cls");
			printf("\n\t����Ĵ����������������룡\n");
		}
	}
	system("cls");
	printf("\n\t��½�ɹ���\n");//��¼�ɹ��˳�PrePage������MainPage
	return 0;
}