/**
 * File name: AbstractView.h
 * Project: Geonkick (A percussive synthesizer)
 *
 * Copyright (C) 2024 Iurie Nistor
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

#ifndef ABSTRACT_VIEW_H
#define ABSTRACT_VIEW_H

#include "geonkick_widget.h"

class AbstractModel;

class AbstractView: public GeonkickWidget {
 public:
        explicit AbstractView(GeonkickWidget* parent, AbstractModel *model);
        void setModel(AbstractModel *model);
        AbstractModel* getModel() const;
        virtual void createView() = 0;
        virtual void updateView() = 0;

 protected:
        virtual void bindModel() = 0;
        virtual void unbindModel() = 0;

 private:
        AbstractModel* viewModel;
};

#endif // ABSTRACT_VIEW_H
