#ifndef HOSPITALWINDOW_H
#define HOSPITALWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class HospitalWindow; }
QT_END_NAMESPACE

/*!
 * \brief Displays data relevant to hospitals
 * The hospital window is the UI shown the hospital. It is where an administrator may see the filled and available beds,
 * the status of the hospital, and interact with this data.
 */
class HospitalWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit HospitalWindow(QWidget *parent = nullptr);
    ~HospitalWindow() override;

private:
    Ui::HospitalWindow *ui;
};
#endif // HOSPITALWINDOW_H
