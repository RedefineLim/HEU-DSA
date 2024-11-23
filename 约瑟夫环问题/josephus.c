#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int position;
    struct Node* next;
} Node;

//创建一个环形链表, 有n个节点
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

//通过一个约瑟夫环, 模拟一个报数过程
void josephus(int n, int m) {
    Node* head = createCircle(n);
    Node* cur = head;
    Node* prev = NULL;

    while (cur != cur->next) {
        for (int i = 1; i < m; ++i) {
            prev = cur;
            cur = cur->next;
        }
        prev->next = cur->next;             //cur指针跟踪节点, prev指针只想cur指针前一个节点, 方便移除节点
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
    printf("请输入总人数n：");
    fflush(stdout);
    scanf("%d", &n);
    printf("请输入报数m：");
    fflush(stdout);
    scanf("%d", &m);
    printf("出列顺序为：");
    fflush(stdout);
    josephus(n, m);
    return 0;
}