#ifndef BACKLIGHTCTRL_H
#define BACKLIGHTCTRL_H

#include <QTimer>
#include "idlenotifier.h"

struct udev_device;
class BacklightCtrl: public QObject
{
    Q_OBJECT

public:
    BacklightCtrl(IdleNotifier &notifier, QObject *parent = nullptr);
    ~BacklightCtrl();
private slots:
    void on_period_changed(int new_period);
    void on_idle();
    void on_resume();
    void on_transition();
private:
    IdleNotifier &m_notifier;
    IdleNotification *m_notification;
    bool m_idle = false;
    struct udev_device *m_device;
    int m_currentBrightness;
    int m_systemBrightness;
    QTimer m_transitionTimer;
};

#endif // BACKLIGHTCTRL_H
