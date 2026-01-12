#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
using namespace std::chrono;


// Перебираем рекурсивно все последовательности индексов длины K
// Итоговая временная сложность: O(N^K * K log K)
// Итоговая пространственная сложность: O(K^2 + R * K)
void getArrayWithZeroSum(const vector<int> &arr, int K, vector<int> &current, set<vector<int> > &results) {
    if ((int) current.size() == K) {
        vector<int> sorted = current;
        sort(sorted.begin(), sorted.end()); // O(K log K)
        auto last = unique(sorted.begin(), sorted.end()); // O(K)
        if (last != sorted.end()) {
            return; // есть дубликаты
        }

        long long sum = 0;
        for (int idx: sorted) {
            sum += arr[idx]; // O(K)
        }
        if (sum == 0) {
            results.insert(sorted); // O(K log R) - вставка в set
        }
        return;
    }

    // O(N) на каждом уровне рекурсии
    // Общее число итераций: N^K
    for (int i = 0; i < (int) arr.size(); ++i) {
        current.push_back(i); // O(1) амортизированно
        getArrayWithZeroSum(arr, K, current, results);
        current.pop_back(); // O(1)
    }
}


// Чтение файла
bool readArrayFromFile(const string &filename, vector<int> &arr, int &K) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return false;
    }

    string line;

    // Первая строка - K
    if (!getline(file, line)) {
        cerr << "K is not in file" << endl;
        return false;
    }

    try {
        K = stoi(line);
    } catch (...) {
        cerr << "Invalid K value: '" << line << "'" << endl;
        return false;
    }

    // Вторая строка - массив
    if (!getline(file, line)) {
        cerr << "Array is missed" << endl;
        return false;
    }

    stringstream ss(line);
    string el;
    while (getline(ss, el, ',')) {
        // Удаляем пробелы по краям
        size_t start = el.find_first_not_of(" \t\r\n");
        size_t end = el.find_last_not_of(" \t\r\n");
        string clean = el.substr(start, end - start + 1);

        try {
            arr.push_back(stoi(clean));
        } catch (...) {
            cerr << "Invalid number: '" << clean << "'" << endl;
            return false;
        }
    }

    file.close();
    return true;
}

int main() {
    string filename = "input.txt";
    vector<int> arr;
    int K;

    if (!readArrayFromFile(filename, arr, K)) {
        return 1;
    }

    int N = arr.size();
    if (K <= 0 || K > N) {
        cout << "N=" << N << ", time(ms)=0\n";
        return 0;
    }

    // Память: O(R * K), где R - число результатов
    set<vector<int> > resultSet;
    vector<int> current; // Используется в рекурсии: O(K)

    auto start = high_resolution_clock::now();
    getArrayWithZeroSum(arr, K, current, resultSet);
    auto end = high_resolution_clock::now();

    for (const auto &comb: resultSet) {
        cout << "[";
        for (size_t i = 0; i < comb.size(); ++i) {
            if (i > 0) cout << ",";
            cout << comb[i];
        }
        cout << "]\n";
    }

    auto ms = duration_cast<milliseconds>(end - start).count();
    cout << "N=" << N << ", time(ms)=" << ms << "\n";

    return 0;
}
