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
#include <sstream>
#include <iostream>
#include "SmartPointer.h"
#include "LinkedList.h"
#include "DynamicArray.h"
#include "tests.h"
#include "timing.h"

using namespace std;

void Initialize();

void RunTimingTests();

template <class T>
void DrawUi(const LinkedList<SmrtPtr<T>>& smartPointerList, const DynamicArray<SmrtPtr<T>>& smartPointerArray,
            const LinkedList<WeakPtr<T>>& weakPointerList, const DynamicArray<WeakPtr<T>>& weakPointerArray,
            bool isArrayMode, bool isWeakMode, int selected) {
    clear();
    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "Use arrow keys to navigate. '^Q' Quit, '^P Timing', '^S' Create Ptr, '^L' LinkedList, '^A' DynamicArray, '^W' Toggle Weak/Smart, '^T' Copy Ptr, '^R' Run Tests");

    mvprintw(1, 0, "%s %s Mode:", isWeakMode ? "Weak" : "Smart", isArrayMode ? "Array" : "Linked List");

    size_t size;
    if (isArrayMode) {
        size = isWeakMode ? weakPointerArray.GetSize() : smartPointerArray.GetSize();
    } else {
        size = isWeakMode ? weakPointerList.GetSize() : smartPointerList.GetSize();
    }

    for (size_t i = 0; i < size; i++) {
        if (isWeakMode) {
            const WeakPtr<T>& weakPtr = isArrayMode ? weakPointerArray.Get(i) : weakPointerList.Get(i);
            if (!weakPtr.Expired()) {
                mvprintw(i + 2, 0, "Weak Pointer %lu (Expired: No, Weak Count: %lu, Smart Count: %lu)",
                         i, weakPtr.GetWeakCount(), weakPtr.GetRefCount());
            } else {
                mvprintw(i + 2, 0, "Weak Pointer %lu (Expired: Yes)", i);
            }
        } else {
            const SmrtPtr<T>& smartPtr = isArrayMode ? smartPointerArray.Get(i) : smartPointerList.Get(i);
            mvprintw(i + 2, 0, "Smart Pointer %lu (Value: %d, Smart Count: %lu, Weak Count: %lu)",
                     i, *smartPtr, smartPtr.GetRefCount(), smartPtr.GetWeakCount());
        }
    }

    refresh();
}

template <class T>
void CreateSmartPointer(LinkedList<SmrtPtr<T>>& smartPointerList, DynamicArray<SmrtPtr<T>>& smartPointerArray, bool isArrayMode) {
    T value;
    mvprintw(22, 0, "Enter value for new Smart Pointer: ");
    echo();
    scanw("%d", &value);
    noecho();

    SmrtPtr<T> smartPtr(new T(value));
    if (isArrayMode) {
        smartPointerArray.Append(smartPtr);
    } else {
        smartPointerList.Append(smartPtr);
    }
}

template <class T>
void CreateWeakPointer(LinkedList<WeakPtr<T>>& weakPointerList, DynamicArray<WeakPtr<T>>& weakPointerArray,
                       const LinkedList<SmrtPtr<T>>& smartPointerList, const DynamicArray<SmrtPtr<T>>& smartPointerArray,
                       bool isArrayMode) {
    int index;
    mvprintw(22, 0, "Enter index of Smart Pointer to create Weak Pointer: ");
    echo();
    scanw("%d", &index);
    noecho();

    if (isArrayMode && index < smartPointerArray.GetSize()) {
        WeakPtr<T> weakPtr(smartPointerArray.Get(index));
        weakPointerArray.Append(weakPtr);
    } else if (!isArrayMode && index < smartPointerList.GetSize()) {
        WeakPtr<T> weakPtr(smartPointerList.Get(index));
        weakPointerList.Append(weakPtr);
    }
}

template <class T>
void CopyPointer(LinkedList<SmrtPtr<T>>& smartPointerList, DynamicArray<SmrtPtr<T>>& smartPointerArray,
                 LinkedList<WeakPtr<T>>& weakPointerList, DynamicArray<WeakPtr<T>>& weakPointerArray,
                 bool isArrayMode, bool isWeakMode) {
    int index;
    mvprintw(22, 0, "Enter index to copy existing Pointer: ");
    echo();
    scanw("%d", &index);
    noecho();

    if (isWeakMode) {
        if (isArrayMode && index < weakPointerArray.GetSize()) {
            WeakPtr<T> weakPtr = weakPointerArray.Get(index);
            weakPointerArray.Append(weakPtr);
        } else if (!isArrayMode && index < weakPointerList.GetSize()) {
            WeakPtr<T> weakPtr = weakPointerList.Get(index);
            weakPointerList.Append(weakPtr);
        }
    } else {
        if (isArrayMode && index < smartPointerArray.GetSize()) {
            SmrtPtr<T> smartPtr = smartPointerArray.Get(index);
            smartPointerArray.Append(smartPtr);
        } else if (!isArrayMode && index < smartPointerList.GetSize()) {
            SmrtPtr<T> smartPtr = smartPointerList.Get(index);
            smartPointerList.Append(smartPtr);
        }
    }
}


void RunTests();

template <class T>
void HandleKeypress(int ch, LinkedList<SmrtPtr<T>>& smartPointerList, DynamicArray<SmrtPtr<T>>& smartPointerArray,
                    LinkedList<WeakPtr<T>>& weakPointerList, DynamicArray<WeakPtr<T>>& weakPointerArray,
                    bool& isArrayMode, bool& isWeakMode, int& selected) {
    switch (ch) {
        case 17: // Ctrl+Q
            endwin();
            exit(0);
        case 19: // Ctrl+S
            if (isWeakMode) {
                CreateWeakPointer(weakPointerList, weakPointerArray, smartPointerList, smartPointerArray, isArrayMode);
            } else {
                CreateSmartPointer(smartPointerList, smartPointerArray, isArrayMode);
            }
            break;
        case 20: // Ctrl+T
            CopyPointer(smartPointerList, smartPointerArray, weakPointerList, weakPointerArray, isArrayMode, isWeakMode);
            break;
        case 12: // Ctrl+L
            isArrayMode = false;
            break;
        case 16: // Ctrl+P
            RunTimingTests();
            break;
        case 1: // Ctrl+A
            isArrayMode = true;
            break;
        case 23: // Ctrl+W
            if (isWeakMode) {


                for (size_t i = 0; i < weakPointerArray.GetSize(); ++i) {
                    weakPointerArray.Set(i, WeakPtr<T>()); // Устанавливаем пустые значения
                }
            }
            isWeakMode = !isWeakMode; // Переключение режима
            break;
        case 18: // Ctrl+R
            RunTests();
            break;
        case KEY_UP:
            selected = (selected > 0) ? selected - 1 : selected;
            break;
        case KEY_DOWN:
            if (isArrayMode) {
                selected = (isWeakMode ? selected < weakPointerArray.GetSize() - 1 : selected < smartPointerArray.GetSize() - 1) ? selected + 1 : selected;
            } else {
                selected = (isWeakMode ? selected < weakPointerList.GetSize() - 1 : selected < smartPointerList.GetSize() - 1) ? selected + 1 : selected;
            }
            break;
        default:
            break;
    }
}
#endif /* UI_h */
