#pragma once
#include <QtWidgets\qproxystyle.h>

class InstantMenuStyle : public QProxyStyle {
public:
    using QProxyStyle::QProxyStyle;

    int styleHint(StyleHint hint, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override
    {
        if (hint == QStyle::SH_Menu_SubMenuPopupDelay) {
            return 0; // instant submenu
        }
        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
};