//
// Created by guoqi on 2024/11/20.
//

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

// �γ̽ṹ��
struct Course {
    string id;              // �γ̱��
    string name;            // �γ�����
    vector<string> prereqs; // ���޿γ�
};

// ��ѧ�ƻ�������
class CourseManager {
private:
    unordered_map<string, Course> courses; // �洢�γ���Ϣ, ��Ϊ�γ̱��

public:
    // 1. �γ̽���Ŀ¼�Ķ���,��ӿγ�, ����γ̴����򱨴�
    void addCourse(const string& id, const string& name, const vector<string>& prereqs) {
        if (courses.find(id) != courses.end()) {
            cout << "�γ�ID�Ѵ���: " << id << endl;
            return;
        }
        courses[id] = {id, name, prereqs};
        cout << "�γ���ӳɹ�: " << name << endl;
    }

    // 2. �γ�Ŀ¼�ı༭��ɾ���γ�
    void removeCourse(const string& id) {
        if (courses.erase(id)) {
            cout << "�γ�ɾ���ɹ�: " << id << endl;
        } else {
            cout << "�γ�δ�ҵ�: " << id << endl;
        }
    }

    // 2. �γ�Ŀ¼�ı༭���޸Ŀγ���Ϣ, �γ̲������򱨴�
    void editCourse(const string& id, const string& newName, const vector<string>& newPrereqs) {
        if (courses.find(id) == courses.end()) {
            cout << "�γ�δ�ҵ�: " << id << endl;
            return;
        }
        courses[id] = {id, newName, newPrereqs};
        cout << "�γ���Ϣ�Ѹ���: " << newName << endl;
    }

    // ��ӡ�γ̱�
    void printCourses() {
        cout << "��ǰ�γ�Ŀ¼��" << endl;
        for (const auto& [id, course] : courses) {
            cout << "�γ̱��: " << course.id << ", ����: " << course.name;
            if (!course.prereqs.empty()) {
                cout << ", ���޿γ�: ";
                for (const auto& prereq : course.prereqs) {
                    cout << prereq << " ";
                }
            }
            cout << endl;
        }
    }

    // 3. �����ѧ�ƻ�
    //ʹ��kahn�����㷨, ���ǵ�ÿ�����γ����ƺ����޿γ̵�����
    void generatePlan(int maxCoursesPerSemester) {
        unordered_map<string, int> indegree;
        for (const auto& [id, course] : courses) {
            indegree[id] = 0; // ��ʼ�����
        }
        for (const auto& [id, course] : courses) {
            for (const string& prereq : course.prereqs) {
                indegree[id]++;
            }
        }

        queue<string> q;
        for (const auto& [id, degree] : indegree) {
            if (degree == 0) {
                q.push(id);
            }
        }

        int semester = 1;
        while (!q.empty()) {
            cout << "�� " << semester << " ѧ�ڵĿγ�: " << endl;
            int count = 0;
            vector<string> nextQueue;

            while (!q.empty() && count < maxCoursesPerSemester) {
                string courseId = q.front();
                q.pop();
                cout << "- " << courses[courseId].name << endl;
                count++;

                for (auto& [id, course] : courses) {
                    auto& prereqs = course.prereqs;
                    auto it = find(prereqs.begin(), prereqs.end(), courseId);
                    if (it != prereqs.end()) {
                        prereqs.erase(it);
                        indegree[id]--;
                        if (indegree[id] == 0) {
                            nextQueue.push_back(id);
                        }
                    }
                }
            }

            for (const string& id : nextQueue) {
                q.push(id);
            }
            semester++;
        }
    }
};

int main() {
    CourseManager manager;
    int choice;

    do {
        cout << "\n=== ��ѧ�ƻ�����ϵͳ ===" << endl;
        cout << "1. ��ӿγ�" << endl;
        cout << "2. ɾ���γ�" << endl;
        cout << "3. �޸Ŀγ�" << endl;
        cout << "4. ��ӡ�γ�Ŀ¼" << endl;
        cout << "5. ���ɽ�ѧ�ƻ�" << endl;
        cout << "6. �˳�" << endl;
        cout << "��ѡ�����: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string id, name, prereq;
                int prereqCount;
                vector<string> prereqs;

                cout << "������γ̱��: ";
                cin >> id;
                cout << "������γ�����: ";
                cin.ignore();
                getline(cin, name);

                cout << "���������޿γ�����: ";
                cin >> prereqCount;
                cout << "���������޿γ̱��(�ÿո����): ";
                for (int i = 0; i < prereqCount; ++i) {
                    cin >> prereq;
                    prereqs.push_back(prereq);
                }

                manager.addCourse(id, name, prereqs);
                break;
            }
            case 2: {
                string id;
                cout << "������Ҫɾ���Ŀγ̱��: ";
                cin >> id;
                manager.removeCourse(id);
                break;
            }
            case 3: {
                string id, newName, prereq;
                int prereqCount;
                vector<string> newPrereqs;

                cout << "������Ҫ�޸ĵĿγ̱��: ";
                cin >> id;
                cout << "�������µĿγ�����: ";
                cin.ignore();
                getline(cin, newName);

                cout << "�������µ����޿γ�����: ";
                cin >> prereqCount;
                cout << "�������µ����޿γ̱��(�ÿո����): ";
                for (int i = 0; i < prereqCount; ++i) {
                    cin >> prereq;
                    newPrereqs.push_back(prereq);
                }

                manager.editCourse(id, newName, newPrereqs);
                break;
            }
            case 4: {
                manager.printCourses();
                break;
            }
            case 5: {
                int maxCoursesPerSemester;
                cout << "������ÿѧ�����γ���: ";
                cin >> maxCoursesPerSemester;
                manager.generatePlan(maxCoursesPerSemester);
                break;
            }
            case 6:
                cout << "�˳�ϵͳ��" << endl;
                break;
            default:
                cout << "��Чѡ�����������롣" << endl;
        }
    } while (choice != 6);

    return 0;
}


//0 C1 ������ƻ��� 2
//1 C2 ��ɢ��ѧ 3
//2 C3 ���ݽṹ 4
//3 C4 ������� 3
//4 C6 �����ԭ�� 3
//5 C7 ����ԭ�� 4
//6 C8 ����ϵͳ 4
