/**
 * File name: ItemList.h
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

#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <unordered_set>
#include <vector>
#include <functional>
#include <algorithm>
#include <ranges>
#include <optional>

template <typename T>
class ItemList {
public:
    virtual ~ItemList() = default;
    bool addItem(const T& item);
    bool removeItem(const T& item);
    const std::vector<T>& getItems() const;
    bool hasItem(const T& item) const;
    size_t size() const;
    const T& at(size_t index) const;
    const T& operator[](size_t index) const;

private:
    std::vector<T> itemList;
    std::unordered_set<T> itemSet;
};

template <typename T>
bool ItemList<T>::addItem(const T& item)
{
        if (hasItem(item))
                return false;

        itemSet.insert(item);
        itemList.push_back(item);
        return true;
}

template <typename T>
bool ItemList<T>::removeItem(const T& item)
{
        if (itemSet.erase(item) > 0) {
                std::erase(itemList, item);
                return true;
        }
        return false;
}

template <typename T>
const std::vector<T>& ItemList<T>::getItems() const
{
        return itemList;
}

template <typename T>
bool ItemList<T>::hasItem(const T& item) const
{
        return itemSet.find(item) != itemSet.end();
}

template <typename T>
std::size_t ItemList<T>::size() const
{
        return itemList.size();
}

template <typename T>
const T& ItemList<T>::at(std::size_t index) const
{
        return itemList[index];
}

template <typename T>
const T& ItemList<T>::operator[](std::size_t index) const
{
        return itemList[index];
}

#endif // ITEMLIST_H
