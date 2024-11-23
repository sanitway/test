#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

// 定义学生结构体
struct Student {
    string name;
    string id;
    int chinese;
    int math;
    int english;
    int total;

    // 构造函数计算总分
    Student(string n, string i, int c, int m, int e) : name(n), id(i), chinese(c), math(m), english(e) {
        total = chinese + math + english;
    }
};

// 读取文件，解析为学生数据
vector<Student> readFile(const string& filename) {
    vector<Student> students;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return students;
    }
    string name, id;
    int chinese, math, english;
    string header;
    getline(file, header); // 跳过第一行表头
    while (file >> name >> id >> chinese >> math >> english) {
        students.emplace_back(name, id, chinese, math, english);
    }
    file.close();
    return students;
}

// 写文件
void writeFile(const string& filename, const vector<Student>& students) {
    ofstream file(filename);
    file << "姓名\t学号\t语文\t数学\t英语\t总分\n";
    for (const auto& student : students) {
        file << student.name << "\t" << student.id << "\t" << student.chinese << "\t" 
             << student.math << "\t" << student.english << "\t" << student.total << "\n";
    }
    file.close();
}

// 合并文件
vector<Student> mergeFiles(const vector<Student>& file1, const vector<Student>& file2) {
    vector<Student> merged = file1;
    merged.insert(merged.end(), file2.begin(), file2.end());
    return merged;
}

// 抽取补考学生
vector<Student> getRetakeStudents(const vector<Student>& students) {
    vector<Student> retakes;
    for (const auto& student : students) {
        if (student.chinese < 60 || student.math < 60 || student.english < 60) {
            retakes.push_back(student);
        }
    }
    // 按学号排序
    sort(retakes.begin(), retakes.end(), [](const Student& a, const Student& b) {
        return a.id < b.id;
    });
    return retakes;
}

// 总分降序排序
void sortByTotal(vector<Student>& students) {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.total > b.total;
    });
}

// 二分法查找补考学生
bool binarySearch(const vector<Student>& students, const string& id, int& comparisons) {
    int left = 0, right = students.size() - 1;
    comparisons = 0;
    while (left <= right) {
        comparisons++;
        int mid = left + (right - left) / 2;
        if (students[mid].id == id) {
            cout << "找到学生信息: " << students[mid].name << ", 总分: " << students[mid].total << endl;
            return true;
        } else if (students[mid].id < id) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return false;
}

// 测试排序算法效率
template <typename Func>
double testSortEfficiency(vector<Student> students, Func sortFunc) {
    auto start = chrono::high_resolution_clock::now();
    sortFunc(students);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    return duration.count();
}

// 主程序
int main() {
    // 读取文件1和文件2
    vector<Student> file1 = readFile("1.txt");
    vector<Student> file2 = readFile("2.txt");

    // 合并文件
    vector<Student> merged = mergeFiles(file1, file2);
    writeFile("3.txt", merged);
    cout << "合并后的文件已保存为 3.txt。\n";

    // 抽取补考学生
    vector<Student> retakes = getRetakeStudents(merged);
    writeFile("4.txt", retakes);
    cout << "补考学生信息已保存为 4.txt。\n";

    // 排序测试
    vector<Student> copy = merged;
    double time1 = testSortEfficiency(copy, sortByTotal);
    cout << "排序时间: " << time1 << " 秒。\n";

    // 二分查找补考学生
    string searchId;
    cout << "请输入要查找的补考学生学号: ";
    cin >> searchId;
    int comparisons;
    if (!binarySearch(retakes, searchId, comparisons)) {
        cout << "未找到学号为 " << searchId << " 的学生。\n";
    } else {
        cout << "比较次数: " << comparisons << endl;
    }

    return 0;
}

