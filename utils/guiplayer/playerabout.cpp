/*
    Virtual Piano test using the MIDI Sequencer C++ library 
    Copyright (C) 2006-2021, Pedro Lopez-Cabanillas <plcl@users.sf.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "playerabout.h"

About::About(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    QString aboutText = ui.AboutTextView->toHtml();
    aboutText.replace("%VERSION%", QStringLiteral(QT_STRINGIFY(VERSION)));
    aboutText.replace("%QT_VERSION%", qVersion());
    ui.AboutTextView->setHtml(aboutText);
    connect(ui.aboutQt, &QPushButton::clicked, qApp, QApplication::aboutQt);
}
