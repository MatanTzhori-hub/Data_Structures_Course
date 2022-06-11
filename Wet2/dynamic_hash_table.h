#ifndef DYNAMIC_HASH_TABLE_H
#define DYNAMIC_HASH_TABLE_H

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

template <typename data_t>
class DynamicHashTable{
    private:
        int max_size;
        int curr_size;
        data_t* hash_array;
        int* graveyard;

        void initializeArrays();
        void rescale(double scale);
        int hashBase(int key);
        int hashStep(int key);
        int hash(int key, int iter);

    public:
        explicit DynamicHashTable(int size = ARRAY_START_SIZE);
        ~DynamicHashTable();
        void killAllEmployees();

        ReturnValue insertElement(data_t data, int key);
        ReturnValue removeElement(int key);
        data_t* findElement(int key, ReturnValue *res);
        int findIndex(int key);
        void mergeToMe(DynamicHashTable<data_t>* other_hash_table);
};



template<class data_t>
DynamicHashTable<data_t>::DynamicHashTable(int size) :  max_size(size), curr_size(0) {
    hash_array = new data_t[size];
    if(!hash_array){
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
        graveyard[i] = EMPTY;
        (hash_array[i]).initializeVal();
    }
    curr_size = 0;
}

template<class data_t>
DynamicHashTable<data_t>::~DynamicHashTable(){
    delete[] hash_array;
    delete[] graveyard;
}

template<class data_t>
void DynamicHashTable<data_t>::killAllEmployees(){
    int count = 0;
    for (int i=0; i<max_size; i++){
        if(graveyard[i]==TAKEN){
            delete hash_array[i].getEmployeePtr();
            count++;
        }
    }
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

// big bug in findElement.. not returning the cell we really want but a copy of it
template<class data_t>
data_t* DynamicHashTable<data_t>::findElement(int key, ReturnValue *res){
    int index = -1;
    int iter = 0;
    int base_index = hash(key, iter);
    if(graveyard[base_index] == TAKEN && hash_array[base_index].getKey() == key){
        *res = ELEMENT_EXISTS;
        return &hash_array[base_index];
    }
    iter++;

    while(index != base_index){
        index = hash(key, iter);
        if(graveyard[index] == EMPTY){
            *res = ELEMENT_DOES_NOT_EXIST;
            return nullptr;
        }
        else if(graveyard[index] == FREED){
            iter++;
            continue;
        }
        else{
            if(hash_array[index].getKey() == key){
                *res = ELEMENT_EXISTS;
                return &hash_array[index];
            }
            iter++;
        }
    }
    *res = ELEMENT_DOES_NOT_EXIST;
    return nullptr;
}

template<class data_t>
int DynamicHashTable<data_t>::findIndex(int key){
    int index = -1;
    int iter = 0;
    int base_index = hash(key, iter);
    if(graveyard[base_index] == TAKEN && hash_array[base_index].getKey() == key){
        return base_index;
    }
    iter++;

    while(index != base_index){
        index = hash(key, iter);
        if(graveyard[index] == EMPTY){
            return -1;
        }
        else if(graveyard[index] == FREED){
            iter++;
            continue;
        }
        else{
            if(hash_array[index].getKey() == key){
                return index;
            }
            iter++;
        }
    }
    return -1;
}

template<class data_t>
ReturnValue DynamicHashTable<data_t>::insertElement(data_t data, int key){
    if (curr_size == (max_size/2)) {
        rescale(UP_SCALE);
    }

    int index = 0;
    int iter = 0;

    ReturnValue ret = MY_FAILURE;
    data_t* temp_data_ptr = findElement(key, &ret);

    if (ret == ELEMENT_EXISTS){
        return MY_FAILURE;
    }

    while (true) {
        index = hash(key, iter);
        if (graveyard[index] != TAKEN) {
            break;
        }
        iter++;
    }

    hash_array[index] = data;
    graveyard[index] = TAKEN;
    curr_size++;
    return MY_SUCCESS;
}

template<class data_t>
ReturnValue DynamicHashTable<data_t>::removeElement(int key){
    int index = findIndex(key);

    if (index == -1){
        return MY_FAILURE;
    }

    graveyard[index] = FREED;
    curr_size--;

    if((curr_size <= ceil(MINIMUM_PROPORTION*max_size)) && (DOWN_SCALE*max_size >= ARRAY_START_SIZE)){
        rescale(DOWN_SCALE);
    }

    return MY_SUCCESS;
}


template<class data_t>
void DynamicHashTable<data_t>::rescale(double scale){
    data_t* old_hash_array = hash_array;
    int* old_graveyard = graveyard;
    int old_max_size = max_size;

    max_size = max_size*scale;
    hash_array = new data_t[max_size];

    if(!hash_array){
        hash_array = old_hash_array;
        graveyard = old_graveyard;
        max_size = old_max_size;
        throw std::bad_alloc();
    }

    graveyard = new int[max_size];
    if(!graveyard){
        hash_array = old_hash_array;
        graveyard = old_graveyard;
        max_size = old_max_size;
        delete[] hash_array;
        throw std::bad_alloc();
    }

    initializeArrays();

    curr_size = 0;
    for(int i = 0; i < old_max_size; i++) {
        if(old_graveyard[i] != TAKEN){
            continue;
        }
        else{
            insertElement(old_hash_array[i], old_hash_array[i].getKey());
        }
    }

    delete[] old_hash_array;
    delete[] old_graveyard;
}

template<class data_t>
void DynamicHashTable<data_t>::mergeToMe(DynamicHashTable<data_t>* other_hash_table) {
    if (other_hash_table == nullptr){
        return;
    }

    for (int i = 0; i < other_hash_table->max_size; i++){
        if (other_hash_table->graveyard[i] == TAKEN){
            this->insertElement(other_hash_table->hash_array[i], other_hash_table->hash_array[i].getKey());
        }
    }

    delete[] other_hash_table->hash_array;
    delete[] other_hash_table->graveyard;
    other_hash_table->hash_array = new data_t[ARRAY_START_SIZE];
    other_hash_table->graveyard = new int[ARRAY_START_SIZE];
    other_hash_table->max_size = ARRAY_START_SIZE;
    other_hash_table->curr_size = 0;
    other_hash_table->initializeArrays();
}


#endif //DYNAMIC_HASH_TABLE_H
