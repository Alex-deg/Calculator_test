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

class MockHistory : public IHistory{
public:
    MOCK_METHOD(void, AddEntry, (const std::string& operation), (override));
    MOCK_METHOD(std::vector<std::string>, GetLastOperations, (size_t count), (const, override));
};

class MockCalculator : public ICalculator {
public:
    MOCK_METHOD(int, Add, (int a, int b), (override));
    MOCK_METHOD(int, Subtract, (int a, int b), (override));
    MOCK_METHOD(int, Multiply, (int a, int b), (override));
    MOCK_METHOD(int, Divide, (int a, int b), (override));
    MOCK_METHOD(void, SetHistory, (IHistory& history), (override));
};

// Тесты для SimpleCalculator с моком истории
class CalculatorTest : public ::testing::Test {
protected:
    StrictMock<MockHistory> mockHistory;
    SimpleCalculator calculator{mockHistory};
};

TEST_F(CalculatorTest, AddCallsHistory) {
    EXPECT_CALL(mockHistory, AddEntry("2 + 2 = 4"));
    EXPECT_EQ(calculator.Add(2, 2), 4);
}

TEST_F(CalculatorTest, SubtractCallsHistory) {
    EXPECT_CALL(mockHistory, AddEntry("5 - 3 = 2"));
    EXPECT_EQ(calculator.Subtract(5, 3), 2);
}

TEST_F(CalculatorTest, MultiplyCallsHistory) {
    EXPECT_CALL(mockHistory, AddEntry("3 * 4 = 12"));
    EXPECT_EQ(calculator.Multiply(3, 4), 12);
}

TEST_F(CalculatorTest, DivideCallsHistory) {
    EXPECT_CALL(mockHistory, AddEntry("10 / 2 = 5"));
    EXPECT_EQ(calculator.Divide(10, 2), 5);
}

TEST_F(CalculatorTest, DivisionByZeroThrows) {
    EXPECT_THROW(calculator.Divide(10, 0), std::exception);
}

TEST_F(CalculatorTest, BoundaryValues) {
    const int max = std::numeric_limits<int>::max();
    const int min = std::numeric_limits<int>::min();

    EXPECT_CALL(mockHistory, AddEntry(_)).Times(4);
    
    EXPECT_EQ(calculator.Add(max, 0), max);
    EXPECT_EQ(calculator.Subtract(min, 0), min);
    EXPECT_EQ(calculator.Multiply(max, 1), max);
    EXPECT_EQ(calculator.Divide(min, -1), -min);
}

// Тесты для InMemoryHistory
class HistoryTest : public ::testing::Test {
protected:
    InMemoryHistory history;
};

TEST_F(HistoryTest, StoresOperations) {
    history.AddEntry("1 + 1 = 2");
    history.AddEntry("2 * 2 = 4");
    
    auto ops = history.GetLastOperations(2);
    ASSERT_EQ(ops.size(), 2);
    EXPECT_EQ(ops[0], "1 + 1 = 2");
    EXPECT_EQ(ops[1], "2 * 2 = 4");
}

TEST_F(HistoryTest, GetLastOperationsReturnsCorrectCount) {
    for (int i = 0; i < 10; i++) {
        history.AddEntry("op " + std::to_string(i));
    }
    
    auto ops = history.GetLastOperations(3);
    ASSERT_EQ(ops.size(), 3);
    EXPECT_EQ(ops[0], "op 7");
    EXPECT_EQ(ops[1], "op 8");
    EXPECT_EQ(ops[2], "op 9");
}

TEST_F(HistoryTest, GetMoreThanExistsReturnsAll) {
    history.AddEntry("op1");
    history.AddEntry("op2");
    
    auto ops = history.GetLastOperations(10);
    EXPECT_EQ(ops.size(), 2);
}

// Тест для неопределенного поведения (лимит истории)
TEST_F(HistoryTest, NoLimitOnHistorySize) {
    // В текущей реализации нет лимита - тест должен проходить
    for (int i = 0; i < 10000; i++) {
        history.AddEntry("op " + std::to_string(i));
    }
    
    EXPECT_EQ(history.GetLastOperations(1).size(), 1);
}

// Интеграционный тест калькулятора с реальной историей
class CalculatorIntegrationTest : public ::testing::Test {
protected:
    InMemoryHistory history;
    SimpleCalculator calculator{history};
};

TEST_F(CalculatorIntegrationTest, OperationsAreLogged) {
    calculator.Add(1, 2);
    calculator.Multiply(3, 4);
    
    auto ops = calculator.getLogs(2);
    ASSERT_EQ(ops.size(), 2);
    EXPECT_EQ(ops[0], "1 + 2 = 3");
    EXPECT_EQ(ops[1], "3 * 4 = 12");
}

TEST_F(CalculatorIntegrationTest, ChangeHistory) {
    InMemoryHistory newHistory;
    calculator.SetHistory(newHistory);
    
    calculator.Add(1, 1);
    auto ops = newHistory.GetLastOperations(1);
    ASSERT_EQ(ops.size(), 1);
    EXPECT_EQ(ops[0], "1 + 1 = 2");
}

// Тест с моком калькулятора
TEST(CalculatorMockTest, HistoryUsesCalculator) {
    StrictMock<MockCalculator> mockCalc;
    InMemoryHistory history;
    
    EXPECT_CALL(mockCalc, Add(2, 3)).WillOnce(Return(5));
    
    // Предположим, что у нас есть метод, который использует калькулятор
    int result = mockCalc.Add(2, 3);
    history.AddEntry("2 + 3 = " + std::to_string(result));
    
    auto ops = history.GetLastOperations(1);
    ASSERT_EQ(ops.size(), 1);
    EXPECT_EQ(ops[0], "2 + 3 = 5");
}

TEST(TestingSum, addTwoPositiveNUmbers){

    // Arrange
    calc::InMemoryHistory imh;
    calc::SimpleCalculator sc(imh);
    
    // Act
    auto result = sc.Add(2, 2);

    // Accert
    ASSERT_EQ(result, 4);
}

TEST(TestingSum, addTwoNegativeNumbers){
    
    // Arrange
    calc::InMemoryHistory imh;
    calc::SimpleCalculator sc(imh);
    
    // Act
    auto result = sc.Add(-10, -5);

    // Accert
    ASSERT_EQ(result, -15);   
}

TEST(TestingSum, addNegativeAndPositiveNumbers){
    
    // Arrange
    calc::InMemoryHistory imh;
    calc::SimpleCalculator sc(imh);
    
    // Act
    auto result = sc.Add(-10, 7);

    // Accert
    ASSERT_EQ(result, -3);   
}

TEST(TestingMultiply, multiplySomeValueByZero){
    
    // Arrange
    calc::InMemoryHistory imh;
    calc::SimpleCalculator sc(imh);
    
    // Act
    auto result = sc.Multiply(-10, 0);

    // Accert
    ASSERT_EQ(result, 0);   
}

TEST(TestingMultiply, multiplySomeValueByMinusOne){
    
    // Arrange
    calc::InMemoryHistory imh;
    calc::SimpleCalculator sc(imh);
    
    // Act
    auto result = sc.Multiply(-10, -1);

    // Accert
    ASSERT_EQ(result, 10);   
}

TEST(TestingMultiply, checkForIntOverflow){
    
    // Arrange
    calc::InMemoryHistory imh;
    calc::SimpleCalculator sc(imh);
    
    // Act
    auto result = sc.Multiply(53583, 40716);

    // Accert
    ASSERT_EQ(result, 2181685428);   
}

TEST(TestingSubtract, subtractNegativeNumberFromPositive){
    
    // Arrange
    calc::InMemoryHistory imh;
    calc::SimpleCalculator sc(imh);
    
    // Act
    auto result = sc.Subtract(0, -1);

    // Accert
    ASSERT_EQ(result, 1);   
}

// TEST(TestingDivide, divideSomeValueByZero){
//    
//     // Arrange
//     calc::InMemoryHistory imh;
//     calc::SimpleCalculator sc(imh);
//
//     // Accert    // Act
//     EXPECT_THROW(sc.Divide(-10, 0), std::invalid_argument);   
// }

TEST(TestingDivide, checkFractionalDivision){
    
    // Arrange
    calc::InMemoryHistory imh;
    calc::SimpleCalculator sc(imh);
    
    // Act
    auto result = sc.Divide(7, 2);

    // Accert
    ASSERT_EQ(result, 3.5);   
}

TEST(TestingLogs, checkLogInfo){
    
    // Arrange
    calc::InMemoryHistory imh;
    calc::SimpleCalculator sc(imh);
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