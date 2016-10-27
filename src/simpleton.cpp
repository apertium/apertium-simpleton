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
#include "ui_simpleton.h"
#include "installer.hpp"
#include "common.hpp"

Simpleton::Simpleton(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Simpleton)
{
    ui->setupUi(this);

    enumModes();
}

Simpleton::~Simpleton() {
    delete ui;
}

void Simpleton::openInstaller() {
    Installer *in = new Installer(this);
    in->setModal(true);
    in->show();
    connect(in, SIGNAL(destroyed()), this, SLOT(enumModes()));
}

void Simpleton::enumModes() {
    QDir appdata(DATALOCATION);
    if (!appdata.exists("usr/share/apertium/modes")) {
        return;
    }

    ui->listModes->clear();
    QDir moded(appdata.absoluteFilePath("usr/share/apertium/modes"));
    QFileInfoList modes = moded.entryInfoList(QStringList() << "*.mode");
    foreach (QFileInfo mode, modes) {
        ui->listModes->addItem(mode.baseName());
    }
    ui->listModes->adjustSize();
}

void Simpleton::runMode() {
    QString name = ui->listModes->currentText();
    if (name.isEmpty()) {
        return;
    }

    QDir appdata(DATALOCATION);
    QDir dir(appdata.absoluteFilePath("usr/share/apertium/modes"));
    if (!dir.exists() || !dir.exists(name+".mode")) {
        return;
    }

    QFile file(dir.absoluteFilePath(name+".mode"));
    if (file.open(QIODevice::ReadOnly) == false) {
        return;
    }
    QString mode = file.readAll();
    file.close();

    mode = mode.trimmed();
    if (mode.isEmpty()) {
        return;
    }

    mode.replace("$1", "-g");
    mode.remove("$2");
    if (mode.indexOf("'/usr/share") == -1) {
        mode.replace(QRegularExpression("(\\s*)(/usr/share/\\S+)(\\s*)"), "\\1\"\\2\"\\3");
    }
    mode.replace("/usr/share", appdata.absolutePath()+"/usr/share");

#ifdef Q_OS_WIN
    // Windows can't handle C:/ paths in ' quotes
    mode.replace("'", "\"");
	#define OS_SEP ";"
#else
	#define OS_SEP ":"
#endif

    mode.prepend("apertium-destxt | ");
    mode.append(" | apertium-retxt");

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PATH", appdata.absoluteFilePath("apertium-all-dev/bin") + OS_SEP + env.value("PATH"));
    env.insert("LC_ALL", "en_US.UTF-8");

    QProcess *run = new QProcess(this);
    run->setProcessEnvironment(env);
    run->setProcessChannelMode(QProcess::MergedChannels);
#ifdef Q_OS_WIN
    run->setNativeArguments(mode);
    run->start("cmd", QStringList() << "/D" << "/Q" << "/S" << "/C");
#else
    run->start("/bin/sh", QStringList() << "-c" << mode);
#endif
    run->waitForStarted();
    run->write(ui->textInput->toPlainText().toUtf8());
    run->closeWriteChannel();
    run->waitForFinished();
    ui->textOutput->setPlainText(QString::fromUtf8(run->readAll()));
    run->deleteLater();
}
