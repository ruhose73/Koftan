//Класс создания и работы с readme файлом(формат Markdown Documentation)
// Created by Mihail on 29.03.2020.

#ifndef TIME_TO_CODING_README_FILE_H
#define TIME_TO_CODING_README_FILE_H
#include <fstream>
#include <iostream>

using namespace std;

class readme_file_class {
public:
    readme_file_class() {
        cout << "Работа с файлом readme.md" << endl;
        ofstream readme;    // создаем объект класса ifstream
        readme.open("readme.md");  // Создаем и открываем файл
        //если открыт файл,то:
        if (readme.is_open()) {
            cout << "Файл создан и открыт" << endl;
            readme << "<h3>Автоматически созданный файл README </h3>" << endl;
            readme << "<hr>" << endl;
            readme.close(); //закрываем существующий файл
        }
            //если файла нет, то создаем и открываем
        else {
            cout << "\nОШИБКА!!! : " << endl;
            cout << "\nНет файла readme.md : " << endl;
            readme.open("readme.md");
            readme << "<h3>Автоматически созданный файл README </h3>" << endl;
            readme << "<hr>" << endl;
            readme.close(); //закрываем существующий файлц
        }

    }

    void writefile() {
        ofstream readme;    // создаем объект класса ifstream
        readme.open("readme.md", ios_base::app);
        cout << "Запись в файл readme.md" << endl;
        readme << "<h5>Последняя версия: 1.0.</h5>" << endl;
        readme << "Последнюю версию можно получить по почте ruhose73@gmail.com" << endl;
        readme << "<hr>" << endl;
        readme << "<h5>Документация</h5>" << endl;
        readme << "Документацию можно получить по почте: ruhose73@gmail.com" << endl;
        readme << "<hr>" << endl;
        readme << "<h5>Установка</h5>" << endl;
        readme << "Установщик программы (INSTALL) находиться в корневом каталоге поставляемой сборки" << endl;
        readme << "<hr>" << endl;
        readme << "<h5>Лицензирование</h5>" << endl;
        readme << "Продукт является opensource проектом" << endl;
        readme << "<hr>" << endl;
        readme << "<h5>Контакты</h5>" << endl;
        readme << "ruhose73@gmail.com" << endl << endl;
        readme << "89629015654" << endl;
        readme << "<hr>" << endl;
        readme.close(); //закрываем файл
    }
};

#endif //TIME_TO_CODING_README_FILE_H
