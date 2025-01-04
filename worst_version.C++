#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // 用於格式化輸出
using namespace std;

int main() {
    // 輸入多行文字
    std::string input;
    std::cout << "請輸入多行文字，結束請按 Ctrl+D (Linux/Mac) 或 Ctrl+Z (Windows):" << std::endl;

    std::vector<std::string> lines; // 儲存每行文字
    while (std::getline(std::cin, input)) {
        lines.push_back(input);
    }

    int lineCount = lines.size(); // 計算行數
    std::cout << "輸入的行數為: " << lineCount << std::endl;

    std::vector<std::vector<std::string>> matrices(lineCount);

    std::vector<std::string> pipelineStages = { "if", "id", "ex", "mem", "wb" };

    for (size_t i = 0; i < lines.size(); ++i) {
        matrices[i].insert(matrices[i].end(), pipelineStages.begin(), pipelineStages.end());
    }
    // 循環處理每行，根據上一行的 "id" 位置進行填充和插入 "if"
    for (size_t i = 1; i < matrices.size(); ++i) { // 從第二行開始處理
        auto& previousRow = matrices[i - 1];
        auto& currentRow = matrices[i];

        // 找到上一行的 "id" 位置
        auto idPos = std::find(previousRow.begin(), previousRow.end(), "id");
        size_t idIndex = (idPos != previousRow.end()) ? (idPos - previousRow.begin()) : 0;

        // 在當前行開始處填充 " " 直到上一行 "id" 的位置
        for (size_t j = 0; j < idIndex; ++j) {
            currentRow.insert(currentRow.begin() + j, " ");
        }
    }

    //op指令部分
    string op = lines[1].substr(0, lines[1].find(' '));
    cout << op << endl<<endl;
//存取目標暫存器和來源暫存器
    std::vector<std::vector<std::string>> regMatrix(lineCount);

    for (size_t i = 0; i < lines.size(); ++i) {
        size_t firstSpace = lines[i].find(' ');
        if (firstSpace != std::string::npos) {
            std::string operands = lines[i].substr(firstSpace + 1);
            size_t pos = 0;
            while ((pos = operands.find('$', pos)) != std::string::npos) {
                size_t end = operands.find_first_of(", )", pos); 
                if (end == std::string::npos) {
                    end = operands.size();
                }
                std::string reg = operands.substr(pos, end - pos);
                regMatrix[i].push_back(reg); 
                pos = end; 
            }
        }
    }
    //測試
    std::cout << "寄存器矩陣：" << std::endl;
    for (size_t i = 0; i < regMatrix.size(); ++i) {
        std::cout << "第 " << i + 1 << " 行: ";
        for (const auto& reg : regMatrix[i]) {
            std::cout << reg << " ";
        }
        std::cout << std::endl;
    }
    // 判斷是否有上一行的目標寄存器等於下一行的來源寄存器
    for (size_t i = 1; i < regMatrix.size(); ++i) {
        if (!regMatrix[i - 1].empty()) {
            std::string targetReg = regMatrix[i - 1][0]; 
            bool matchFound = false;

            // 檢查下一行的來源暫存器
            for (size_t j = 1; j < regMatrix[i].size(); ++j) { 
                if (regMatrix[i][j] == targetReg) {
                    matchFound = true;
                    break;
                }
            }
        }
    }

    





















    size_t maxColumns = 0;
    for (const auto& row : matrices) {
        if (row.size() > maxColumns) {
            maxColumns = row.size();
        }
    }

    for (size_t col = 0; col < maxColumns; ++col) {
        std::cout << "Cycle " << col + 1 << ": ";

        for (size_t row = 0; row < matrices.size(); ++row) {
            if (col < matrices[row].size()) {
                std::cout << matrices[row][col] << " "; 
            }
            else {
                std::cout << "  "; 
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
