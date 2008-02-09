#ifndef MTADVANCEDGROUPBOX_H
#define MTADVANCEDGROUPBOX_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>

class MTAdvancedGroupBox : public QWidget
{
    Q_OBJECT

protected:
    void init() {
        agb_vlayout = new QVBoxLayout(this);
        agb_vlayout->setMargin(0); agb_vlayout->setSpacing(0);
            agb_groupbox = new QGroupBox(tr("Advanced"), this);
            agb_groupbox->setCheckable(true);
            agb_groupbox->setChecked(false);
            agb_groupbox->setVisible(false);
            QObject::connect(agb_groupbox, SIGNAL(clicked(bool)), this, SLOT(setChecked(bool)));
                agb_glayout = new QGridLayout(agb_groupbox);
#ifdef Q_WS_MAC
                agb_glayout->setContentsMargins(9, 9, 9, 9);
#else
                agb_glayout->setContentsMargins(9, 6, 9, 9);
#endif
                agb_glayout->setSpacing(6);
        agb_vlayout->addWidget(agb_groupbox);
        if (!agb_external_checkbox) {
                agb_checkbox = new QCheckBox(tr("Advanced"), this);
                agb_checkbox->setChecked(false);
                QObject::connect(agb_checkbox, SIGNAL(clicked(bool)), this, SLOT(setChecked(bool)));
            agb_vlayout->addWidget(agb_checkbox);
        } else {
            agb_checkbox->setText(tr("Advanced"));
            agb_checkbox->setChecked(false);
            QObject::connect(agb_checkbox, SIGNAL(clicked(bool)), this, SLOT(setChecked(bool)));
        }
    };

public:
    MTAdvancedGroupBox(QWidget * parent = 0):
    QWidget(parent) { agb_external_checkbox = false; init(); };
    MTAdvancedGroupBox(QString title, QWidget * parent = 0):
    QWidget(parent) { agb_external_checkbox = false; init(); setTitle(title); };
    MTAdvancedGroupBox(QCheckBox * checkbox, QWidget * parent = 0):
    QWidget(parent) { agb_external_checkbox = true; agb_checkbox = checkbox; init(); };
    MTAdvancedGroupBox(QString title, QCheckBox * checkbox, QWidget * parent = 0):
    QWidget(parent) { agb_external_checkbox = true; agb_checkbox = checkbox; init(); setTitle(title); };

    bool isChecked() { return agb_checkbox->isChecked(); };
    void addLayout(QLayout * layout, int row, int column, Qt::Alignment alignment = 0)
    { agb_glayout->addLayout(layout, row, column, alignment); };
    void addLayout(QLayout * layout, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment = 0)
    { agb_glayout->addLayout(layout, row, column, rowSpan, columnSpan, alignment); };
    void addWidget(QWidget * widget, int row, int column, Qt::Alignment alignment = 0)
    { agb_glayout->addWidget(widget, row, column, alignment); };
    void addWidget(QWidget * widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment = 0)
    { agb_glayout->addWidget(widget, fromRow, fromColumn, rowSpan, columnSpan, alignment); };
    QGridLayout * gridLayout() { return agb_glayout; };

public slots:
    void setChecked(bool checked) {
        agb_checkbox->setChecked(checked);
        agb_groupbox->setChecked(checked);
        agb_checkbox->setVisible(!checked);
        agb_groupbox->setVisible(checked);
        if (agb_external_checkbox) { this->setVisible(checked); }
    };
    void setTitle(QString title) {
        agb_checkbox->setText(title);
        agb_groupbox->setTitle(title);
    };
    void setStatusTip(QString statustip) {
        agb_checkbox->setStatusTip(statustip);
        agb_groupbox->setStatusTip(statustip);
    };
    void setEnabled(bool enabled) {
        this->QWidget::setEnabled(enabled);
        if (agb_external_checkbox) { agb_checkbox->setEnabled(enabled); }
    };
    void setDisabled(bool disabled) {
        this->QWidget::setDisabled(disabled);
        if (agb_external_checkbox) { agb_checkbox->setDisabled(disabled); }
    };

private:
    bool agb_external_checkbox;
    QGridLayout * agb_glayout;
    QVBoxLayout * agb_vlayout;
    QGroupBox * agb_groupbox;
    QCheckBox * agb_checkbox;
};

#endif // MTADVANCEDGROUPBOX_H
