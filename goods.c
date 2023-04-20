#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include"head.h"
#include"goods.h"

//��Ʒ����PrePage����ѡ��ǰ�����ﳵ���ǿ���Ʒ������Աֱ�ӽ���Ʒ����
void GoodsFindAndChange() {
	int n;
	while (1) {
		system("cls");
		printf("\t***********************************��Ʒ�������*********************************\n\n");
		if (pLoggedInUser->grade == 0) {//�ж��û��ȼ�������Աֻ�ܿ���Ʒ�����ܹ���û�й��ﳵ
			GoodsList();
			return;
		}
		else {
			printf("\n\t----------------------------------��  ��  ��  ��--------------------------------\n\n");
			printf("\t***********************-0--����                  ����--0-***********************\n\n");
			printf("\t***********************-1--�����Ʒ            ���ﳵ--2-***********************\n\n");
			printf("\t�����������ţ�");
			n = -1;
			scan(&n);
			if (n == 0)
				return;
			else if (n == 1)
				GoodsList();
			else if (n == 2)
				GoodsTrolley();
			else
				printf("����������\n\t");
		}
	}
}

//�ͷ���Ʒ����
static void releaseGoods(struct CGoods* head) {
	struct CGoods* p1 = head;
	while (p1 != NULL) {
		head = head->next;
		free(p1);
		p1 = head;
	}
	head = NULL;
}
//��������
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

//��ӡָ���ĵ�����Ʒ
static const void GoodsSinglePrint(struct CGoods* pNow) {
	printf("%-4d", pNow->goodsNum);
	printf("%24s", pNow->brand);
	printf("%20s", pNow->kind);
	printf("%9d��", pNow->concentration);
	printf("%11d", pNow->netContent);
	printf("%13d", pNow->boxesNumber);
	printf("%13.2f", pNow->price);
	printf("%13d����%03dƿ", pNow->stock / pNow->boxesNumber, pNow->stock % pNow->boxesNumber);
	printf("%10d\n\t", pNow->sales);
	printf("-------------------------------------------------------------------------------------------------------------------------------------\n\t");
}
//��ӡָ������Ʒ�б�
static const void GoodsPrint(struct CGoods* head) {
	struct CGoods* pNow = head;
	printf("\t���               ��Ʒ����               ��Ʒ���   �ƾ�Ũ��   ÿƿ������(ml)  ÿ��ƿ��  �۸�(Ԫ)/ƿ      ���(��+ƿ)       ����(ƿ)\n");
	printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
	pNow = head;
	if (pNow == NULL)//�б����
		printf("��Ʒ�б�Ϊ��\n\t");
	while (pNow != NULL) {//�����Ʒ�б�
		GoodsSinglePrint(pNow);
		pNow = pNow->next;
	}

}

//��Ʒģ������
static struct CGoods* GoodsSearch(struct CGoods* head, const char* keyword) {
	struct CGoods* pNow = head;
	struct CGoods* result = NULL;
	int len = strlen(keyword);

	while (pNow != NULL) {
		if (strstr(pNow->brand, keyword) != NULL||strstr(pNow->kind,keyword)!=NULL) {
			// �ҵ�ƥ�����Ʒ
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
		printf("\n\tδ�ҵ�ƥ����Ʒ����������");
		Sleep(1000);
		return head;
	}
	else
		return result;
}

//��Ʒ�������
static void GoodsList() {
	struct CGoods* pNow;
	struct CGoods* head = copyGoods(headGoods);//������Ʒ�����ṩ���ɸѡ��ɸѡ�����Ϲ��ܣ���ֹ�˺���Ʒ����
	int goodsnum = 1;
	int n = 10;
	int units = 1, boxesnum = 0;
	char keystr[40];
	while (1)
	{
		system("cls");
		printf("\t************************************************************��Ʒ�������************************************************************\n\n");
		printf("\t-----------------------------------------------------------��  Ʒ  ��  ��-----------------------------------------------------------\n\n");
		GoodsPrint(head);
		if (pLoggedInUser->grade == 0) {//����Ա����
			printf("\n\t-----------------------------------------------------------��  ��  ��  ��-----------------------------------------------------------\n\n");
			printf("\t                      ********************-0--����                              ����--0-********************\n\n");
			printf("\t                      ********************-1--ɸѡ����Ʒ�б�          ��������Ʒ�б�--2-********************\n\n");
			printf("\t                      ********************-3--����Ϊ��ʼ�б�          �۸��ɵ�������--4-********************\n\n");
			printf("\t                      ********************-5--�����ɸ�������              �������Ʒ--6-********************\n\n");
			printf("\t                      ********************-7--�޸���Ʒ��Ϣ                  ɾ����Ʒ--8-********************\n\n");
			printf("\t                      --------------------------------------------------------------------------------------\n\n");
			printf("\n\t�����������ţ�");
			n = -1;
			scan(&n);
			if (n == 0)
				return;
			else if (n == 1)
				head = GoodsFilter(head);
			else if (n == 2) {
				printf("\t����ؼ���(ֱ�ӻس�����)��");
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
				printf("\t�޸���Ʒ��Ϣ��������Ʒ���[���]  ����[0]\n\t");
				scan(&goodsnum);
				if (goodsnum == 0)continue;
				pNow = headGoods;
				while (pNow != NULL && pNow->goodsNum != goodsnum)
					pNow = pNow->next;
				if (pNow != NULL) {
					printf("\t�޸�  ���[1]  Ʒ��[2]  ����[3]  ������[4]  ÿ��ƿ��[5]  �۸�[6]  ����[7]  ����[0]\n\t");
					n = -1;
					scan(&n);
					if (n == 0)break;
					if (n <= 7 && n >= 1) {
						printf("\t");
						GoodsChange(pNow, n);
						SaveGoodsData();//������Ϣ���ļ�
					}
					else {
						printf("\t�������\n");
						Sleep(500);
					}
					head = copyGoods(headGoods);
				}
				else {
					printf("\t��Ʒ������\n\t");
					Sleep(500);
				}
			}
			else if (n == 8) {
				printf("\tɾ����Ʒ��Ϣ��������Ʒ���[���]  ����[0]\n\t");
				scan(&goodsnum);
				if (goodsnum == 0)continue;
				//ֻ��ɾ����ָ����һ���ڵ㣬�������ڱ�ʹ���ɾͷ�ڵ�
				pNow = (struct CGoods*)malloc(sizeof(struct CGoods));
				pNow->next = headGoods;
				while (pNow->next != NULL && pNow->next->goodsNum != goodsnum)
					pNow = pNow->next;
				if (pNow->next != NULL) {
					while (1)
					{
						printf("\tȷ��ɾ��[1]  �������������\n\t");
						scanf_s("%d", &n);
						getchar();
						if (n == 1) {
							GoodsDelete(pNow);
							SaveGoodsData();//������Ϣ���ļ�
						}
						head = copyGoods(headGoods);
						break;
					}
				}
				else {
					printf("\t��Ʒ������\n\t");
					Sleep(500);
				}
			}
			else {
				printf("\t����������\n\t");
				Sleep(500);
			}
		}
		else {//�û�����
			printf("\n\t-----------------------------------------------------------��  ��  ��  ��-----------------------------------------------------------\n\n");
			printf("\t                      ********************-0--����                              ����--0-********************\n\n");
			printf("\t                      ********************-1--ɸѡ����Ʒ�б�          ��������Ʒ�б�--2-********************\n\n");
			printf("\t                      ********************-3--����Ϊ��ʼ�б�          �۸��ɵ�������--4-********************\n\n");
			printf("\t                      ********************-5--�����ɸ�������        �����Ʒ�����ﳵ--6-********************\n\n");
			printf("\t                      --------------------------------------------------------------------------------------\n\n");
			printf("\n\t�����������ţ�");
			n = -1;
			scan(&n);
			if (n == 0)
				return;
			else if (n == 1)
				head = GoodsFilter(head);
			else if (n == 2) {
				printf("\t����ؼ���(ֱ�ӻس�����)��");
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
				printf("\t�����Ʒ�����ﳵ��������Ʒ���[���]  ����[0]\n\t");
				scan(&goodsnum);
				if (goodsnum == 0)
					continue;
				else {
					pNow = headGoods;
					while (pNow != NULL && pNow->goodsNum != goodsnum)
						pNow = pNow->next;
					if (pNow == NULL) {
						printf("\t��Ʒ��Ų����ڣ����������ţ�\n\t");
						Sleep(1000);
					}
					else
						GoodsTrolleyAdd(pNow);
				}
			}
			else {
				printf("\t����������\n\t");
				Sleep(1000);
			}
		}
	}
}

//��Ʒɸѡ����
static struct CGoods* GoodsFilter(struct CGoods* head) {
	float minprice = -1, maxprice = -1;
	int n = -1;//����ѡ�����
	char kind[20];//�ݴ��û��������Ʒ���
	struct CGoods* pNow = head;//pNow��ԭ�б����Ѱ�ҷ�����������Ʒ
	struct CGoods* pNewhead = NULL;//������ͷ
	struct CGoods* pNew1 = NULL;//������������Ʒ������ĺ�ָ��
	//���������߼�
	printf("\t����۸�ɸѡ[1]  ����[0]  ���������������۸�ɸѡ\n\t");
	scan(&n);
	if (n == 0)
		return head;
	else if (n == 1)
	{
		while (1)
		{
			while (1)//������߼ۣ��ų�����
			{
				printf("\t��������Ҫ��ÿƿ��߼�(����0)�� ");
				scanf_s("%f", &maxprice);
				getchar();
				if (maxprice > 0)
					break;
				else
					printf("\t������߼۷Ƿ�\n\t");
			}
			while (1)//������ͼۣ��ų�-1����ĸ���
			{
				printf("\t��������Ҫ��ÿƿ��ͼ�(���ڵ���0)��");
				scanf_s("%f", &minprice);
				getchar();
				if (minprice >= 0)
					break;
				else
					printf("\t������ͼ۷Ƿ�\n\t");
			}
			if (minprice > maxprice)//��ͼ۴�����߼ۣ���������
			{
				printf("\t��ͼ۲��ɴ�����߼�\n\t");
				continue;
			}
			else
				break;
		}
	}
	//������Ʒ����
	printf("\t�������ˮ����(0����)(1����)��");
	gets(kind);
	if (strlen(kind) == 1 && kind[0] == '0')
		return head;
	system("cls");
	while (pNow != NULL) {
		if (((n != 1) || ((pNow->price >= minprice) && (pNow->price <= maxprice))) && ((!strcmp(kind, "1")) || (!strcmp(kind, pNow->kind)))) {
			//����ɸѡ�����������Ӳ���
			if (pNewhead == NULL) {//����Ϊ�յ��������
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
	//ɸѡ��Ҫ�½�����
	if (pNewhead == NULL) {
		printf("\tû�з�����������Ʒ����������\n\t");
		Sleep(1000);
		return head;//ɸѡ�������Ʒ�򷵻�ԭ��Ʒ����
	}
	else {
		pNew1->next = NULL;
		return pNewhead;//����ɸѡ����
	}
}

//������Ʒ�۸��������
static struct CGoods* ascendSortGoodsByPrice(struct CGoods* head) {
	if (head == NULL || head->next == NULL)return head;//������͵��������������ų�
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
//������Ʒ�����ݼ�����
static struct CGoods* decendSortGoodsBySales(struct CGoods* head) {
	if (head == NULL || head->next == NULL)return head;//������͵��������������ų�
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

//������Ʒ
static void GoodsAdd() {
	int n = -1;//�����û�ѡ�����
	char brand[40] = { '\0' };//�ݴ�Ҫ��ӵ���Ʒ����
	struct CGoods* pNow = headGoods;
	printf("\t������Ҫ��ӵ���Ʒ���ƣ�����[0]\n\t");
	gets(brand);
	if (strlen(brand) == 1 && brand[0] == '0')//�ȶ��Ƿ�Ϊ0
		return;
	while (pNow != NULL && strcmp(pNow->brand, brand))
		pNow = pNow->next;
	if (pNow == NULL) {
		printf("\t�Ƿ�ȷ�����%s����Ʒ�б�ȷ���밴[1]�������������������\n\t", brand);//������Ļ�������ʾ
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
			//��ʼֵ�趨
			int max = 0;
			struct CGoods* p = headGoods;
			while (p != NULL) {
				if (p->goodsNum > max)
					max = p->goodsNum;
				p = p->next;
			}
			pNew->goodsNum = max + 1;//�����Զ���ʼ����Ʒ��Ϣ
			strcpy_s(pNew->brand, 40, brand);
			strcpy_s(pNew->kind, 20, "Not set");
			pNew->netContent = 0;
			pNew->boxesNumber = 1;
			pNew->concentration = 0;
			pNew->price = 0;
			pNew->stock = 0;
			pNew->sales = 0;
			system("cls");
			printf("\t���               ��Ʒ����               ��Ʒ���   �ƾ�Ũ��   ÿƿ������(ml)  ÿ��ƿ��  �۸�(Ԫ)/ƿ      ���(��+ƿ)       ����(ƿ)\n");
			printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
			GoodsSinglePrint(pNew);
			GoodsChange(pNew, 5);//�����˹���ʼ����Ʒ��Ϣ

			system("cls");
			printf("\t���               ��Ʒ����               ��Ʒ���   �ƾ�Ũ��   ÿƿ������(ml)  ÿ��ƿ��  �۸�(Ԫ)/ƿ      ���(��+ƿ)       ����(ƿ)\n");
			printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
			GoodsSinglePrint(pNew);
			GoodsChange(pNew, 1);

			system("cls");
			printf("\t���               ��Ʒ����               ��Ʒ���   �ƾ�Ũ��   ÿƿ������(ml)  ÿ��ƿ��  �۸�(Ԫ)/ƿ      ���(��+ƿ)       ����(ƿ)\n");
			printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
			GoodsSinglePrint(pNew);
			GoodsChange(pNew, 3);

			system("cls");
			printf("\t���               ��Ʒ����               ��Ʒ���   �ƾ�Ũ��   ÿƿ������(ml)  ÿ��ƿ��  �۸�(Ԫ)/ƿ      ���(��+ƿ)       ����(ƿ)\n");
			printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
			GoodsSinglePrint(pNew);
			GoodsChange(pNew, 4);

			system("cls");
			printf("\t���               ��Ʒ����               ��Ʒ���   �ƾ�Ũ��   ÿƿ������(ml)  ÿ��ƿ��  �۸�(Ԫ)/ƿ      ���(��+ƿ)       ����(ƿ)\n");
			printf("\t-------------------------------------------------------------------------------------------------------------------------------------\n\t");
			GoodsSinglePrint(pNew);
			GoodsChange(pNew, 6);

			SaveGoodsData();//������Ʒ���ݵ��ļ�
			printf("\t�����Ʒ�ɹ�\n");
			Sleep(500);
			return;
			//��ӳɹ���ص���ӽ���������
		}
		else
			return;
	}
	else {
		printf("\t��Ʒ�Ѵ��ڣ�����������Ʒ��\n\t");
		Sleep(1000);
	}
}

//ɾ����Ʒ��ɾ��������ָ�����һ����Ʒ
static void GoodsDelete(struct CGoods* pNow) {
	struct CGoods* p = pNow->next;
	pNow->next = p->next;
	if (headGoods == p)//���ɾ������ͷ�ڵ�
		headGoods = p->next;
	else if (p->next == NULL)//���ɾ������β�ڵ�
		tailGoods = pNow;
	free(p);
	printf("ɾ���ɹ�");
}

//�޸���Ʒ��Ϣ��1�Ŀ��stock��2����brand��3�����kind��4��ÿƿ��������5��ÿ��ƿ����6��ÿ��۸�7������
static void GoodsChange(struct CGoods* pNow, int slectNum) {
	int n, unit;//n����ѡ����룬unit������Ĳ�����λ
	float fudianshu;//ר���ڱ��渡������
	char cache[40];//���û�������л���
	//����֧������ͬ���ݣ���ѭ���ڲ����ɹ���break�����������continue
	if (slectNum == 1) {
		while (1)
		{
			printf("%s���ڵĿ��Ϊ%d����%dƿ\n\t", pNow->brand, pNow->stock / pNow->boxesNumber, pNow->stock % pNow->boxesNumber);
			printf("���[1]  ����[2]  �޸�[3]  ����[0]\n\t");
			n = -1;
			scan(&n);
			if (n == 0)
				return;
			if (n == 1) {
				printf("\t����������ĵ�λ ��[1] �� ƿ[2]\n\t");
				unit = -1;
				scan(&unit);
				if ((unit != 1) && (unit != 2)) {//����ֻ��Ϊ1��2
					printf("\t���뵥λ����\n\t");
					Sleep(500);
					continue;
				}
				printf("\t����������ӵ�������");
				n = -1;
				scan(&n);
				if (n < 0) {
					printf("\t����Ƿ�\n\t");//������Ϊ����
					Sleep(500);
					continue;
				}
				if (unit == 1)pNow->stock += n * pNow->boxesNumber;
				else pNow->stock += n;
				break;
			}
			else if (n == 2) {
				printf("\t����������ĵ�λ ��[1] �� ƿ[2]\n\t");
				unit = -1;
				scan(&unit);
				if ((unit != 1) && (unit != 2)) {
					printf("\t���뵥λ����\n\t");
					Sleep(500);
					continue;
				}
				printf("\t����������ٵ�������");
				n = -1;
				scan(&n);
				if (unit == 1) {//����Ϊ��λ����
					if (pNow->stock < n * pNow->boxesNumber) {
						printf("\t��治��\n\t");//�������������㹻����Թ�����
						Sleep(500);
						continue;
					}
					else
						pNow->stock -= n * pNow->boxesNumber;
				}
				else {//��ƿΪ��λ����
					if (pNow->stock < n) {
						printf("\t��治��\n\t");
						Sleep(500);
						continue;
					}
					else pNow->stock -= n;
				}
				break;
			}
			else if (n == 3) {
				printf("\t����������ĵ�λ ��[1] �� ƿ[2]\n\t");
				unit = -1;
				scan(&unit);
				if ((unit != 1) && (unit != 2)) {
					printf("\t���뵥λ����\n\t");
					Sleep(500);
					continue;
				}
				printf("\t�����������������");
				n = -1;
				scan(&n);
				if (n <= 0) {
					printf("\t�����Ƿ�\n\t");//�ų�����
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
				printf("\t���������룡\n\t");
				Sleep(500);
				continue;
			}
		}
	}
	else if (slectNum == 2) {
		printf("����������Ʒ��(0����)��");
		gets(cache);
		if (strlen(cache) == 1 && cache[0] == '0')
			return;
		strcpy_s(pNow->brand, 40, cache);
	}
	else if (slectNum == 3) {
		printf("����������Ʒ���(0����)��");
		gets(cache);
		if ((strlen(cache) == 1) && (cache[0] == '0'))return;
		strcpy_s(pNow->kind, 20, cache);
		while (1)
		{
			printf("\t�纬�оƾ������������������Ϊ0��");
			n = -1;
			scan(&n);
			if ((n < 0) || (n >= 100)) {
				printf("\t����Ƿ�\n");
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
			printf("�������µ�ÿƿ������(ml)(0����)��");
			n = -1;
			scan(&n);
			if (n == 0)return;
			if (n < 0) {
				printf("\t����������Ƿ�\n\t");
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
			printf("�������µ�ÿ��ƿ��(0����)��");
			n = -1;
			scan(&n);
			if (n == 0)return;
			if (n < 0) {
				printf("\tÿ��ƿ������Ƿ�\n\t");
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
			printf("�������µ�ÿƿ�۸�(0����)��");
			scanf_s("%f", &fudianshu);
			getchar();
			if ((fudianshu <= 0.000001) && (fudianshu >= -0.000001))return;
			if (fudianshu < 0) {
				printf("\tÿƿ�۸񲻿�Ϊ����\n\t");
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
			printf("�������޸ĺ������(0����)��");
			n = -1;
			scan(&n);
			if (n == 0)return;
			if (n < 0) {
				printf("\t��������Ϊ����\n\t");
				Sleep(500);
				continue;
			}
			else {
				pNow->sales = n;
				break;
			}
		}
	}
	printf("\t���ݲ����ɹ�\n\n");
	Sleep(500);
}