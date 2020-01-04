        auto oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator1)].get();
        auto widget = new OscillatorGroupBox(this, oscillator);
        widget->setPosition(0, 0);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), widget, updateGui());
        widget->show();

        oscillator = oscillators[static_cast<int>(Oscillator::Type::Oscillator2)].get();
        widget = new OscillatorGroupBox(this, oscillator);
        widget->setPosition(8 + 224, 0);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), widget, updateGui());
        widget->show();

        oscillator = oscillators[static_cast<int>(Oscillator::Type::Noise)].get();
        widget = new OscillatorGroupBox(this, oscillator);
        widget->setPosition(2 * (8 + 224), 0);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), widget, updateGui());
        widget->show();

        auto generalWidget = new GeneralGroupBox(this, api);
        generalWidget->setPosition(3 * (8 + 224), 0);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), generalWidget, updateGui());
        generalWidget->show();

        auto effectsWidget = new EffectsGroupBox(api, this);
        effectsWidget->setFixedSize(500, 82);
        effectsWidget->setPosition(2 * (8 + 224), 285);
        RK_ACT_BIND(this, updateGui, RK_ACT_ARGS(), effectsWidget, updateGui()); 
        effectsWidget->show();
