//
//  main.cpp
//  laboratory1
//
//  Created by Илья Атмажитов on 11.09.2024.
//


#include "ncurses.h"
#include "../include/UI.h"
#include "../include/SmartPointer.h"
#include "../include/LinkedList.h"
#include "../include/DynamicArray.h"

int main() {
    LinkedList<SmrtPtr<int>> smartPointerList;
    DynamicArray<SmrtPtr<int>> smartPointerArray;
    LinkedList<WeakPtr<int>> weakPointerList;
    DynamicArray<WeakPtr<int>> weakPointerArray;

    // Переменные состояния
    bool isArrayMode = false;
    bool isWeakMode = false;
    int selected = 0;

    Initialize();

    while (true) {
        DrawUi(smartPointerList, smartPointerArray, weakPointerList, weakPointerArray, isArrayMode, isWeakMode, selected);

        int ch = getch();
        HandleKeypress(ch, smartPointerList, smartPointerArray, weakPointerList, weakPointerArray, isArrayMode, isWeakMode, selected);
    }

    endwin();
    return 0;
}


