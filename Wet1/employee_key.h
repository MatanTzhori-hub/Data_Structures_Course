#ifndef EMPLOYEE_KEY_H
#define EMPLOYEE_KEY_H

struct Employee_Key{

    int employee_id;
    int employee_salary;

    public:
    Employee_Key(int employee_id, int employee_salary);
    ~Employee_Key();
    bool operator>(Employee_Key& other);
    bool operator<(Employee_Key& other);
};

#endif // EMPLOYEE_KEY_H