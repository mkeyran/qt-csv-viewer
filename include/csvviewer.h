#ifndef CSVVIEWER_H
#define CSVVIEWER_H

#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QTimer>


class CSVViewer : public QMainWindow
{
    Q_OBJECT

public:
    CSVViewer(QWidget *parent = nullptr);
    void loadCSV(const QString &filename, bool useHeader);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void copySelection();

private:
    QTableView *tableView;
    QStandardItemModel *model;
    QAction *copyAction;

};

#endif // CSVVIEWER_H