#include "stdafx.h"
#include "ARWidget.h"


ARWidget::ARWidget(QWidget* parent)
    :QWidget(parent)
{
    setDisplayCallback([this](FramePtr frame) {
        postFrame(frame);
    });

    connect(this, &ARWidget::postFrame, this, &ARWidget::receiveFrame);

    std::thread(display).detach();
}


ARWidget::~ARWidget()
{
}


void ARWidget::receiveFrame(FramePtr frame)
{
    // is thread safe?
    m_frame = frame;
    update();
}

void ARWidget::paintEvent(QPaintEvent * event)
{
    if (m_frame)
    {
        QPainter painter(this);
        QImage img(m_frame->data, m_frame->cols, m_frame->rows, QImage::Format_RGB888);
        painter.drawImage(0, 0, img);
    }
}