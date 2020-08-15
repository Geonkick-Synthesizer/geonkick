/**
 * File name: ViewState.h
 * Project: Geonkick (A kick synthesizer)
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

#ifndef GEONGKICK_VIEW_STATE_H
#define GEONGKICK_VIEW_STATE_H

#include "globals.h"

#include <RkObject.h>

class ViewState: public RkObject {
 public:
        enum class Type : int {
                Controls = 0,
                Kit      = 1,
                Presets  = 2,
                Samples  = 3
        };

        ViewState(RkObject *parent, ViewState::Type state = ViewState::Type::Controls);
        ~ViewState() = default;
        RK_DECL_ACT(stateChanged,
                    stateChanged(ViewState::Type state),
                    RK_ARG_TYPE(ViewState::Type),
                    RK_ARG_VAL(state));
        ViewState::Type state() const;
        void setState(ViewState::Type state);

 private:
        Type viewState;
};

#endif // GEONGKICK_VIEW_STATE_H
