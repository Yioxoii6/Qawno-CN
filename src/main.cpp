// This file is part of qawno.
//
// qawno is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// qawno is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with qawno. If not, see <http://www.gnu.org/licenses/>.

#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>

#include <qawno.h>
#include "MainWindow.h"

#include <string.h>

class ColourTranslator : public QTranslator
{
public:
  QString translate(const char* context, const char* sourceText, const char* disambiguation, int n) const override {
    static QChar color[] = { 'c', 'o', 'l', 'o', 'r' };
    static QChar Color[] = { 'C', 'o', 'l', 'o', 'r' };
    static QChar COLOR[] = { 'C', 'O', 'L', 'O', 'R' };
    static QChar colour[] = { 'c', 'o', 'l', 'o', 'u', 'r' };
    static QChar Colour[] = { 'C', 'o', 'l', 'o', 'u', 'r' };
    static QChar COLOUR[] = { 'C', 'O', 'L', 'O', 'U', 'R' };
    return QString(sourceText).replace(color, 5, colour, 6).replace(Color, 5, Colour, 6).replace(COLOR, 5, COLOUR, 6);
  }
};

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QCoreApplication::setApplicationName("Qawno");
  QCoreApplication::setApplicationVersion(QAWNO_VERSION_STRING);
  QCoreApplication::setOrganizationName("Zeex");
  QCoreApplication::setOrganizationDomain("zeex.github.io");
  QCoreApplication::installTranslator(new ColourTranslator());

  // [中文汉化] 装入 Qt 官方中文翻译（qt_zh_CN.qm）。
  // 为什么必须装：qawno 的右键菜单（撤销/重做/剪切/复制/粘贴/删除/全选）、
  // 各标准对话框的按钮（确定/取消/打开/保存）以及文件选择框，全部由 Qt 内部提供，
  // 程序自己的 .ui 里没有这些字符串 —— 只有装了翻译器它们才会显示中文。
  // 先找 exe 同目录（随程序一起分发，不依赖 Qt 安装路径），找不到再退回 Qt 自带的 translations 目录。
  {
    QTranslator* qtZh = new QTranslator();
    if (qtZh->load(QCoreApplication::applicationDirPath() + QStringLiteral("/qt_zh_CN.qm"))) {
      QCoreApplication::installTranslator(qtZh);
    } else {
      QTranslator* qtZhFallback = new QTranslator();
      if (qtZhFallback->load(QLocale(QLocale::Chinese, QLocale::China), QStringLiteral("qt"),
                             QStringLiteral("_"),
                             QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        QCoreApplication::installTranslator(qtZhFallback);
      }
    }
  }

  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}
