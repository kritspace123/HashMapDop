//
// Created by Антон on 18.11.2023.
//

#ifndef DOPHASHMAP_HASHMAP_H
#define DOPHASHMAP_HASHMAP_H

#include <iostream>
#include <fstream>
#include <string>
#include <functional>

using namespace std;


template <typename K, typename V>
class HashMap {
private:
    struct Node {
        K m_Key;
        V m_Value;
        Node *m_Next;

        Node(const K &k, const V &v) : m_Key(k), m_Value(v), m_Next(nullptr) {}
    };

    Node **m_Table;
    int m_Size; /// количество элементов
    int m_Capacity; ///Размер массива хэштаблицы
    double m_LoadFactor = 0.75;
    hash <K> Hasher;

public:
    HashMap();


    void clear();

    ///Правило 3-х
    ~HashMap();

    HashMap(const HashMap &other);

    HashMap &operator=(const HashMap &other);

    void Resize();///Функция для увеличения размера хеш-таблицы
    V ElementSeatch(const K &key);///Поиск элемента в коллекции по заданному ключу.
    void Insert(const K &key, const V &value); ///Добавление нового элемента с заданными ключом и значением в коллекцию.
    void remove(const K &key);/// Удаление из коллекции элемента с заданным ключом.
    void PrintMap();

    // Iterator class
    class Iterator {
    private:
        const HashMap &m_Map;
        int m_Index;
        Node *m_Indicator;

    public:
        Iterator(const HashMap &HashMap);

        Iterator(const HashMap &HashMap, int Index, Node *Indicator);

        bool HasNext() const;

        void Next();///ищем следующий непустой бакет
        const K &getKey() const;

        const V &getValue() const;

        Iterator &operator++();

        void MoveToTheNext();
    };

    Iterator GetIterator() const;

    Iterator Begin() const;
    Iterator End() const; ///последний бакет

    int getTotalSize() const; ///вывод количества элементов в мапе

    int getUniqueSize() const; ///вывод количества уникальных элементов
};

#endif //DOPHASHMAP_HASHMAP_H
