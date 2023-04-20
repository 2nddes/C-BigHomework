#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include"order.h"
#include"head.h"

//��������
void OrderAdd(double totalCost, int goodsNum, int nums) {
	//ָ������ӵĶ���
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
	//������ų�ʼ��
	int max = 0;
	struct COrder* p = headOrder;
	while (p != NULL) {
		if (p->orderNum > max)
			max = p->orderNum;
		p = p->next;
	}
	//ָ�򱻹������Ʒ
	struct CGoods* pNow = headGoods;
	while (pNow->goodsNum != goodsNum) {
		pNow = pNow->next;
	}
	//��������Ʒ������Ʒ������ʼ��
	strcpy(pNew->goodsName, pNow->brand);
	pNew->bottleNums = nums;
	//��Ʒ��س�ʼ��
	if (totalCost >= Gift.threshold && Gift.nums > 0) {
		strcpy(pNew->giftName, Gift.gift);
		pNew->giftNum = Gift.nums;
	}
	else {
		strcpy(pNew->giftName, "��");
		pNew->giftNum = 0;
	}
	//��Ʒ��ų�ʼ��
	pNew->orderNum = max + 1;
	//����ϵͳʱ��Ϊ����ʱ���ʼ��
	time_t now;
	struct tm* tm_now;
	time(&now);
	tm_now = localtime(&now);
	pNew->time = *tm_now;
	//������ʼ��
	pNew->orderStatus = 1;//����״̬
	pNew->StatementOFChangingOrRefunding = 0;//�˻���״̬
	pNew->singleMoney = (float)totalCost;//�������
	strcpy(pNew->userName, pLoggedInUser->userName);//�����û���
	//�ͻ���Ϣ��ʼ��
	while (1) {//��ΪĬ�ϵ�ַ���ֶ�����
		printf("\n\t��ǰĬ���ջ���Ϣ��\n\t");
		printf("�ջ���ַ��%s\n\t", pLoggedInUser->Position);
		printf("�ջ���������%s\n\t", pLoggedInUser->RealName);
		printf("�ջ����ֻ��ţ�%lld\n\t", pLoggedInUser->phoneNumber);	
		printf("\n\t%s�Ķ���ʹ�� Ĭ���ջ���Ϣ[1]  �����ջ���Ϣ[2]\n\t",pNow->brand);
		int n1;
		scan(&n1);
		if (n1 == 1) {
			strcpy(pNew->address, pLoggedInUser->Position);
			strcpy(pNew->realName, pLoggedInUser->RealName);
			pNew->teleNum = pLoggedInUser->phoneNumber;
			break;
		}
		else if (n1 == 2) {
			printf("\t�������ջ���ַ��");
			gets(pNew->address);

			printf("\n\t�������ջ����ֻ��ţ�");
			scanf("%lld", &pNew->teleNum);
			getchar();

			printf("\n\t�������ջ���������");
			gets(pNew->realName);

			break;
		}
		else {
			printf("\t����Ĵ����������������룡");
			Sleep(500);
		}
	}
}

void OrderStatusChangeAutomatic() {
	time_t now = time(NULL);
	struct tm nowTime = *localtime(&now);
	nowTime.tm_wday -= 15;//15��ǰ������
	time_t timeBefore_15_Day = mktime(&nowTime);

	struct COrder* p = headOrder;
	while (p != NULL) {
		//orderStatus Ϊ 1 ��ʾ��������δ���״̬
		if (p->orderStatus == 1) {
			time_t tempTime = mktime(&p->time);
			//�Զ��������µ�ʱ�䳬��15*24h�Ķ�����Ϊ�����
			if (difftime(timeBefore_15_Day, tempTime) <= 0) {
				p->orderStatus = 0;
			}
		}
		p = p->next;
	}

	SaveOrderData();
}
//����15�죬����״̬�Զ���Ϊ�����


//�������˵�
void OrderFindAndChange() {
	OrderStatusChangeAutomatic();
	while (1) {
		system("cls");
		printf("\n\t************************  ����ϵͳ  ****************************\n");
		printf("\n\t---------------------------���˵�-------------------------------\n");
		printf("\n\t**************-0--����                  ����--0-****************\n");
		printf("\n\t**************-1--��ѯ����          �޸Ķ���--2-****************\n");
		printf("\n\t**************-3--ɾ������          ��Ʒ�˻�--4-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
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
			printf("\n\t������Ĵ����������������룡\n");
		}
	}
}

//������Ʒ
void GiftSet() {
	int n;
	while (1)
	{
		system("cls");
		printf("\t**************************��Ʒ����******************************\n");
		printf("\n\t---------------------------���˵�-------------------------------\n");
		printf("\n\t**************-0--����                  ����--0-****************\n");
		printf("\n\t**************-1--������Ʒ          ɾ����Ʒ--2-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t�����õ���Ʒ��%s\t", Gift.gift);
		printf("����������%d\t", Gift.nums);
		printf("�����ż���%.2f\n", Gift.threshold);
		printf("\n\t�����������ţ�");
		scan(&n);
		if (n == 0) {
			return;
		}
		else if (n == 1) {
			printf("\n\t������Ҫ���õ���Ʒ��(0����)��");
			gets(Gift.gift);
			if (!strcmp(Gift.gift, "0")) {
				strcpy(Gift.gift, "��");
				continue;
			}
			else {//���������������ż�
				Gift.nums = 0;
				Gift.threshold = 0;
				while (1)
				{
					printf("\n\t������������\n\t");
					scanf("%d", &n);
					getchar();
					if (n <= 0) {
						printf("\n\t����Ƿ���\n\t");
						Sleep(500);
						continue;
					}
					printf("\n\t����������Ʒ�����Ѷ��ż���");
					float f = -1;
					scanf("%f", &f);
					getchar();
					if (f <= 0) {
						printf("\n\t����Ƿ���\n\t");
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
			strcpy(Gift.gift, "��");
			Gift.nums = 0;
			Gift.threshold = 0;
			SaveGiftData();
			printf("\n\tɾ���ɹ�\n");
			Sleep(500);
			continue;
		}
		else {
			printf("\n\t�������\n\t");
		}
	}
}

// ��ȡһ����ǰ��ʱ��
struct tm GetOneMonthAgoTimestamp() {
	time_t now = time(NULL);
	struct tm one_month_ago_tm = *localtime(&now);
	one_month_ago_tm.tm_mon -= 1;
	mktime(&one_month_ago_tm);
	return one_month_ago_tm;
}

//��ӡ����
void OrderPrint(int fullOrder) {
	struct COrder* p = headOrder;
	//����Ա�û�
	if (pLoggedInUser->grade == 0) {
		// ��ȡһ����ǰ��ʱ���
		struct tm oneMonthAgo = GetOneMonthAgoTimestamp();

		// ��������������ӡ���һ���µĶ���
		struct COrder* currOrder = headOrder;
		if (fullOrder == 1) {
			system("cls");
			printf("\n\t*******************************************************   ��ʷ����   *********************************************\n\n");
			printf("\n\t���ڴ�ӡ�������һ�����ڶ�����Ϣ!\n");
			printf("\n\t-----------------------------------------------------------------------------------------------------------------");
			char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
			printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
			printf("\t-----------------------------------------------------------------------------------------------------------------");
			while (currOrder != NULL) {
				if (difftime(mktime(&(currOrder->time)), mktime(&oneMonthAgo)) >= 0) {
					// ��ӡ������Ϣ
					printf("\n\t%8d", currOrder->orderNum);
					printf("\t%4d/%2d/%2d %2d:%2d:%2d", currOrder->time.tm_year + 1900, currOrder->time.tm_mon + 1, currOrder->time.tm_mday, currOrder->time.tm_hour, currOrder->time.tm_min, currOrder->time.tm_sec);
					printf("%20s", currOrder->realName);
					printf("%18lld", currOrder->teleNum);
					if (currOrder->orderStatus == 1) printf("\tδ���");
					else printf("\t�����");
					printf("\t\t%.2f\n", currOrder->singleMoney);
					printf("\n\t                ��Ʒ��%dƿ%-30s\t��Ʒ��%d %30s", currOrder->bottleNums,currOrder->goodsName, currOrder->giftNum, currOrder->giftName);

					printf("\n\t                �ջ���ַ��%s", currOrder->address);
					printf("\n\t-----------------------------------------------------------------------------------------------------------------");
				}
				currOrder = currOrder->next;
			}
		}
		else if (fullOrder == 2) {
			system("cls");
			printf("\n\t*******************************************************   ��ʷ����   *********************************************\n\n");
			printf("\n\t���ڴ�ӡ��������������Ϣ!\n");
			printf("\n\t-----------------------------------------------------------------------------------------------------------------");
			char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
			printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
			printf("\t-----------------------------------------------------------------------------------------------------------------");
			while (currOrder != NULL) {
				// ��ӡ������Ϣ

				printf("\n\t%8d", currOrder->orderNum);
				printf("\t%4d/%2d/%2d %2d:%2d:%2d", currOrder->time.tm_year + 1900, currOrder->time.tm_mon + 1, currOrder->time.tm_mday, currOrder->time.tm_hour, currOrder->time.tm_min, currOrder->time.tm_sec);
				printf("%20s", currOrder->realName);
				printf("%18lld", currOrder->teleNum);
				if (currOrder->orderStatus == 1) printf("\tδ���");
				else printf("\t�����");
				printf("\t\t%.2f\n", currOrder->singleMoney);
				printf("\n\t                ��Ʒ��%dƿ%-30s\t��Ʒ��%d %30s", currOrder->bottleNums, currOrder->goodsName, currOrder->giftNum, currOrder->giftName);

				printf("\n\t                �ջ���ַ��%s", currOrder->address);
				printf("\n\t-----------------------------------------------------------------------------------------------------------------");
				currOrder = currOrder->next;
			}
		}

	}
	//��ͨ�û�
	else {
		struct COrder* currOrder = headOrder;
		char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
		printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
		printf("\t-----------------------------------------------------------------------------------------------------------------");
		while (currOrder != NULL) {

			if (strcmp(pLoggedInUser->userName, currOrder->userName) == 0) {
				// ��ӡ������Ϣ
				printf("\n\t%8d", currOrder->orderNum);
				printf("\t%4d/%2d/%2d %2d:%2d:%2d", currOrder->time.tm_year + 1900, currOrder->time.tm_mon + 1, currOrder->time.tm_mday, currOrder->time.tm_hour, currOrder->time.tm_min, currOrder->time.tm_sec);
				printf("%20s", currOrder->realName);
				printf("%18lld", currOrder->teleNum);
				if (currOrder->orderStatus == 1) printf("\tδ���");
				else printf("\t�����");
				printf("\t\t%.2f\n", currOrder->singleMoney);
				printf("\n\t                ��Ʒ��%dƿ%-30s\t��Ʒ��%d %30s", currOrder->bottleNums, currOrder->goodsName, currOrder->giftNum, currOrder->giftName);

				printf("\n\t                �ջ���ַ��%s", currOrder->address);
				printf("\n\t-----------------------------------------------------------------------------------------------------------------");
			}
			currOrder = currOrder->next;
		}
	}
}

//��ѯ����
void OrderSeek() {
	int beginMonth;//��ʼ�Ķ����·�
	int beginDay;//��ʼ�Ķ�����
	int beginHour;//��ʼ��ʱ��
	int endMonth;//�����Ķ����·�
	int endDay;//�����Ķ�����
	int endHour;//������ʱ��
	int orderNum;//�������
	while (1) {
		system("cls");
		printf("\t**************************������ѯ******************************\n");
		printf("\n\t---------------------------���˵�-------------------------------\n");
		printf("\n\t**************-0--����                  ����--0-****************\n");
		printf("\n\t**************-1--��ʱ���ѯ      ����Ų�ѯ--2-****************\n");
		printf("\n\t**************-3--��״̬ɸѡ    �û����ж���--4-****************\n");
		printf("\n\t**************-5--�鿴�˻�����  �鿴��������--6-****************\n");
		printf("\n\t----------------------------------------------------------------\n");
		printf("\n\t�����������ţ�");
		int n = -1;
		scan(&n);
		if (n == 0)
			return;
		else if (n == 1) {
			printf("\n\t��������ʼ�·ݣ�");
			scanf("%d", &beginMonth);
			getchar();

			printf("\n\t��������ʼ�գ�");
			scanf("%d", &beginDay);
			getchar();

			printf("\n\t��������ʼСʱ��");
			scanf("%d", &beginHour);
			getchar();

			printf("\n\t����������·ݣ�");
			scanf("%d", &endMonth);
			getchar();

			printf("\n\t����������գ�");
			scanf("%d", &endDay);
			getchar();

			printf("\n\t���������Сʱ��");
			scanf("%d", &endHour);
			getchar();

			struct COrder* p = headOrder;
			int orderNumble = 0;
			system("cls");
			char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
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
								if (p->orderStatus == 1) printf("\tδ���");
								else printf("\t�����");
								printf("\t\t%.2f\n", p->singleMoney);
								printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
								printf("\n\t                �ջ���ַ��%s", p->address);
								printf("\n\t-----------------------------------------------------------------------------------------------------------------");
								orderNumble++;
							}
							else {
								if (!strcmp(pLoggedInUser->userName, p->userName)) {
									printf("\n\t%8d", p->orderNum);
									printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
									printf("%20s", p->realName);
									printf("%18lld", p->teleNum);
									if (p->orderStatus == 1) printf("\tδ���");
									else printf("\t�����");
									printf("\t\t%.2f\n", p->singleMoney);
									printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
									printf("\n\t                �ջ���ַ��%s", p->address);
									printf("\n\t-----------------------------------------------------------------------------------------------------------------");
									orderNumble++;
								}
							}
						}
					}
				}
				p = p->next;
			}
			printf("\n\t�ҵ�%d������\n\t", orderNumble);
			printf("\n\t�س�������\n\t");
			while (getchar() != '\n');
		}
		else if (n == 2) {
			printf("\n\t�����붩����ţ�");
			orderNum = -1;
			scan(&orderNum);
			struct COrder* p = headOrder;
			while (p != NULL) {
				if (p->orderNum == orderNum) {
					if (pLoggedInUser->grade == 0) {
						system("cls");
						char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
						printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
						printf("\t-----------------------------------------------------------------------------------------------------------------");
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\tδ���");
						else printf("\t�����");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                �ջ���ַ��%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						break;
					}
					else {
						if (strcmp(pLoggedInUser->userName, p->userName)) {
							printf("\n\t�˻��ڲ����ڴ˶�����\n\t");
							break;
						}
						else {
							system("cls");
							char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
							printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
							printf("\t-----------------------------------------------------------------------------------------------------------------");
							printf("\n\t%8d", p->orderNum);
							printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
							printf("%20s", p->realName);
							printf("%18lld", p->teleNum);
							if (p->orderStatus == 1) printf("\tδ���");
							else printf("\t�����");
							printf("\t\t%.2f\n", p->singleMoney);
							printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
							printf("\n\t                �ջ���ַ��%s", p->address);
							printf("\n\t-----------------------------------------------------------------------------------------------------------------");
							break;
						}
					}
				}
				p = p->next;
			}
			if (p == NULL)printf("\n\tδ�ҵ�����\n\t");
			printf("\n\t�س�������\n\t");
			while (getchar() != '\n');
		}
		else if (n == 3) {
			while (1)
			{
				printf("\n\t��ѯ����ɶ���[1]  ��ѯδ��ɶ���[2]  ����[0]\n\t");
				n = -1;
				scan(&n);
				if (n == 0)
					return;
				else if (n == 1) {
					struct COrder* p = headOrder;
					int orderNumble = 0;
					system("cls");
					char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
					printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
					printf("\t-----------------------------------------------------------------------------------------------------------------");
					while (p != NULL) {
						if (pLoggedInUser->grade == 0) {
							if (p->orderStatus == 0) {
								printf("\n\t%8d", p->orderNum);
								printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
								printf("%20s", p->realName);
								printf("%18lld", p->teleNum);
								if (p->orderStatus == 1) printf("\tδ���");
								else printf("\t�����");
								printf("\t\t%.2f\n", p->singleMoney);
								printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
								printf("\n\t                �ջ���ַ��%s", p->address);
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
								if (p->orderStatus == 1) printf("\tδ���");
								else printf("\t�����");
								printf("\t\t%.2f\n", p->singleMoney);
								printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
								printf("\n\t                �ջ���ַ��%s", p->address);
								printf("\n\t-----------------------------------------------------------------------------------------------------------------");
								orderNumble++;
							}
						}
						p = p->next;
					}
					printf("\n\t�ҵ�%d������\n\t", orderNumble);
					printf("\n\t�س�������\n\t");
					while (getchar() != '\n');
					break;
				}
				else if (n == 2) {
					struct COrder* p = headOrder;
					int orderNumble = 0;
					system("cls");
					char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
					printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
					printf("\t-----------------------------------------------------------------------------------------------------------------");
					while (p != NULL) {
						if (pLoggedInUser->grade == 0) {
							if (p->orderStatus == 1) {
								printf("\n\t%8d", p->orderNum);
								printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
								printf("%20s", p->realName);
								printf("%18lld", p->teleNum);
								if (p->orderStatus == 1) printf("\tδ���");
								else printf("\t�����");
								printf("\t\t%.2f\n", p->singleMoney);
								printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
								printf("\n\t                �ջ���ַ��%s", p->address);
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
								if (p->orderStatus == 1) printf("\tδ���");
								else printf("\t�����");
								printf("\t\t%.2f\n", p->singleMoney);
								printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
								printf("\n\t                �ջ���ַ��%s", p->address);
								printf("\n\t-----------------------------------------------------------------------------------------------------------------");
								orderNumble++;
							}
						}
						p = p->next;
					}
					printf("\n\t�ҵ�%d������\n\t", orderNumble);
					printf("\n\t�س�������\n\t");
					while (getchar() != '\n');
					break;
				}
				else {
					printf("\n\t�������\n\t");
					Sleep(500);
					continue;
				}
			}
		}
		else if (n == 4) {
			if (pLoggedInUser->grade == 0) {
				//����Ա�鿴�����û�����
				printf("\n\t�������ѯ���û�����");
				char userName[40];
				gets(userName);
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (!strcmp(p->userName, userName)) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\tδ���");
						else printf("\t�����");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                �ջ���ַ��%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t�ҵ�%d������\n\t", orderNumble);
				printf("\n\t�س�������\n\t");
				while (getchar() != '\n');
			}
			else {
				//�����ǰ�û��Ķ���
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (!strcmp(p->userName, pLoggedInUser->userName)) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\tδ���");
						else printf("\t�����");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                �ջ���ַ��%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t�ҵ�%d������\n\t", orderNumble);
				printf("\n\t�س�������\n\t");
				while (getchar() != '\n');
			}
		}
		else if (n == 5) {
			if (pLoggedInUser->grade == 0) {
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (p->StatementOFChangingOrRefunding == 1) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\tδ���");
						else printf("\t�����");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                �ջ���ַ��%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t�ҵ�%d������\n\t", orderNumble);
				printf("\n\t�س�������\n\t");
				while (getchar() != '\n');

			}
			else {
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (strcmp(p->userName, pLoggedInUser->userName) == 0 && p->StatementOFChangingOrRefunding == 1) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\tδ���");
						else printf("\t�����");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                �ջ���ַ��%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t�ҵ�%d������\n\t", orderNumble);
				getchar();
			}
		}
		else if (n == 6) {
			if (pLoggedInUser->grade == 0) {
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (p->StatementOFChangingOrRefunding == 2) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\tδ���");
						else printf("\t�����");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                �ջ���ַ��%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t�ҵ�%d������\n\t", orderNumble);
				printf("\n\t�س�������\n\t");
				while (getchar() != '\n');

			}
			else {
				struct COrder* p = headOrder;
				int orderNumble = 0;
				system("cls");
				char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
				printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
				printf("\t-----------------------------------------------------------------------------------------------------------------");
				while (p != NULL) {
					if (strcmp(p->userName, pLoggedInUser->userName) == 0 && p->StatementOFChangingOrRefunding == 2) {
						printf("\n\t%8d", p->orderNum);
						printf("\t%4d/%2d/%2d %2d:%2d:%2d", p->time.tm_year + 1900, p->time.tm_mon + 1, p->time.tm_mday, p->time.tm_hour, p->time.tm_min, p->time.tm_sec);
						printf("%20s", p->realName);
						printf("%18lld", p->teleNum);
						if (p->orderStatus == 1) printf("\tδ���");
						else printf("\t�����");
						printf("\t\t%.2f\n", p->singleMoney);
						printf("\n\t                ��Ʒ����%30s\t��Ʒ��%d*%30s", p->goodsName, p->giftNum, p->giftName);
						printf("\n\t                �ջ���ַ��%s", p->address);
						printf("\n\t-----------------------------------------------------------------------------------------------------------------");
						orderNumble++;
					}
					p = p->next;
				}
				printf("\n\t�ҵ�%d������\n\t", orderNumble);
				printf("\n\t�س�������\n\t");
				while (getchar() != '\n');

			}
		}
		else {
			printf("\n\t����������\n\t");
			Sleep(500);
			continue;
		}
	}
}

//�޸Ķ���
void OrderChange() {
	system("cls");
	printf("\n\t*******************************************************   �޸Ķ���   *********************************************\n\n");
	int num_ = -1;
	while (pLoggedInUser->grade == 0) {
		printf("\n\t�����1���µĶ��� [1]    �����ʷ�������� [2]");
		printf("\n\t�ڴ����룺");
		num_ = -1;
		scan(&num_);
		if (num_ == 1)break;
		else if (num_ == 2)break;
		else printf("\n\t��������\n");
	}
	OrderPrint(num_);

	struct COrder* p = headOrder;
	printf("\n\n\t----------------------------------------------------------------------------------------------------------------\n\n");
	//�ҵ���Ҫ���޸ĵĶ����ĵ�ַ
	int num = -1;
	while (1) {
		p = headOrder;
		printf("\n\t��������Ҫ�����޸ĵĶ����ı��,����[0]�ɷ��أ�");
		scan(&num);
		if (num == 0) {
			system("cls");
			return;
		}
		else if (num >= 1 && num <= 100000) {
			//�ǹ���Ա�û�
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
					printf("\n\t������������ı�Ŷ�Ӧ�Ķ������밴��������������룡");
					getchar();
				}
				else break;//�ҵ�������Ķ���
			}
			//����Ա�û�
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
					printf("\n\t������������ı�Ŷ�Ӧ�Ķ������밴��������������룡");
					getchar();
				}
				else break;//�ҵ�������Ķ���
			}
		}
	}
	printf("\n\t-----------------------------------------------------��  ��  ��  ��---------------------------------------------\n\n");
	printf("\t               ********************-0--����                              ����--0-********************\n\n");
	printf("\t               ********************-1--�޸��ջ���ַ              �޸��ջ�������--2-********************\n\n");
	printf("\t               ********************-3--�޸��ջ��˵绰           �޸Ķ������״̬--4-********************\n\n");
	printf("\t               -------------------------------------------------------------------------------------\n\n");

	while (1) {
		printf("\n\t������\"�����˵�\"�еĲ�����ţ�");
		int n1 = -1;
		scan(&n1);
		if (n1 == 0) {
			system("cls");
			return;
		}
		else if (n1 == 1) {
			while (1) {
				printf("\n\t����ǰ���ջ���ַΪ%s", p->address);
				printf("\n\t�޸ĸ���Ʒ�ջ���ַ������[1],����������[0]:");
				int n2 = -1;
				scan(&n2);
				if (n2 == 1) {
					printf("\n\t�������µ��ջ���ַ(������25������)��");
					char tempPlace[100];
					gets(tempPlace);
					if (strlen(tempPlace) > 50) {
						printf("\n\t��ַ���������������룡\n");
					}
					else {
						strcpy(p->address, tempPlace);
						printf("\n\t�ö����µ��ջ���ַΪ%s\n", tempPlace);
						printf("\n\t����������ء�");
						SaveOrderData();
						getchar();
						break;
					}
					break;
				}
				else if (n2 == 0) {
					break;
				}
				else printf("\n\t�����������������룡\n");
			}
			//break;
		}
		else if (n1 == 2) {
			while (1) {
				printf("\n\t�ö�����ǰ���ջ���Ϊ%s", p->realName);
				printf("\n\t�޸Ķ�����ǰ���ջ���������[1],����������[0]:");
				int n2 = -1;
				scan(&n2);
				if (n2 == 1) {
					printf("\n\t�������µ��ջ�������(������10������)��");
					char tempName[40];
					gets(tempName);
					if (strlen(tempName) > 20) {
						printf("\n\t�������������������룡\n");
					}
					else {
						strcpy(p->realName, tempName);
						printf("\n\t�ö����µ��ջ���Ϊ%s\n", tempName);
						printf("\n\t����������ء�");
						SaveOrderData();
						getchar();
						break;
					}
					break;
				}
				else if (n2 == 0) {
					break;
				}
				else printf("\n\t�����������������룡\n");
			}
			break;
		}
		else if (n1 == 3) {
			int n2 = -1;
			while (1) {
				n2 = -1;
				printf("\n\t��ǰ�ջ��˵ĵ绰Ϊ��%lld\n", p->teleNum);
				printf("\t����[0]����,����[1]�޸��ջ��˵绰��");
				scan(&n2);
				if (n2 == 0) {
					break;
				}
				if (n2 == 1) {
					while (1) {
						printf("\n\t�������µ��ջ��˵绰��");
						long long tempTeleNum = -1;
						scanf("%lld", &tempTeleNum);
						getchar();
						if (tempTeleNum >= 10000000000 && tempTeleNum <= 100000000000) {
							printf("\n\t�ջ����µ绰����%lld\n", tempTeleNum);
							printf("\n\t������������ء�");
							SaveOrderData();
							getchar();
							p->teleNum = tempTeleNum;
							break;
						}
						else printf("\n\t�����������������룡\n");
					}
					break;
				}
				else printf("\n\t�����������������룡\n");
			}
			break;
		}
		else if (n1 == 4) {
			if (p->orderStatus == 1) {
				printf("\n\t���Ϊ%d�Ķ�����δ���\n", p->orderNum);
				printf("\n\tע�⣺�����µ�ʱ�䳬��15��Ķ�����״̬���Զ�ת��Ϊ\"�����\"��");
				while (1) {
					printf("\n\t����[0]�ɷ��أ�����[1]������״̬��Ϊ\"�����\"");
					int n2 = -1;
					scan(&n2);
					if (n2 == 0) {
						break;
					}
					else if (n2 == 1) {
						int n3 = -1;
						while (1) {
							n3 = -1;
							printf("\n\t�ò������ɳ������Ƿ�ȷ���޸ģ�����[0]���أ�����[1]ȷ�ϸ��ģ�");
							scan(&n3);
							if (n3 == 0) break;
							else if (n3 == 1) {
								p->orderStatus = 0;
								printf("\n\t���ѳɹ����ģ������������");
								SaveOrderData();
								getchar();
								break;
							}
							else printf("\n\t���������������������룡\n");
						}
						break;
					}
					else printf("\n\t���������������������룡\n");
				}
				break;
			}
			else {
				printf("\n\t���Ϊ%d�Ķ�������ɣ�����״̬�����ٸ��ģ�������������ء�", p->orderNum);
				getchar();
				break;
			}
		}
		else if (n1 == 5) {
			
			printf("\n\t�������޸ĺ�ʱ�䣺\n");

			struct tm start;
			int timeStart[6]; // ����int������time�����ڴ洢ʱ��
			int timeEnd[6];
			const int month_days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // ÿ���µ���������ʱ���������꣩
			// �Ӽ����ж���ʱ�䣬����Ϊ������ʱ����
			
			
				printf("\n\t��������ʼʱ�䣬��ʽΪ yyyy mm dd hh mm ss���ֱ��Ӧ�ꡢ�¡��ա�ʱ���֡��룩��");
				printf("\n\t");
				int i = 0;
				int valid = 0;
				for (i = 0; i < 6; i++) {
					valid = 0;
					while (!valid) {
						if (scanf("%d", &timeStart[i]) != 1) {
							printf("\t������Ч�����������룺");
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

			printf("\n\t���ʼʱ��Ϊ%4d��%2d��%2d��%2dʱ%2d��%2d��\n", start.tm_year + 1900, start.tm_mon + 1,
				start.tm_mday, start.tm_hour, start.tm_min, start.tm_sec);

			p->time = start;
			SaveOrderData();
			getchar();
			
		}
		else if (n1 == 6) {
			printf("\n\t�������޸ĺ�ƿ��");
			int n3 = -1;
			scan(&n3);
			p->bottleNums = n3;
			SaveOrderData();
			getchar();
		}
		else printf("\n\t�����������������룡\n");
	}
	SaveOrderData();
}

//ɾ������
void OrderDelete() {
	system("cls");
	printf("\n\t*******************************************************   ����ɾ��   *********************************************\n\n");
	int num_ = -1;
	while (pLoggedInUser->grade == 0) {
		printf("\n\t�����1���µĶ���[1], �����ʷ��������[2]");
		printf("\n\t�ڴ����룺");
		num_ = -1;
		scan(&num_);
		if (num_ == 1)break;
		else if (num_ == 2)break;
		else printf("\n\t��������\n");
	}
	OrderPrint(num_);

	printf("\n\n\t----------------------------------------------------------------------------------------------------------------\n");
	printf("\n\t*******************************************************   ����ɾ��   *********************************************\n\n");
	int num = -1;
	struct COrder* p = headOrder;
	struct COrder* p0 = NULL;
	//�ҵ���Ҫ��ɾ���Ķ����ı��
	while (1) {
		p = headOrder;
		p0 = NULL;
		printf("\n\t��������Ҫɾ���Ķ����ı��,����[0]�ɷ��أ�");
		scan(&num);
		if (num == 0) {
			system("cls");
			return;
		}
		else if (num >= 1 && num <= 100000) {
			//�ǹ���Ա�û�
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
					printf("\n\t������������ı�Ŷ�Ӧ�Ķ������밴��������������룡");
					getchar();
				}
				else break;//�ҵ�������Ķ���
			}
			//����Ա�û�
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
					printf("\n\t������������ı�Ŷ�Ӧ�Ķ������밴��������������룡");
					getchar();
				}
				else break;//�ҵ�������Ķ���
			}
		}
	}


	if (p->orderStatus == 1) {
		printf("\n\t�ö�����δ��ɣ����ܱ�ɾ����������������أ�");
		getchar();
	}
	else if (p->orderStatus == 0) {
		int n2 = -1;
		while (1) {
			n2 = -1;
			printf("\n\t����[1]ɾ���ö���������[0]���أ�");
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
				printf("\n\t���ѳɹ�ɾ���ö���������������أ�");
				SaveOrderData();
				getchar();
				break;
			}
			else printf("\n\t�����������������룡\n");
		}
	}
}

//�˻���
void GoodsChangingAndRefunding() {
	//��һ�� ����������������˻����Ķ�����Ϣ
	struct COrder* currOrder = headOrder;
	//����Ա�û�
	if (pLoggedInUser->grade == 0) {
		system("cls");
		printf("\n\t*******************************************************   ��ʷ����   *********************************************\n\n");
		printf("\n\t���ڴ�ӡ�����������������˻����Ķ���!\n");
		printf("\n\t-----------------------------------------------------------------------------------------------------------------");
		char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
		printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
		printf("\t-----------------------------------------------------------------------------------------------------------------");
		while (currOrder != NULL) {
			// ��ӡ�����˻���״̬��������Ϣ
			if ((currOrder->StatementOFChangingOrRefunding == 1 || currOrder->StatementOFChangingOrRefunding == 2)) {
				printf("\n\t%8d", currOrder->orderNum);
				printf("\t%4d/%2d/%2d %2d:%2d:%2d", currOrder->time.tm_year + 1900, currOrder->time.tm_mon + 1, currOrder->time.tm_mday, currOrder->time.tm_hour, currOrder->time.tm_min, currOrder->time.tm_sec);
				printf("%20s", currOrder->realName);
				printf("%18lld", currOrder->teleNum);
				if (currOrder->orderStatus == 1) printf("\tδ���");
				else printf("\t�����");
				printf("\t\t%.2f\n", currOrder->singleMoney);
				printf("\n\t                �˻���״̬��");
				if (currOrder->StatementOFChangingOrRefunding == 1)printf("�����˻�");
				else printf("���ڻ���");
				printf("\n\t-----------------------------------------------------------------------------------------------------------------");
			}
			currOrder = currOrder->next;
		}


		//�ҵ���Ҫ�޸ĵĶ���
		struct COrder* p = headOrder;
		int num = -1;
		while (1) {
			p = headOrder;
			printf("\n\t��������Ҫ���в����ĵĶ����ı��,����[0]�ɷ��أ�");
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
					printf("\n\t������������ı�Ŷ�Ӧ�Ķ������밴��������������룡");
					getchar();
				}
				else break;//�ҵ�������Ķ���

			}
			else printf("\n\t������������������!\n");
		}

		while (1) {
			num = -1;
			if (p->StatementOFChangingOrRefunding == 1)printf("\n\t��ǰ���������˻�");
			else printf("\n\t��ǰ�������ڻ���");
			printf("\n\t��׼����������[1], ��������������[2], ȡ������������[0]:");
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
				printf("\n\t��׼�ɹ���\n");
				break;
			}
			else if (num == 2) {
				p->StatementOFChangingOrRefunding = 0;
				SaveOrderData();
				printf("\n\t�ɹ��������룡\n");
				break;
			}
			else printf("\n\t������������������ֶ�Ӧ�ı�ţ����������룡\n");
		}
	}
	//�ǹ���Ա�û�
	else if (pLoggedInUser->grade > 0) {
		system("cls");
		printf("\n\t*******************************************************   ��ʷ����   *********************************************\n\n");
		printf("\n\t���ڴ�ӡ�������д���δ���״̬�Ķ���!\n");
		printf("\n\t-----------------------------------------------------------------------------------------------------------------");
		char* arrName[6] = { "�������", "�µ�ʱ��", "�ռ�������", "�ռ����ֻ���" ,"�������״̬", "�������" };
		printf("\n\t%-8s\t%19s%20s%18s%18s\t%s\n", arrName[0], arrName[1], arrName[2], arrName[3], arrName[4], arrName[5]);
		printf("\t-----------------------------------------------------------------------------------------------------------------");
		while (currOrder != NULL) {
			// ��ӡ������Ϣ
			if (currOrder->orderStatus == 1 && currOrder->StatementOFChangingOrRefunding != 3 && strcmp(currOrder->userName, pLoggedInUser->userName) == 0) {
				printf("\n\t%8d", currOrder->orderNum);
				printf("\t%4d/%2d/%2d %2d:%2d:%2d", currOrder->time.tm_year + 1900, currOrder->time.tm_mon + 1, currOrder->time.tm_mday, currOrder->time.tm_hour, currOrder->time.tm_min, currOrder->time.tm_sec);
				printf("%20s", currOrder->realName);
				printf("%18lld", currOrder->teleNum);
				if (currOrder->orderStatus == 1) printf("\tδ���");
				else printf("\t�����");
				printf("\t\t%.2f\n", currOrder->singleMoney);
				printf("\n\t                �˻���״̬��");
				if (currOrder->StatementOFChangingOrRefunding == 1)printf("�����˻�");
				else if (currOrder->StatementOFChangingOrRefunding == 2) printf("���ڻ���");
				else printf("δ������˻�����");
				printf("\n\t-----------------------------------------------------------------------------------------------------------------");
			}
			currOrder = currOrder->next;
		}


		//�ҵ���Ҫ�޸ĵĶ���
		struct COrder* p = headOrder;
		int num = -1;
		while (1) {
			p = headOrder;
			printf("\n\t��������Ҫ���в����ĵĶ����ı��,����[0]�ɷ��أ�");
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
					printf("\n\t������������ı�Ŷ�Ӧ�Ķ������밴��������������룡");
					getchar();
				}
				else break;//�ҵ�������Ķ���

			}
			else printf("\n\t������������������!\n");
		}

		while (1) {
			num = -1;
			//���������˻���״̬
			if (p->StatementOFChangingOrRefunding == 1 || p->StatementOFChangingOrRefunding == 2) {
				if (p->StatementOFChangingOrRefunding == 1) printf("\n\t��ǰ���������˻�");
				else if (p->StatementOFChangingOrRefunding == 2) printf("\n\t��ǰ�������ڻ���");
				int n1 = -1;
				while (1) {
					n1 = -1;
					printf("\n\tȡ������������[1], ����������[0]:");
					scan(&n1);
					if (n1 == 0) {
						
						break;
					}
					else if (n1 == 1) {
						p->StatementOFChangingOrRefunding = 0;
						printf("\n\t���ѳɹ�ȡ�����룡\n");
						break;
					}
				}
			}
			//����δ�����˻���״̬
			else {
				printf("\n\t��ǰ����δ�����˻���״̬\n");
				printf("\n\t�����˻�������[1], ���뻻��������[2], ȡ������������[0]:");
				scan(&num);
				if (num == 0) {
					system("cls");
					break;
				}
				else if (num == 1) {
					p->StatementOFChangingOrRefunding = 1;
					SaveOrderData();
					printf("\n\t����ɹ��������ĵȴ��̼Ҵ���\n");
					getchar();
					break;
				}
				else if (num == 2) {
					p->StatementOFChangingOrRefunding = 2;
					SaveOrderData();
					printf("\n\t����ɹ��������ĵȴ��̼Ҵ���\n");
					getchar();
					break;
				}
				else printf("\n\t������������������ֶ�Ӧ�ı�ţ����������룡\n");
			}
		}
	}
}

//�����˻���ɵ���Ϣ�ı�
void InformationChangeForRefunding(struct COrder* p) {

	struct CGoods* pGood = headGoods; struct CGoods* pGood0 = NULL;
	struct CAccount* pAccou = headAccount; struct CAccount* pAccou0 = NULL;
	//��Ҫ�ı���� �û��������۶�,�û���Ա�ȼ�����Ʒ���,��Ʒ����

	while (pGood != NULL && strcmp(pGood->brand, p->realName) == 0) {
		pGood0 = pGood;
		pGood = pGood->next;
	}

	//�û��ȼ������۸���
	float totalCost = p->singleMoney;
	pLoggedInUser->totalAmount += totalCost;
	//*********GradeChange(pLoggedInUser);

	//��Ʒ��桢��������
	pGood->stock += p->bottleNums;
	pGood->sales -= p->bottleNums;

	//���޸ĺ�ĸ�����Ϣ�洢���ļ�����ȥ

	SaveGoodsData(headGoods);
	SaveAccountData(headAccount);

}

