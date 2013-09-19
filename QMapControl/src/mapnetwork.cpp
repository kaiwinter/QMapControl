/*
*
* This file is part of QMapControl,
* an open-source cross-platform map widget
*
* Copyright (C) 2007 - 2008 Kai Winter
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with QMapControl. If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: kaiwinter@gmx.de
* Program URL   : http://qmapcontrol.sourceforge.net/
*
*/

#include "mapnetwork.h"
#include <QWaitCondition>

#include <QMutexLocker>

namespace qmapcontrol
{
    MapNetwork::MapNetwork(ImageManager* parent)
        :parent(parent), http(new QHttp(this)), loaded(0)
    {
        connect(this->http, SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)),
                this, SLOT(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)));

        connect(this->http, SIGNAL(requestFinished(int, bool)),
                this, SLOT(requestFinished(int, bool)));
    }

    MapNetwork::~MapNetwork()
    {
        http->clearPendingRequests();
        delete http;
    }


    void MapNetwork::loadImage(const QString& host, const QString& url)
    {
        // qDebug() << "getting: " << QString(host).append(url);
        // http->setHost(host);
        // int getId = http->get(url);

        http->setHost(host);
        QHttpRequestHeader header("GET", url);
        header.setValue("User-Agent", "Mozilla");
        header.setValue("Host", host);
        int getId = http->request(header);

        QMutexLocker lock(&vectorMutex);
        loadingMap[getId] = url;
    }

    void MapNetwork::requestFinished(int id, bool error)
    {
        // sleep(1);
        // qDebug() << "MapNetwork::requestFinished" << http->state() << ", id: " << id;
        if (error)
        {
            qDebug() << "network error: " << http->errorString();
            //restart query

        }
        else
        {

            QString url;
            {
                QMutexLocker lock(&vectorMutex);
                // check if id is in map?
                if (loadingMap.contains(id))
                {
                    url = loadingMap[id];
                    loadingMap.remove(id);
                }
            }

            if(!url.isEmpty())
            {
                // qDebug() << "request finished for id: " << id << ", belongs to: " << notifier.url << endl;
                QByteArray ax;

                if (http->bytesAvailable()>0)
                {
                    QPixmap pm;
                    ax = http->readAll();

                    if (pm.loadFromData(ax))
                    {
                        loaded += pm.size().width()*pm.size().height()*pm.depth()/8/1024;
                        // qDebug() << "Network loaded: " << (loaded);
                        parent->receivedImage(pm, url);
                    }
                    else
                    {
                        qDebug() << "NETWORK_PIXMAP_ERROR: " << ax;
                    }
                }
            }

        }

        if (loadQueueSize() == 0)
        {
            // qDebug () << "all loaded";
            parent->loadingQueueEmpty();
    }

    int MapNetwork::loadQueueSize() const
    {
        QMutexLocker lock(&vectorMutex);
        return loadingMap.size();
        }
    }

    void MapNetwork::abortLoading()
    {
        http->clearPendingRequests();

        QMutexLocker lock(&vectorMutex);
        loadingMap.clear();
    }

    bool MapNetwork::imageIsLoading(QString url)
    {
        QMutexLocker lock(&vectorMutex);
        return loadingMap.values().contains(url);
    }

    }

    int MapNetwork::loadQueueSize() const
    {
        QMutexLocker lock(&vectorMutex);
        return loadingMap.size();
    void MapNetwork::setProxy(QString host, int port)
    {
#ifndef Q_WS_QWS
        // do not set proxy on qt/extended
        http->setProxy(host, port);
#endif
    }

    void MapNetwork::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
    {
        qDebug() << "Proxy Aut req" << proxy.hostName() << &authenticator;
        QDialog dialog;
        QGridLayout layout;
        QLabel username, password;
        username.setText("Username:");
        password.setText("Password:");
        layout.addWidget(&username, 0, 0);
        layout.addWidget(&password, 1, 0);
        QLineEdit user, pass;
        pass.setEchoMode(QLineEdit::Password);
        connect(&user, SIGNAL(returnPressed()), &dialog, SLOT(accept()));
        connect(&pass, SIGNAL(returnPressed()), &dialog, SLOT(accept()));
        layout.addWidget(&user, 0, 1);
        layout.addWidget(&pass, 1, 1);
        QPushButton button;
        button.setText("OK");
        connect(&button, SIGNAL(clicked()), &dialog, SLOT(accept()));
        layout.addWidget(&button, 2, 0, 1, 2, Qt::AlignCenter);
        dialog.setLayout(&layout);
        dialog.exec();
        authenticator->setUser(user.text());
        authenticator->setPassword(pass.text());
    }
}
