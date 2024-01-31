#ifndef SUSPENDCTRL_H
#define SUSPENDCTRL_H

#include <QObject>
#include "idlenotifier.h"

class SuspendCtrl : public QObject
{
    Q_OBJECT
public:
    explicit SuspendCtrl(IdleNotifier &notifier, QObject *parent = nullptr);

private slots:
    void on_period_changed(int new_period);
    void on_idle();
    void on_resume();

private:
    IdleNotifier &m_notifier;
    IdleNotification *m_notification;
    bool m_idle = false;
};

#endif // SUSPENDCTRL_H
