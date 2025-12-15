#include "hangman.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <windows.h>
#include <map>

using namespace std;

void setupConsole() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
}

bool isCyrillic(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return (uc >= 192 && uc <= 223) || (uc >= 224 && uc <= 255);
}

bool isLatin(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isLetter(char c) {
    unsigned char uc = static_cast<unsigned char>(c);
    return isLatin(c) || isCyrillic(c);
}

char toLowerRussian(char c) {
    unsigned char uc = static_cast<unsigned char>(c);

    if (uc >= 192 && uc <= 223) {
        return static_cast<char>(uc + 32);
    }
    else if (c >= 'A' && c <= 'Z') {
        return tolower(c);
    }

    return c;
}

bool isRussianWord(const string& word) {
    for (char c : word) {
        if (!isLetter(c)) {
            return false;
        }
    }
    return true;
}

string decryptWord(const string& encrypted) {
    string decrypted = encrypted;
    for (char& c : decrypted) {
        c = c - 3;
    }
    return decrypted;
}

vector<string> loadWords(const string& filename) {
    vector<string> words;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Ошибка: не получается открыть файл " << filename << endl;
        return words;
    }

    string encrypted;
    while (file >> encrypted) {
        string word = decryptWord(encrypted);
        if (isRussianWord(word)) {
            words.push_back(word);
        }
    }

    file.close();
    return words;
}

bool matchesDifficulty(const string& word, Difficulty difficulty) {
    int length = word.length();

    switch (difficulty) {
    case EASY:   return length >= 3 && length <= 5;
    case MEDIUM: return length >= 6 && length <= 8;
    case HARD:   return length >= 9;
    case EXPERT: return true;
    default:     return true;
    }
}

bool matchesCategory(const string& word, WordCategory category) {
    if (category == ALL_CATEGORIES) return true;

    static map<string, WordCategory> wordCategories = {
        {"код", BASIC_TERMS}, {"цикл", BASIC_TERMS}, {"тип", BASIC_TERMS},
        {"бит", BASIC_TERMS}, {"байт", BASIC_TERMS}, {"флаг", BASIC_TERMS},
        {"массив", BASIC_TERMS}, {"список", BASIC_TERMS}, {"класс", BASIC_TERMS},
        {"объект", BASIC_TERMS}, {"метод", BASIC_TERMS}, {"поле", BASIC_TERMS},
        {"алгоритм", BASIC_TERMS}, {"программа", BASIC_TERMS},

        {"си", LANGUAGES}, {"ада", LANGUAGES}, {"паскаль", LANGUAGES},
        {"питон", LANGUAGES}, {"джава", LANGUAGES}, {"котлин", LANGUAGES},
        {"голанг", LANGUAGES}, {"руст", LANGUAGES}, {"свифт", LANGUAGES},
        {"джаваскрипт", LANGUAGES}, {"типескрипт", LANGUAGES},

        {"реакт", TECHNOLOGIES}, {"вью", TECHNOLOGIES}, {"ангуляр", TECHNOLOGIES},
        {"ноде", TECHNOLOGIES}, {"экспресс", TECHNOLOGIES}, {"спринг", TECHNOLOGIES},
        {"дотнет", TECHNOLOGIES}, {"ларавел", TECHNOLOGIES}, {"фласк", TECHNOLOGIES},
        {"джанго", TECHNOLOGIES},

        {"клиент", ARCHITECTURE}, {"сервер", ARCHITECTURE}, {"прокси", ARCHITECTURE},
        {"микросервис", ARCHITECTURE}, {"монолит", ARCHITECTURE},
        {"база", ARCHITECTURE}, {"таблица", ARCHITECTURE}
    };

    auto it = wordCategories.find(word);
    if (it != wordCategories.end()) {
        return it->second == category;
    }

    return true;
}

string getRandomWord(const vector<string>& words, WordCategory category, Difficulty difficulty) {
    if (words.empty()) return "";

    vector<string> filteredWords;
    for (const auto& word : words) {
        if (matchesCategory(word, category) && matchesDifficulty(word, difficulty)) {
            filteredWords.push_back(word);
        }
    }

    if (filteredWords.empty()) {
        return words[rand() % words.size()];
    }

    return filteredWords[rand() % filteredWords.size()];
}

string getCategoryName(WordCategory category) {
    switch (category) {
    case BASIC_TERMS:   return "Базовые термины";
    case LANGUAGES:     return "Языки программирования";
    case TECHNOLOGIES:  return "Технологии и фреймворки";
    case ARCHITECTURE:  return "Архитектура";
    case ALL_CATEGORIES:return "Все категории";
    default:            return "Неизвестно";
    }
}

string getDifficultyName(Difficulty difficulty) {
    switch (difficulty) {
    case EASY:   return "Легкая (3-5 букв)";
    case MEDIUM: return "Средняя (6-8 букв)";
    case HARD:   return "Сложная (9+ букв)";
    case EXPERT: return "Эксперт (все слова)";
    default:     return "Неизвестно";
    }
}

void displayHangman(int wrongAttempts) {
    vector<string> stages = {
        "",
        " O\n",
        " O\n |\n",
        " O\n/|\n",
        " O\n/|\\\n",
        " O\n/|\\\n/\n",
        " O\n/|\\\n/ \\\n"
    };

    cout << "\n_______\n";
    cout << "|     |\n";
    cout << "|" << stages[wrongAttempts];
    cout << "|\n";
    cout << "|_____\n\n";
}

void displayWord(const string& word, const vector<bool>& guessed) {
    cout << "Слово: ";
    for (size_t i = 0; i < word.length(); i++) {
        if (guessed[i]) {
            cout << word[i] << " ";
        }
        else {
            cout << "_ ";
        }
    }
    cout << endl;
}

void showStatistics(const string& word, const vector<char>& triedLetters,
    int totalAttempts, int gameTime, bool won) {
    cout << "\n=====================================\n";
    cout << "          СТАТИСТИКА ИГРЫ\n";
    cout << "=====================================\n";

    if (won) {
        cout << "ПОБЕДА!\n";
    }
    else {
        cout << "ВЫ ПРОИГРАЛИ\n";
    }

    cout << "Загаданное слово: " << word << "\n";
    cout << "Длина слова: " << word.length() << " букв\n";
    cout << "Время игры: " << gameTime << " секунд\n";
    cout << "Всего попыток: " << totalAttempts << "\n";

    int correctAttempts = 0;
    for (char letter : triedLetters) {
        if (word.find(letter) != string::npos) {
            correctAttempts++;
        }
    }

    cout << "Правильных букв: " << correctAttempts << "\n";
    cout << "Ошибочных букв: " << (totalAttempts - correctAttempts) << "\n";

    if (totalAttempts > 0) {
        int accuracy = (correctAttempts * 100) / totalAttempts;
        cout << "Точность: " << accuracy << "%\n";
    }

    cout << "Использованные буквы: ";
    for (char c : triedLetters) {
        cout << c << " ";
    }
    cout << "\n=====================================\n";
}

void clearInput() {
    cin.clear();
    cin.ignore(10000, '\n');
}

void playGame() {
    setupConsole();
    srand(static_cast<unsigned int>(time(0)));

    vector<string> words = loadWords("words.txt");
    if (words.empty()) {
        cout << "Файл со словами не найден или пуст!" << endl;
        cout << "Создайте файл words.txt с помощью file_generator.cpp" << endl;
        return;
    }

    cout << "===================================\n";
    cout << "             'ВИСЕЛИЦА'\n";
    cout << "===================================\n\n";

    int categoryChoice;
    WordCategory category = ALL_CATEGORIES;

    while (true) {
        cout << "Выберите категорию слов:\n";
        cout << "1. Базовые термины\n";
        cout << "2. Языки программирования\n";
        cout << "3. Технологии и фреймворки\n";
        cout << "4. Архитектура\n";
        cout << "5. Все категории\n";
        cout << "Ваш выбор (1-5): ";

        if (cin >> categoryChoice) {
            if (categoryChoice >= 1 && categoryChoice <= 5) {
                category = static_cast<WordCategory>(categoryChoice - 1);
                break;
            }
        }
        cout << "Неверный ввод! Попробуйте снова.\n";
        clearInput();
    }

    int difficultyChoice;
    Difficulty difficulty = EXPERT;

    while (true) {
        cout << "\nВыберите сложность:\n";
        cout << "1. Легкая (3-5 букв)\n";
        cout << "2. Средняя (6-8 букв)\n";
        cout << "3. Сложная (9+ букв)\n";
        cout << "4. Эксперт (все слова)\n";
        cout << "Ваш выбор (1-4): ";

        if (cin >> difficultyChoice) {
            if (difficultyChoice >= 1 && difficultyChoice <= 4) {
                difficulty = static_cast<Difficulty>(difficultyChoice - 1);
                break;
            }
        }
        cout << "Неверный ввод! Попробуйте снова.\n";
        clearInput();
    }

    clearInput();

    cout << "\nКатегория: " << getCategoryName(category) << endl;
    cout << "Сложность: " << getDifficultyName(difficulty) << endl;
    cout << "===================================\n\n";

    string word = getRandomWord(words, category, difficulty);
    for (char& c : word) {
        c = toLowerRussian(c);
    }

    vector<bool> guessed(word.length(), false);
    vector<char> triedLetters;
    int wrongAttempts = 0;
    int totalAttempts = 0;
    time_t startTime = time(0);

    cout << "Слово загадано! Длина слова: " << word.length() << " букв.\n";
    cout << "У вас есть 6 попыток.\n";
    cout << "Введите '00' для выхода.\n\n";
    


    while (wrongAttempts < 6) {
        displayHangman(wrongAttempts);
        displayWord(word, guessed);

        cout << "\nИспользованные буквы: ";
        for (char c : triedLetters) {
            cout << c << " ";
        }
        cout << "\nОшибок: " << wrongAttempts << " из 6\n";

        string input;
        cout << "\nВведите букву (или 00 для выхода): \n";
        cout << "**************************************\n";
        getline(cin, input);

        if (input == "00") {
            cout << "\nИгра прервана пользователем.\n";
            showStatistics(word, triedLetters, totalAttempts,
                difftime(time(0), startTime), false);
            return;
        }

        if (input.empty() || input.length() != 1) {
            cout << "Введите одну букву!\n";
            continue;
        }

        char letter = toLowerRussian(input[0]);

        if (!isLetter(letter)) {
            cout << "Это не буква.\n";
            continue;
        }

        bool alreadyTried = false;
        for (char tried : triedLetters) {
            if (tried == letter) {
                alreadyTried = true;
                break;
            }
        }

        if (alreadyTried) {
            cout << "Вы уже пробовали эту букву!\n";
            continue;
        }

        triedLetters.push_back(letter);
        totalAttempts++;

        bool found = false;
        for (size_t i = 0; i < word.length(); i++) {
            if (word[i] == letter) {
                guessed[i] = true;
                found = true;
            }
        }

        if (!found) {
            wrongAttempts++;
            cout << "Буквы '" << letter << "' нет в слове.\n";
        }
        else {
            cout << "Есть такая буква!\n";
        }

        bool allGuessed = true;
        for (bool g : guessed) {
            if (!g) {
                allGuessed = false;
                break;
            }
        }

        if (allGuessed) {
            cout << "\nПОЗДРАВЛЯЮ С ПОБЕДОЙ!\n";
            displayHangman(wrongAttempts);
            cout << "Загаданное слово: " << word << endl;
            break;
        }
    }

    if (wrongAttempts >= 6) {
        displayHangman(6);
        cout << "\nВЫ ПРОИГРАЛИ...\n";
        cout << "Загаданное слово: " << word << endl;
    }

    time_t endTime = time(0);
    int gameTime = difftime(endTime, startTime);

    showStatistics(word, triedLetters, totalAttempts, gameTime, wrongAttempts < 6);
}
