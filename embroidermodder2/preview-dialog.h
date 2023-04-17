#ifndef PREVIEW_DIALOG_H
#define PREVIEW_DIALOG_H

#include <QFileDialog>

class ImageWidget;

class PreviewDialog : public QFileDialog
{
    Q_OBJECT

public:
    PreviewDialog(QWidget* parent = 0,
                  const QString& caption = QString(),
                  const QString& directory = QString(),
                  const QString& filter = QString());
    ~PreviewDialog();

private:
    ImageWidget* imgWidget;
};

#endif
