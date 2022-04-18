#ifndef EMPLOYEE_KEY_H
#define EMPLOYEE_KEY_H

class Employee_Key{

    int employee_id;
    int employee_salary;

    public:
    Employee_Key(int employee_id, int employee_salary);
    ~Employee_Key();

    int getId() const { return employee_id; }
    int getSalary() const { return employee_salary; }
    void setId(int new_id){ employee_id = new_id; }
    void setSalary(int new_salary){ employee_salary = new_salary; }
    
    bool operator>(const Employee_Key& other);
    bool operator<(const Employee_Key& other);
};

bool operator==(const Employee_Key& first, const Employee_Key& second);
bool operator==(const Employee_Key& employee_key, int id_to_compare);
bool operator==(int id_to_compare, const Employee_Key& employee_key);
bool operator!=(const Employee_Key& first, const Employee_Key& second);

#endif // EMPLOYEE_KEY_H