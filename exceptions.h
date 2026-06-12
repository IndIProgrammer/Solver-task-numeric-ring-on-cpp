#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class InvalidInputException : public std::runtime_error {
public:
    explicit InvalidInputException(const std::string& msg) : std::runtime_error(msg) {}
};

class NoSolutionException : public std::runtime_error {
public:
    NoSolutionException() : std::runtime_error("No") {}
};

class ReturnToModeMenuException : public std::runtime_error {
public:
    ReturnToModeMenuException() : std::runtime_error("Return to mode menu") {}
};

class EmptyListException : public std::runtime_error {
public:
    EmptyListException() : std::runtime_error("Ошибка: Список пуст!") {}
};

#endif
