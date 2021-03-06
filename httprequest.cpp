#include <QNetworkRequest>
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
	QNetworkRequest req;
	req.setUrl(QUrl(str));
	req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	nam->post(req, QByteArray());
}

void httpRequest::finishedSlot(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  // bytes
		QString string(QString::fromLocal8Bit(bytes));
		emit result(string);
	}
	else {
		emit result("NetworkError: "+QString::number(reply->error()));
	}
	reply->deleteLater();
}
