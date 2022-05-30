#include "dynamic_hash_table.h"

int main(){
    struct Dummy{
        char data;
        int id;

        Dummy(char data, int id) : data(data), id(id){};
        Dummy() = default;
        ~Dummy() = default;
        int getKey(){ return id; };
    };

    DynamicHashTable<Dummy> my_table(4);

    Dummy dumb1('a', 1);
    Dummy dumb2('b', 2);
    Dummy dumb3('c', 3);

    my_table.insertElement(dumb1, dumb1.getKey());
    my_table.insertElement(dumb2, dumb2.getKey());
    my_table.insertElement(dumb3, dumb3.getKey());

    return 0;
}
