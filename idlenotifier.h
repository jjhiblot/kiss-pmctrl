#ifndef IDLENOTIFIER_H
#define IDLENOTIFIER_H

#include "qwayland-ext-idle-notify-v1.h"
#include <QtWaylandClient/QWaylandClientExtension>

class IdleNotification;
class IdleNotifier : public QWaylandClientExtensionTemplate<IdleNotifier>, public QtWayland::ext_idle_notifier_v1
{
    Q_OBJECT

public:
    IdleNotifier();
    IdleNotification *create_notification(int timeout);

private slots:
    void handleExtensionActive();
private:
    bool m_active;
};

class IdleNotification : public QWaylandClientExtensionTemplate<IdleNotification>, public QtWayland::ext_idle_notification_v1
{
    Q_OBJECT

public:
    IdleNotification(struct ::ext_idle_notification_v1 *wl_object);
    ~IdleNotification();
    void ext_idle_notification_v1_idled() override;
    void ext_idle_notification_v1_resumed() override;
signals:
    void idled();
    void resumed();
};

#endif // IDLENOTIFIER_H
