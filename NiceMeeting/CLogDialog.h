#pragma once

#include <QtWidgets/QDialog>
#include "ui_CLogDialog.h"

class CLogDialog : public QDialog
{
    Q_OBJECT

public:
    CLogDialog(QWidget *parent = nullptr);
    ~CLogDialog();

    QString getRoomID() const;
    bool isCameraEnabled() const;
    bool isMicEnabled() const;

private:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QPoint mouse_pos;
    QPoint window_pos;
    QPoint diff_pos;

private slots:
	void on_btnJion_clicked();
	void on_btnMin_clicked();
	void on_btnClose_clicked();

private:
    Ui::CLogDialogClass ui;
};

