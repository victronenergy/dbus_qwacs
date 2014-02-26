#include "httprequest.h"
#include "QsLog.h"

httpRequest::httpRequest(QObject *parent) :
	QObject(parent)
{
	nam = new QNetworkAccessManager(this);
	QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
}

void httpRequest::getURL(QString str)
{
	nam->get(QNetworkRequest(QUrl(str)));
}

void httpRequest::postURL(QString str)
{
	nam->post(QNetworkRequest(QUrl(str)), QByteArray());
}

void httpRequest::finishedSlot(QNetworkReply* reply)
{
	QVariant statusCodeV =	reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  // bytes
		QString string(QString::fromLocal8Bit(bytes));
		emit result(string);
	}
	else {
		QLOG_ERROR() << "httpRequest::finishedSlot: " << statusCodeV.toString();
	}
	reply->deleteLater();
}
