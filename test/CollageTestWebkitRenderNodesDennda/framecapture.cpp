/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "framecapture.h"

#include <iostream>
#include <QtWebKit>

FrameCapture::FrameCapture(): QObject(), m_percent(0)
{
    connect(&m_page, SIGNAL(loadProgress(int)), this, SLOT(printProgress(int)));
    connect(&m_page, SIGNAL(loadFinished(bool)), this, SLOT(saveResult(bool)));
}

void FrameCapture::load(const QUrl &url, const QString &outputFileName)
{
    std::cout << "Loading " << qPrintable(url.toString()) << std::endl;
    m_percent = 0;
    int index = outputFileName.lastIndexOf('.');
    m_fileName = (index == -1) ? outputFileName + ".png" : outputFileName;
    m_page.mainFrame()->load(url);
    m_page.mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    m_page.mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    m_page.setViewportSize(QSize(1024, 768));
}

void FrameCapture::printProgress(int percent)
{
    if (m_percent >= percent)
        return;

    while (m_percent++ < percent)
        std::cout << "#" << std::flush;
}

void FrameCapture::saveResult(bool ok)
{
    std::cout << std::endl;
    qDebug() << "bla";

    // crude error-checking
    if (!ok) {
        std::cerr << "Failed loading " << qPrintable(m_page.mainFrame()->url().toString()) << std::endl;
        emit finished();
        return;
    }

    // save each frame in different image files
    saveFrame(m_page.mainFrame());

    emit finished();
}

inline int getColorimetricDistance(const QRgb &x, const QRgb &y) {
    // Returns squared distance between two colors.
    // Actually this shouldn't use RGB colorspace (for a proper 'distance')
    return pow(qRed(x) - qRed(y), 2.) +
           pow(qGreen(x) - qGreen(y), 2.) +
           pow(qBlue(x) - qBlue(y), 2.);
}

std::map<QRgb, int>
findDominantColors(const QImage &image, int max_dist, int min_cov)
{
    QRgb rgb, old;
    std::map<QRgb, int> colors, result;
    std::map<QRgb, int>::iterator it;
    int oldcount;
    double total = image.width() * image.height();
    bool merge;
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            merge = false;
            rgb = image.pixel(i, j);
            for (it = colors.begin(); it != colors.end(); it++) {
                old = (*it).first;
                oldcount = (*it).second;
                if (getColorimetricDistance(rgb, old) < pow(max_dist, 2.)) {
                    // This color is similar enough to a color we already
                    // have. Merge them afterwards.

                    // It's ok, let's just break here. Don't find closest.
                    merge = true;
                    break;
                }
            }
            if (merge) {
                QRgb first = qRgb((qRed(rgb) + qRed(old)) / 2,
                                  (qGreen(rgb) + qGreen(old)) / 2,
                                  (qBlue(rgb) + qBlue(old)) / 2);
                int second = oldcount++;
                colors.erase(it);
                colors.insert(std::pair<QRgb, int>(first, second));
            }
            else {
                colors.insert(std::pair<QRgb, int>(rgb, 1));
            }
        }
    }
    double cov = 0.;
    for (it = colors.begin(); it != colors.end(); it++) {
        cov = (*it).second / total;
        qDebug() << qRed((*it).first) << qGreen((*it).first) << qBlue((*it).first) << "covers" << cov * 100 << "%";
        if (min_cov < cov) {
            result.insert(std::pair<QRgb, int>((*it).first, cov));
        }
    }
    return result;
}

QRgb findDominantColor(const QImage &image) {
    QRgb rgb;
    int r = 0, g = 0, b = 0, total = image.width() * image.height();
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            rgb = image.pixel(i, j);
            r += qRed(rgb);
            g += qGreen(rgb);
            b += qBlue(rgb);
        }
    }
    qDebug() << "color: " << r/total << " " << g/total << " " << b/total;
    return qRgb(r/total, g/total, b/total);
}

void FrameCapture::saveFrame(QWebFrame *frame)
{
    qint64 start = QDateTime::currentMSecsSinceEpoch();
    static int frameCounter = 0;

    QWebElement document = frame->documentElement();
    QWebElementCollection paragraphs = document.findAll("body");
    qDebug() << paragraphs.count();

    foreach(QWebElement p, paragraphs) {
        qDebug() << p.toPlainText();
        qDebug() << p.geometry();
        QString fileName(m_fileName);
        if (frameCounter) {
            int index = m_fileName.lastIndexOf('.');
            fileName = fileName.insert(index, "_frame" + QString::number(frameCounter));
        }

        QImage image(p.geometry().size(), QImage::Format_ARGB32_Premultiplied);
        qDebug() << p.geometry().size();
        image.fill(Qt::transparent);

        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        p.render(&painter);
        painter.end();

        image.save(fileName);
        findDominantColor(image);
        ++frameCounter;
        break;
    }
    qint64 end = QDateTime::currentMSecsSinceEpoch();
    qDebug() << "Painting took " << (end - start) / 1000 << "secs";
}


