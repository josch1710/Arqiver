/*
 * Copyright (C) Pedram Pourang (aka Tsu Jan) 2018-2022 <tsujan2000@gmail.com>
 *
 * Arqiver is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arqiver is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <https://spdx.org/licenses/GPL-3.0+.html>
 */

#ifndef ARQIVER_MAIN_UI_H
#define ARQIVER_MAIN_UI_H

#include <QMainWindow>
#include <QString>
#include <QCollator>
#include <QTreeWidgetItem>
#include <QDropEvent>
#include <QLabel>
#include <QProgressBar>

#include "backends.h"
#include "label.h"
#include "config.h"

namespace Arqiver {

class TreeWidgetItem : public QTreeWidgetItem {
public:
  TreeWidgetItem(QTreeWidgetItem *parent = nullptr, int type = QTreeWidgetItem::Type) :
  QTreeWidgetItem(parent, type) {
    collator_.setNumericMode(true);
    iconSize_ = QSize(16, 16);
    backend_ = nullptr;
  }

  QVariant data(int column, int role) const override;
  bool operator<(const QTreeWidgetItem& other) const override;

  void setBackend(Backend* B) {
      backend_ = B;
  }
  void setIconSize(const QSize& size) {
      iconSize_ = size;
  }

private:
  QPixmap emblemizedPixmap(const QString icon, const QSize& icnSize,
                           bool selected, bool lock) const;

  QCollator collator_;
  QSize iconSize_;
  Backend *backend_;
};

namespace Ui {
	class mainWin;
};

class mainWin : public QMainWindow {
  Q_OBJECT

public:
  mainWin();
  ~mainWin();

  void loadArguments(const QStringList& args);

  Config& getConfig() {
    return config_;
  }

  void adjustColumnSizes(bool stretch);

protected:
  bool eventFilter(QObject *watched, QEvent *event);

private slots:
  void newArchive();
  void openArchive();
  void addFiles();
  void addDirs();
  void removeFiles();
  void extractFiles();
  void autoextractFiles();
  void nextAutoExtraction();
  void autoArchiveFiles();
  void simpleArchivetFiles();
  void simpleExtractFiles();
  void extractSelection();
  void viewFile(QTreeWidgetItem *it);
  void onEnterPressed(QTreeWidgetItem *it);
  void extractDraggedItems();
  void labelContextMenu(const QPoint& p);
  void listContextMenu(const QPoint& p);
  void onChangingExpansion(QTreeWidgetItem *item);

  void procStarting();
  void procFinished(bool, const QString& msg);
  void procUpdate(int percent, const QString& txt);
  void openEncryptedList(const QString& path);

  void filter(const QString&);
  void reallyApplyFilter();

  void selectionChanged();

  void prefDialog();

  void aboutDialog();

private:
  void closeEvent(QCloseEvent *event);
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
  void changeEvent(QEvent *event);
  QHash<QString, QTreeWidgetItem*> cleanTree(const QStringList& list); // returns the remaining items
  void updateTree();
  QString allArchivingTypes();
  QString archivingTypes();
  QHash<QString, QString> supportedMimeTypes();
  QString openingTypes();
  QString filterToExtension(const QString& filter);
  bool pswrdDialog(bool listEncryptionBox = false, bool forceListEncryption = false);
  void enableActions(bool enable);
  bool subTreeIsEncrypted(QTreeWidgetItem *item);
  void hideChildlessDir(QTreeWidgetItem *item);

  QLabel *iconLabel_;
  Label *textLabel_;
  QProgressBar *statusProgress_;

  Ui::mainWin *ui;
  Backend *BACKEND;
  QStringList axFileList_, aaFileList_, saFileList_;
  QString lastPath_, lastFilter_;
  QString lastPswrd_;
  QString lastMsg_;
  bool canmodify_;
  bool updateTree_;
  bool scrollToCurrent_;
  bool expandAll_;
  bool close_;
  bool processIsRunning_;
  QTimer *filterTimer_;
  bool reapplyFilter_;

  Config config_;

  bool isRoot_;
};

#endif

}
