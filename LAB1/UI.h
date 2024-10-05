//
//  UI.h
//  laboratory1
//
//  Created by Илья Атмажитов on 28.09.2024.
//

#ifndef UI_h
#define UI_h


#include <ncurses.h>
#include <utility>
#include <string>
#include <sstream>
#include <iostream>
#include "SmartPointer.h"
#include "LinkedList.h"
#include "DynamicArray.h"

void initialize() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
}


template <class T>
void draw_ui(const LinkedList<SmrtPtr<T> >& smartPointerList, const DynamicArray<SmrtPtr<T> >& smartPointerArray, bool isArrayMode, int selected) {
    clear();
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "Use arrow keys to navigate. '^Q' Quit, '^S' Create SmartPtr, '^L' LinkedList Mode, '^T' Copy SmrtPtr, '^A' DynamicArray Mode");

    if (isArrayMode) {
        mvprintw(1, 0, "Dynamic Array Mode:");
        for (size_t i = 0; i < smartPointerArray.GetSize(); i++) {
            const SmrtPtr<T>& ptr = smartPointerArray.Get(i);
            mvprintw(i + 2, 0, "Smart Pointer %lu (Value: %d, Count: %lu)", i, *ptr, ptr.GetRefCount() - 1);
        }
    } else {
        mvprintw(1, 0, "Linked List Mode:");
        for (size_t i = 0; i < smartPointerList.GetSize(); i++) {
            const SmrtPtr<T>& ptr = smartPointerList.Get(i);
            mvprintw(i + 2, 0, "Smart Pointer %lu (Value: %d, Count: %lu)", i, *ptr, ptr.GetRefCount() - 1);
        }
    }

    refresh();
}


template <class T>
void create_smart_pointer(LinkedList<SmrtPtr<T> >& smartPointerList, DynamicArray<SmrtPtr<T> >& smartPointerArray, bool isArrayMode) {
    T value;
    mvprintw(22, 0, "Enter value for new Smart Pointer: ");
    echo();
    scanw("%d", &value);
    noecho();

    SmrtPtr<int> smartPtr(new T(value));
    if (isArrayMode) {
        smartPointerArray.Append(smartPtr);
    } else {
        smartPointerList.Append(smartPtr);
    }
}

template <class T>
void create_smart_pointer_reference(LinkedList<SmrtPtr<T> >& smartPointerList, DynamicArray<SmrtPtr<T> >& smartPointerArray, bool isArrayMode) {
    int index;
    mvprintw(22, 0, "Enter index to reference existing Smart Pointer: ");
    echo();
    scanw("%d", &index);
    noecho();

    if (isArrayMode && index < smartPointerArray.GetSize()) {
        SmrtPtr<T> smartPtr = smartPointerArray.Get(index);
        smartPointerArray.Append(smartPtr);
    } else if (!isArrayMode && index < smartPointerList.GetSize()) {
        SmrtPtr<T> smartPtr = smartPointerList.Get(index);
        smartPointerList.Append(smartPtr);
    }
}


template <class T>
void handle_keypress(int ch, LinkedList<SmrtPtr<T> >& smartPointerList, DynamicArray<SmrtPtr<T> >& smartPointerArray, bool& isArrayMode, int& selected) {
    switch (ch) {
        case 17: // Ctrl+Q
            endwin();
            exit(0);
        case 19: { // Ctrl+S: Create new smart pointer
            create_smart_pointer(smartPointerList, smartPointerArray, isArrayMode);
            break;
        }
        case 20: { // Ctrl+T: Create smart pointer referencing existing
            create_smart_pointer_reference(smartPointerList, smartPointerArray, isArrayMode);
            break;
        }
        case 12: { // Ctrl+L: Switch to LinkedList mode
            isArrayMode = false;
            break;
        }
        case 1: { // Ctrl+A: Switch to DynamicArray mode
            isArrayMode = true;
            break;
        }
        case KEY_UP:
            selected = (selected > 0) ? selected - 1 : selected;
            break;
        case KEY_DOWN:
            selected = (isArrayMode ? selected < smartPointerArray.GetSize() - 1 : selected < smartPointerList.GetSize() - 1) ? selected + 1 : selected;
            break;
        default:
            break;
    }
}



#endif /* UI_h */
