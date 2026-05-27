#include "CAgoraKickService.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

CAgoraKickService::CAgoraKickService(QObject* parent)
	: QObject(parent)
{
}

void CAgoraKickService::kickFromChannel(const QString& appId, const QString& channelName,
	unsigned int uid, const QString& customerId, const QString& customerSecret)
{
	if (appId.isEmpty() || channelName.isEmpty() || uid == 0) {
		finish(false, QString::fromUtf8(u8"\u53c2\u6570\u65e0\u6548\uff0c\u65e0\u6cd5\u8e22\u51fa\u7528\u6237\u3002"));
		return;
	}

	auto* manager = new QNetworkAccessManager(this);
	const QUrl url(QStringLiteral("https://api.agora.io/dev/v1/kicking-rule"));
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
	request.setRawHeader("Accept", "application/json");
	const QByteArray auth = (customerId + ':' + customerSecret).toUtf8().toBase64();
	request.setRawHeader("Authorization", "Basic " + auth);

	QJsonObject body;
	body.insert(QStringLiteral("appid"), appId);
	body.insert(QStringLiteral("cname"), channelName);
	body.insert(QStringLiteral("uid"), static_cast<qint64>(uid));
	body.insert(QStringLiteral("time"), 600);
	body.insert(QStringLiteral("time_in_seconds"), 600);
	body.insert(QStringLiteral("privileges"), QJsonArray{ QStringLiteral("join_channel") });

	QNetworkReply* reply = manager->post(request, QJsonDocument(body).toJson(QJsonDocument::Compact));
	connect(reply, &QNetworkReply::finished, this, [this, reply]() {
		const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		const QByteArray bodyBytes = reply->readAll();
		const bool ok = reply->error() == QNetworkReply::NoError && statusCode / 100 == 2;

		QString message;
		if (ok) {
			message = QString::fromUtf8(u8"\u7528\u6237\u5df2\u88ab\u8e22\u51fa\u4f1a\u8bae\u3002");
		} else {
			QString detail;
			const QJsonDocument doc = QJsonDocument::fromJson(bodyBytes);
			if (doc.isObject()) {
				const QJsonObject obj = doc.object();
				detail = obj.value(QStringLiteral("message")).toString();
				if (detail.isEmpty()) {
					detail = obj.value(QStringLiteral("reason")).toString();
				}
			}
			if (detail.isEmpty() && !bodyBytes.isEmpty()) {
				detail = QString::fromUtf8(bodyBytes);
			}
			if (detail.isEmpty()) {
				detail = reply->errorString();
			}
			message = QString::fromUtf8(u8"\u8e22\u51fa\u5931\u8d25\uff1a") + detail;
		}

		reply->deleteLater();
		finish(ok, message);
	});
}

void CAgoraKickService::finish(bool success, const QString& message)
{
	emit kickFinished(success, message);
}
