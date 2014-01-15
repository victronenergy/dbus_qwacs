#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

class httpRequest : public QObject
{
	Q_OBJECT

public:
	httpRequest(QObject *parent = 0);

	void getURL(QString str);

signals:
	void result(QString str);

private slots:
	void finishedSlot(QNetworkReply* reply);

private:
	QNetworkAccessManager* nam;
};

#endif // HTTPREQUEST_Hpublic QObject
