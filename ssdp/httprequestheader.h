#include <QObject>

class HttpRequestHeader : public QObject
{
public:
	HttpRequestHeader(const QString &str);
	QString value(const QString &key) const;
	bool hasKey(const QString &key) const;
private:
	bool parse(const QString &str);
	bool parseLine(const QString &line);
	void addValue(const QString &key, const QString &value);

	QList<QPair<QString, QString> > values;
};
