#include <QDebug>
#include <QSettings>
#include <libudev.h>
#include "backlightctrl.h"

BacklightCtrl::BacklightCtrl(IdleNotifier &notifier, QObject *parent): QObject(parent)
  ,m_notifier(notifier)
{
    QSettings settings("viavi", "settings");
    settings.setFallbacksEnabled(true);
    int period = settings.value("pm/backlight_off", 2).toInt() * 1000;
    QString devname = settings.value("Hardware/backlight", "primary").toString();
    auto instance = udev_new();

    m_device = udev_device_new_from_syspath(instance, QString("/sys/class/backlight/" + devname).toStdString().c_str());

    m_notification = nullptr;
    emit this->on_period_changed(period);
    connect(&m_transitionTimer, &QTimer::timeout, this, &BacklightCtrl::on_transition);
    udev_unref(instance);
}

BacklightCtrl::~BacklightCtrl()
{
    udev_device_unref(m_device);
}

void BacklightCtrl::on_period_changed(int new_period)
{
    if (m_notification) {
        delete m_notification;
        m_notification = nullptr;
    }
    if (new_period) {
        m_notification = m_notifier.create_notification(new_period);
        connect(m_notification, &IdleNotification::idled, this, &BacklightCtrl::on_idle);
        connect(m_notification, &IdleNotification::resumed, this, &BacklightCtrl::on_resume);
    }
}

void BacklightCtrl::on_idle()
{
    if (m_idle)
        return;

    qDebug() << "turning off backlight";
    m_idle = true;

    udev_device_set_sysattr_value(m_device, "brightness", NULL); // clear udev's cache
    m_currentBrightness = m_systemBrightness = QString(udev_device_get_sysattr_value(m_device, "brightness")).toInt();
    m_transitionTimer.start(20);
}

void BacklightCtrl::on_resume()
{
    if (!m_idle)
        return;

    m_idle = false;
    m_currentBrightness = 10;
    m_transitionTimer.start(20);
    qDebug() << "turning on backlight";
}

void BacklightCtrl::on_transition()
{
    if (m_idle) {
        m_currentBrightness /= 1.1;
        if (m_currentBrightness <= 0) {
            m_currentBrightness = 0;
            m_transitionTimer.stop();
        }
    }
    else {
        m_currentBrightness *= 2;
        if (m_currentBrightness >= m_systemBrightness) {
            m_currentBrightness = m_systemBrightness;
            m_transitionTimer.stop();
        }
    }

    udev_device_set_sysattr_value(m_device, "brightness", std::to_string(m_currentBrightness).c_str());
}
