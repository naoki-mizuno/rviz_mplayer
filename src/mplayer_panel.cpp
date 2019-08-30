#include "mplayer_panel.h"

#include <pluginlib/class_list_macros.h>

namespace rviz_mplayer {

MPlayerPanel::MPlayerPanel(QWidget* parent /* = nullptr */)
    : rviz::Panel{parent}
{
    auto layout = new QVBoxLayout();

    mplayer_ = new QMPwidgetSizeHint(-1, 200);
    layout->addWidget(mplayer_);

    auto h_layout = new QHBoxLayout();
    // Enable checkbox
    checkbox_enable_ = new QCheckBox("Enable");
    checkbox_enable_->setEnabled(true);
    h_layout->addWidget(checkbox_enable_);
    // URI
    h_layout->addWidget(new QLabel("URI"));
    lineedit_uri_ = new QLineEdit("");
    h_layout->addWidget(lineedit_uri_);
    layout->addLayout(h_layout);

    setLayout(layout);

    connect(checkbox_enable_, &QCheckBox::stateChanged,
            this, &MPlayerPanel::mplayer_start_stop);
    connect(lineedit_uri_, &QLineEdit::editingFinished,
            this, &MPlayerPanel::mplayer_changed_uri);
}

MPlayerPanel::~MPlayerPanel() {
    if (mplayer_->state() != QMPwidget::NotStartedState) {
        mplayer_->stop();
    }
}

void
MPlayerPanel::load(const rviz::Config& config) {
    rviz::Panel::load(config);

    bool start;
    QString tmp_text;

    if (config.mapGetString("URI", &tmp_text)) {
        lineedit_uri_->setText(tmp_text);
    }
    if (config.mapGetBool("Enabled", &start)) {
        // Note: Signals mplayer_start_stop
        checkbox_enable_->setChecked(start);
    }
}

void
MPlayerPanel::save(rviz::Config config) const {
    rviz::Panel::save(config);

    config.mapSetValue("Enabled", checkbox_enable_->isChecked());
    config.mapSetValue("URI", lineedit_uri_->text());
}

void
MPlayerPanel::mplayer_start_stop(bool start) {
    if (start) {
        mplayer_->start();
        lineedit_uri_->setEnabled(true);

        if (!lineedit_uri_->text().isEmpty()) {
            mplayer_->load(lineedit_uri_->text());
        }
    }
    else {
        mplayer_->stop();
        lineedit_uri_->setDisabled(true);
    }
}

void
MPlayerPanel::mplayer_changed_uri() {
    // Note: This method is fired when line content is edited
    const auto uri = lineedit_uri_->text();
    if (prev_uri_ != uri && mplayer_->state() != QMPwidget::NotStartedState) {
        mplayer_->load(uri);
        prev_uri_ = uri;
    }
}
}

PLUGINLIB_EXPORT_CLASS(rviz_mplayer::MPlayerPanel, rviz::Panel)
