// (C) PROTEI protei.com

#pragma once

#include "ICalculator.h"
#include <vector>
#include <string>

namespace calc
{

class SimpleCalculator : public ICalculator
{
public:

    explicit SimpleCalculator(IHistory& history);

    void SetHistory(IHistory& history) override;

    int Add(int a, int b) override;
    int Subtract(int a, int b) override;
    int Multiply(int a, int b) override;
    int Divide(int a, int b) override;

    std::vector<std::string> getLogs(size_t count);

private:
    void LogOperation(int a, const char* op, int b, int result);

    IHistory& m_history;
};

} // namespace calc