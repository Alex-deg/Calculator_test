#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "SimpleCalculator.h"
#include "InMemoryHistory.h"
#include "IHistory.h"

using ::testing::_;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::AtLeast;
using ::testing::Throw;
using namespace calc;

// Мока на историю
class MockHistory : public IHistory{
public:
    MOCK_METHOD(void, AddEntry, (const std::string& operation), (override));
    MOCK_METHOD(std::vector<std::string>, GetLastOperations, (size_t count), (const, override));
};

// Мока на калькулятор
class MockCalculator : public ICalculator {
public:
    MOCK_METHOD(int, Add, (int a, int b), (override));
    MOCK_METHOD(int, Subtract, (int a, int b), (override));
    MOCK_METHOD(int, Multiply, (int a, int b), (override));
    MOCK_METHOD(int, Divide, (int a, int b), (override));
    MOCK_METHOD(void, SetHistory, (IHistory& history), (override));
};

// Тесты для SimpleCalculator с мокой истории
class CalculatorTest : public ::testing::Test {
protected:
    // Arrange
    StrictMock<MockHistory> mockHistory; // мока истории
    SimpleCalculator calculator{mockHistory}; // объект калькулятора
};

// При запуске метода Add с параметрами (2, 2) ожидается вызов AddEntry  
// с аргументом "2 + 2 = 4"
TEST_F(CalculatorTest, AddCallsHistory) {
    // Accert
    EXPECT_CALL(mockHistory, AddEntry("2 + 2 = 4"));
              // Act      
    EXPECT_EQ(calculator.Add(2, 2), 4); // 4 - ожидаемый результат при сложении 2 и 2
}

// При запуске метода Subtract с параметрами (5, 3) ожидается вызов AddEntry
// с аргументом "5 - 3 = 2"
TEST_F(CalculatorTest, SubtractCallsHistory) {
    // Accert
    EXPECT_CALL(mockHistory, AddEntry("5 - 3 = 2"));
              // Act
    EXPECT_EQ(calculator.Subtract(5, 3), 2); // 2 - ожидаемый результат при вычитании 5 и 3 
}

// При запуске метода Multiply с параметрами (3, 4) ожидается вызов AddEntry
// с аргументом "3 * 4 = 12"
TEST_F(CalculatorTest, MultiplyCallsHistory) {
    // Accert
    EXPECT_CALL(mockHistory, AddEntry("3 * 4 = 12"));
              // Act
    EXPECT_EQ(calculator.Multiply(3, 4), 12); // 12 - ожидаемый результат при умножении 3 и 4 
}

// При запуске метода Divide с параметрами (10, 2) ожидается вызов AddEntry
// с аргументом "10 / 2 = 5"
TEST_F(CalculatorTest, DivideCallsHistory) {
    // Accert
    EXPECT_CALL(mockHistory, AddEntry("10 / 2 = 5"));
              // Act
    EXPECT_EQ(calculator.Divide(10, 2), 5); // 5 - ожидаемый результат при делении 10 и 2 
}

// Проверка того, что бросится исключение при делении на 0
// TEST_F(CalculatorTest, DivisionByZeroThrows) {
//     // Accert    // Act
//     EXPECT_THROW(calculator.Divide(10, 0), std::exception);
// }

// Проверка граничных значений
// TEST_F(CalculatorTest, BoundaryValues) {

//     // Act

//     // Получаем максимальное и минимальное значение для типа, 
//     // с которым работает калькулятор - int 
//     const int max = std::numeric_limits<int>::max(); // максимум
//     const int min = std::numeric_limits<int>::min(); // минимум

//     EXPECT_CALL(mockHistory, AddEntry(_)).Times(4); // при запуске методов, представленных ниже
//                                                     // ожидаются 4 вызова AddEntry с произвольными параметрами
    
//     // Accert

//     EXPECT_EQ(calculator.Add(max, 0), max);      // при прибавлении 0 к максимуму ожидается максимум
//     EXPECT_EQ(calculator.Subtract(min, 0), min); // при вычитании из минимума 0   ожидается минимум
//     EXPECT_EQ(calculator.Multiply(max, 1), max); // при умножении максимума на 1  ожидается максимуум
//     EXPECT_EQ(calculator.Divide(min, -1), -min); // при делении минимума на -1    должен поменяться знак
// }

// Тесты для InMemoryHistory
class HistoryTest : public ::testing::Test {
protected:
    // Arrange
    InMemoryHistory history;
};

// Проверка на то, успешно ли метод AddEntry сохраняет историю операций
TEST_F(HistoryTest, StoresOperations) {

    // Act

    history.AddEntry("1 + 1 = 2"); // добавление нескольких операций для примера
    history.AddEntry("2 * 2 = 4");
    auto ops = history.GetLastOperations(2); 

    // Accert

    ASSERT_EQ(ops.size(), 2); // проверка на то, что количество на входе равно количеству на выходе
    EXPECT_EQ(ops[0], "1 + 1 = 2"); // сверяем все ли корректно сохранилось
    EXPECT_EQ(ops[1], "2 * 2 = 4");
}

// Проверка на то, действительно ли GetLastOperations(n) выдает ровно n последних операций 
TEST_F(HistoryTest, GetLastOperationsReturnsCorrectCount) {

    // Act

    // добавляем 10 операций для примера
    for (int i = 0; i < 10; i++) {
        history.AddEntry("op " + std::to_string(i));
    }
    
    auto ops = history.GetLastOperations(3); // допустим возьмем 3 последние операции

    // Accert

    ASSERT_EQ(ops.size(), 3); // проверяем 3 ли операции нам вернулись
    EXPECT_EQ(ops[0], "op 7"); // проверяем на правильность содержимого
    EXPECT_EQ(ops[1], "op 8");
    EXPECT_EQ(ops[2], "op 9");
}

// Проверка на то, можно ли вернуть больше операций чем есть на самом деле с помощью метода GetLastOperations()
TEST_F(HistoryTest, GetMoreThanExistsReturnsAll) {

    // Act

    history.AddEntry("op1"); // заполняем историю двумя операциями
    history.AddEntry("op2");
    
    auto ops = history.GetLastOperations(10); // а просим вернуть 10

    // Accert

    EXPECT_EQ(ops.size(), 2); // ожидаем, что вернется все которые имеются, но не больше
}

// Тест для неопределенного поведения (лимит истории)
TEST_F(HistoryTest, NoLimitOnHistorySize) {

    // Act

    for (int i = 0; i < 1000000; i++) {
        history.AddEntry("op " + std::to_string(i));
    }
    
    // Accert

    EXPECT_EQ(history.GetLastOperations(1).size(), 1);
}

// Тест с моком калькулятора
TEST(CalculatorMockTest, HistoryUsesCalculator) {
    
    // Arrange

    StrictMock<MockCalculator> mockCalc; // мока калькулятора
    InMemoryHistory history; // объект истории операций
    
    // Act

    // При запуске Add ожидается получить результат 5
    EXPECT_CALL(mockCalc, Add(2, 3)).WillOnce(Return(5));
    
    // Предположим, что у нас есть метод, который использует калькулятор
    int result = mockCalc.Add(2, 3);
    history.AddEntry("2 + 3 = " + std::to_string(result)); // записываем операцию в историю
    
    auto ops = history.GetLastOperations(1);

    // Accert

    ASSERT_EQ(ops.size(), 1); // проверяем на то, что в истории ровно 1 операция
    EXPECT_EQ(ops[0], "2 + 3 = 5"); // проверяем на корректность сохраненную операцию
}


// Тестирование операций
class OperationsCheck : public ::testing::Test {
protected:
    // Arrange
    InMemoryHistory history;
    SimpleCalculator sc{history};
};

// Проверка сложения 2 положительных чисел
TEST_F(OperationsCheck, addTwoPositiveNumbers){
    
    // Act
    auto result = sc.Add(2, 2);

    // Accert
    ASSERT_EQ(result, 4);
}

// Проверка сложения двух отрицательных чисел
TEST_F(OperationsCheck, addTwoNegativeNumbers){
    
    // Act
    auto result = sc.Add(-10, -5);

    // Accert
    ASSERT_EQ(result, -15);   
}

// Проверка сложения положительного и отрицпательного
TEST_F(OperationsCheck, addNegativeAndPositiveNumbers){
    
    // Act
    auto result = sc.Add(-10, 7);

    // Accert
    ASSERT_EQ(result, -3);   
}

// Проверка умножения на 0
TEST_F(OperationsCheck, multiplySomeValueByZero){
    
    // Act
    auto result = sc.Multiply(-10, 0);

    // Accert
    ASSERT_EQ(result, 0);   
}

// Проверка умножения на -1
TEST_F(OperationsCheck, multiplySomeValueByMinusOne){
    
    // Act
    auto result = sc.Multiply(-10, -1);

    // Accert
    ASSERT_EQ(result, 10);   
}

// TEST_F(OperationsCheck, checkForIntOverflow){
//    
//     // Act
//     auto result = sc.Multiply(53583, 40716);
//
//     // Accert
//     ASSERT_EQ(result, 2181685428);   
// }

// Проверка вычитания отрицательного числа из положительного
TEST_F(OperationsCheck, subtractNegativeNumberFromPositive){
    
    // Act
    auto result = sc.Subtract(0, -1);

    // Accert
    ASSERT_EQ(result, 1);   
}

// Проверка дробного деления
TEST_F(OperationsCheck, checkFractionalDivision){
    
    // Act
    auto result = sc.Divide(7, 2);

    // Accert
    ASSERT_EQ(result, 3.5);   
}

// Проверка на запись логов (тестировка проблемных мест)
TEST_F(OperationsCheck, checkLogInfo){
    
    // Arrange
    std::vector<std::string> expect = { 
        "7 / 2 = 3.5",
        "2 - 2 = 0",
        "5 + 100 = 105"
    };

    // Act
    sc.Divide(7, 2);
    sc.Add(2, -2);
    sc.Subtract(5, -100);

    // Accert
    ASSERT_EQ(sc.getLogs(3), expect);   
}