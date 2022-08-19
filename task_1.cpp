#include <iostream>
#include <fstream>
#include <string>


class Address {

public:
    std::string get_full_address()
    {
        return city + ", " + street + ", " + house_num + ", " + std::to_string(apart_num);
    }
 
    Address(std::string new_city = "Не указан", std::string new_street = "Не указана", std::string new_house_num = "Не указан", int new_apart_num = 0) :
        city{new_city}, street{new_street}, house_num{new_house_num}, apart_num{new_apart_num} {}   //Чтобы можно было создать массив объектов нужен конструктор по-умолчанию
    

private:

    std::string city;
    std::string street;
    std::string house_num;
    int apart_num{0};

};

bool read_address(std::fstream& fin, std::string& city, std::string& street, std::string& house_num, int& apart_num)
{
    while (!fin.eof())
    {
        if ((fin >> city) && (fin >> street) && (fin >> house_num) && (fin >> apart_num)) return true;
        else return false;
    }
    return false;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    const std::string in_file_name = "in.txt";
    std::fstream fin(in_file_name, std::fstream::in);
    if (fin.is_open())
    {
        const int min_addr_count = 1;       //Минимальное количество адресов в файле
        int addr_count = 0;
        if (fin >> addr_count)
        {
            if (addr_count < min_addr_count)
            {
                std::cout << "В файле указано количество адресов " << addr_count << " что меньше минимального " << min_addr_count << std::endl;
                return 1;
            }
            std::string city, street, house_num;
            int apart_num = 0;
            Address** addr_list = new Address * [addr_count];
            for (int i = 0; i < addr_count; ++i)                //Читаем адреса из файла
            {
                if (read_address(fin, city, street, house_num, apart_num))
                {
                    addr_list[i] = new Address(city, street, house_num, apart_num);
                }
                else
                {
                    std::cout << "Ошибка чтения " << i << " ого/его адреса" << std::endl;
                    delete[] addr_list;
                    addr_list = nullptr;
                    fin.close();
                    return 2;
                }
            } 
            fin.close();
            //Выводми адреса в выходной файл
            const std::string out_file_name = "out.txt";
            std::fstream fout(out_file_name, std::fstream::out);
            if (fout.is_open())
            {
                if (fout << addr_count << std::endl)
                {
                    for (int i = addr_count - 1; i >=0 ; --i)
                    {
                        if (!(fout << addr_list[i]->get_full_address()  << std::endl))
                        {
                            std::cout << "Ошибка записи " << i << "(ого/его) адреса в файл " << out_file_name << std::endl;
                            for (int j = 0; j < addr_count; ++j) { delete addr_list[j]; }
                            delete[] addr_list;
                            addr_list = nullptr;
                            fout.close();
                            return 3;
                        }
                    }
                    fout.close();
                }
                else
                {
                    std::cout << "Ошибка записи количества адресов в файл " << out_file_name << std::endl;
                    for (int j = 0; j < addr_count; ++j) { delete addr_list[j]; }
                    delete[] addr_list;
                    addr_list = nullptr;
                    fout.close();
                    return 4;
                }
            }
            else
            {
                std::cout << "Ошибка создания выходного файла " << out_file_name << std::endl;
                for (int j = 0; j < addr_count; ++j) { delete addr_list[j]; }
                delete[] addr_list;
                addr_list = nullptr;
                return 5;
            }
            for (int j = 0; j < addr_count; ++j) { delete addr_list[j]; }
            delete[] addr_list;
            addr_list = nullptr;
        }
        else
        {
            std::cout << "Ошибка чтения количества адресов из файла " << in_file_name << std::endl;
            return 6;
        }    
    }
    else
    {
        std::cout << "Ошибка открытия файла " << in_file_name << std::endl;
        return 7;
    }
}

