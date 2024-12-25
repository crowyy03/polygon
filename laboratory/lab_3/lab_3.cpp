#include <iostream> 
#include <iomanip> 
#include <vector> 
#include <cmath> 
#include <string>
#include <fstream> 
#include <map> 
#include <set> 
#include <queue> 
#include <stack> 
#include <deque>

using namespace std;

int main(){
    ifstream fin("test1.txt"); 
    ofstream fein("test_result.txt"); 
    int n, ss = 0; fin >> n; // O(1) время, O(1) память

    vector <string> ass_str(n); // O(n) память (в худшем случае, если строки длинные)

    for (int i = 0; i < n; ++i) {
        fin >> ass_str[i]; // O(1) время (в каждой итерации, в предположении, что чтение строки - операция с константным временем)
        if (ass_str[i] != "null") // O(1) время (сравнение строк)
            ss += stoi(ass_str[i]); // O(1) время (в предположении, что stoi() работает за константное время)
    }
    
    int aver = ss / n; // O(1) время, O(1) память

    fein << "average: " << aver << endl; // O(1) время

    for (int i = 0; i < n; ++i){
        if (ass_str[i] == "null") // O(1) время
            ass_str[i] = to_string(aver); // O(1) время (в предположении, что to_string() работает за константное время)

        fein << ass_str[i] << " "; // O(1) время (в каждой итерации)
    }

    fin.close(); 
    fein.close(); 
    
    return 0;
}
