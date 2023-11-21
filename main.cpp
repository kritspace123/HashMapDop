#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <cmath>

#define M 11
#define T 4294967296 * 4294967296

using namespace std;

class Matrix {
private:
    int m_Rows;
    int m_Cols;
    int **m_Arr;
public:
    //Конструктора по умолчанию для инициализации матрицы размерности 0
    Matrix();
    //Конструктор, инициализации единичной матрицы указанной размерности
    Matrix(int n);
    //Конструктора, принимающего размерность и массив элементов, которые нужно расположить на главной
    //диагонали, остальные элементы заполнить нулями
    Matrix(int n, const int *a);
    //Конструктор, принимающей кол-во строк и кол-во столбцов матрицы, - создаёт матрицу и заполняет её нулями
    Matrix(int n , int m);
    //Конструктор копирования
    Matrix(const Matrix& other);


    friend ostream& operator<<(ostream& in, const Matrix& Matrix);
    friend istream& operator>>(istream& out, Matrix& Matrix);

    // Перегрузка оператора +
    Matrix operator+(const Matrix& other) const;
    //перегрузка оператора -
    Matrix operator-(const Matrix& other) const;
    //перегрузка оператора -
    Matrix operator*(const Matrix& other) const;
    //перегрузка оператора  ==
    bool operator==(const Matrix& other) const;
    //перегрузка оператора  !=
    bool operator!=(const Matrix& other) const;
    //перегрузка оператора ! для транспонирования матрицы
    Matrix operator!();
    //перегрузка оператора (,) -построение минора
    Matrix operator()(int a, int b) const;
    // copy
    Matrix& operator=(const Matrix& other);
    //Доп задания:
    int* operator[](int index) const;

    int GetRows() const{
        return m_Rows;
    }

    int GetCols() const{
        return m_Cols;
    }

    // Деструктор
    ~Matrix();
};

namespace std {
    template <>
    struct hash<Matrix> {
        size_t operator()(const Matrix& matrix) const {
            // Необходимо выбрать подходящую хеш-функцию на основе данных матрицы
            // Например, можно объединить хеш-значения отдельных элементов матрицы
            size_t hashValue = 0;
            for (int i = 0; i < matrix.GetRows(); ++i) {
                int * rows = matrix[i];
                for (int j = 0; j < matrix.GetCols(); ++j) {
                    int b = rows[j];
                    hashValue = (hashValue * M + hash<int>()(b)) % T;
                }
            }
            return hashValue;
        }
    };
}


template <typename K, typename V>
class HashMap{
private:
    struct Node{
        K m_Key;
        V m_Value;
        Node* m_Next;
        Node(const K& k, const V& v) : m_Key(k), m_Value(v), m_Next(nullptr) {}
    };

    Node** m_Table;
    int m_Size; /// количество элементов
    int m_Capacity; ///Размер массива хэштаблицы
    double m_LoadFactor = 0.75;
    hash<K> Hasher;

public:
    HashMap();

    void clear();
    ///Правило 3-х
    ~HashMap();
    HashMap(const HashMap& other);
    HashMap& operator=(const HashMap& other);

    void Resize();///Функция для увеличения размера хеш-таблицы
    V ElementSeatch(const K& key);///Поиск элемента в коллекции по заданному ключу.
    void Insert(const K& key, const V& value); ///Добавление нового элемента с заданными ключом и значением в коллекцию.
    void remove(const K& key);/// Удаление из коллекции элемента с заданным ключом.
    void PrintMap();

    // Iterator class
    class Iterator {
    private:
        const HashMap& m_Map;
        int m_Index;
        Node* m_Indicator;

    public:
        Iterator(const HashMap& HashMap);
        Iterator(const HashMap& HashMap, int Index, Node* Indicator);

        bool HasNext() const;
        void Next();///ищем следующий непустой бакет
        const K& getKey() const;
        const V& getValue() const;

        Iterator& operator++();

        void MoveToTheNext();
    };

    Iterator GetIterator() const;

    Iterator Begin() const;
    Iterator End() const; ///последний бакет

    int getTotalSize() const; ///вывод количества элементов в мапе

    int getUniqueSize() const; ///вывод количества уникальных элементов

};



///Реализация функций классов
Matrix::~Matrix() {
    if (m_Arr != nullptr) {// Проверка: была ли выделена память и нужно ли её освобождать
        for (int i = 0; i < m_Rows; ++i) {
            delete[] m_Arr[i];
        }
        delete[] m_Arr;
    }
}
Matrix::Matrix() : m_Rows(0), m_Cols(0), m_Arr(nullptr) {}
Matrix::Matrix(int n) : m_Rows(n), m_Cols(n), m_Arr(nullptr) {
    m_Arr = new int *[n]; //выделяем память ( создаем массив указателей на указатели )
    for (int i = 0; i < n; ++i) {
        m_Arr[i] = new int[n]; //выделяем память под массив размера n
        for (int j = 0; j < n; ++j) {
            if (j == i) {
                m_Arr[j][j] = 1; //Заполняем диагональ 1
            } else {
                m_Arr[i][j] = 0; //вне диагонали 0
            }
        }
    }
}
Matrix::Matrix(int n, const int *a) : m_Rows(n), m_Cols(n), m_Arr(nullptr) {
    m_Arr = new int *[n]; //выделяем память ( создаем массив указателей на указатели )
    int cou = 0;
    for (int i = 0; i < n; ++i) {
        m_Arr[i] = new int[n]; //выделяем память под массив размера n
        for (int j = 0; j < n; ++j) {
            if (j == i) {
                m_Arr[j][j] = a[cou++]; //Заполняем диагональ элементами из массива (a)
            } else {
                m_Arr[i][j] = 0; //вне диагонали 0
            }
        }
    }
}
Matrix::Matrix(int n , int m) : m_Rows(n), m_Cols(m), m_Arr(nullptr) {
    m_Arr = new int *[n]; //выделяем память ( создаем массив указателей на указатели )
    for (int i = 0; i < n; ++i) {
        m_Arr[i] = new int[m]; //выделяем память под массив размера n
        for (int j = 0; j < n; ++j) {
            m_Arr[i][j] = 0;
        }
    }
}
Matrix::Matrix(const Matrix& other):m_Rows(other.m_Rows), m_Cols(other.m_Cols), m_Arr(other.m_Arr) {}
//перегрузка оператора << - для вывода матрицы
ostream& operator<<(ostream& out, const Matrix& Matrix){
    for( int i = 0; i < Matrix.m_Rows; ++i){
        for (int j = 0; j < Matrix.m_Cols; ++j) {
            out << Matrix.m_Arr[i][j] << ' ';
        }
        out << endl;
    }
    return out;
}
//перегрузка оператора >> - для заполнения матрицы элементами из файла
istream& operator >> (istream& in, Matrix& Matrix){
    Matrix.m_Arr = new int*[Matrix.m_Rows];
    for (int i = 0; i < Matrix.m_Rows; ++i) {
        Matrix.m_Arr[i] = new int[Matrix.m_Cols];
    }
    for( int i = 0; i < Matrix.m_Rows; ++i){
        for (int j = 0; j < Matrix.m_Cols; ++j) {
            in >> Matrix.m_Arr[i][j];
        }
    }
    return in;
}

Matrix Matrix::operator+(const Matrix& other) const{
    if(m_Rows != other.m_Rows || m_Cols != other.m_Cols){
        cerr << "Сложение не возможно: не совпадает размерность матриц";
        exit(1);
    }
    Matrix result(m_Rows, m_Cols);
    for(int i = 0; i < m_Rows; ++i){
        for (int j = 0; j < m_Cols; ++j) {
            result.m_Arr[i][j] = m_Arr[i][j] + other.m_Arr[i][j];
        }
    }
    return result;
}
Matrix Matrix::operator-(const Matrix& other) const{
    if(m_Rows != other.m_Rows || m_Cols != other.m_Cols){
        cerr << "вычитание не возможно: не совпадает размерность матриц";
        exit(1);
    }
    Matrix result(m_Rows, m_Cols);
    for(int i = 0; i < m_Rows; ++i){
        for (int j = 0; j < m_Cols; ++j) {
            result.m_Arr[i][j] = m_Arr[i][j] - other.m_Arr[i][j];
        }
    }
    return result;
}
Matrix Matrix::operator*(const Matrix& other) const{ //возмжно утечка памяти
    if(m_Cols != other.m_Rows){
        cerr << "умножение не возможно: не совпадает размерность матриц";
        exit(1);
    }
    Matrix result(m_Rows, other.m_Cols);
    for(int i = 0; i < m_Rows; ++i){
        for (int j = 0; j < other.m_Cols; ++j) {
            for (int k =0; k < m_Cols; ++k){
                result.m_Arr[i][j] += m_Arr[i][k] * other.m_Arr[k][j];
            }
        }
    }
    return result;
}
bool Matrix::operator==(const Matrix& other) const{
    if(m_Rows != other.m_Rows || m_Cols != other.m_Cols){
        cerr << "Не совпадает размерность матриц";
        exit(1);
    }
    for (int i = 0; i < m_Rows; ++i){
        for (int j = 0; j < m_Cols; ++j) {
            if(m_Arr[i][j] != other.m_Arr[i][j]){
                return false;
            }
        }
    }
    return true;
}
bool Matrix::operator!=(const Matrix& other) const{
    if(m_Rows != other.m_Rows || m_Cols != other.m_Cols){
        cerr << "Не совпадает размерность матриц";
        exit(1);
    }
    for (int i = 0; i < m_Rows; ++i){
        for (int j = 0; j < m_Cols; ++j) {
            if(m_Arr[i][j] != other.m_Arr[i][j]){
                return true;
            }
        }
    }
    return false;
}
Matrix Matrix::operator!(){
    Matrix A(m_Cols, m_Rows);
    for (int i = 0; i < m_Cols; ++i) {
        for (int j = 0; j < m_Rows; ++j) {
            A.m_Arr[i][j] = m_Arr[j][i];
        }
    }
    return A;
}
Matrix Matrix::operator()(int a, int b) const{
    if(m_Rows != m_Cols){
        cerr << "Не возможно построить минор";
        exit(1);
    }
    if(a > m_Rows || b > m_Cols){
        cerr << "Ошибка: выход за размер матрицы";
        exit(1);
    }
    a = a-1;
    b = b-1;
    Matrix A(m_Rows-1);
    int c = 0, d = 0;
    for( int i = 0; i < m_Rows; ++i){
        if(i == a){
            continue;
        }
        for (int j = 0; j < m_Cols; ++j) {
            if(j == b){
                continue;
            }
            A.m_Arr[c][d] = m_Arr[i][j];
            d++;
            if(d == m_Cols-1){
                d = 0;
                c++;
            }
        }
    }
    return A;
}
Matrix& Matrix::operator=(const Matrix& other){
    if (this == &other) {
        return *this; // Проверка на самоприсваивание
    }
    // Освобождение текущих ресурсов
    for (int i = 0; i < m_Rows; ++i) {
        delete[] m_Arr[i];
    }
    delete[] m_Arr;
    m_Rows = other.m_Rows;
    m_Cols = other.m_Cols;
    m_Arr = new int*[m_Rows];
    for(int i = 0; i < m_Rows; ++i){
        m_Arr[i] = new int[m_Cols];
        for(int j =0; j < m_Cols; ++j){
            m_Arr[i][j] = other.m_Arr[i][j];
        }
    }
    return *this;
}
int* Matrix::operator[](int index) const{
    if(index >= m_Rows){
        cerr << "Ошибка: выход за размер матрицы";
        exit(1);
    }
    return m_Arr[index];
}



template <typename K, typename V>
HashMap<K, V>::HashMap() : m_Size(0), m_Capacity(M) {
    m_Table = new Node*[m_Capacity]();
}

template <typename K, typename V>
void HashMap<K, V>::clear() {
    for (int i = 0; i < m_Capacity; ++i) {
        Node* current = m_Table[i];
        while (current != nullptr) {
            Node* next = current->m_Next;
            delete current;
            current = next;
        }
        m_Table[i] = nullptr;
    }
    m_Size = 0;
}

template <typename K, typename V>
HashMap<K, V>::HashMap(const HashMap& other){
    // Выделение памяти для новой таблицы
    m_Table = new Node*[m_Capacity];
    // Инициализация каждого элемента таблицы
    for (int i = 0; i < m_Capacity; ++i) {
        if (other.m_Table[i] != nullptr) {
            // Копирование элементов из существующей таблицы в новую
            m_Table[i] = new Node(other.m_Table[i]->m_Key, other.m_Table[i]->m_Value);
            Node* current = m_Table[i];
            Node* otherCurrent = other.m_Table[i]->m_Next;
            while (otherCurrent != nullptr) {
                // Копирование элементов из связанных списков
                current->m_Next = new Node(otherCurrent->m_Key, otherCurrent->m_Value);
                current = current->m_Next;
                otherCurrent = otherCurrent->m_Next;
            }
        } else {
            m_Table[i] = nullptr;
        }
    }
}

template <typename K, typename V>
HashMap<K, V>::~HashMap(){
    // Освобождение памяти при удалении объекта
    clear();
    delete[] m_Table;
}

template <typename K, typename V>
HashMap<K, V>& HashMap<K, V>::operator=(const HashMap& other) {
    if (this != &other) {  // Check for self-assignment
        // Free resources of the current object
        clear();

        // Copy data from the other object
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;
        m_LoadFactor = other.m_LoadFactor;

        // Allocate memory for the new table
        m_Table = new Node*[m_Capacity];

        // Copy elements from the other object
        for (int i = 0; i < m_Capacity; ++i) {
            if (other.m_Table[i] != nullptr) {
                m_Table[i] = new Node(other.m_Table[i]->m_Key, other.m_Table[i]->m_Value);
                Node* current = m_Table[i];
                Node* otherCurrent = other.m_Table[i]->m_Next;
                while (otherCurrent != nullptr) {
                    current->m_Next = new Node(otherCurrent->m_Key, otherCurrent->m_Value);
                    current = current->m_Next;
                    otherCurrent = otherCurrent->m_Next;
                }
            } else {
                m_Table[i] = nullptr;
            }
        }
    }
    return *this;
}

template <typename K, typename V>
void HashMap<K, V>::Resize() {
    int newCapacity = m_Capacity * 2;
    Node** m_NewTable = new Node*[newCapacity];
    for (int i = 0; i < m_Capacity; ++i) {
        Node* m_Indicator = m_Table[i]; ///указателем на узел в хеш-таблице
        while (m_Indicator != nullptr) {
            Node* m_Next = m_Indicator->m_Next;
            int index = Hasher(m_Indicator->m_Key) % newCapacity;
            m_Indicator->m_Next = m_NewTable[index];
            m_NewTable[index] = m_Indicator;
            m_Indicator = m_Next;
        }
    }

    delete[] m_Table;
    m_Table = m_NewTable;
    m_Capacity = newCapacity;
}

template <typename K, typename V>
V HashMap<K, V>::ElementSeatch(const K &key) {
    int index = Hasher(key)%m_Capacity;
    Node* Indicator = m_Table[index];

    while(Indicator != nullptr){
        if(Indicator->m_Key == key){
            return Indicator->m_Value;
        }
        Indicator = Indicator->m_Next;
    }
    ///что-то нужно возвращать, если такого ключа нет
}

template <typename K, typename V>
void HashMap<K, V>::Insert(const K &key, const V &value){ ///???Нужно ли m_Size преобразовать в double static_cast<double>
    if (m_Size / m_Capacity >= m_LoadFactor) {
        Resize();
    }

    int index = Hasher(key) % m_Capacity; ///гарантируем,что индекс остаеётся в пределах допустимых границ
    Node* m_Indicator = m_Table[index]; ///указатель на начало цепочки
    Node* prev = nullptr; ///указатель на предыдущий элемент в списке

    while (m_Indicator != nullptr) {
        if (m_Indicator->m_Key == key) {///элемент с таким ключом key уже существует в хеш-таблице. => обновляем значение этого элемента
            m_Indicator->m_Value = value;
            return;
        }
        ///Двигаем указатели
        prev = m_Indicator;
        m_Indicator = m_Indicator->m_Next;
    }

    /// вставляет новый элемент в хеш-таблицу
    Node* newNode = new Node(key, value);

    if (prev != nullptr) {
        prev->m_Next = newNode;
    } else {
        m_Table[index] = newNode;
    }
    ++m_Size;
}

template <typename K, typename V>
void HashMap<K, V>::remove(const K &key) { ///удаления элемента из хеш-таблицы
    int index = Hasher(key) % m_Capacity;
    Node* m_Indicator = m_Table[index];
    Node* prev = nullptr;

    while (m_Indicator != nullptr) {
        if (m_Indicator->m_Key == key) { ///Удаляем элемент
            if (prev != nullptr) {
                prev->m_Next = m_Indicator->m_Next;
            } else {///обновляет указатель на начало цепочки
                m_Table[index] = m_Indicator->m_Next;
            }
            delete m_Indicator;
            --m_Size;
            return;
        }
        prev = m_Indicator;
        m_Indicator = m_Indicator->m_Next;
    }
}

template <typename K, typename V>
void HashMap<K, V>::PrintMap() {
    Node* m_Indicator;
    for(int i = 0; i < m_Capacity; i++){
        cout << i << ": ";
        m_Indicator = m_Table[i];
        while(m_Indicator != nullptr){
            cout << m_Indicator->m_Value << " ";
            m_Indicator = m_Indicator->m_Next;
        }
        cout << endl;
    }

}

template<typename K, typename V>
typename HashMap<K, V>::Iterator HashMap<K, V>::Begin() const {
    int index = 0;
    while (index < m_Capacity && m_Table[index] == nullptr) {
        index++;
    }
    if (index < m_Capacity) {
        return Iterator(*this, index, m_Table[index]);
    } else {
        return End();
    }
} ///ищем первый не пустой бакет
template<typename K, typename V>

typename HashMap<K, V>::Iterator HashMap<K, V>::End() const {
    return Iterator(*this, m_Capacity, nullptr);
}

template <typename K, typename V>
int HashMap<K, V>::getTotalSize() const {
    return m_Size;
}

template <typename K, typename V>
int HashMap<K,V>::getUniqueSize() const {
    int count = 0;

    auto it = this->Begin();
    auto EndIt = this->End();
    V* ContainerValues = new V[m_Size];
    while (it.HasNext()) {
        V value =it.getValue();
        int flag = 1;
        for (int i = 0; i < count; ++i) {
            if(ContainerValues[i] == value){
                flag = 0;
            }
        }
        if(flag == 1){
            ContainerValues[count] = value;
            count++;
        }
        ++it;
    }
    delete[] ContainerValues;
    return count;
}

template<typename K, typename V>
HashMap<K, V>::Iterator::Iterator(const HashMap <K, V> &HashMap): m_Map(HashMap), m_Index(0), m_Indicator(nullptr) {
    MoveToTheNext();
}

template<typename K, typename V>
HashMap<K, V>::Iterator::Iterator(const HashMap <K, V> &HashMap, int Index, Node *Indicator) : m_Map(HashMap), m_Index(Index), m_Indicator(Indicator) {};

template<typename K, typename V>
bool HashMap<K, V>::Iterator::HasNext() const {
    return m_Indicator != nullptr;
}

template<typename K, typename V>
void HashMap<K, V>::Iterator::Next() {
    m_Indicator = m_Indicator->m_Next;
    if (!m_Indicator) {
        m_Index++;
        MoveToTheNext();
    }
}

template<typename K, typename V>
const K& HashMap<K, V>::Iterator::getKey() const {
    return m_Indicator->m_Key;
}

template<typename K, typename V>
const V& HashMap<K, V>::Iterator::getValue() const {
    return m_Indicator->m_Value;
}

template<typename K, typename V>
typename HashMap<K, V>::Iterator& HashMap<K, V>::Iterator::operator++() {
    Next();
    return *this;
}

template<typename K, typename V>
void HashMap<K,V>::Iterator::MoveToTheNext() {
    while (m_Index < m_Map.m_Capacity && m_Map.m_Table[m_Index] == nullptr) {
        m_Index++;
    }

    if (m_Index < m_Map.m_Capacity) {
        m_Indicator = m_Map.m_Table[m_Index];
    } else {
        m_Indicator = nullptr;
    }
} ///ищем следующий непустой бакет

template<typename K, typename V>
typename HashMap<K,V>::Iterator HashMap<K,V>::GetIterator() const {
    return Iterator(*this);
}

int main() {
    system("chcp 65001"); // Русификация ввода/вывода
    ifstream F("input.txt"); //открываем файл "input.txt"
    ofstream Out("output.txt"); //открываем файл "output.txt"

    if(!F && !Out){
        cout << "Не удалось открыть файл"<< endl;
        return 1;
    }

    char keyType, valueType, comand;
    F >> keyType >> valueType;

    switch (keyType) {
        case ('I'): {
            int N;
            F >> N;
            int key;
            switch (valueType) {
                case 'I':{
                    int value;
                    HashMap<int, int> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;}
                case 'D':{
                    double value;
                    HashMap<int, double> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
                case 'S': {
                    string value;
                    HashMap<int, string> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
            }
            break;
        }
        case ('D'): {
            int N;
            F >> N;
            double key;
            switch (valueType) {
                case 'I':{
                    int value;
                    HashMap<double, int> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;}
                case 'D':{
                    double value;
                    HashMap<double, double> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;}
                case 'S': {
                    string value;
                    HashMap<double, string> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
            }
            break;
        }
        case ('S'): {
            int N;
            F >> N;
            string key;
            switch (valueType) {
                case 'I':{
                    int value;
                    HashMap<string, int> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;}
                case 'D':{
                    double value;
                    HashMap<string , double> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
                case 'S': {
                    string value;
                    HashMap<string, string> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
            }
            break;
        }
        case ('M'):{
            int n ;
            int N;
            F >> n >> N;
            Matrix  key(n,n);
            switch (valueType) {
                case 'I':{
                    int value;
                    HashMap<Matrix, int> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;}
                case 'D':{
                    double value;
                    HashMap<Matrix, double> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
                case 'S': {
                    string value;
                    HashMap<Matrix, string> m_HashMap;
                    for (int i = 0; i < N; ++i) {
                        F >> comand;
                        if (comand == 'A') {
                            F >> key >> value;
                            m_HashMap.Insert(key, value);
                        } else {
                            F >> key;
                            m_HashMap.remove(key);
                        }
                    }
                    cout << m_HashMap.getTotalSize() << " " << m_HashMap.getUniqueSize();
                    break;
                }
            }
            break;
        }

    }


    return 0;
}
