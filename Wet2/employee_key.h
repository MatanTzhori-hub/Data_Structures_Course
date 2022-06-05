#ifndef EMPLOYEE_KEY_H
#define EMPLOYEE_KEY_H

#define MAX_SALARY 2147483647

class EmployeeKey{

    int employee_id;
    int employee_salary;

    public:
    EmployeeKey() = default;
    EmployeeKey(int employee_id, int employee_salary);
    ~EmployeeKey() = default;

    int getId() const { return employee_id; }
    int getSalary() const { return employee_salary; }
    void setId(int new_id){ employee_id = new_id; }
    void setSalary(int new_salary){ employee_salary = new_salary; }
    
    bool operator>(const EmployeeKey& other) const;
    bool operator<(const EmployeeKey& other) const;
    bool operator>=(const EmployeeKey& other) const;
    bool operator<=(const EmployeeKey& other) const;
};

bool operator==(const EmployeeKey& first, const EmployeeKey& second);
bool operator==(const EmployeeKey& employee_key, int id_to_compare);
bool operator==(int id_to_compare, const EmployeeKey& employee_key);
bool operator!=(const EmployeeKey& first, const EmployeeKey& second);

#endif // EMPLOYEE_KEY_H