/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXDE-Qt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org/
 *
 * Copyright: 2015 LXQt team
 * Authors:
 *   zjesclean@gmail.com
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "app-itemdelegate.h"
#include "app-model.h"
#include "settings.h"
#include <QApplication>
#include <QDebug>
#include <QPainter>

AppItemDelegate::AppItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void AppItemDelegate::init()
{
    m_iconSize     = Settings::instance().iconSize();
    m_maxTextWidth = Settings::instance().maxTextWidth();
}

void AppItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyle* style = option.widget ? option.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &option, painter, option.widget);

    QPixmap img = index.model()->data(index, AppRole::Icon).value<QPixmap>();

    painter->drawPixmap(option.rect.left() + 2, option.rect.top() + 2, m_iconSize, m_iconSize, img);

    QString text = index.model()->data(index, AppRole::Display).toString();
    if (text.length() > m_maxTextWidth) {
        text = text.left(m_maxTextWidth) + "...";
    }

    painter->setFont(option.font);

    painter->drawText(QRectF(option.rect.left() + m_iconSize + 5, option.rect.top() + 2,
                          option.rect.width() - m_iconSize - 5, m_iconSize),
        Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, text);
}

QSize AppItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QFontMetrics m(option.font);
    QString      text = index.model()->data(index, AppRole::Display).toString();

    if (text.length() > m_maxTextWidth) {
        text = text.left(m_maxTextWidth) + "...";
    }

    return QSize(m_iconSize + 10 + m.horizontalAdvance(text), m_iconSize + 4);
}
