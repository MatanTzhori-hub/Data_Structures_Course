#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#define ARRAY_START_SIZE    4
#define UP_SCALE            2
#define DOWN_SCALE          0.5
#define NO_SCALE            1
#define MINIMUM_PROPORTION  0.25

#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include "rank_tree.h" // only here because of enum ReturnValue, need to figure out where to place it


typedef enum {FREED = -1, EMPTY, TAKEN} IndexStatus;

template <class data_t>
class DynamicHashTable {
private:
    int max_size;
    int curr_size;
    data_t* array_ptr;
    int* graveyard;

    void rescale(double scale);
    int hashBase(data_t* data);
    int hashStep(data_t* data);
    int hash(data_t* data, int k);
    bool isEmptyForInsert(int index);
    void initializeArrays();

public:
    explicit DynamicHashTable(int size = ARRAY_START_SIZE);
    ~DynamicHashTable();

    void findElement(int key);
};




template<class data_t>
DynamicHashTable<data_t>::DynamicHashTable(int size) :  max_size(size), curr_size(0) {
    array_ptr = new data_t[size];
    if(!array_ptr){
        throw std::bad_alloc();
    }
    graveyard = new int[size];
    if(!graveyard){
        throw std::bad_alloc();
    }
    initializeArrays();
}

template<class data_t>
DynamicHashTable<data_t>::~DynamicHashTable() {
    delete[] array_ptr;
    delete[] graveyard;
}


template<class data_t>
void DynamicHashTable<data_t>::rescale(double scale) {
    int new_size = max_size*scale;
    DynamicHashTable* new_hash = new DynamicHashTable<data_t>(new_size);
    if(!new_hash){
        throw std::bad_alloc();
    }
    for(int i = 0; i < max_size; i++) {
        if(graveyard[i] != TAKEN){
            continue;
        }
        else{
            new_hash->insertData(*(array_start_ptr + i));
        }
    }
    updateFields(new_hash);
    delete new_hash;
}

template<class data_t>
int DynamicHashTable<data_t>::hashBase(data_t* data) {
    return data->getKey() % max_size;
}

template<class data_t>
int DynamicHashTable<data_t>::hashStep(data_t* data) {
    return 1;
}

template<class data_t>
int DynamicHashTable<data_t>::hash(data_t* data, int k) {
    int num = k * hashStep(data);
    num += hashBase(data);
    return (num % max_size);
}

template<class data_t>
bool DynamicHashTable<data_t>::isEmptyForInsert(int index) {
    if(graveyard[index] != TAKEN){
        return true;
    }
    return false;
}

template<class data_t>
void DynamicHashTable<data_t>::initializeArrays() {
    for (int i = 0; i < max_size; ++i) {
        *(array_start_ptr + i) = nullptr;
        graveyard[i] = EMPTY;
    }
    curr_size = 0;
}

template<class data_t>
void DynamicHashTable<data_t>::findElement(int key) {
}

#endif //DYNAMIC_ARRAY_H
