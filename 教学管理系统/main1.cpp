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

// 课程结构体
struct Course {
    string id;              // 课程编号
    string name;            // 课程名称
    vector<string> prereqs; // 先修课程
};

// 教学计划管理类
class CourseManager {
private:
    unordered_map<string, Course> courses; // 存储课程信息, 键为课程编号

public:
    // 1. 课程进修目录的读入,添加课程, 如果课程存在则报错
    void addCourse(const string& id, const string& name, const vector<string>& prereqs) {
        if (courses.find(id) != courses.end()) {
            cout << "课程ID已存在: " << id << endl;
            return;
        }
        courses[id] = {id, name, prereqs};
        cout << "课程添加成功: " << name << endl;
    }

    // 2. 课程目录的编辑：删除课程
    void removeCourse(const string& id) {
        if (courses.erase(id)) {
            cout << "课程删除成功: " << id << endl;
        } else {
            cout << "课程未找到: " << id << endl;
        }
    }

    // 2. 课程目录的编辑：修改课程信息, 课程不存在则报错
    void editCourse(const string& id, const string& newName, const vector<string>& newPrereqs) {
        if (courses.find(id) == courses.end()) {
            cout << "课程未找到: " << id << endl;
            return;
        }
        courses[id] = {id, newName, newPrereqs};
        cout << "课程信息已更新: " << newName << endl;
    }

    // 打印课程表
    void printCourses() {
        cout << "当前课程目录：" << endl;
        for (const auto& [id, course] : courses) {
            cout << "课程编号: " << course.id << ", 名称: " << course.name;
            if (!course.prereqs.empty()) {
                cout << ", 先修课程: ";
                for (const auto& prereq : course.prereqs) {
                    cout << prereq << " ";
                }
            }
            cout << endl;
        }
    }

    // 3. 输出教学计划
    //使用kahn排序算法, 考虑到每周最大课程限制和先修课程的限制
    void generatePlan(int maxCoursesPerSemester) {
        unordered_map<string, int> indegree;
        for (const auto& [id, course] : courses) {
            indegree[id] = 0; // 初始化入度
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
            cout << "第 " << semester << " 学期的课程: " << endl;
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
        cout << "\n=== 教学计划管理系统 ===" << endl;
        cout << "1. 添加课程" << endl;
        cout << "2. 删除课程" << endl;
        cout << "3. 修改课程" << endl;
        cout << "4. 打印课程目录" << endl;
        cout << "5. 生成教学计划" << endl;
        cout << "6. 退出" << endl;
        cout << "请选择操作: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string id, name, prereq;
                int prereqCount;
                vector<string> prereqs;

                cout << "请输入课程编号: ";
                cin >> id;
                cout << "请输入课程名称: ";
                cin.ignore();
                getline(cin, name);

                cout << "请输入先修课程数量: ";
                cin >> prereqCount;
                cout << "请输入先修课程编号(用空格隔开): ";
                for (int i = 0; i < prereqCount; ++i) {
                    cin >> prereq;
                    prereqs.push_back(prereq);
                }

                manager.addCourse(id, name, prereqs);
                break;
            }
            case 2: {
                string id;
                cout << "请输入要删除的课程编号: ";
                cin >> id;
                manager.removeCourse(id);
                break;
            }
            case 3: {
                string id, newName, prereq;
                int prereqCount;
                vector<string> newPrereqs;

                cout << "请输入要修改的课程编号: ";
                cin >> id;
                cout << "请输入新的课程名称: ";
                cin.ignore();
                getline(cin, newName);

                cout << "请输入新的先修课程数量: ";
                cin >> prereqCount;
                cout << "请输入新的先修课程编号(用空格隔开): ";
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
                cout << "请输入每学期最多课程数: ";
                cin >> maxCoursesPerSemester;
                manager.generatePlan(maxCoursesPerSemester);
                break;
            }
            case 6:
                cout << "退出系统。" << endl;
                break;
            default:
                cout << "无效选择，请重新输入。" << endl;
        }
    } while (choice != 6);

    return 0;
}


//0 C1 程序设计基础 2
//1 C2 离散数学 3
//2 C3 数据结构 4
//3 C4 汇编语言 3
//4 C6 计算机原理 3
//5 C7 编译原理 4
//6 C8 操作系统 4
