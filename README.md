# XiaofeiHangman

![Status](https://img.shields.io/badge/Status-Archived-red) ![Language](https://img.shields.io/badge/Language-C%2B%2B-blue) ![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

**[中文]** | [English](#english-description)

---

> ⚠️ **注意 / Note**
>
> **[CN]** 这是一个很久以前编写的 C++ 练手小项目，目前已**停止更新**。代码可能比较古早，仅供留念或参考。
>
> **[EN]** This is a mini-project written a long time ago for C++ practice and is **no longer updated**. The code is archived for nostalgia or reference purposes.

---

## 📖 项目简介 (Introduction)

**XiaofeiHangman** 是一个运行在 Windows 平台上的经典“刽子手 (Hangman)”猜词游戏。
通过猜测英文单词中的字母来解谜，如果你猜错了太多次，屏幕上的火柴人就会被“挂”起来。你的目标是在小人牺牲前猜出完整的单词。

### ✨ 功能特性 (Features)

根据游戏界面截图，本项目包含以下功能：

1.  **经典玩法**：输入英文字母猜测隐藏的单词，每次错误都会导致火柴人离死亡更近一步。
2.  **图形化界面 (GUI)**：
    *   可视化的火柴人绘制过程（确保小人安全！）。
    *   自带屏幕软键盘，支持鼠标点击或键盘输入。
3.  **辅助功能**：
    *   **随机单词**：一键开始新的随机挑战。
    *   **词库选择**：支持选择不同的单词库进行挑战（如截图中的 `14Kwds.ini`）。
    *   **错题本**：支持将猜不出来的单词“加至错题本”，方便复习。
    *   **放弃猜词**：直接查看答案。

### 🛠️ 技术栈 (Tech Stack)

*   **Language:** C++
*   **IDE:** Visual Studio (包含 `.sln` 解决方案文件)
*   **Platform:** Windows Desktop

---

<a name="english-description"></a>
## 📖 English Description

**XiaofeiHangman** is a classic Hangman word-guessing game built for Windows.
The goal is to guess the hidden English word letter by letter. If you make too many wrong guesses, the stick figure on the screen will be "hanged". Try to solve the puzzle before the stick man is doomed!

### ✨ Features

Based on the gameplay screenshots:

1.  **Classic Gameplay**: Guess the word by inputting English letters. Every mistake draws a part of the stick figure.
2.  **GUI Interface**:
    *   Visual progress of the hangman drawing ("Keep the little man safe!").
    *   On-screen keyboard interface allowing mouse interaction.
3.  **Utilities**:
    *   **Random Word**: Start a new challenge instantly.
    *   **Dictionary Selection**: Supports choosing different vocabulary lists (e.g., `14Kwds.ini`).
    *   **Mistake Notebook**: Add difficult words to a specific list for later review.
    *   **Give Up**: Reveal the answer immediately.

### 🛠️ Tech Stack

*   **Language:** C++
*   **IDE:** Visual Studio (Project includes `.sln` file)
*   **Platform:** Windows Desktop

---

## ⚖️ License

[GPL-3.0 license](LICENSE)
