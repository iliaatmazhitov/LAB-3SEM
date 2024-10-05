//
//  main.cpp
//  laboratory1
//
//  Created by Илья Атмажитов on 11.09.2024.
//


#include "UI.h"

int main() {
    initialize();

    LinkedList<SmrtPtr<int> > smartPointerList;
    DynamicArray<SmrtPtr<int> > smartPointerArray;
    int selected = 0;
    bool isArrayMode = true;

    while (true) {
        draw_ui(smartPointerList, smartPointerArray, isArrayMode, selected);
        int ch = getch();
        handle_keypress(ch, smartPointerList, smartPointerArray, isArrayMode, selected);
    }

    endwin();
    return 0;
    

}
