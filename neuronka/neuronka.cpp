// Фаннур Fannur AI
//

#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <math.h>
#include <mutex>
#include <time.h>

using namespace std;
ofstream fout;
ifstream fin;
ifstream fin1;
ifstream fin2;
ifstream fin3;
mutex mtx;

float getCens() {
    string buffer;
    float x = 0;
    fin.open("other.txt");
    getline(fin, buffer);
    for (int i = 0; i < 2; i++) {
        x += (int(buffer[i]) - 48) * pow(10, -(i + 1));
    }
    fin.close();
    return x;
}

float getCycN() {
    string buffer;
    float x = 0;
    fin.open("other.txt");
    getline(fin, buffer);
    getline(fin, buffer);
    for (int i = 0; i < 2; i++) {
        x += (int(buffer[i]) - 48) * pow(10, -i);
    }
    fin.close();
    return x;
}

int getMemN() {
    string buffer;
    float x = 0;
    fin.open("other.txt");
    getline(fin, buffer);
    getline(fin, buffer);
    getline(fin, buffer);
    for (int i = 0; i < size(buffer); i++) {
        x += (int(buffer[i]) - 48) * pow(10, -i);
    }
    x = x * pow(10, size(buffer) - 1);
    fin.close();
    return x;
}

int MemCostyl() {
    string buffer;
    float x = 0;
    fin.open("other.txt");
    getline(fin, buffer);
    getline(fin, buffer);
    getline(fin, buffer);
    x = int(buffer[0]) * pow(10, size(buffer) - 1);
    fin.close();
    return x;
}

int counter1 = 0;
int counter2 = 0;
int counter3 = 0;
int MN = 0;
int MemN = getMemN();
string input;
string input1;
string prediction;
string thoughts;
int cycN = getCycN();
int operCoefs[12000];
string operMem[12000];
float cens = getCens();
const int NeuronNumber = 600000;

int module(int x) {
    if (x < 0) {
        x = -x;
    }
    return x;
}

float getWeight(int Adress, int Target);

class neuron {
public:
    float weight = 2;
    float In = 0;
    float Out = 0;
    float Kostlout = 0;
    int Range = 0;
    int Start = 0;
    int End = 0;
    int Adress = 0;
    int Target = 0;
    int GetCount = 0;
    float Akcent = 0;

    void NeurAct(string input, neuron web[], int& counter) {
        int i = 0;
        int N = 0;
        for ( ; i < size(input) ; i++) {
            if (input[i] == ' ' and i > Start) {
                End = i;
                if (counter == i) {
                    web[Adress + 1].Start = End;
                    N = 0;
                    break;
                }
                else if(counter != 0 and Start - End != 1){
                    for (int j = 0; j < counter - Start; j++) {
                        N = int((Start - End) / 2) - j;
                        int t = 0;
                        if (N <= 0) {
                            N = j - int((Start - End) / 2) + t;
                            if (N == 0) {
                                N = 1;
                                t = 1;
                            }
                        }
                    }
                }
                else if(Start - End == 1){
                    N = 2;
                }
                else {
                    N = int(End / 2);
                }
            }
        }
        int neighbour = 0;
        for (int i = 1; i < Range * 2 + 1; i++) {
            if (counter - i == 0) {
            }
            else if (counter - i <= 0) {
                neighbour += int(input[i - counter]);
            }
            else {
                neighbour += int(input[counter - i]);
            }
        }
        if (weight == 2) {
            weight = getWeight(Adress, Target);
        }
        mtx.lock();
        Out = int(((int(input[counter]) * N + neighbour / (Range * 2)) + In / GetCount) * weight) / (1 + module(((int(input[counter]) * N + neighbour / (Range * 2)) + In / GetCount) * weight));
        Kostlout = int(((int(input[counter]) * N + neighbour / (Range * 2)) + In / GetCount) * Akcent) / (1 + module(((int(input[counter]) * N + neighbour / (Range * 2)) + In / GetCount) * Akcent));
        for (int i = 1; i < int(Range / weight) * 2 + 1; i++) {
            if (Target - i == Adress) {
            }
            else if (Target - i <= 0) {
                web[i - Target].In += Out * Akcent / (i - Target + 1);
                web[i - Target].GetCount++;
            }
            else {
                web[Target - i].In += Out * Akcent / (Target - i + 1);
                web[Target - i].GetCount++;
            }
        }
        GetCount = 0;
        In = fmod(In * pow(weight, Target - Adress) * -Akcent, 9);
       counter++;
       mtx.unlock();
    }

    void NeurAct2(neuron web[]) {
        Out = int((In / GetCount) * weight) / (1 + module((In / GetCount) * weight));
        Kostlout = int((In / GetCount) * Akcent) / (1 + module((In / GetCount) * Akcent));
        for (int i = 1; i < int(Range / weight) * 2 + 1; i++) {
            if (Target - i == Adress) {
            }
            else if (Target - i <= 0) {
                web[i - Target].In += Out * Akcent / (i - Target + 1);
                web[i - Target].GetCount++;
            }
            else {
                web[Target - i].In += Out * Akcent / (Target - i + 1);
                web[Target - i].GetCount++;
            }
        }
        GetCount = 0;
        In = fmod(In * pow(weight, Target - Adress) * -Akcent, 9);
    }
};

neuron Network[NeuronNumber];

float getWeight(int Adress, int Target) {
    string buffer;
    float x = 0;
    fin.open(to_string(Adress) + "W.txt");
    if (!fin.is_open()) {
        fout.open(to_string(Adress) + "W.txt");
        for (int r = 0; r < NeuronNumber / 12000; r++) {
            for (int j = 0; j < 72000; j++) {
                    srand(time(NULL));
                    fout << (rand() % 9);
            }
            fout << endl;
        }
        fout.close();
    }
    fin.open(to_string(Adress) + "W.txt");
    fin.clear();
    fin.seekg(0);
    getline(fin, buffer, '\n');
    for (int i = 0; i < Target / 12000; i++) {
        getline(fin, buffer, '\n');
    }
    for (int i = 0; i < 6; i++) {
        x += (int(buffer[i + (Target % 12000) * 6]) - 48) * pow(10, -(i + 1));
    }
    fin.close();
    return x;
}

void getAkcent() {
    string bufferA;
    fin3.open("A.txt");
    if (!fin3.is_open()) {
        fout.open("A.txt");
        for (int r = 0; r < NeuronNumber / 12000; r++) {
            for (int j = 0; j < 12000; j++) {
                for (int i = 0; i < 6; i++) {
                    srand(time(NULL));
                    fout << to_string(rand() % 9);
                }
            }
            fout << endl;
        }
        fout.close();
    }
    fin3.open("A.txt");
    fin3.clear();
    fin3.seekg(0);
    getline(fin3, bufferA, '\n');
    for (int i = 0; i < NeuronNumber / 12000; i++) {
        getline(fin3, bufferA, '\n');
        for (int j = 0; j < 72000; j++) {
            Network[j / 6 + i * 12000].Akcent += (int(bufferA[j]) - 48) * pow(10, -(j % 6 + 1));
        }
    }
    fin3.close();
}

bool checkZero(int arr[], int& rty) {
    int i = 0;
    while (arr[i] != 0 and i < 12000) {
        i++;
    }
    if (arr[i] != 0) {
        return false;
    }
    else if (arr[i] == 0) {
        rty = i;
        return true;
    }
}

bool checkLow(int arr[], int coef, int& rty) {
    int i = 0;
    while (arr[i] > coef and i < 12000) {
        i++;
    }
    if (arr[i] >= coef) {
        return false;
    }
    else if (arr[i] < coef) {
        rty = i;
        return true;
    }
}

void getRange() {
    string bufferR;
    float r = 0;
    fin1.open("R.txt");
    if (!fin1.is_open()) {
        fout.open("R.txt");
        for (int r = 0; r < NeuronNumber / 12000; r++) {
            for (int j = 0; j < 12000; j++) {
                srand(time(NULL));
                fout << to_string(rand() % 5);
                for (int i = 0; i < 5; i++) {
                    srand(time(NULL));
                    fout << to_string(rand() % 9);
                }
            }
            fout << endl;
        }
        fout.close();
    }
    fin1.open("R.txt");
    fin1.clear();
    fin1.seekg(0);
    for (int i = 0; i < NeuronNumber / 12000; i++) {
        getline(fin1, bufferR, '\n');
        for (int j = 0; j < 72000; j++) {
            Network[j / 6 + i * 12000].Range += (int(bufferR[j]) - 48) * pow(10, -(j % 6 + 1)) * 1000000;
        }
    }
    fin1.close();
}

void getTarget() {
    string bufferT;
    float t = 0;
    fin2.open("T.txt");
    if (!fin2.is_open()) {
        fout.open("T.txt");
        for (int r = 0; r < NeuronNumber / 12000; r++) {
            for (int j = 0; j < 12000; j++) {
                srand(time(NULL));
                fout << to_string(rand() % 5);
                for (int i = 0; i < 5; i++) {
                    srand(time(NULL));
                    fout << to_string(rand() % 9);
                }
            }
            fout << endl;
        }
        fout.close();
    }
    fin2.open("T.txt");
    fin2.clear();
    fin2.seekg(0);
    for (int i = 0; i < NeuronNumber / 12000; i++) {
        getline(fin2, bufferT, '\n');
        for (int j = 0; j < 72000; j++) {
            Network[j / 6 + i * 12000].Target += (int(bufferT[j]) - 48) * pow(10, -(j % 6 + 1)) * 1000000;
        }
    }
    fin2.close();
}

void loadNet(int N) {
    thread A(getAkcent);
    thread R(getRange);
    thread T(getTarget);
    for (int i = 0; i < N; i++) {
        Network[i].Adress = i;
    }
    A.join();
    R.join();
    T.join();
}

void OperMem(string input) {
    int k = 0;
    int rty = 0;
    for (int i = 0; i < size(input); i++) {
        if (input[i] == ' ' or input[i] == '.' or input[i] == ',' or input[i] == '\0') {
            if (k < 12000) {
                k++;
            }
            else {
                break;
            }
        }
    }
    int* MemBuf = new int[k];
    int* MemBuf2 = new int[k];
    for (int i = 0; i < size(input); i++) {
        int b = 0;
        if (b < k and (input[i] == ' ' or input[i] == '.' or input[i] == ',' or input[i] == '\0')) {
            MemBuf[b] = i;
            MemBuf2[b] = i;
            b++;
        }
    }
    for (int i = 1; i < k; i++) {
        int rng = MemBuf[i] - MemBuf[i - 1];
        for (int j = MemBuf[i - 1]; j < MemBuf[i]; j++) {
            MemBuf[i - 1] = 0;
            MemBuf[i - 1] += Network[j].Kostlout;
        }
        MemBuf[i - 1] = MemBuf[i - 1] / rng;
    }
    for (int i = 0; i < k - 1; i++) {
        int yuy = 0;
        if (MemBuf[i] >= cens and (checkLow(operCoefs, MemBuf[i], rty) or checkZero(operCoefs, rty))) {
            operCoefs[rty] = MemBuf[i];
            int ijt = 0;
            while (operMem[rty][ijt] != '\0') {
                operMem[rty][ijt] = '\0';
                ijt++;
            }
            for (int ij = 0; ij < MemBuf2[i + 1] - MemBuf2[i]; ij++) {
                operMem[rty][ij] = input[ij + 1 + MemBuf2[i]];
            }
        }
    }
    delete[] MemBuf;
    delete[] MemBuf2;
}

void ReadMemory() {
    string buffer;
    int CtR;
    int ctr = 1;
    if (input != input1 or Network[555001].Out > 0.5) {
        MN = int(Network[555000].Out * MemCostyl()) % MemN;
        for (int i = 0; i < size(input1); i++) {
            if (input1[i] == ' ') {
                ctr++;
            }
        }
        string* InBuf = new string[ctr];
        int ji = 0;
        for (int i = 0; i < ctr; i++) {
            while (input1[ji] != ' ') {
                InBuf[i] += input1[ji];
                ji++;
            }
            ji++;
        }
        CtR = ctr;
        for (;;) {
            buffer = "Memory";
            buffer += to_string(MN);
            fin.open(buffer);
            if (!fin.is_open()) {
                return;
            }
            getline(fin, buffer);
            ctr = 1;
            for (int i = 0; i < size(buffer); i++) {
                if (buffer[i] == ' ') {
                    ctr++;
                }
            }
            string* MemBuf = new string[ctr];
            int j = 0;
            for (int i = 0; i < ctr; i++) {
                while (buffer[j] != ' ') {
                    MemBuf[i] += buffer[j];
                    j++;
                }
                j++;
            }
            float cTr = 0;
            for (int i = 0; i < ctr; i++) {
                for (int j = 0; j < CtR; j++) {
                    if (MemBuf[i] == InBuf[j]) {
                        cTr++;
                    }
                }
            }
            cTr = cTr / CtR;
            if (cTr > 0.85) {
                MN++;
                break;
            }
            fin.close();
        }
    }
    else {
        for (int i = 0; i < size(input); i++) {
            if (input[i] == ' ') {
                ctr++;
            }
        }
        string* InBuf = new string[ctr];
        int ji = 0;
        for (int i = 0; i < ctr; i++) {
            while (input[ji] != ' ') {
                InBuf[i] += input[ji];
                ji++;
            }
            ji++;
        }
        CtR = ctr;
        for (;;) {
            buffer = "Memory";
            buffer += to_string(MN);
            fin.open(buffer);
            if (!fin.is_open()) {
                return;
            }
            getline(fin, buffer);
            ctr = 1;
            for (int i = 0; i < size(buffer); i++) {
                if (buffer[i] == ' ') {
                    ctr++;
                }
            }
            string* MemBuf = new string[ctr];
            int j = 0;
            for (int i = 0; i < ctr; i++) {
                while (buffer[j] != ' ') {
                    MemBuf[i] += buffer[j];
                    j++;
                }
                j++;
            }
            float cTr = 0;
            for (int i = 0; i < ctr; i++) {
                for (int j = 0; j < CtR; j++) {
                    if (MemBuf[i] == InBuf[j]) {
                        cTr++;
                    }
                }
            }
            cTr = cTr / CtR;
            if (cTr > 0.85) {
                MN++;
                break;
            }
            fin.close();
        }
    }
    counter3 = 0;
    for (int i = 0; i < size(buffer); i++) {
        Network[i + 360000].NeurAct(buffer, Network, counter3);
    }
    for (int i = 0; i < 30000; i++) {
        int ggg = 0;
        fin.open("Mem" + to_string(i) + '_' + to_string(MN - 1));
        getline(fin, buffer);
        for (int oi = 0; oi < 6; oi++) {
           ggg  += pow(10, -oi - 1) * (int(buffer[oi]) - 48);
        }
        Network[i + 565000].Out = ggg;
        fin.close();
    }
}

void NNActivity();
void Predictor();

int main() {
    for (int i = 0; i < 12000; i++) {
        operCoefs[i] = 0;
    }
    getline(cin, input, '\n');
    thread work(NNActivity);
    work.detach();
    for (;;) {
        if (input1 == "0N3") {
            break;
        }
        getline(cin, input1, '\n');
    }
}

void NNActivity() {
    loadNet(NeuronNumber);
    counter1 = 0;
    for (; ;) {
        mtx.lock();
        if (input1 != "") {
            input = input1;
        }
        mtx.unlock();
        thread MemWork(ReadMemory);
        thread predictor(Predictor);
        for (int itteration = 0; itteration < cycN; itteration++) {
            for (int cycle = 0; cycle < size(input); cycle++) {
                Network[cycle].NeurAct(input, Network, counter1);
            }
            OperMem(input);
            int tyn = 0;
            while (operMem[tyn] != "") {
                tyn++;
            }
            counter1 = 0;
            for (int i = 0; i < tyn + 1; i++) {
                int j = 0;
                for (; j < size(operMem[i]); j++) {
                    Network[j + 80000].NeurAct(operMem[i], Network, counter1);
                }
            }
            counter1 = 0;
            if (thoughts != "") {
                for (int cycle = 0; cycle < size(thoughts); cycle++) {
                    Network[cycle + 200000].NeurAct(thoughts, Network, counter1);
                }
                counter1 = 0;
            }
            predictor.join();
            MemWork.join();
            for (int out = 0; out < NeuronNumber; out++) {
                Network[out].NeurAct2(Network);
                if (out % (NeuronNumber / 1000) == 0) {
                    cout << char(abs(Network[599999].In * 10000));
                }
            }
        }
    }
}

void Predictor() {
    if (input1 != "") {
        input = input1;
    }
    for (int i = 0; i < size(input) % 5; i++) {
        for (int cycle = 0; cycle < size(input); cycle++) {
            Network[cycle + 280000].NeurAct(input, Network, counter2);
        }
    }
    counter2 = 0;
    int prctr = 0;
    while (prctr < 30000 and int(prediction[prctr]) != 59000) {
        prediction += char(abs(Network[280001 + prctr].In * 10000));
        prctr++;
    }
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
