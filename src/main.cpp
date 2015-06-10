/*
* Copyright (C) 2015, Tino Didriksen <mail@tinodidriksen.com>
*
* This file is part of apertium-simpleton
*
* apertium-simpleton is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* apertium-simpleton is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with apertium-simpleton.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "simpleton.hpp"
#include "common.hpp"
#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    app.setOrganizationDomain("tinodidriksen.com");
    app.setOrganizationName("Tino Didriksen Consult");
    app.setApplicationName("Apertium Simpleton");
    app.setQuitOnLastWindowClosed(true);

    QSettings::setDefaultFormat(QSettings::IniFormat);

    Simpleton w;
    w.show();

    QDir appdata(DATALOCATION);
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    if (!appdata.exists("apertium-all-dev/bin") || !appdata.exists("usr/share/apertium/modes")) {
#else
    if (!appdata.exists("usr/share/apertium/modes")) {
#endif
        w.openInstaller();
    }

    return app.exec();
}
