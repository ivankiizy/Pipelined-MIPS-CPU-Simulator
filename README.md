# 環境設置

## 必須安裝的軟體與工具

### Runtime
- Visual Studio Community Edition (適用於 C++ 開發)
- C++ 編譯器 (Visual Studio 已包含)

### Package Manager
- 無

### 依賴項
- 若有使用額外的第三方庫，請列出相關安裝指令或套件：無
---

## 編譯與執行步驟

### 編譯指令
1. 打開 Visual Studio Community Edition。
2. 開啟專案檔案 (.sln)。
3. 在 Visual Studio 中選擇適當的構建設定（Debug/Release）。
4. 點擊 "生成" 或按下 `Ctrl+Shift+B` 來編譯專案。

### 執行指令
1. 編譯完成後，點擊 "啟動" 或按下 `Ctrl+F5` 執行程式。
2. 若需要傳遞額外參數，可以在 "命令列引數" 中設定。

---

## 範例執行

1. 範例：
```
lw $2, 8($0)
lw $3, 16($0) 
beq $2, $3, 1
add $4, $2, $3
sw $4, 24($0)
```
- 直接將指令貼上至終端機，輸入完成後按下 Ctrl+D (Linux/Mac) or Ctrl+Z (Windows) 結束指令輸入。
