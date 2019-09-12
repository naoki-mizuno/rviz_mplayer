#include "mplayer_panel.h"

#include <pluginlib/class_list_macros.h>

namespace rviz_mplayer {

MPlayerPanel::MPlayerPanel(QWidget* parent /* = nullptr */)
    : rviz::Panel{parent}
{
    auto layout = new QVBoxLayout();

    mplayer_ = new QMPwidgetSizeHint(-1, 200);
    layout->addWidget(mplayer_);

    auto v_layout = new QVBoxLayout();

    auto h_layout_1 = new QHBoxLayout();
    // Enable checkbox
    checkbox_enable_ = new QCheckBox("Enable");
    checkbox_enable_->setEnabled(true);
    h_layout_1->addWidget(checkbox_enable_);
    // Optional arguments
    h_layout_1->addWidget(new QLabel("Args"));
    lineedit_args_ = new QLineEdit("");
    h_layout_1->addWidget(lineedit_args_);
    v_layout->addLayout(h_layout_1);

    auto h_layout_2 = new QHBoxLayout();
    // URI
    h_layout_2->addWidget(new QLabel("URI"));
    lineedit_uri_ = new QLineEdit("");
    h_layout_2->addWidget(lineedit_uri_);
    v_layout->addLayout(h_layout_2);

    layout->addLayout(v_layout);
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

    if (config.mapGetString("Args", &tmp_text)) {
        lineedit_args_->setText(tmp_text);
    }
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
    config.mapSetValue("Args", lineedit_args_->text());
}

void
MPlayerPanel::mplayer_start_stop(bool start) {
    if (start) {
        auto args = lineedit_args_->text().split(" ");

        mplayer_->start(args);
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
