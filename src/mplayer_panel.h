#include <ros/ros.h>
#include <rviz/panel.h>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

// MPlayer widget
#include "qmpwidget.h"

namespace rviz_mplayer {
class QMPwidgetSizeHint;
class MPlayerPanel : public rviz::Panel {
Q_OBJECT
public:
    explicit MPlayerPanel(QWidget* parent = nullptr);

    ~MPlayerPanel() override;

    void load(const rviz::Config& config) override;

    void save(rviz::Config config) const override;

public Q_SLOTS:

    void
    mplayer_start_stop(bool start);

    void
    mplayer_changed_uri();

protected:
    QCheckBox* checkbox_enable_;
    QLineEdit* lineedit_uri_;
    QString prev_uri_;
    QMPwidgetSizeHint* mplayer_;

    // TODO: Add play/pause/forward/rewind buttons
};

class QMPwidgetSizeHint : public QMPwidget {
public:
    QMPwidgetSizeHint(const int w, const int h, QWidget* parent = nullptr)
        : QMPwidget{parent}
        , w_{w}
        , h_{h}
    {
    }

    QSize
    sizeHint() const override {
        return QSize{w_, h_};
    }

protected:
    int w_, h_;
};
}
