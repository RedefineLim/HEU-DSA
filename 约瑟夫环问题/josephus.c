#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int position;
    struct Node* next;
} Node;

//����һ����������, ��n���ڵ�
Node* createCircle(int n) {
    Node* head = NULL, *tail = NULL, *temp = NULL;
    for (int i = 1; i <= n; ++i) {
        temp = (Node*)malloc(sizeof(Node));
        temp->position = i;
        temp->next = NULL;
        if (head == NULL) {
            head = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = temp;
        }
    }
    tail->next = head;
    return head;
}

//ͨ��һ��Լɪ��, ģ��һ����������
void josephus(int n, int m) {
    Node* head = createCircle(n);
    Node* cur = head;
    Node* prev = NULL;

    while (cur != cur->next) {
        for (int i = 1; i < m; ++i) {
            prev = cur;
            cur = cur->next;
        }
        prev->next = cur->next;             //curָ����ٽڵ�, prevָ��ֻ��curָ��ǰһ���ڵ�, �����Ƴ��ڵ�
        printf("%d ", cur->position);
        Node* temp = cur;
        cur = cur->next;
        free(temp);
    }
    printf("%d\n", cur->position);
    free(cur);
}

int main() {
    int n, m;
    printf("������������n��");
    fflush(stdout);
    scanf("%d", &n);
    printf("�����뱨��m��");
    fflush(stdout);
    scanf("%d", &m);
    printf("����˳��Ϊ��");
    fflush(stdout);
    josephus(n, m);
    return 0;
}