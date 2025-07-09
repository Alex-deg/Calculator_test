#include <iostream>
#include <gtest/gtest.h>
#include "SimpleCalculator.h"
#include "InMemoryHistory.h"

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



int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}