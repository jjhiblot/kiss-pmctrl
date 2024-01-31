#include <QDebug>
#include <QSettings>
#include "suspendctrl.h"

SuspendCtrl::SuspendCtrl(IdleNotifier &notifier, QObject *parent): QObject(parent)
  ,m_notifier(notifier)
{
    QSettings settings("viavi", "settings");
    settings.setFallbacksEnabled(true);
    int period = settings.value("pm/suspend", 10).toInt() * 1000;

    m_notification = nullptr;
    emit this->on_period_changed(period);
}

void SuspendCtrl::on_period_changed(int new_period)
{
    if (m_notification) {
        delete m_notification;
        m_notification = nullptr;
    }
    if (new_period) {
        m_notification = m_notifier.create_notification(new_period);
        connect(m_notification, &IdleNotification::idled, this, &SuspendCtrl::on_idle);
        connect(m_notification, &IdleNotification::resumed, this, &SuspendCtrl::on_resume);
    }
}

void SuspendCtrl::on_idle()
{
    if (m_idle)
        return;

    qDebug() << "suspending";
    m_idle = true;
    ::system("systemctl suspend");
}

void SuspendCtrl::on_resume()
{
    if (!m_idle)
        return;

    m_idle = false;
    qDebug() << "out of suspend";

}
