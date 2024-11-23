#include<stdio.h>
#include<utility>
#include"Stack.h"
#include"LinkedList.h"

//ʵ�ֱ��ʽ��ֵ
//3+(5/2)%4*2+(1-3)*4=-1
//2*((30/3%4*1)+22/(41-6)%4)/3=1


//����Node�ڵ����ֲ������������
struct Node {
	enum class Type { OP, NUM };
	Type t;
	union {
		char op;
		int num;
	};
	Node(Type t, char data) :t(t), op(data) {};
	Node(Type t, int data) :t(t), num(data) {};
};

//��ÿ���ڵ��зֱ�õ����Զ�Ӧ�Ĳ������Ͳ�����
Node getNode(char** str) {
	if (**str >= '0' && **str <= '9') {
		int num;
		sscanf(*str, "%d", &num);
		while (**str >= '0' && **str <= '9')++(*str);
		return Node(Node::Type::NUM, num);
	}
	else {
		++(*str);
		return Node(Node::Type::OP, *(*str - 1));
	}
}

//����׺���ʽת��Ϊ��׺���ʽ��ʹ��ջ����������������ȼ�������
LinkedList<Node> toSuffixExp(LinkedList<Node> infixExpList) {
	LinkedList<Node> suffixExpList;
	Stack<Node> st;
	for (auto data = infixExpList.getHead(); data; data = data->getNext()) {
		if (data->data.t == Node::Type::NUM)suffixExpList.add(data->data);
		else {
			while (!st.isEmpty()) {
				if ((data->data.op == '+' || data->data.op == '-') && (st.front().op != '(')) {
					suffixExpList.add(st.front());
					st.pop();
				}
				else if ((data->data.op == '*' || data->data.op == '/' || data->data.op == '%') && (st.front().op == '*' || st.front().op == '/' || st.front().op == '%')) {
					suffixExpList.add(st.front());
					st.pop();
				}
				else if (data->data.op == ')' && st.front().op != '(') {
					suffixExpList.add(st.front());
					st.pop();
				}
				else if (data->data.op == ')' && st.front().op == '(') {
					st.pop();
					break;
				}
				else {
					break;
				}
			}
			if (data->data.op != ')')
				st.push(data->data);
		}
	}
	while (!st.isEmpty()) {
		suffixExpList.add(st.front());
		st.pop();
	}
	return suffixExpList;
}

//����׺���ʽת��Ϊǰ׺���ʽ��ͬ��ʹ��ջ����������������ȼ�������
LinkedList<Node> toPrefixExp(LinkedList<Node> infixExpList) {
	LinkedList<Node> li;
	Stack<Node> s1, s2;
	for (auto i = infixExpList.getEnd(); i; i = i->getLast()) {
		if (i->data.t == Node::Type::NUM) {
			s2.push(i->data);
		}
		else {
			char op = i->data.op;
			if (op != '(' && op != ')') {
				while (!s1.isEmpty()) {
					if ((op == '+' || op == '-') && (s1.front().op == '*' || s1.front().op == '/' || s1.front().op == '%')) {
						s2.push(s1.front());
						s1.pop();
					}
					else {
						break;
					}
				}
				s1.push(i->data);
			}
			else if (op == ')') {
				s1.push(i->data);
			}
			else {
				while (s1.front().op != ')') {
					s2.push(s1.front());
					s1.pop();
				}
				s1.pop();
			}
		}
	}
	while (!s1.isEmpty()) {
		s2.push(s1.front());
		s1.pop();
	}
	while (!s2.isEmpty()) {
		li.add(s2.front());
		s2.pop();
	}
	return li;
}

//����ǰ׺���ʽ��ֵ��ʹ��ջ���洢�������������ݲ��������м���
int calculatePrefixExp(LinkedList<Node> prefixExpList) {
	Stack<Node> st;
	for (auto i = prefixExpList.getHead(); i; i = i->getNext()) {
		auto data = i->data;
		while (!st.isEmpty() && st.front().t == Node::Type::NUM && data.t == Node::Type::NUM) {
			int num1 = st.front().num;
			int num2 = data.num;
			st.pop();
			char op = st.front().op;
			st.pop();
			int ans = 0;
			if (op == '+')ans = num1 + num2;
			else if (op == '-')ans = num1 - num2;
			else if (op == '*')ans = num1 * num2;
			else if (op == '/')ans = num1 / num2;
			else if (op == '%')ans = num1 % num2;
			data = Node(Node::Type::NUM, ans);
		}
		st.push(data);
	}
	return st.front().num;
}

//�����׺���ʽ��ֵ��ͬ��ʹ��ջ���洢�������������ݲ��������м���
int calculateSuffixExp(LinkedList<Node> suffixExpList) {
	Stack<Node> st;
	for (auto i = suffixExpList.getHead(); i; i = i->getNext()) {
		if (i->data.t == Node::Type::NUM)st.push(i->data);
		else {
			int num2 = st.front().num;
			st.pop();
			int num1 = st.front().num;
			st.pop();
			char op = i->data.op;
			int ans = 0;
			if (op == '+')ans = num1 + num2;
			else if (op == '-')ans = num1 - num2;
			else if (op == '*')ans = num1 * num2;
			else if (op == '/')ans = num1 / num2;
			else if (op == '%')ans = num1 % num2;
			st.push(Node(Node::Type::NUM, ans));
		}
	}
	return st.front().num;
}

//��ȡ�û��������׺���ʽ������ת������ֵ�������������׺��ǰ׺���ʽ���������
void expression() {
	char str[100];
	printf("��������ʽ:\n");
    fflush(stdout);
	scanf("%s", str);
	LinkedList<Node> infixExpList;
	LinkedList<Node> suffixExpList;
	LinkedList<Node> prefixExpList;
	char* temp = &str[0];
	while (*temp) {
		infixExpList.add(getNode(&temp));
	}
	suffixExpList = toSuffixExp(infixExpList);
	prefixExpList = toPrefixExp(infixExpList);
	printf("��׺���ʽ:");
	for (auto data = suffixExpList.getHead(); data; data = data->getNext()) {
		if (data->data.t == Node::Type::OP)printf("%c ", data->data.op);
		else printf("%d ", data->data.num);
	}
	printf("\n");
	printf("��׺���ʽ��ֵ���:%d\n", calculateSuffixExp(suffixExpList));
	printf("ǰ׺���ʽ:");
	for (auto data = prefixExpList.getHead(); data; data = data->getNext()) {
		if (data->data.t == Node::Type::OP)printf("%c ", data->data.op);
		else printf("%d ", data->data.num);
	}
	printf("\n");
	printf("ǰ׺���ʽ��ֵ���:%d\n", calculatePrefixExp(prefixExpList));
}

int main() {
	printf("���ʽ��ֵ����\n");
    fflush(stdout);
    expression();
	return 0;
}