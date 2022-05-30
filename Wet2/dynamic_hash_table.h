#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#define ARRAY_START_SIZE    4
#define UP_SCALE            2
#define DOWN_SCALE          0.5
#define NO_SCALE            1
#define MINIMUM_PROPORTION  0.25
#define PRIME_N             7

#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include "rank_tree.h" // only here because of enum ReturnValue, need to figure out where to place it


typedef enum {FREED = -1, EMPTY, TAKEN} IndexStatus;

template <typename data_t>
class DynamicHashTable {

private:
    class HT_Item{
        data_t data;
        int key;

        HT_Item(data_t data, int key) : data(data), key(key){};
        HT_Item() = default;

        friend class DynamicHashTable;
    };

    int max_size;
    int curr_size;
    HT_Item** array_ptr;
    int* graveyard;

    void initializeArrays();
    void rescale(double scale);
    int hashBase(int key);
    int hashStep(int key);
    int hash(int key, int iter);

public:
    explicit DynamicHashTable(int size = ARRAY_START_SIZE);
    ~DynamicHashTable();

    void insertElement(data_t data, int key);
    void deleteElement(int key);
    data_t* findElement(int key);
};



template<class data_t>
DynamicHashTable<data_t>::DynamicHashTable(int size) :  max_size(size), curr_size(0) {
    array_ptr = new HT_Item *[size];
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
void DynamicHashTable<data_t>::initializeArrays() {
    for (int i = 0; i < max_size; ++i) {
        array_ptr[i] = nullptr;
        graveyard[i] = EMPTY;
    }
    curr_size = 0;
}

template<class data_t>
DynamicHashTable<data_t>::~DynamicHashTable() {
    for (int i = max_size; i > 0; --i) {
        if(array_ptr[i] != nullptr){
            delete array_ptr[i];
        }
    }
    delete[] array_ptr;
    delete[] graveyard;
}

template<class data_t>
int DynamicHashTable<data_t>::hashBase(int key){
    return key % max_size;
}

template<class data_t>
int DynamicHashTable<data_t>::hashStep(int key){
    return 1;
}

template<class data_t>
int DynamicHashTable<data_t>::hash(int key, int iter){
    return (hashBase(key) + iter * hashStep(key)) % max_size;
}

template<class data_t>
data_t* DynamicHashTable<data_t>::findElement(int key){
    int index;
    int iter = 0;
    while(true){
        index = hash(key, iter);
        if(graveyard[index] == EMPTY){
            return nullptr;
        }
        else if(graveyard[index] == FREED){
            iter++;
            continue;
        }
        else{
            if(array_ptr[index]->key == key){
                return &(array_ptr[index]->data);
            }
            iter++;
        }
    }
    return nullptr;
}

template<class data_t>
void DynamicHashTable<data_t>::insertElement(data_t data, int key){
    if (curr_size == (max_size/2)) {
        rescale(UP_SCALE);
    }

    HT_Item* new_item = new HT_Item(data, key);
    int index;
    int iter = 0;

    data_t* exist = findElement(key);
    if (exist != nullptr) {
        return;
    }

    while (true) {
        index = hash(key, iter);
        if (graveyard[index] != TAKEN) {
            break;
        }
        iter++;
    }
    array_ptr[index] = new_item;
    graveyard[index] = TAKEN;
    curr_size++;
}

template<class data_t>
void DynamicHashTable<data_t>::deleteElement(int key){
    if (findElement(key) == nullptr) {
        return;
    }

    int index;
    int iter = 0;
    while(true){
        index = hash(key, iter);
        if(graveyard[index] != TAKEN){
            iter++;
            continue;
        }
        else{
            if(array_ptr[index]->key == key){
                break;
            }
            iter++;
        }
    }
    delete array_ptr[index];
    curr_size--;
    graveyard[index] = FREED;
    array_ptr[index] = nullptr;


    if((curr_size <= ceil(MINIMUM_PROPORTION*max_size)) && (DOWN_SCALE*max_size >= ARRAY_START_SIZE)){
        rescale(DOWN_SCALE);
    }
    
    // else if((curr_size <= ceil(MINIMUM_PROPORTION*max_size)) && (DOWN_SCALE*max_size < ARRAY_START_SIZE)){
    //     rescale(NO_SCALE);
    // }
}


template<class data_t>
void DynamicHashTable<data_t>::rescale(double scale){
    HT_Item** old_array_ptr = array_ptr;
    int* old_graveyard = graveyard;
    int old_max_size = max_size;

    max_size = max_size*scale;
    curr_size = 0;
    array_ptr = new HT_Item*[max_size];
    if(!array_ptr){
        throw std::bad_alloc();
    }
    graveyard = new int[max_size];
    if(!graveyard){
        throw std::bad_alloc();
    }

    for(int i = 0; i < old_max_size; i++) {
        if(graveyard[i] != TAKEN){
            continue;
        }
        else{
            insertElement(old_array_ptr[i]->data, old_array_ptr[i]->key);
            delete old_array_ptr[i];
        }
    }

    delete old_array_ptr;
    delete old_graveyard;
}

#endif //DYNAMIC_ARRAY_H
