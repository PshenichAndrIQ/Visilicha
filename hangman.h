#pragma once

#include <vector>
#include <string>

enum WordCategory {
    BASIC_TERMS,
    LANGUAGES,
    TECHNOLOGIES,
    ARCHITECTURE,
    ALL_CATEGORIES
};

enum Difficulty {
    EASY,      // 3-5 букв
    MEDIUM,    // 6-8 букв
    HARD,      // 9+ букв
    EXPERT     // Все слова
};

std::vector<std::string> loadWords(const std::string& filename);
std::string getRandomWord(const std::vector<std::string>& words, WordCategory category = ALL_CATEGORIES, Difficulty difficulty = EXPERT);
void displayHangman(int wrongAttempts);
void displayWord(const std::string& word, const std::vector<bool>& guessed);
std::string getCategoryName(WordCategory category);
std::string getDifficultyName(Difficulty difficulty);
void showStatistics(const std::string& word, const std::vector<char>& triedLetters, int totalAttempts, int gameTime, bool won);
void playGame();
