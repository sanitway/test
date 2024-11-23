#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

// ����ѧ���ṹ��
struct Student {
    string name;
    string id;
    int chinese;
    int math;
    int english;
    int total;

    // ���캯�������ܷ�
    Student(string n, string i, int c, int m, int e) : name(n), id(i), chinese(c), math(m), english(e) {
        total = chinese + math + english;
    }
};

// ��ȡ�ļ�������Ϊѧ������
vector<Student> readFile(const string& filename) {
    vector<Student> students;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "�޷����ļ�: " << filename << endl;
        return students;
    }
    string name, id;
    int chinese, math, english;
    string header;
    getline(file, header); // ������һ�б�ͷ
    while (file >> name >> id >> chinese >> math >> english) {
        students.emplace_back(name, id, chinese, math, english);
    }
    file.close();
    return students;
}

// д�ļ�
void writeFile(const string& filename, const vector<Student>& students) {
    ofstream file(filename);
    file << "����\tѧ��\t����\t��ѧ\tӢ��\t�ܷ�\n";
    for (const auto& student : students) {
        file << student.name << "\t" << student.id << "\t" << student.chinese << "\t" 
             << student.math << "\t" << student.english << "\t" << student.total << "\n";
    }
    file.close();
}

// �ϲ��ļ�
vector<Student> mergeFiles(const vector<Student>& file1, const vector<Student>& file2) {
    vector<Student> merged = file1;
    merged.insert(merged.end(), file2.begin(), file2.end());
    return merged;
}

// ��ȡ����ѧ��
vector<Student> getRetakeStudents(const vector<Student>& students) {
    vector<Student> retakes;
    for (const auto& student : students) {
        if (student.chinese < 60 || student.math < 60 || student.english < 60) {
            retakes.push_back(student);
        }
    }
    // ��ѧ������
    sort(retakes.begin(), retakes.end(), [](const Student& a, const Student& b) {
        return a.id < b.id;
    });
    return retakes;
}

// �ֽܷ�������
void sortByTotal(vector<Student>& students) {
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.total > b.total;
    });
}

// ���ַ����Ҳ���ѧ��
bool binarySearch(const vector<Student>& students, const string& id, int& comparisons) {
    int left = 0, right = students.size() - 1;
    comparisons = 0;
    while (left <= right) {
        comparisons++;
        int mid = left + (right - left) / 2;
        if (students[mid].id == id) {
            cout << "�ҵ�ѧ����Ϣ: " << students[mid].name << ", �ܷ�: " << students[mid].total << endl;
            return true;
        } else if (students[mid].id < id) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return false;
}

// ���������㷨Ч��
template <typename Func>
double testSortEfficiency(vector<Student> students, Func sortFunc) {
    auto start = chrono::high_resolution_clock::now();
    sortFunc(students);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    return duration.count();
}

// ������
int main() {
    // ��ȡ�ļ�1���ļ�2
    vector<Student> file1 = readFile("1.txt");
    vector<Student> file2 = readFile("2.txt");

    // �ϲ��ļ�
    vector<Student> merged = mergeFiles(file1, file2);
    writeFile("3.txt", merged);
    cout << "�ϲ�����ļ��ѱ���Ϊ 3.txt��\n";

    // ��ȡ����ѧ��
    vector<Student> retakes = getRetakeStudents(merged);
    writeFile("4.txt", retakes);
    cout << "����ѧ����Ϣ�ѱ���Ϊ 4.txt��\n";

    // �������
    vector<Student> copy = merged;
    double time1 = testSortEfficiency(copy, sortByTotal);
    cout << "����ʱ��: " << time1 << " �롣\n";

    // ���ֲ��Ҳ���ѧ��
    string searchId;
    cout << "������Ҫ���ҵĲ���ѧ��ѧ��: ";
    cin >> searchId;
    int comparisons;
    if (!binarySearch(retakes, searchId, comparisons)) {
        cout << "δ�ҵ�ѧ��Ϊ " << searchId << " ��ѧ����\n";
    } else {
        cout << "�Ƚϴ���: " << comparisons << endl;
    }

    return 0;
}

