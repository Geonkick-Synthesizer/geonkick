/**
 * File name: percussion_model.cpp
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
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

#include "percussion_model.h"

// void KitModel::increasePercussionChannel(PercussionIndex index)
// {
//         auto id = getPercussionId(index);
//         auto channel = geonkickApi->getPercussionChannel(id);
//         if (channel < 0)
//                 return;

//         if (++channel > static_cast<decltype(channel)>(geonkickApi->numberOfChannels() - 1))
//                 channel = 0;
//         if (geonkickApi->setPercussionChannel(id, channel))
//                 action modelUpdated();
// }

// void KitModel::decreasePercussionChannel(PercussionIndex index)
// {
//         auto id = getPercussionId(index);
//         auto channel = geonkickApi->getPercussionChannel(id);
//         if (channel < 0)
//                 return;

//         if (channel - 1 < 0)
//                 channel = geonkickApi->numberOfChannels() - 1;
//         else
//                 channel--;
//         if (geonkickApi->setPercussionChannel(id, channel))
//                 action modelUpdated();
// }

// void KitModel::moveSelectedPercussion(bool down)
// {
//         if (geonkickApi->moveOrdrepedPercussionId(geonkickApi->currentPercussion(), down ? 1 : -1))
//                 action modelUpdated();
// }

// void KitModel::setPercussionKey(PercussionIndex index, KeyIndex keyIndex)
// {
//         auto id = getPercussionId(index);
//         if (id > -1) {
//                 auto refKey = geonkickApi->percussionsReferenceKey();
//                 auto key = refKey + keyIndex;
//                 auto res = geonkickApi->setPercussionPlayingKey(id, key);
//                 if (res)
//                         action modelUpdated();
//         }
// }

// int KitModel::percussionKeyIndex(PercussionIndex index) const
// {
//         auto id = getPercussionId(index);
//         if (id < 0)
//                 return keysNumber() - 1;

//         int keyIndex = geonkickApi->getPercussionPlayingKey(id);
//         if (keyIndex < 0)
//                 return keysNumber() - 1;
//         keyIndex -= geonkickApi->percussionsReferenceKey();
//         if (keyIndex < 0 || keyIndex > static_cast<decltype(keyIndex)>(keysNumber() - 1))
//                 return keysNumber() - 1;
//         else
//                 return keyIndex;
// }

// void KitModel::setPercussionName(PercussionIndex index, const std::string &name)
// {
//         auto res = geonkickApi->setPercussionName(getPercussionId(index), name);
//         if (res)
// //                 geonkickApi->notifyUpdateGui();
// // }

// std::string KitModel::percussionName(PercussionIndex index) const
// {
//          return geonkickApi->getPercussionName(getPercussionId(index));
// }

// int KitModel::percussionChannel(PercussionIndex index) const
// {
//         return geonkickApi->getPercussionChannel(getPercussionId(index));
// }

// bool KitModel::canCopy() const
// {
//         auto n = geonkickApi->ordredPercussionIds().size();
//         if (n > 0 && n < geonkickApi->getPercussionsNumber())
//                 return true;
//         return false;
// }

// bool KitModel::canRemove() const
// {
//         if (geonkickApi->ordredPercussionIds().size() > 1)
//                 return true;
//         return false;
// }

// void KitModel::playPercussion(PercussionIndex index)
// {
//         geonkickApi->playKick(getPercussionId(index));
// }

// void KitModel::selectPercussion(PercussionIndex index)
// {
//         auto id = getPercussionId(index);
//         if (id > -1 && id != static_cast<decltype(id)>(geonkickApi->currentPercussion())) {
//                 if (geonkickApi->setCurrentPercussion(id))
//                         geonkickApi->notifyUpdateGui();
//         }
// }

// bool KitModel::percussionSelected(PercussionIndex index) const
// {
//         auto id = getPercussionId(index);
//         return static_cast<decltype(id)>(geonkickApi->currentPercussion()) == id;
// }
