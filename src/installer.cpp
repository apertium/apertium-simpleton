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

#include "installer.hpp"
#include "ui_installer.h"
#include "common.hpp"
#include <QtGui>
#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <cstdlib>

inline QString formatBytes(double val) {
    const char *suf = "B";
    if (val >= 1024) {
        val /= 1024;
        suf = "KiB";
    }
    if (val >= 1024) {
        val /= 1024;
        suf = "MiB";
    }
    return QString("%1 %2").arg(val, 0, 'f', 2).arg(suf);
}

Installer::Installer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Installer)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    ui->setupUi(this);

    QProgressDialog wait(this);
    wait.setLabelText("Checking for new Required Core Tools ...");
    wait.show();

    if (qnam.networkAccessible() != QNetworkAccessManager::Accessible) {
        wait.hide();
        QMessageBox::critical(this, "Network Inaccessible", "Can't check for updates as you appear to be offline.");
        return;
    }

    QSettings conf;
    QWidget *w = new QWidget;
    QGridLayout *g = new QGridLayout;
    QPushButton *b = 0;

#if defined(Q_OS_WIN)
    #define OS_PATH "win32"
#elif defined(Q_OS_MAC)
    #define OS_PATH "osx"
#elif defined(Q_OS_LINUX)
    #define OS_PATH "osx"
#else
    #error "Not yet specialized for other OSs"
#endif

    int row = 0;
    g->addWidget(new QLabel("Name"), row, 0, 1, 1, Qt::AlignLeft);
    g->addWidget(new QLabel("Size"), row, 1, 1, 1, Qt::AlignHCenter);
    g->addWidget(new QLabel("Action"), row, 2, 1, 1, Qt::AlignRight);

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    {
        reply = qnam.head(QNetworkRequest(QUrl("http://apertium.projectjj.com/" OS_PATH "/nightly/apertium-all-dev.7z")));
        QEventLoop loop;
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        QString lm = reply->rawHeader("Last-Modified");
        QString cl = reply->rawHeader("Content-Length");
        QString label("Install");
        if (conf.contains("apertium-all-dev")) {
            label = "Reinstall";
            if (conf.value("apertium-all-dev").toString() != lm) {
                label = "Update";
            }
        }

        ++row;
        g->addWidget(new QLabel("Required Core Tools"), row, 0, 1, 1, Qt::AlignLeft);
        g->addWidget(new QLabel(formatBytes(cl.toUInt())), row, 1, 1, 1, Qt::AlignRight);
        g->addWidget(b = new QPushButton(label), row, 2, 1, 1, Qt::AlignRight);
        b->setProperty("name", "apertium-all-dev");
        b->setProperty("url", QString("http://apertium.projectjj.com/" OS_PATH "/nightly/apertium-all-dev.7z"));
        b->setProperty("bsize", cl);
        b->setProperty("lm", lm);
        connect(b, SIGNAL(clicked()), this, SLOT(installpkg()));
    }
#endif

    {
        wait.setLabelText("Checking for new language pairs ...");
        reply = qnam.get(QNetworkRequest(QUrl("http://apertium.projectjj.com/" OS_PATH "/nightly/data.php")));
        QEventLoop loop;
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        QString body = reply->readAll();
        QRegularExpression rx("<tr><td>(apertium-\\w+-\\w+)</td><td>[^<]+</td><td>(\\d+)</td><td>([^<]+)</td>.*?</tr>");
        QRegularExpressionMatchIterator it = rx.globalMatch(body);
        while (it.hasNext()) {
            QRegularExpressionMatch m = it.next();

            QString name = m.captured(1);
            QString label("Install");
            if (conf.contains(name)) {
                label = "Reinstall";
                if (conf.value(name).toString() != m.captured(3)) {
                    label = "Update";
                }
            }

            ++row;
            g->addWidget(new QLabel(name), row, 0, 1, 1, Qt::AlignLeft);
            g->addWidget(new QLabel(formatBytes(m.captured(2).toUInt())), row, 1, 1, 1, Qt::AlignRight);
            g->addWidget(b = new QPushButton(label), row, 2, 1, 1, Qt::AlignRight);
            b->setProperty("name", name);
            b->setProperty("url", QString("http://apertium.projectjj.com/" OS_PATH "/nightly/data.php?deb=")+name);
            b->setProperty("bsize", m.captured(2));
            b->setProperty("lm", m.captured(3));
            connect(b, SIGNAL(clicked()), this, SLOT(installpkg()));
        }
    }

    w->setLayout(g);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setWidget(w);
}

Installer::~Installer() {
    delete ui;
}

void Installer::installpkg() {
    QString url = sender()->property("url").toString();
    uint bsize = sender()->property("bsize").toUInt();

    wait = new QProgressDialog(this);
    wait->setModal(true);
    wait->setMaximum(bsize+1);
    wait->setLabelText(QString("Downloading ") + url + " ...");
    wait->show();

    reply = qnam.get(QNetworkRequest(QUrl(url)));
    reply->setReadBufferSize(bsize*2);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(dlProgress(qint64,qint64)));
    loop.exec();

    QString appdata_path = DATALOCATION;
    QDir().mkpath(appdata_path);
    QDir appdata(appdata_path);
    appdata.remove("data.tmp");
    appdata.remove("data.tar");
    appdata.remove("data.tar.xz");
    appdata.remove("debian-binary");
    appdata.remove("control.tar.gz");

    QFile file(appdata.filePath("data.tmp"));
    file.open(QIODevice::WriteOnly);
    file.write(reply->readAll());
    file.close();

    QDir exep(QCoreApplication::applicationDirPath());
    wait->setLabelText("Unpacking ...");
    QProcess *up = new QProcess(this);
    up->setWorkingDirectory(appdata_path);
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    up->start(exep.absoluteFilePath("7z"), QStringList() << "x" << "-y" << "data.tmp");
#else
    up->start("ar", QStringList() << "x" << "data.tmp");
#endif
    up->waitForStarted();
    up->waitForFinished();
    up->deleteLater();

    if (appdata.exists("data.tar")) {
        QProcess *up = new QProcess(this);
        up->setWorkingDirectory(appdata_path);
        up->start(exep.absoluteFilePath("7z"), QStringList() << "x" << "-y" << "data.tar");
        up->waitForStarted();
        up->waitForFinished();
        up->deleteLater();
    }

    if (appdata.exists("data.tar.xz")) {
        QProcess *up = new QProcess(this);
        up->setWorkingDirectory(appdata_path);
        up->start("tar", QStringList() << "-Jxf" << "data.tar.xz");
        up->waitForStarted();
        up->waitForFinished();
        up->deleteLater();
    }

    wait->close();
    wait->deleteLater();
    appdata.remove("data.tmp");
    appdata.remove("data.tar");
    appdata.remove("data.tar.xz");
    appdata.remove("debian-binary");
    appdata.remove("control.tar.gz");

    QSettings conf;
    conf.setValue(sender()->property("name").toString(), sender()->property("lm").toString());
}

void Installer::dlProgress(qint64 value, qint64) {
    wait->setValue(value);
}
