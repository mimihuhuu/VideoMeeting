#include "AgoraRestAuth.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QRegularExpression>

AgoraRestCredentials AgoraRestCredentials::loadDefault()
{
	AgoraRestCredentials creds;
	const QStringList candidates = {
		QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("key_and_secret.txt")),
		QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("../key_and_secret.txt")),
		QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("../../key_and_secret.txt")),
	};

	for (const QString& path : candidates) {
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			continue;
		}
		const QString content = QString::fromUtf8(file.readAll());
		const QRegularExpression re(
			QStringLiteral(R"((?i)(customer[_\s-]?id|key)\s*[:：=]\s*(\S+))"));
		const QRegularExpression re2(
			QStringLiteral(R"((?i)(customer[_\s-]?secret|secret)\s*[:：=]\s*(\S+))"));
		const QRegularExpressionMatch m = re.match(content);
		if (m.hasMatch()) {
			creds.customerId = m.captured(2);
		}
		const QRegularExpressionMatch m2 = re2.match(content);
		if (m2.hasMatch()) {
			creds.customerSecret = m2.captured(2);
		}
		if (creds.isValid()) {
			return creds;
		}
	}
	return creds;
}
