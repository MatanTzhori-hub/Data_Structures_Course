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
        void initializeVal(){ data = 0; id = 0;};
    };

    DynamicHashTable<Dummy> my_table(4);

    Dummy dumb1('a', 1);
    Dummy dumb2('b', 13);
    Dummy dumb3('c', 8);
    Dummy dumb4('d', 999);
    Dummy dumb5('e', 5);
    Dummy dumb6('f', 23);
    Dummy dumb7('g', 2);
    Dummy dumb8('h', 43);
    Dummy dumb9('i', 88);
    Dummy dumb10('j', 1000);
    Dummy dumb11('k', 20);
    Dummy dumb12('l', 34);
    Dummy dumb13('m', 9);
    Dummy dumb14('n', 677);
    Dummy dumb15('o', 420);
    Dummy dumb16('p', 69);
    Dummy dumb17('q', 666);
    Dummy dumb18('r', 404);

    Dummy out;
    ReturnValue res;

    res = my_table.insertElement(dumb1, dumb1.getKey());
    res = my_table.insertElement(dumb2, dumb2.getKey());
    res = my_table.insertElement(dumb3, dumb3.getKey());
    res = my_table.insertElement(dumb4, dumb4.getKey());
    res = my_table.insertElement(dumb5, dumb5.getKey());
    res = my_table.insertElement(dumb6, dumb6.getKey());
    res = my_table.insertElement(dumb7, dumb7.getKey());
    res = my_table.insertElement(dumb8, dumb8.getKey());
    res = my_table.insertElement(dumb9, dumb9.getKey());
    res = my_table.insertElement(dumb10, dumb10.getKey());
    res = my_table.insertElement(dumb11, dumb11.getKey());
    res = my_table.insertElement(dumb12, dumb12.getKey());
    res = my_table.insertElement(dumb13, dumb13.getKey());
    res = my_table.insertElement(dumb14, dumb14.getKey());
    res = my_table.insertElement(dumb15, dumb15.getKey());
    res = my_table.insertElement(dumb16, dumb16.getKey());
    res = my_table.insertElement(dumb17, dumb17.getKey());
    res = my_table.insertElement(dumb18, dumb18.getKey());

    res = my_table.findElement(1, &out);
    res = my_table.findElement(13, &out);
    res = my_table.findElement(69, &out);
    res = my_table.findElement(404, &out);
    res = my_table.findElement(444, &out);
    res = my_table.findElement(108, &out);

    res = my_table.removeElement(13);
    res = my_table.removeElement(999);
    
    res = my_table.removeElement(dumb11.getKey());
    res = my_table.removeElement(dumb12.getKey());
    res = my_table.removeElement(dumb13.getKey());
    res = my_table.removeElement(dumb14.getKey());
    res = my_table.removeElement(dumb15.getKey());
    res = my_table.removeElement(dumb16.getKey());
    res = my_table.removeElement(dumb17.getKey());
    res = my_table.removeElement(dumb18.getKey());

    res = my_table.insertElement(dumb11, dumb11.getKey());
    res = my_table.removeElement(dumb11.getKey());
    res = my_table.insertElement(dumb12, dumb12.getKey());
    res = my_table.removeElement(dumb12.getKey());
    res = my_table.insertElement(dumb13, dumb13.getKey());
    res = my_table.removeElement(dumb13.getKey());
    res = my_table.insertElement(dumb14, dumb14.getKey());
    res = my_table.removeElement(dumb14.getKey());
    res = my_table.insertElement(dumb15, dumb15.getKey());
    res = my_table.removeElement(dumb15.getKey());
    res = my_table.insertElement(dumb16, dumb16.getKey());
    res = my_table.removeElement(dumb16.getKey());
    res = my_table.insertElement(dumb17, dumb17.getKey());
    res = my_table.removeElement(dumb17.getKey());
    res = my_table.insertElement(dumb18, dumb18.getKey());
    res = my_table.removeElement(dumb18.getKey());
    

    res = my_table.findElement(1, &out);
    res = my_table.findElement(13, &out);
    res = my_table.findElement(69, &out);
    res = my_table.findElement(404, &out);
    res = my_table.findElement(444, &out);
    res = my_table.findElement(108, &out);

    res = my_table.findElement(1, &out);
    res = my_table.findElement(13, &out);

    return 0;
}
