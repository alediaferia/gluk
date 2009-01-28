/***************************************************************************
 *   Copyright (C) 2009 Alessandro Diaferia <alediaferia@gmail.com>        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/
#include "ebuild.h"
#include <QTextStream>
#include <QUrl>
#include <QFileInfo>
#include <QRegExp>
#include <QStringList>

#include <KDebug>

Ebuild::Ebuild(const QString &name, QObject *parent) : QFile(name, parent)
{}

Ebuild::~Ebuild()
{}

QString Ebuild::packageName()
{
    QString name = QFileInfo(fileName()).completeBaseName();
    // TODO: ask f0x
    kDebug() << name;
    name.remove(QRegExp("\\-[0-9].*."));
    return name;
}

QStringList Ebuild::useFlags()
{
    QString flags = value("IUSE");
    return flags.split(" ", QString::SkipEmptyParts);
}

QString Ebuild::description()
{
    return value("DESCRIPTION");
}

QString Ebuild::license()
{
    return value("LICENSE");
}

QUrl Ebuild::sourceUrl()
{
    QString src = value("SRC_URI");
    src = expandVars(src);

    return QUrl(src);
}

QUrl Ebuild::homePage()
{
    return QUrl(value("HOMEPAGE"));
}

QStringList Ebuild::keywords()
{
    return value("KEYWORDS").split(" ", QString::SkipEmptyParts);
}

QString Ebuild::value(const QString &key)
{
    if (!open(QIODevice::ReadOnly | QIODevice::Text)) {
        kDebug() << "unable to open";
        return QString();
    }

    QString result;
    
    QTextStream stream(this);

    bool keyFound = false;
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (!line.startsWith(key)) {
            continue;
        } else {
            keyFound = true;
            line.remove(key + "=\"");
        }

        if (keyFound) {
            if (line.contains("\"")) {
                line.remove("\"");
                result.append(line);
                break;
            }
            result.append(line);
        }
    }

    close();

    return result;
}

QString Ebuild::expandVars(const QString &src)
{
    QString source = src;

    QFileInfo info(fileName());

    const QString name = info.fileName();
    QString PN = packageName();
//     const QString P = QString(name).remove(QRegExp(".ebuild$"));
//     const QString PV = QString(name).section(QRegExp("\\-[0-9].*\\."), 0).rightRef(1).toString().leftRef(1);

    source.replace("{PN}", PN);
//     source.replace("{P}", P);
//     source.replace("{PV}", PV);

    return source;
}
