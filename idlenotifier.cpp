#include <QDebug>
#include "idlenotifier.h"
#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

IdleNotifier::IdleNotifier():
    QWaylandClientExtensionTemplate(1)
  ,QtWayland::ext_idle_notifier_v1()
  ,m_active(false)
{
    connect(this, &IdleNotifier::activeChanged, this, &IdleNotifier::handleExtensionActive);
}

IdleNotification *IdleNotifier::create_notification(int timeout)
{

    QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
    struct ::wl_seat *seat = static_cast<wl_seat *>(native->nativeResourceForIntegration("wl_seat"));
    struct ::ext_idle_notification_v1 *r = this->get_idle_notification(timeout, seat);
    return new IdleNotification(r);
}

void IdleNotifier::handleExtensionActive()
{
    m_active = true;
}

IdleNotification::IdleNotification(struct ::ext_idle_notification_v1 *wl_object)
    :QWaylandClientExtensionTemplate<IdleNotification>(1)
    ,QtWayland::ext_idle_notification_v1(wl_object)
{
}

IdleNotification::~IdleNotification()
{
    this->destroy();

}

void IdleNotification::ext_idle_notification_v1_idled()
{
    emit this->idled();
}

void IdleNotification::ext_idle_notification_v1_resumed()
{
    emit this->resumed();
}
