/**
 * File name: ItemList.cpp
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "ItemList.h"

template <typename T>
void ItemList<T>::addItem(T& item)
{
        if (itemSet.find(std::ref(item)) == itemSet.end()) {
                itemSet.insert(std::ref(item));
                itemList.push_back(item);
        }
}

template <typename T>
void ItemList<T>::removeItem(const T& item)
{
        if (itemSet.erase(std::ref(item))) {
                auto it = std::find(itemList.begin(), itemList.end(), item);
                if (it != itemList.end())
                        itemList.erase(it);
        }
}

template <typename T>
const std::vector<T>& ItemList<T>::getItems() const
{
        return itemList;
}

template <typename T>
bool ItemList<T>::hasItem(const T& item) const
{
        return std::ranges::contains(itemSet, std::cref(item));
}

template <typename T>
void ItemList<T>::sortItems()
{
        std::ranges::sort(itemList);
}

std::size_t ItemList<T>::size() const
{
        return pathList.size();
}

const T& ItemList<T>::at(std::size_t index)
{
        if (index >= pathList.size())
                return std::nullopt;
        return pathList[index];
}

T& ItemList<T>::at(std::size_t index) const
{
        if (index >= pathList.size())
                return std::nullopt;
        return pathList[index];
}

T& ItemList<T>::operator[](std::size_t index)
{
        return pathList[index];
}

const T& ItemList<T>::operator[](std::size_t index) const
{
        return pathList[index];
}
