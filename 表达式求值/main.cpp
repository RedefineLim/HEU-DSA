#include<stdio.h>
#include<utility>
#include"Stack.h"
#include"LinkedList.h"

//实现表达式求值
//3+(5/2)%4*2+(1-3)*4=-1
//2*((30/3%4*1)+22/(41-6)%4)/3=1


//定义Node节点区分操作数与操作符
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

//从每个节点中分别得到各自对应的操作数和操作符
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

//将中缀表达式转换为后缀表达式，使用栈来处理操作符的优先级和括号
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

//将中缀表达式转换为前缀表达式，同样使用栈来处理操作符的优先级和括号
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

//计算前缀表达式的值，使用栈来存储操作数，并根据操作符进行计算
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

//计算后缀表达式的值，同样使用栈来存储操作数，并根据操作符进行计算
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

//读取用户输入的中缀表达式，调用转换和求值函数，并输出后缀和前缀表达式及其计算结果
void expression() {
	char str[100];
	printf("请输入表达式:\n");
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
	printf("后缀表达式:");
	for (auto data = suffixExpList.getHead(); data; data = data->getNext()) {
		if (data->data.t == Node::Type::OP)printf("%c ", data->data.op);
		else printf("%d ", data->data.num);
	}
	printf("\n");
	printf("后缀表达式求值结果:%d\n", calculateSuffixExp(suffixExpList));
	printf("前缀表达式:");
	for (auto data = prefixExpList.getHead(); data; data = data->getNext()) {
		if (data->data.t == Node::Type::OP)printf("%c ", data->data.op);
		else printf("%d ", data->data.num);
	}
	printf("\n");
	printf("前缀表达式求值结果:%d\n", calculatePrefixExp(prefixExpList));
}

int main() {
	printf("表达式求值问题\n");
    fflush(stdout);
    expression();
	return 0;
}