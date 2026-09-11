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

#ifndef CONTEXTMENUCHINESE_H
#define CONTEXTMENUCHINESE_H

// 右键菜单汉化（Qt 标准菜单的补充翻译）
//
// 背景（实测结论，不是猜的）：
//   编辑器与输出框都是 QPlainTextEdit，它们的右键菜单由 Qt 内部提供
//   （撤销/重做/剪切/复制/粘贴/删除/全选）。这些词条在 Qt 5.15.2 官方
//   qt_zh_CN.qm 里【一条都没有】—— 用 lconvert 把该 qm 转成 .ts 后统计共
//   1395 条翻译，其中 &Undo / &Redo / Cu&t / &Copy / &Paste / &Delete 全部缺失，
//   只有 "Select All" 存在且属于 Qt3 遗留上下文 Q3TextEdit，Qt5 的
//   QPlainTextEdit 并不会去查它。因此装 qt_zh_CN.qm 只能解决消息框按钮
//   （OK -> 确定）与文件对话框（Open -> 打开），右键菜单依然是英文。
//
// 做法：
//   createStandardContextMenu() 在 QPlainTextEdit 里【不是虚函数】，无法重写；
//   但 contextMenuEvent() 是虚函数。于是在子类的 contextMenuEvent 里调用
//   基类的 createStandardContextMenu() 拿到 QMenu，再把各项文字换成中文。
//   （与 Qt 官方文档给出的写法一致。）

#include <QMenu>
#include <QAction>
#include <QHash>
#include <QString>
#include <QList>

// 把 Qt 标准右键菜单里的英文项换成中文。
// 已经带中文的项（例如将来某个 Qt 版本自带了翻译）不会被改动。
inline void qawnoTranslateContextMenu(QMenu *menu) {
  if (!menu) return;

  static const QHash<QString, QString> kMap = {
    { QStringLiteral("&Undo"),            QStringLiteral("撤销(&U)")   },
    { QStringLiteral("&Redo"),            QStringLiteral("重做(&R)")   },
    { QStringLiteral("Cu&t"),             QStringLiteral("剪切(&T)")   },
    { QStringLiteral("&Copy"),            QStringLiteral("复制(&C)")   },
    { QStringLiteral("&Paste"),           QStringLiteral("粘贴(&P)")   },
    { QStringLiteral("&Delete"),          QStringLiteral("删除(&D)")   },
    { QStringLiteral("Delete"),           QStringLiteral("删除(&D)")   },  // Qt 5.15 这一项没有 & 前缀
    { QStringLiteral("Select All"),       QStringLiteral("全选(&A)")   },
    { QStringLiteral("&Select All"),      QStringLiteral("全选(&A)")   },
    // 下面几条 Qt 在特定情况下才加，一并备着
    { QStringLiteral("&Copy Link"),       QStringLiteral("复制链接(&C)") },
    { QStringLiteral("&Clear"),           QStringLiteral("清空(&C)")   },
  };

  const QList<QAction *> actions = menu->actions();
  for (QAction *action : actions) {
    if (!action) continue;
    // 关键：Qt 把快捷键拼在 text 里、用 \t 分隔（实测 "&Undo\tCtrl+Z" 就是这样），
    // 所以必须先按 \t 拆开、只查标签部分，拼回去时快捷键原样保留。
    const QString text = action->text();
    const int tabPos = text.indexOf(QLatin1Char('\t'));
    const QString label  = (tabPos >= 0) ? text.left(tabPos) : text;
    const QString suffix = (tabPos >= 0) ? text.mid(tabPos)  : QString();

    const auto it = kMap.constFind(label);
    if (it != kMap.constEnd()) {
      action->setText(it.value() + suffix);
    }
  }
}

#endif // CONTEXTMENUCHINESE_H
