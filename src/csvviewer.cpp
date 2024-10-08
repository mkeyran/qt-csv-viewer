#include "csvviewer.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QHeaderView>
#include <QKeyEvent>
#include <QAction>
#include <QClipboard>
#include <QApplication>
#include <QMimeData>

CSVViewer::CSVViewer(QWidget *parent)
    : QMainWindow(parent)
{
    tableView = new QTableView(this);
    model = new QStandardItemModel(this);
    tableView->setModel(model);
    setCentralWidget(tableView);
    setWindowTitle("CSV Viewer");
    resize(800, 600);

    // Enable sorting
    tableView->setSortingEnabled(true);

    // Make the table read-only
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Enable copy functionality
    copyAction = new QAction("Copy", this);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, this, &CSVViewer::copySelection);
    addAction(copyAction);

    tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    tableView->addAction(copyAction);
}

void CSVViewer::loadCSV(const QString &filename, bool useHeader)
{
     QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open the file.");
        return;
    }

    QTextStream in(&file);
    model->clear();

    QStringList headers;
    int row = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = parseCSVLine(line);

        if (row == 0) {
            if (useHeader) {
                headers = fields;
                model->setHorizontalHeaderLabels(headers);
            } else {
                for (int i = 0; i < fields.size(); ++i) {
                    headers << QString("Column %1").arg(i + 1);
                }
                model->setHorizontalHeaderLabels(headers);

                QList<QStandardItem *> items;
                for (const QString &field : fields) {
                    items.append(new QStandardItem(field));
                }
                model->appendRow(items);
            }
        } else {
            QList<QStandardItem *> items;
            for (const QString &field : fields) {
                items.append(new QStandardItem(field));
            }
            model->appendRow(items);
        }

        row++;
    }

    file.close();
    setWindowTitle("CSV Viewer - " + filename);
    
    // Resize columns to content
    tableView->resizeColumnsToContents();

    // If columns are too narrow, give them a minimum width
    for (int i = 0; i < model->columnCount(); ++i) {
        if (tableView->columnWidth(i) < 100) {
            tableView->setColumnWidth(i, 100);
        }
    }

    // Enable alternate row colors for better readability
    tableView->setAlternatingRowColors(true);
}

void CSVViewer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void CSVViewer::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    QTimer::singleShot(0, this, [this]() {
        raise();
        activateWindow();
    });
}

QStringList CSVViewer::parseCSVLine(const QString &line)
{
    QStringList fields;
    QString field;
    bool inQuotes = false;
    
    for (int i = 0; i < line.length(); ++i) {
        QChar current = line[i];
        
        if (inQuotes) {
            if (current == '"' && i < line.length() - 1 && line[i+1] == '"') {
                // Double quotes inside quoted field
                field += '"';
                ++i; // Skip next quote
            } else if (current == '"') {
                // End of quoted field
                inQuotes = false;
            } else {
                // Regular character inside quotes
                field += current;
            }
        } else {
            if (current == '"') {
                // Beginning of quoted field
                inQuotes = true;
            } else if (current == ',') {
                // End of unquoted field
                fields.append(field.trimmed());
                field.clear();
            } else {
                // Regular character outside quotes
                field += current;
            }
        }
    }
    
    // Add the last field
    fields.append(field.trimmed());
    
    return fields;
}


void CSVViewer::copySelection()
{
    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty())
        return;

    // Sort the indexes by row, then column
    std::sort(selectedIndexes.begin(), selectedIndexes.end(), [](const QModelIndex &a, const QModelIndex &b) {
        if (a.row() != b.row())
            return a.row() < b.row();
        return a.column() < b.column();
    });

    // Generate HTML table
    QString htmlTable = "<table border='1' cellspacing='0' cellpadding='2'>";
    int currentRow = selectedIndexes.first().row();
    htmlTable += "<tr>";

    QString plainText;

    for (const QModelIndex &index : selectedIndexes) {
        if (index.row() != currentRow) {
            htmlTable += "</tr><tr>";
            plainText += "\n";
            currentRow = index.row();
        }
        
        QString cellData = model->data(index).toString();
        htmlTable += "<td>" + cellData.toHtmlEscaped() + "</td>";
        plainText += cellData + "\t";
    }

    htmlTable += "</tr></table>";

    // Create mime data with both HTML and plain text
    QMimeData *mimeData = new QMimeData;
    mimeData->setHtml(htmlTable);
    mimeData->setText(plainText.trimmed());

    // Set the mime data to the clipboard
    QApplication::clipboard()->setMimeData(mimeData);
}
