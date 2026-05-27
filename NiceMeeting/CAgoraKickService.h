#pragma once

#include <QObject>
#include <QString>

class CAgoraKickService : public QObject
{
	Q_OBJECT

public:
	explicit CAgoraKickService(QObject* parent = nullptr);

	void kickFromChannel(const QString& appId, const QString& channelName, unsigned int uid,
		const QString& customerId, const QString& customerSecret);

signals:
	void kickFinished(bool success, const QString& message);

private:
	void finish(bool success, const QString& message);
};
