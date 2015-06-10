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

#pragma once
#ifndef INSTALLER_HPP_fc74fd919e58451d89f5d0bba814925a
#define INSTALLER_HPP_fc74fd919e58451d89f5d0bba814925a

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProgressDialog>

namespace Ui {
class Installer;
}

class Installer : public QDialog {
    Q_OBJECT

public:
    explicit Installer(QWidget *parent = 0);
    ~Installer();

public slots:
    void installpkg();
    void dlProgress(qint64, qint64);

private:
    Ui::Installer *ui;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QProgressDialog *wait;
};

#endif // INSTALLER_HPP
