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


    //插入的ARRAY
    std::vector<std::string> pipelineStages = { "if","id", "ex", "mem", "wb"};




    //這裡可以根據不同指令寫死   (Worst case)
    /*
    string op = lines[1].substr(0, lines[1].find(' '));
    if (op == "lw") {
        std::vector<std::string> pipelineStages1= { "lw: if","lw: id", "lw: ex  01 010 11", "lw: mem 010 11", "lw: wb 11" };
        for (size_t i = 0; i < lines.size(); ++i) {
        matrices[i].insert(matrices[i].end(), pipelineStages1.begin(), pipelineStages1.end());
    }
    }
    else if (op == "add") {
        std::vector<std::string> pipelineStages1 = { "add: if","add: id", "add: ex 10 000 10", "add: mem 000 10", "add: wb 10" };
        matrices[i].insert(matrices[i].end(), pipelineStages2.begin(), pipelineStages2.end());
    }
    else if (op == "beq") {
        std::vector<std::string> pipelineStages1 = { "beq: if","beq: id", "beq: ex X0 100 0X", "beq: mem 100 0X", "beq: wb 0X" };
        matrices[i].insert(matrices[i].end(), pipelineStages3.begin(), pipelineStages3.end());
    }
    else if (op == "sw") {
        std::vector<std::string> pipelineStages1 = { "sw: if","sw: id", "sw: ex X1 001 0X", "sw: mem 001 0X", "sw: wb 0X" };
        matrices[i].insert(matrices[i].end(), pipelineStages4.begin(), pipelineStages4.end());
    }
    //  注意這部分如果寫死下一部分的指令要改
    */
    //每個指令都有固定的 EX  XXXX 、 MEM  XX、 WB X  




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


    

//這一部份有bug
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
            //主要是lw部分 這個完成add部分照抄
            if (op == "lw" && matchFound == 1) {

                auto& previousRow = matrices[i - 1];
                auto& currentRow = matrices[i];

                // 找出上一行的所有 "id" 的位置
                std::vector<size_t> idPositions;
                for (size_t j = 0; j < previousRow.size(); ++j) {
                    if (previousRow[j] == "id") {
                        idPositions.push_back(j);
                    }
                }
                // 根据上一行的 "id" 位置填充 "if"
                for (size_t pos : idPositions) {
                    currentRow.insert(currentRow.begin() + pos, " ");
                    currentRow[pos] = "if";
                }

                // 找出上一行 "ex" 和 "wb" 的位置
                auto exePos = std::find(previousRow.begin(), previousRow.end(), "ex");
                auto memPos = std::find(previousRow.begin(), previousRow.end(), "mem");

                // 根据上一行 "ex" 和 "wb" 之间的位置填充 "id"
                if (exePos != previousRow.end() && memPos != previousRow.end()) {
                    size_t exeIndex = exePos - previousRow.begin() - 1;
                    size_t memIndex = memPos - previousRow.begin();
                    currentRow[exeIndex + 1] = "id";
                    currentRow[memIndex] = "id";
                }
            }




        }
    }
 //到這裡  





















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
