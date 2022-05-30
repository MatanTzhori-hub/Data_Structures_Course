#include "dynamic_hash_table.h"

int main(){
    struct Dummy{
        char data;
        int id;

        Dummy(char data, int id) : data(data), id(id){};
        Dummy() = default;
        ~Dummy() = default;
        Dummy& operator=(Dummy& dumm) = default;
        int getKey(){ return id; };
    };

    DynamicHashTable<Dummy> my_table(4);

    Dummy dumb1('a', 1);
    Dummy dumb2('b', 13);
    Dummy dumb3('c', 8);
    Dummy dumb4('d', 999);
    Dummy dumb5('e', 5);
    Dummy dumb6('f', 23);

    my_table.insertElement(dumb1, dumb1.getKey());
    my_table.insertElement(dumb2, dumb2.getKey());
    my_table.insertElement(dumb3, dumb3.getKey());
    my_table.insertElement(dumb4, dumb4.getKey());
    my_table.insertElement(dumb5, dumb5.getKey());
    my_table.insertElement(dumb6, dumb6.getKey());

    my_table.deleteElement(13);
    my_table.deleteElement(999);

    Dummy out;
    ReturnValue res = my_table.findElement(1, &out);
    res = my_table.findElement(13, &out);

    return 0;
}
