//
// Created by Антон on 18.11.2023.
//

#include "HashMap.h"
#define M 31


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
