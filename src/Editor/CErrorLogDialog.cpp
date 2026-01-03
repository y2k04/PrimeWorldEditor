#include "Editor/CErrorLogDialog.h"
#include "ui_CErrorLogDialog.h"

#include "Editor/UICommon.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/qt_sinks.h>

CErrorLogDialog::CErrorLogDialog(QWidget *pParent)
    : QDialog(pParent)
    , ui(std::make_unique<Ui::CErrorLogDialog>())
{
    ui->setupUi(this);
    connect(ui->CloseButton, &QPushButton::clicked, this, &CErrorLogDialog::close);

    constexpr int max_lines = 300;
    auto axio = spdlog::get("axio");
    axio->sinks().push_back(std::make_shared<spdlog::sinks::qt_color_sink_mt>(ui->ErrorLogTextEdit, max_lines, false, true));
}

CErrorLogDialog::~CErrorLogDialog()
{
    spdlog::get("axio")->sinks().pop_back();
}
