#pragma once

#include <QString>

struct AgoraRestCredentials
{
	QString customerId;
	QString customerSecret;

	bool isValid() const { return !customerId.isEmpty() && !customerSecret.isEmpty(); }
	static AgoraRestCredentials loadDefault();
};
