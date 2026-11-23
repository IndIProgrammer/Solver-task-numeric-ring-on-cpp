#include "list.h"
#include "exceptions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <windows.h>

int getValidatedModeInput(const std::string& prompt, int min, int max) {
    std::string rawInput;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, rawInput) || rawInput.empty()) {
            std::cout << "Ошибка: ввод не должен быть пустым!\n";
            continue;
        }

        bool isNegative = false;
        size_t startIndex = 0;
        if (rawInput[0] == '-') {
            isNegative = true;
            startIndex = 1;
            if (rawInput.length() == 1) {
                std::cout << "Ошибка: введите целое число!\n";
                continue;
            }
        }

        bool isAllDigits = true;
        for (size_t i = startIndex; i < rawInput.length(); ++i) {
            if (rawInput[i] < '0' || rawInput[i] > '9') {
                isAllDigits = false;
                break;
            }
        }

        if (!isAllDigits) {
            std::cout << "Ошибка: введите целое число!\n";
            continue;
        }

        // Защита от переполнения: если строка огромная, она точно вне диапазона min/max
        if (rawInput.length() - startIndex > 6) {
            std::cout << "Ошибка: введите число от " << min << " до " << max << "!\n";
            continue;
        }

        int value = std::stoi(rawInput);
        if (value < min || value > max) {
            std::cout << "Ошибка: введите число от " << min << " до " << max << "!\n";
        } else {
            return value;
        }
    }
}

std::string getInputFromKeyboard() {
    std::string input;
    while (true) {
        std::cout << "Введите строку цифр (или '0' для возврата): ";
        if (!std::getline(std::cin, input) || input.empty()) {
            std::cout << "Ошибка: строка не должна быть пустой!\n";
            continue;
        }

        if (input == "0") {
            throw ReturnToModeMenuException();
        }

        bool valid = true;
        for (char c : input) {
            if (c < '0' || c > '9') {
                valid = false;
                break;
            }
        }
        if (!valid) {
            std::cout << "Ошибка: строка должна содержать только цифры!\n";
        } else {
            return input;
        }
    }
}

std::string getInputFromFile() {
    const std::string filename = "input.txt";

    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл '" << filename << "'!\n";
        std::cout << "Убедитесь, что файл создан в папке с проектом/запускаемым файлом.\n";
        throw ReturnToModeMenuException();
    }

    std::string input;
    infile >> input;
    infile.close();

    bool valid = true;
    for (char c : input) {
        if (c < '0' || c > '9') {
            valid = false;
            break;
        }
    }
    if (!valid || input.empty()) {
        std::cout << "Ошибка в файле '" << filename << "': строка должна содержать только цифры и не быть пустой!\n";
        throw ReturnToModeMenuException();
    }

    std::cout << "Данные успешно прочитаны из файла '" << filename << "'\n";
    return input;
}

std::string generateRandomInput() {
    int length = getValidatedModeInput("Введите длину генерируемой строки (от 3 до 1000) или 0 для отмены: ", 0, 1000);
    if (length == 0) {
        throw ReturnToModeMenuException();
    }
    if (length < 3) {
        std::cout << "Ошибка: минимальная длина строки для решения задачи — 3 символа!\n";
        throw ReturnToModeMenuException();
    }

    std::string input = "";
    input += static_cast<char>('1' + std::rand() % 9);
    for (int i = 1; i < length; ++i) {
        input += static_cast<char>('0' + std::rand() % 10);
    }

    if (length <= 60) {
        std::cout << "Сгенерированная строка: " << input << std::endl;
    } else {
        std::cout << "Сгенерирована строка длины " << length << " (Начало: " << input.substr(0, 30) << "...)\n";
    }
    return input;
}

void showMainMenu() {
    std::cout << "\nГЛАВНОЕ МЕНЮ\n";
    std::cout << "1. Описание задачи 'Числовое кольцо'\n";
    std::cout << "2. Решить задачу 'Числовое кольцо'\n";
    std::cout << "0. Выход\n";
}

void showDescriptionMenu() {
    std::cout << "\nОписание задачи \"Числовое кольцо\" \n";
    std::cout << " В кольце записаны N чисел, составляющих по часовой "
                 "стрелке 3 числа: два слагаемых и их сумму.\n"
                 "Программа по строке чисел, считая её кольцом, находит какое-нибудь решение в виде A+B=C. \n"
                 "Если решение не найдено то выведет \"No\". Все цифры числа должны входить в числа только один раз и в "
                 "порядке следования в кольце. Цифр в кольце может быть не более 1000.\n";
}

void saveOutputToFile(const std::string& result) {
    std::cout << "Хотите сохранить результат в файл?\n";
    std::cout << "1. Да, сохранить (в файл output.txt)\n";
    std::cout << "0. Нет, вернуться в меню выбора\n";

    int saveChoice = getValidatedModeInput("Ваш выбор: ", 0, 1);

    if (saveChoice == 1) {
        const std::string outFilename = "output.txt";
        std::ofstream outfile(outFilename);
        if (outfile.is_open()) {
            outfile << result << std::endl;
            outfile.close();
            std::cout << "Результат успешно сохранен в '" << outFilename << "'\n";
        } else {
            std::cout << "Ошибка: не удалось открыть файл для записи!\n";
        }
    }
}

void processRingTask() {
    while (true) {
        std::cout << "\nСпособы ввода данных:\n";
        std::cout << "1. Ввод с клавиатуры\n";
        std::cout << "2. Ввод из файла (input.txt)\n";
        std::cout << "3. Случайная генерация (до 1000 цифр)\n";
        std::cout << "0. Вернуться в главное меню\n";

        int mode = getValidatedModeInput("Выберите способ ввода: ", 0, 3);
        if (mode == 0) return;

        std::string inputData;
        try {
            switch (mode) {
                case 1: inputData = getInputFromKeyboard(); break;
                case 2: inputData = getInputFromFile(); break;
                case 3: inputData = generateRandomInput(); break;
                default: continue;
            }

            CircularList ring;
            ring.loadFromString(inputData);
            std::string result = ring.findSolution();

            std::cout << "Результат: " << result << std::endl;
            saveOutputToFile(result);

        } catch (const ReturnToModeMenuException&) {
            continue;
        } catch (const InvalidInputException& e) {
            std::cout << e.what() << std::endl;
            saveOutputToFile(e.what());
        } catch (const NoSolutionException& e) {
            std::cout << "Результат: " << e.what() << std::endl;
            saveOutputToFile(e.what());
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
            saveOutputToFile("Ошибка: " + std::string(e.what()));
        } catch (...) {
            std::cout << "Неизвестная ошибка" << std::endl;
            saveOutputToFile("Неизвестная ошибка");
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    while (true) {
        showMainMenu();
        int choice = getValidatedModeInput("Выберите пункт меню: ", 0, 2);

        if (choice == 0) {
            std::cout << "Выход из программы...\n";
            break;
        }
        if (choice == 1) {
            showDescriptionMenu();
        }
        if (choice == 2) {
            processRingTask();
        }
    }
    return 0;
}
