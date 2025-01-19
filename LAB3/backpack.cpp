//
// Created by Илья Атмажитов on 17.01.2025.
//


#include "backpack.h"

ostream& operator<<(ostream& os, Item item) {
    os << "{" << item.price << ", " << item.weight << "}";
    return os;
}