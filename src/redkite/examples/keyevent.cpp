/**
 * File name: painter.cpp
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
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

#include "RkMain.h"
#include "RkWidget.h"
#include "RkLog.h"
#include "RkEvent.h"
#include "RkEventQueue.h"

class KeyExampleWidget: public RkWidget {
  public:
        KeyExampleWidget(RkMain *app) : RkWidget(app) {}

  protected:
        void keyPressEvent(const std::shared_ptr<RkKeyEvent> &event) final
        {
                int diff = event->modifiers() & static_cast<int>(Rk::KeyModifiers::Shift) ? 0x61 - 0x41 : 0;
                RK_LOG_DEBUG("key: " << static_cast<unsigned char>(static_cast<int>(event->key()) + diff)
                             << "[0x" << std::hex << static_cast<int>(event->key()) + diff << "]");
        }
};

int main(int arc, char **argv)
{
    RkMain app(arc, argv);

    auto widget = new KeyExampleWidget(&app);
    widget->setTitle("Key Example Widget");
    widget->setSize(350, 350);
    widget->show();

    return app.exec();
}
