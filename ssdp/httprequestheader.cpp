#include <QString>
#include <QStringList>
#include <QLatin1String>
#include <QLatin1Char>
#include <QPair>

#include "httprequestheader.h"

HttpRequestHeader::HttpRequestHeader(const QString &str)
{
	parse(str);
}

bool HttpRequestHeader::parse(const QString &str)
{
    QStringList lst;
    int pos = str.indexOf(QLatin1Char('\n'));
    if (pos > 0 && str.at(pos - 1) == QLatin1Char('\r'))
        lst = str.trimmed().split(QLatin1String("\r\n"));
    else
        lst = str.trimmed().split(QLatin1String("\n"));
    lst.removeAll(QString()); // No empties

    if (lst.isEmpty())
        return true;

    QStringList lines;
    QStringList::Iterator it = lst.begin();
    for (; it != lst.end(); ++it) {
        if (!(*it).isEmpty()) {
            if ((*it)[0].isSpace()) {
                if (!lines.isEmpty()) {
                    lines.last() += QLatin1Char(' ');
                    lines.last() += (*it).trimmed();
                }
            } else {
                lines.append((*it));
            }
        }
    }

    it = lines.begin();
    for (; it != lines.end(); ++it) {
		/* skip the first line, we're interested in what comes after */
		if (it == lines.begin()) continue;
        if (!parseLine(*it)) {
            return false;
        }
    }
    return true;
}

/*!
    Returns the first value for the entry with the given \a key. If no entry
    has this \a key, an empty string is returned.
*/
QString HttpRequestHeader::value(const QString &key) const
{
    QString lowercaseKey = key.toLower();
    QList<QPair<QString, QString> >::ConstIterator it = values.constBegin();
    while (it != values.constEnd()) {
        if ((*it).first.toLower() == lowercaseKey)
            return (*it).second;
        ++it;
    }
    return QString();
}

/*!
    Returns true if the HTTP header has an entry with the given \a
    key; otherwise returns false.
*/
bool HttpRequestHeader::hasKey(const QString &key) const
{
    QString lowercaseKey = key.toLower();
    QList<QPair<QString, QString> >::ConstIterator it = values.constBegin();
    while (it != values.constEnd()) {
        if ((*it).first.toLower() == lowercaseKey)
            return true;
        ++it;
    }
    return false;
}

/*!
    Adds a new entry with the \a key and \a value.
*/
void HttpRequestHeader::addValue(const QString &key, const QString &value)
{
    values.append(qMakePair(key, value));
}

bool HttpRequestHeader::parseLine(const QString &line)
{
    int i = line.indexOf(QLatin1Char(':'));
    if (i == -1)
        return false;

    addValue(line.left(i).trimmed(), line.mid(i + 1).trimmed());

    return true;
}
