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
    void addItem(T& item);
    void removeItem(const T& item);
    const std::vector<T>& getItems() const;
    bool hasItem(const T& item) const;
    void sortItems();
    size_t size() const;
    T& at(size_t index);
    const T& at(size_t index) const;
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

private:
    std::unordered_set<std::reference_wrapper<T>> itemSet;
    std::vector<T> itemList;
};

#endif // ITEMLIST_H
